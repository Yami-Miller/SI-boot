/*==============================================================================
                                Includes
==============================================================================*/
#include "BootInfoHandler.h"
#include "MainMemoryMap.h"
#include "DrvSpiNVM.h" //NVM_WriteBufferDev2, NVM_ReadBufferDev2
#include "CommonFunctions.h"
#include "DrvInternalFlash.h"
#include "FWU_GlobalDef.h"
#include <string.h>
#include "GlobalDef.h"
/*==============================================================================
                                Defines
==============================================================================*/    
#define BIH_MAGIC_NUMBER 0XDEADBEEF
#define BIH_BUFFER_SIZE 200
/*==============================================================================
                                Structures
================================================================================*/  
#pragma pack(1)
typedef struct __ImagesInfo
{
    FirmwareImageInfo_t m_mainImagesInfo[DM_MAIN_IMAGES_IN_FLASH];
    FirmwareImageInfo_t m_SIImagesInfo[DM_SI_IMAGES_IN_FLASH];
    uint8_t m_numOfMainImagesInflash;
    uint8_t m_numOfSIImagesInflash;
}ImagesInfo_t;

typedef struct __BootInfo
{
    uint32_t m_magicNumber;
    BootMode_t m_bootFlag;
    uint8_t m_mainBootComProtocolVersion;
    ImagesInfo_t m_imagesInfo;
}BootInfo_t;

#pragma pack(4)

/*==============================================================================
                            Local/Private Variables
==============================================================================*/
static BootInfo_t g_bootInfo;

/*==============================================================================
                            Local Function Definitions
==============================================================================*/
static void bih_FirstRunInitlazation(BootInfo_t* bootInfo);
static void bih_CopyRuningImageToFlash(FirmwareImageInfo_t* imageSaveLocationInfo);
static void bih_eraseImage(FirmwareImageInfo_t* imageInfo);

/*==============================================================================
                           API/Public Function Implementation
==============================================================================*/
bool BIH_InitBootInfo(void)
{
    HAL_StatusTypeDef bootInfoStatus;
    
    uint32_t infoAddress = DM_BOOT_INFO_ADDRESS;
    bootInfoStatus = NVM_ReadBufferDev2(infoAddress, (uint8_t*)&g_bootInfo, sizeof(BootInfo_t));        
 
    if(bootInfoStatus != HAL_OK)
    {
        return false;
    }
    
    if(g_bootInfo.m_magicNumber != BIH_MAGIC_NUMBER)
    {
        bih_FirstRunInitlazation(&g_bootInfo);
    }
    
    return true;
}

bool BIH_SetBootMode(BootMode_t bootMode)
{
    g_bootInfo.m_bootFlag = bootMode;
    
    return BIH_SaveBootInfo();
}

BootMode_t BIH_GetBootMode(void)
{
    return g_bootInfo.m_bootFlag;
}

uint8_t BIH_GetBootComProtocolVersion(void)
{
    return g_bootInfo.m_mainBootComProtocolVersion;
}

bool BIH_SetBootComProtocolVersion(uint8_t bootComProtocolVersion)
{
    if(bootComProtocolVersion != g_bootInfo.m_mainBootComProtocolVersion)
    {
        g_bootInfo.m_mainBootComProtocolVersion = bootComProtocolVersion;
        return BIH_SaveBootInfo();
    }
    return true;
}

FirmwareImageInfo_t* BIH_FindEmptySlotForImage(FWUImageDevice_t imageDestinationDevice)
{
    FirmwareImageInfo_t* imagesInfo = NULL;
    uint8_t numOfAvailableImages = 0;
    FirmwareImageInfo_t* emptySlot = NULL;
    
    if(imageDestinationDevice == MAIN_IMAGE)
    {
        imagesInfo = g_bootInfo.m_imagesInfo.m_mainImagesInfo;
        numOfAvailableImages = g_bootInfo.m_imagesInfo.m_numOfMainImagesInflash;
    }
    else if(imageDestinationDevice == SI_IMAGE)
    {
        imagesInfo = g_bootInfo.m_imagesInfo.m_SIImagesInfo;
        numOfAvailableImages = g_bootInfo.m_imagesInfo.m_numOfSIImagesInflash;
    }
    
    for(uint8_t i = 0; i < numOfAvailableImages; ++i )
    {
        if(imagesInfo[i].m_imageType != FWU_GOLDEN)
        {
            imagesInfo[i].m_imageType = FWU_UPDATE;
            emptySlot = &(imagesInfo[i]);
            return emptySlot;
        }
    }

    return emptySlot;
}

bool BIH_SetUpdateIomageToGolden(void)
{
    bool result = false;
    bool needToSave = false;
    FirmwareImageInfo_t* imagesInfo = g_bootInfo.m_imagesInfo.m_mainImagesInfo;
    uint8_t numOfAvailableImages = g_bootInfo.m_imagesInfo.m_numOfMainImagesInflash;
    
    for(uint8_t i = 0; i < numOfAvailableImages; ++i )
    {
        switch(imagesInfo[i].m_imageType)
        {
            case FWU_GOLDEN:
                imagesInfo[i].m_imageType = FWU_OLD;
                needToSave = true;
                break;
            case FWU_UPDATE:
                imagesInfo[i].m_imageType = FWU_GOLDEN;
                needToSave = true;
                break;
            default:
                continue;
        }
    }
    
    if(needToSave)
    {
        result = BIH_SaveBootInfo();
    }
    
    return result;
}

