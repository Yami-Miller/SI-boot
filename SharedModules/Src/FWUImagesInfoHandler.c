/**************************************************************************************
** @FileName: BootInfoHandler.c
** @Description: Handles managing the boot info 
** @DevelopmentEnviorment: Keil
** @Notes: Shared module to Main-Boot and Main-Program 
**************************************************************************************/
/*==============================================================================
                                Includes
==============================================================================*/
#include "FWUImagesInfoHandler.h"
#include "SIMemoryMap.h"
#include "DrvNand.h"
#include "CommonFunctions.h"
#include "DrvInternalFlash.h"
 
/*==============================================================================
                                Defines
==============================================================================*/    
#define imgh_MAGIC_NUMBER 0XDEADBEEF
#define BUFFER_SIZE 200
/*==============================================================================
                                Structures
================================================================================*/  
typedef struct __ImagesInfo
{
    uint32_t m_magicNumber;
    FirmwareImageInfo_t m_SIImagesInfo[DAT_NUM_OF_FW_IMAGES];
    uint8_t m_numOfSIImagesInflash;
}ImagesInfo_t;

/*==============================================================================
                            Local/Private Variables
==============================================================================*/
static ImagesInfo_t g_imagesInfo;

/*==============================================================================
                            Local Function Definitions
==============================================================================*/
static void imgh_FirstRunInitlazation(ImagesInfo_t* imagesInfo);
static void imgh_eraseImage();
static void imgh_CopyRuningImageToFlash(FirmwareImageInfo_t* imageSaveLocationInfo);

/*==============================================================================
                           API/Public Function Implementation
==============================================================================*/
bool IMGH_InitFWUImagesInfo(void)
{
    HAL_StatusTypeDef bootInfoStatus;
    
    bootInfoStatus = DAT_ReadBuffer(DAT_FW_IMAGES_INFO, (uint8_t*)&g_imagesInfo, sizeof(ImagesInfo_t));        
 
    if(bootInfoStatus != HAL_OK)
    {
        return false;
    }
    
    if(g_imagesInfo.m_magicNumber != imgh_MAGIC_NUMBER)
    {
        imgh_FirstRunInitlazation(&g_imagesInfo);
    }
    
    return true;
}

FirmwareImageInfo_t* IMGH_FindEmptySlotForImage(void)
{
    FirmwareImageInfo_t* imagesInfo = g_imagesInfo.m_SIImagesInfo;
    uint8_t numOfAvailableImages = g_imagesInfo.m_numOfSIImagesInflash;
    FirmwareImageInfo_t* emptySlot = NULL;
    
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

bool IMGH_SetUpdateIomageToGolden(void)
{
    bool result = false;
    bool needToSave = false;
    FirmwareImageInfo_t* imagesInfo = g_imagesInfo.m_SIImagesInfo;
    uint8_t numOfAvailableImages = g_imagesInfo.m_numOfSIImagesInflash;
    
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
        result = IMGH_SaveImagestInfo();
    }
    
    return result;
}

FirmwareImageInfo_t* IMGH_FindImage(FWUImage_t imageType)
{    
    for(uint8_t i = 0; i < g_imagesInfo.m_numOfSIImagesInflash ; ++i )
    {
        if(g_imagesInfo.m_SIImagesInfo[i].m_imageType == imageType && g_imagesInfo.m_SIImagesInfo[i].m_imageSize != 0)
        {
            return &(g_imagesInfo.m_SIImagesInfo[i]);
        }
    }

    return NULL;
}

bool IMGH_SaveImagestInfo(void)
{
    HAL_StatusTypeDef status = HAL_OK;
    
    DAT_EraseFile(DAT_FW_IMAGES_INFO_FILE_NAME);
    status = DAT_WriteBuffer(DAT_FW_IMAGES_INFO, (uint8_t*)&g_imagesInfo, sizeof(g_imagesInfo), false);        
    
    if(status != HAL_OK)
    {
        return false;
    }
    return true;
}
/*==============================================================================
                            Local Function Implementation
==============================================================================*/
static void imgh_FirstRunInitlazation(ImagesInfo_t* imagesInfo)
{
    uint32_t imageAddress = DAT_FW_IMAGE_ADDR;
    
    imagesInfo->m_numOfSIImagesInflash = DAT_NUM_OF_FW_IMAGES;
    
    for(uint8_t i = 0; i < imagesInfo->m_numOfSIImagesInflash; ++i)
    {
        imagesInfo->m_SIImagesInfo[i].m_imageType = FWU_NO_IMAGE;
        imagesInfo->m_SIImagesInfo[i].m_imageAddress = imageAddress;
        imagesInfo->m_SIImagesInfo[i].m_imageSize = 0;
        imagesInfo->m_SIImagesInfo[i].m_checkSum = 0;
        
        imageAddress += DAT_FW_IMAGE_SIZE;
    }
    
    imgh_CopyRuningImageToFlash(&(imagesInfo->m_SIImagesInfo[0]));
    
    imagesInfo->m_magicNumber = imgh_MAGIC_NUMBER;
    
    IMGH_SaveImagestInfo();
}

static void imgh_CopyRuningImageToFlash(FirmwareImageInfo_t* imageSaveLocationInfo)
{
    uint32_t writeAddress = imageSaveLocationInfo->m_imageAddress;
    uint32_t appAddress = APP_ADDRESS;
    const uint32_t endAppAddress = APP_ADDRESS + DAT_FW_IMAGE_SIZE;

    HAL_StatusTypeDef internalFlashStatus = HAL_OK;
    uint8_t bytesToRead = 0;
    static uint8_t buffer[BUFFER_SIZE];
    uint32_t checksum = 0;
    
    imgh_eraseImage();
    
    while(appAddress < endAppAddress && internalFlashStatus == HAL_OK)
    {
        bytesToRead = CF_Min(BUFFER_SIZE, endAppAddress - appAddress);
        IntF_ReadFromInternalFlash(appAddress, buffer, bytesToRead);
        
        checksum += CF_CalcBufferSum(buffer, bytesToRead);
        internalFlashStatus = DAT_WriteBuffer(writeAddress, buffer, bytesToRead, false);
        
        if(internalFlashStatus != HAL_OK)
        {
            break;
        }

        writeAddress += bytesToRead;
        appAddress += bytesToRead;
    } 
    
    imageSaveLocationInfo->m_checkSum = checksum;
    imageSaveLocationInfo->m_imageSize = DAT_FW_IMAGE_SIZE;
    imageSaveLocationInfo->m_imageType = FWU_GOLDEN;
}

static void imgh_eraseImage()
{
		DAT_EraseFile(DAT_MAIN_IMAGE_FILE_NAME);
		DAT_EraseFile(DAT_SI_IMAGE_FILE_NAME);
}

