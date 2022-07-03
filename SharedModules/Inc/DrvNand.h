/**************************************************************************************
**
** FILE NAME:  DrvNand.h
**
** PURPOSE: External NAND Flash driver definition and functions declaration
**
** FUNCTION(S):
**     Global:
**     Function Name           - Description
**
** DESCRIPTION:
**
** DEVELOPMENT ENVIRONMENT:
**
** NOTES:
**
**************************************************************************************/
#ifndef NAND_DRIVER_H
#define NAND_DRIVER_H

/*==============================================================================
==                              Includes
==============================================================================*/
#include "Globaldef.h"
#include "stm32f4xx_hal.h"

/*==============================================================================
==                              Defines
==============================================================================*/
#define NAND_4G_DEVICE     1

#define NAND_DEV        &hnand1
#define NAND_WP_GPIO    GPIOD, GPIO_PIN_3

#define ADDR_5TH_CYCLE(__ADDRESS__)       (uint8_t)((__ADDRESS__) >> 32)      /* 5th addressing cycle */

#define NVM_PAGE_SIZE         2048                    // page size
#define NVM_NUM_OF_PAGES_IN_A_BLOCK 64
#define NVM_BLOCK_SIZE (NVM_PAGE_SIZE * NVM_NUM_OF_PAGES_IN_A_BLOCK) // block size = page size * 64 pages

#ifdef NAND_4G_DEVICE
#define NUM_OF_BLOCKS_IN_PLANE 2048 // plane size = block size * 2048 blocks
#else
#define NUM_OF_BLOCKS_IN_PLANE 1024 // plane size = block size * 1024 blocks
#endif

#define NVM_PLANE_SIZE        (NVM_BLOCK_SIZE * NUM_OF_BLOCKS_IN_PLANE)  // plane size = block size * 1024 blocks
#define NVM_PLANE_TOTAL (2)
#define NVM_NAND_SIZE (NVM_PLANE_SIZE * NVM_PLANE_TOTAL)

#define NVM_ADDRESS_CYCLES    5
#define NAND_CMD_READ_ID2     0x65

#define NVM_COL_ADDR_1        0
#define NVM_COL_ADDR_2        1
#define NVM_ROW_ADDR_1        2
#define NVM_ROW_ADDR_2        3
#define NVM_ROW_ADDR_3        4


#define NVM_WRITE_PROTECT()     HAL_GPIO_WritePin( NAND_WP_GPIO, GPIO_PIN_RESET )
#define NVM_WRITE_UNPROTECT()   HAL_GPIO_WritePin( NAND_WP_GPIO, GPIO_PIN_SET )

/*==============================================================================
==                            Type Definitions
==============================================================================*/
typedef struct
{
  /*<! NAND memory electronic signature maker and device IDs */

  uint8_t Maker_Id;
  uint8_t Device_Id;
  uint8_t Third_Id;
  uint8_t Fourth_Id;
  uint8_t Fiveth_Id;
}NAND_IDType;

typedef struct
{
   uint32_t pageSize;
   uint32_t blockSize;
   uint32_t planeSize;
}NandSizesType;
/*==============================================================================
==                                Enums
==============================================================================*/
/*=============================================================================
==                              Structures
==============================================================================*/
/*==============================================================================
==                          External/Public Constants
==============================================================================*/
/*==============================================================================
==                          External/Public Variables
==============================================================================*/
/*==============================================================================
==                      External/Public Function Protoypes
==============================================================================*/
/**
 * @brief Write Buffer
 * This function writes a buffer to file in the flash memory.
 * @param address: The address for writing the buffer to there - the file name is calculated according to this address.
 * @param pData: Pointer to the data buffer write.
 * @param PacketSize: Number of bytes to write. 
 * @param append: If destroy the content. (true it means to appending content and not destroying)
 * @return HAL_StatusTypeDef: Success or fail.
 */
HAL_StatusTypeDef DAT_WriteBuffer(uint32_t address, uint8_t *pData, uint32_t PacketSize, bool append );
/**
 * @brief Read Buffer
 * This function reads a buffer from a file in the flash memory.
 * @param address: The address for reading the buffer from there - the file name is calculated according to this address.
 * @param pData: Pointer to the data buffer read.
 * @param PacketSize: Number of bytes to read. 
 * @return HAL_StatusTypeDef: Success or fail.
 */
HAL_StatusTypeDef DAT_ReadBuffer(uint32_t address, uint8_t *pData, uint32_t PacketSize );
/**
 * @brief Erase File
 * This function removes all content of the given file
 * @param file_name: The name of the file to remove
 * @return true: Successed remove the file's content
 * @return false: Failed remove the file's content 
 */
HAL_StatusTypeDef DAT_EraseFile(char *file_name);

#endif //NAND_DRIVER_H