FirmwareImageInfo_t* BIH_FindImage(FWUImage_t imageType)
{
    FirmwareImageInfo_t* imagesInfo = g_bootInfo.m_imagesInfo.m_mainImagesInfo;
    uint8_t numOfAvailableImages = g_bootInfo.m_imagesInfo.m_numOfMainImagesInflash;
    
    for(uint8_t i = 0; i < numOfAvailableImages ; ++i )
    {
        if(imagesInfo[i].m_imageType == imageType && imagesInfo[i].m_imageSize != 0)
        {
            return &(imagesInfo[i]);
        }
    }

    return NULL;
}

bool BIH_SaveBootInfo(void)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint32_t address = DM_BOOT_INFO_ADDRESS;
    
    NVM_SectorEraseDev2(address);
    status = NVM_WriteBufferDev2(address, (uint8_t*)&g_bootInfo, sizeof(BootInfo_t));        
    
    if(status != HAL_OK)
    {
        return false;
    }
    return true;
}

/*==============================================================================
                            Local Function Implementation
==============================================================================*/
static void bih_FirstRunInitlazation(BootInfo_t* bootInfo)
{
    uint32_t imageAddress = DM_MAIN_UPDATE_BASE_ADDRESS;

    bootInfo->m_bootFlag = BOOT_NORMAL;
    bootInfo->m_mainBootComProtocolVersion = 0;
    
    bootInfo->m_imagesInfo.m_numOfMainImagesInflash = DM_MAIN_IMAGES_IN_FLASH;
    bootInfo->m_imagesInfo.m_numOfSIImagesInflash = DM_SI_IMAGES_IN_FLASH;
        
    for(uint8_t i = 0; i < bootInfo->m_imagesInfo.m_numOfMainImagesInflash; ++i)
    {
        bootInfo->m_imagesInfo.m_mainImagesInfo[i].m_imageType = FWU_NO_IMAGE;
        bootInfo->m_imagesInfo.m_mainImagesInfo[i].m_imageAddress = imageAddress;
        bootInfo->m_imagesInfo.m_mainImagesInfo[i].m_imageSize = 0;
        bootInfo->m_imagesInfo.m_mainImagesInfo[i].m_checkSum = 0;
        
        imageAddress += DM_MAIN_MAX_IMAGE_SIZE;
    }
    
    //Create Golden Image
    bih_CopyRuningImageToFlash(&(bootInfo->m_imagesInfo.m_mainImagesInfo[0]));
    
    imageAddress = DM_SI_UPDATE_ADDRESS;
    for(uint8_t i = 0; i < g_bootInfo.m_imagesInfo.m_numOfSIImagesInflash; ++i)
    {
        bootInfo->m_imagesInfo.m_SIImagesInfo[i].m_imageType = FWU_NO_IMAGE;
        bootInfo->m_imagesInfo.m_SIImagesInfo[i].m_imageAddress = imageAddress;
        bootInfo->m_imagesInfo.m_SIImagesInfo[i].m_imageSize = 0;
        bootInfo->m_imagesInfo.m_SIImagesInfo[i].m_checkSum = 0;
        
        imageAddress += DM_SI_MAX_IMAGE_SIZE;
    }
    
    bootInfo->m_magicNumber = BIH_MAGIC_NUMBER;
    
    BIH_SaveBootInfo();
}

static void bih_CopyRuningImageToFlash(FirmwareImageInfo_t* imageSaveLocationInfo)
{
    uint32_t writeAddress = imageSaveLocationInfo->m_imageAddress;
    uint32_t appAddress= APP_ADDRESS;
    const uint32_t endAppAddress = APP_ADDRESS + DM_MAIN_MAX_IMAGE_SIZE;

    HAL_StatusTypeDef internalFlashStatus = HAL_OK;
    uint8_t bytesToRead = 0;
    static uint8_t buffer[BIH_BUFFER_SIZE];
    uint32_t checksum = 0;
    
    bih_eraseImage(imageSaveLocationInfo);
    
    while(appAddress < endAppAddress && internalFlashStatus == HAL_OK)
    {
        bytesToRead = CF_Min(BIH_BUFFER_SIZE, endAppAddress - appAddress);
        IntF_ReadFromInternalFlash(appAddress, buffer, bytesToRead);
        
        checksum += CF_CalcBufferSum(buffer, bytesToRead);
        internalFlashStatus = NVM_WriteBufferDev2(writeAddress, buffer, bytesToRead);
        
        if(internalFlashStatus != HAL_OK)
        {
            break;
        }
                
        writeAddress += bytesToRead;
        appAddress += bytesToRead;
    } 
    
    imageSaveLocationInfo->m_checkSum = checksum;
    imageSaveLocationInfo->m_imageSize = DM_MAIN_MAX_IMAGE_SIZE;
    imageSaveLocationInfo->m_imageType = FWU_GOLDEN;
}

static void bih_eraseImage(FirmwareImageInfo_t* imageInfo)
{
    uint8_t sectorsToErase = 0;
    uint32_t address = 0;

    sectorsToErase = DM_MAIN_MAX_IMAGE_SIZE / NVM_SECTOR_SIZE;
    
    //calculate the start of the sector
    address = imageInfo->m_imageAddress;
    for(int i = 0; i < sectorsToErase; ++i)
    {
        NVM_SectorEraseDev2(address);
        address += NVM_SECTOR_SIZE;
    }
}


