/**************************************************************************************
** FILE NAME: DrvInternalFlash.c
** DESCRIPTION: Driver for writing and reading to internal flash
** DEVELOPMENT ENVIRONMENT: keil
** NOTES:
**************************************************************************************/
/*==============================================================================
                                 Includes
==============================================================================*/
#include "DrvInternalFlash.h"

#include <string.h> // memcpy

/*==============================================================================
                             Function Definitions
==============================================================================*/
HAL_StatusTypeDef IntF_EraseSectorsUnsafe(uint32_t startSector, uint8_t numOfSectorsToErase)
{
    uint32_t                SectorError     = 0;
    FLASH_EraseInitTypeDef  EraseInitStruct;
    HAL_StatusTypeDef       flashStatus     = HAL_OK;

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase       = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange    = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.NbSectors       = numOfSectorsToErase;
    EraseInitStruct.Sector          = startSector;

    /* Perform Erase Sector */
    do
    {
        flashStatus = HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError);
     } while(flashStatus != HAL_OK);

    return flashStatus;
}

HAL_StatusTypeDef IntF_EraseSectorsSafe(uint32_t startSector, uint8_t numOfSectorsToErase)
{
    HAL_StatusTypeDef lockStatus = HAL_OK;
    HAL_StatusTypeDef eraseStatus = HAL_OK;

    lockStatus = HAL_FLASH_Unlock();
    if(lockStatus != HAL_OK)
    {
        return lockStatus;
    }

    eraseStatus = IntF_EraseSectorsUnsafe(startSector, numOfSectorsToErase);

    HAL_FLASH_Lock();
    return eraseStatus;
}

HAL_StatusTypeDef IntF_WriteToInternalFlashUnsafe(uint32_t address, uint8_t* buffer, size_t bufferSize, size_t* bytesWritten)
{
    uint64_t data = 0;
    HAL_StatusTypeDef flashStatus = HAL_ERROR;
    size_t i = 0;
    uint8_t*          pDataAddress;

    for (; i < bufferSize; ++i)
    {
        data = buffer[i];
        pDataAddress = ((uint8_t*)address) + i;
        
        flashStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, (uint32_t)pDataAddress, data);
        if (HAL_OK != flashStatus)
        {
            break;
        }
    }
    
    *bytesWritten = i;
    
    return flashStatus;
}

HAL_StatusTypeDef IntF_WriteToInternalFlashSafe(uint32_t address, uint8_t* buffer, size_t bufferSize, size_t* bytesWritten)
{
    HAL_StatusTypeDef lockStatus = HAL_OK;
    HAL_StatusTypeDef writeStatus = HAL_OK;
    
    lockStatus = HAL_FLASH_Unlock();
    if(lockStatus != HAL_OK)
    {
        return lockStatus;
    }

    writeStatus = IntF_WriteToInternalFlashUnsafe(address, buffer, bufferSize, bytesWritten);

    HAL_FLASH_Lock();
    return writeStatus;
}

HAL_StatusTypeDef IntF_ReadFromInternalFlash(uint32_t address, uint8_t* buffer, size_t bytesToRead)
{
    if (memcpy((void *)buffer, (void *)address, bytesToRead) == NULL)
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}
