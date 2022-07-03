/**************************************************************************************
** @FileName: BootInfoHandler
** @Description: Handles managing the boot info 
** @DevelopmentEnviorment: Keil
** @Notes: Shared module to Main-Boot and Main-Program 
**************************************************************************************/
#ifndef BOOT_INFO_HANDLER_H
#define BOOT_INFO_HANDLER_H

/*==============================================================================
                                Includes
==============================================================================*/
#include <stdbool.h>
#include <stdint.h>
#include "FWU_GlobalDef.h"
/*==============================================================================
                        External/Public Function Protoypes
==============================================================================*/
/*******************************************************************************
** @Function   : BIH_InitBootInfo
** @Description: initialize the boot info struct with the data in the external flash
** @Inputs     : None
** @Outputs    : None
** @Return     : true - if successfull false - if not
*******************************************************************************/
bool BIH_InitBootInfo(void);

/*******************************************************************************
** @Function   : BIH_SaveBootInfo
** @Description: save the boot info to external flash
** @Inputs     : None
** @Outputs    : None
** @Return     : true if saved successfully, false if not
*******************************************************************************/
bool BIH_SaveBootInfo(void);

/*******************************************************************************
** @Function   : BIH_GetBootMode
** @Description: returns the boot mode of the console
** @Inputs     : None
** @Outputs    : None
** @Return     : The current boot mode
*******************************************************************************/
BootMode_t BIH_GetBootMode(void);

/*******************************************************************************
** @Function   : BIH_SetBootMode
** @Description: Set a new boot mode 
** @Inputs     : None
** @Outputs    : None
** @Return     : true if saved successfully, false if not
*******************************************************************************/
bool BIH_SetBootMode(BootMode_t bootMode);

/*******************************************************************************
** @Function   : BIH_GetBootComProtocolVersion
** @Description: return main boot communication protocol 
** @Inputs     : None
** @Outputs    : None
** @Return     : the version of the communication protocol
*******************************************************************************/
uint8_t BIH_GetBootComProtocolVersion(void);

/*******************************************************************************
** @Function   : BIH_SetBootComProtocolVersion
** @Description: set a new communication protocol
** @Inputs     : None
** @Outputs    : None
** @Return     : true if saved successfully, false if not
*******************************************************************************/
bool BIH_SetBootComProtocolVersion(uint8_t bootComProtocolVersion);

/*******************************************************************************
** @Function   : BIH_FindEmptySlotForImage
** @Description: find a place in memory where a new update can be saved
** @Inputs     : @imageDestinationDevice - The type of image(SI image/ Main Image)
** @Outputs    : None
** @Return     : FirmwareImageInfo_t with the information of the free space for the update
*******************************************************************************/
FirmwareImageInfo_t* BIH_FindEmptySlotForImage(FWUImageDevice_t imageDestinationDevice);

/*******************************************************************************
** @Function   : BIH_SetUpdateIomageToGolden
** @Description: change the images prevoiusly defiened as update and set it to golden. 
                 change the images prevoiusly defiened as Golden and set it to Old. 
** @Inputs     : None
** @Outputs    : None
** @Return     : true if new defenition saved successfully, false if not
*******************************************************************************/
bool BIH_SetUpdateIomageToGolden(void);

/*******************************************************************************
** @Function   : BIH_FindImage
** @Description: find the specified image type in memory
** @Inputs     : @imageType - The type of image( FWU_UPDATE/ FWU_GOLDEN/ FWU_OLD)
** @Outputs    : None
** @Return     : FirmwareImageInfo_t* with the data of the requested type. If can't find the image returns NULL.
*******************************************************************************/
FirmwareImageInfo_t* BIH_FindImage(FWUImage_t imageType);

#endif //BOOT_INFO_HANDLER_H
