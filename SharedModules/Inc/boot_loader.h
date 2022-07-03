/**
  ******************************************************************************
  * @file    boot_loader.h
  * @author  Noam Yulzari
  * @date    18-August-2020
  * @brief   Contains all the programs needed in boot mood shared for both main-boot and si-boot
  ******************************************************************************
*/

#ifndef BOOT_LOADER_H
#define BOOT_LOADER_H

/*==============================================================================
                                Includes
==============================================================================*/
#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include "FWU_GlobalDef.h"

/*==============================================================================
                              Type Definitions
==============================================================================*/
typedef void (*pFunction)(void); /*!< Function pointer definition */
typedef HAL_StatusTypeDef (*ReadFunction_t)(uint32_t sAddress, uint8_t *pBuffer, uint32_t size); /*!< Read function definition */

/*==============================================================================
                                Structs
=============================================================================*/
typedef struct __BootParams
{
    uint32_t m_appAddress; //the adress of the app in the internal flash
    uint32_t m_appImageStartSectorNum; //the sector number of the start of the image
    uint32_t m_savedImagesBaseAddress; //the base address of the saved images in external flash
    uint32_t m_maxImageSize; //the max size of an image.
    ReadFunction_t m_externalFlashReadFunc; //function that will be called to read the image from external flash
    uint8_t m_appImageSizeInSectors; //how many sectors those the image occupies
}BootParams_t;

/*==============================================================================
                        External/Public Function Protoypes
==============================================================================*/
/*******************************************************************************
** @Function   : BL_JumpToApplication
** @Description: starts the acctual application based on APP_ADDRESS
** @Inputs     : @bootInfo
** @Outputs    : None
** @Return     : None
*******************************************************************************/
void BL_JumpToApplication(const BootParams_t* bootInfo);

/*******************************************************************************
** @Function   : BL_LoadImageToInetnalFlash
** @Description: Load the image from external flash to internal
** @Inputs     : @bootInfo
                 @updateInfo - the information about the image in external flash that needs to be copied
** @Outputs    : None
** @Return     : true if saved successfully, false if not
*******************************************************************************/
bool BL_LoadImageToInetnalFlash(const BootParams_t* bootInfo, FirmwareImageInfo_t* updateInfo);

#endif //BOOT_LOADER_H
