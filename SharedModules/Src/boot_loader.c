/**
  ******************************************************************************
  * @file    boot_loader.c
  * @author  Noam Yulzari
  * @date    18-August-2020
  * @brief   Contains all the programs needed in boot mood shared for both main-boot and si-boot
  ******************************************************************************
*/

/*==============================================================================
                                    Includes
==============================================================================*/
#include "boot_loader.h"
#include "DrvInternalFlash.h"
#include "GlobalDef.h"
#include "CommonFunctions.h"

/*==============================================================================
                                    Defines
==============================================================================*/
#define BUFFER_SIZE 200
#define NUM_OF_IMAGES_IN_FLASH (2)

/*==============================================================================
                            External/Public Variables
==============================================================================*/
extern UART_HandleTypeDef huart3;

/*==============================================================================
                                Structures
==============================================================================*/
    
/*==============================================================================
                            Local/Private Variables
==============================================================================*/

/*==============================================================================
                            Local Function Definitions
==============================================================================*/
static uint64_t bl_CalcCheckSumForBuffer(uint8_t* buffer, uint16_t bufferSize);
static HAL_StatusTypeDef HAL_DeInit_NoGPIO(void);

/*==============================================================================
                           API/Public Function Implementation
==============================================================================*/
void BL_JumpToApplication(const BootParams_t* bootInfo)
{
    uint32_t  JumpAddress = *(__IO uint32_t*)(bootInfo->m_appAddress + 4);
    pFunction Jump        = (pFunction)JumpAddress;
    
    HAL_UART_DeInit(&huart3);
    HAL_DeInit_NoGPIO();

    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;

    SCB->VTOR = bootInfo->m_appAddress;

    __set_MSP(*(__IO uint32_t*)bootInfo->m_appAddress);
    Jump();
}

bool BL_LoadImageToInetnalFlash(const BootParams_t* bootInfo, FirmwareImageInfo_t* updateInfo)
{
    static uint8_t buffer[BUFFER_SIZE];
    uint32_t imageAddress = updateInfo->m_imageAddress;
    const uint32_t endImageAddress = imageAddress + updateInfo->m_imageSize;
    HAL_StatusTypeDef status = HAL_OK;
    uint32_t appAddress = bootInfo->m_appAddress;
    uint32_t bytesWritten = 0;
    uint32_t checksum = 0;
    uint16_t bytesToRead = 0;
        
    status = HAL_FLASH_Unlock();
    if (status != HAL_OK)
    {
        return false;
    }
        
    status = IntF_EraseSectorsUnsafe(bootInfo->m_appImageStartSectorNum, bootInfo->m_appImageSizeInSectors);
    if (status != HAL_OK)
    {
        return false;
    }

    while (imageAddress < endImageAddress && status == HAL_OK)
    {
        bytesToRead = CF_Min(BUFFER_SIZE, endImageAddress - imageAddress);
        status = bootInfo->m_externalFlashReadFunc(imageAddress, buffer, bytesToRead);
        if(status != HAL_OK) break;

        status = IntF_WriteToInternalFlashUnsafe(appAddress, buffer, bytesToRead, &bytesWritten);
        if(status != HAL_OK || bytesWritten != bytesToRead) break;
        
        checksum += bl_CalcCheckSumForBuffer(buffer, bytesToRead);
        imageAddress += bytesToRead;
        appAddress += bytesWritten;
    }
    
    HAL_FLASH_Lock();
    
    if(checksum != updateInfo->m_checkSum)
    {
        return false;
    }
        
    return status == HAL_OK;
}

/*==============================================================================
                            Local Function Implementation
==============================================================================*/
static uint64_t bl_CalcCheckSumForBuffer(uint8_t* buffer, uint16_t bufferSize)
{
    uint64_t tmpCsum = 0;
    for(uint16_t i = 0; i < bufferSize; i++ )
    {
        tmpCsum += buffer[i];
    }

    return tmpCsum;
}

static HAL_StatusTypeDef HAL_DeInit_NoGPIO(void)
{
   /* The default function is not used because it deletes the values (APB1 is removed in this version)
    inside the register used to save the statup reason, watchdog/power on/software reset (CR-1503)*/
    
  /* Reset of all peripherals */
  __HAL_RCC_APB1_FORCE_RESET();
  __HAL_RCC_APB1_RELEASE_RESET();

  __HAL_RCC_APB2_FORCE_RESET();
  __HAL_RCC_APB2_RELEASE_RESET();

  __HAL_RCC_AHB2_FORCE_RESET();
  __HAL_RCC_AHB2_RELEASE_RESET();

  __HAL_RCC_AHB3_FORCE_RESET();
  __HAL_RCC_AHB3_RELEASE_RESET();

  /* De-Init the low level hardware */
  HAL_MspDeInit();
    
  /* Return function status */
  return HAL_OK;
}
