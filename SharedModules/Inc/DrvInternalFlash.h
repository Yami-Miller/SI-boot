/**************************************************************************************
** FILE NAME: DrvInternalFlash.H
** DESCRIPTION: Driver for writing and reading to internal flash
** DEVELOPMENT ENVIRONMENT: keil
** NOTES:
**************************************************************************************/
#ifndef DRV_INTERNAL_FLASH_H
#define DRV_INTERNAL_FLASH_H

/*==============================================================================
                                Defines
==============================================================================*/
#include <stdbool.h>
#include <stdint.h> //uint32_t
#include <stddef.h> //size_t
#include "stm32f4xx_hal.h" //HAL_StatusTypeDef

#ifdef __cplusplus
    extern "C"{
#endif
/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/
/*******************************************************************************
** @Function   : IntF_EraseSectorsUnsafe
** @Description: Erase the sector in the given range. 
                 This function should be called when HAL_FLASH_UNLOCK was already called
** @Inputs     : startSector - Initial FLASH sector to erase must be a value of FLASHEx_Sectors
                 numOfSectorsToErase - the number of sectors that will be erased starting from the start sector
** @Outputs    : None
** @Return     : true if the operation was successful. false otherwise
*******************************************************************************/
HAL_StatusTypeDef IntF_EraseSectorsUnsafe(uint32_t startSector, uint8_t numOfSectorsToErase);

/*******************************************************************************
** @Function   : intf_EraseSectorsSafe
** @Description: Erase the sector in the given range. 
** @Inputs     : startSector - Initial FLASH sector to erase must be a value of FLASHEx_Sectors
                 numOfSectorsToErase - the number of sectors that will be erased starting from the start sector
** @Outputs    : None
** @Return     : true if the operation was successful. false otherwise
*******************************************************************************/
HAL_StatusTypeDef IntF_EraseSectorsSafe(uint32_t startSector, uint8_t numOfSectorsToErase);

/*******************************************************************************
** @Function   : IntF_WriteToInternalFlashUnsafe
** @Description: Write to internal flash. before writing erase all the sectors that are going to be written on.
                 This function should be called when HAL_FLASH_UNLOCK was already called
** @Inputs     : address - where the data should be written.
                 buffer - the data to write
                 bufferSize - the size of the buffer
** @Outputs    : bytesWritten - the number of bytes written successfully to flash.
** @Return     : true if the operation was successful. false otherwise
*******************************************************************************/
HAL_StatusTypeDef IntF_WriteToInternalFlashUnsafe(uint32_t address, uint8_t* buffer, size_t bufferSize, size_t* bytesWritten);

/*******************************************************************************
** @Function   : IntF_WriteToInternalFlashUnsafe
** @Description: Write to internal flash. before writing erase all the sectors that are going to be written on.
** @Inputs     : address - where the data should be written.
                 buffer - the data to write
                 bufferSize - the size of the buffer
                 bytesWritten - the number of bytes written successfully to flash.
** @Outputs    : None
** @Return     : true if the operation was successful. false otherwise
*******************************************************************************/
HAL_StatusTypeDef IntF_WriteToInternalFlashSafe(uint32_t address, uint8_t* buffer, size_t bufferSize, size_t* bytesWritten);

/*******************************************************************************
** @Function   : IntF_ReadFromInternalFlash
** @Description: Reads data from internal flash.
** @Inputs     : @address - address to read from
**               @bytesToRead   - Number of bytes to read
** @Outputs    : @buffer - buffer to save the data to.
** @Return     : true - succedded, false - failed
*******************************************************************************/
HAL_StatusTypeDef IntF_ReadFromInternalFlash(uint32_t address, uint8_t* buffer, size_t bytesToRead);

#ifdef __cplusplus
    }
#endif

#endif //DRV_INTERNAL_FLASH_H
