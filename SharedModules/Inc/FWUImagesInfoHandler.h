/**************************************************************************************
** @FileName: BootInfoHandler.h
** @Description: Handles managing the images info
** @DevelopmentEnviorment: Keil
** @Notes: Shared module to SI-Boot and SI-Program 
**************************************************************************************/
#ifndef IMAGES_INFO_HANDLER_H
#define IMAGES_INFO_HANDLER_H

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
** @Function   : IMGH_InitFWUImagesInfo
** @Description: initialize the images info struct with the data in the external flash
** @Inputs     : None
** @Outputs    : None
** @Return     : true - if successfull false - if not
*******************************************************************************/
bool IMGH_InitFWUImagesInfo(void);

/*******************************************************************************
** @Function   : IMGH_SaveImagestInfo
** @Description: save the images info to external flash
** @Inputs     : None
** @Outputs    : None
** @Return     : true if saved successfully, false if not
*******************************************************************************/
bool IMGH_SaveImagestInfo(void);

/*******************************************************************************
** @Function   : IMGH_FindEmptySlotForImage
** @Description: find a place in memory where a new update can be saved
** @Inputs     : None
** @Outputs    : None
** @Return     : FirmwareImageInfo_t with the information of the free space for the update
*******************************************************************************/
FirmwareImageInfo_t* IMGH_FindEmptySlotForImage(void);

/*******************************************************************************
** @Function   : IMGH_SetUpdateIomageToGolden
** @Description: change the images prevoiusly defiened as update and set it to golden. 
                 change the images prevoiusly defiened as Golden and set it to Old. 
** @Inputs     : None
** @Outputs    : None
** @Return     : true if new defenition saved successfully, false if not
*******************************************************************************/
bool IMGH_SetUpdateIomageToGolden(void);

/*******************************************************************************
** @Function   : IMGH_FindImage
** @Description: find the specified image type in memory
** @Inputs     : @imageType - The type of image( FWU_UPDATE/ FWU_GOLDEN/ FWU_OLD)
** @Outputs    : None
** @Return     : FirmwareImageInfo_t* with the data of the requested type. If can't find the image returns NULL.
*******************************************************************************/
FirmwareImageInfo_t* IMGH_FindImage(FWUImage_t imageType);

#endif //IMAGES_INFO_HANDLER_H
