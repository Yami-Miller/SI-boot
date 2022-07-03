/**************************************************************************************
** FILE NAME:                 DrvUART.h
** DESCRIPTION:               Header file For UART communication
** DEVELOPMENT ENVIRONMENT:   Keil
** NOTES:
**************************************************************************************/
#ifndef DRV_UART_H
#define DRV_UART_H

/*==============================================================================
==                              Includes
==============================================================================*/
#include "Globaldef.h"
#include "stm32f4xx_hal.h"

/*==============================================================================
==                              Defines
==============================================================================*/
#define TBUF_SIZE   256        /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/
#define RBUF_SIZE   256         /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/

/*----------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
#if TBUF_SIZE < 2
#error TBUF_SIZE is too small.  It must be larger than 1.
#elif ((TBUF_SIZE & (TBUF_SIZE-1)) != 0)
#error TBUF_SIZE must be a power of 2.
#endif

#if RBUF_SIZE < 2
#error RBUF_SIZE is too small.  It must be larger than 1.
#elif ((RBUF_SIZE & (RBUF_SIZE-1)) != 0)
#error RBUF_SIZE must be a power of 2.
#endif

/*----------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
struct buf_st_r
{
  volatile u8 in;                                // Next In Index
  volatile u8 out;                               // Next Out Index
  u8 buf [RBUF_SIZE];                   // Buffer
};

struct buf_st_t
{
  volatile u16 in;                                // Next In Index
  volatile u16 out;                               // Next Out Index
  u8 buf [TBUF_SIZE];                   // Buffer
};
/*==============================================================================
==                            Type Definitions
==============================================================================*/
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
/*******************************************************************************
** FUNCTION NAME:   USART1_ReceiveChar
** DESCRIPTION:     Receive a character by USART
** INPUTS:          None
** OUTPUTS:         None
** RETURN:          Received Char
*******************************************************************************/
u8 UART_GetCharFromRxBuffer(void);
/*******************************************************************************
** FUNCTION NAME:   UART_AddByteToRxBuffer
** DESCRIPTION:   Adding byte to Rx buffer
** INPUTS:          None
** OUTPUTS:         None
** RETURN:          Received Char
*******************************************************************************/
bool UART_AddByteToRxBuffer(u8 data) ;
/*******************************************************************************
** FUNCTION NAME:   UART_SendBuffer
** DESCRIPTION:   Transmit a buffer data by USART communication
** INPUTS:          char c - char to send to UART
** OUTPUTS:         None
** RETURN:          true if success false if fail
*******************************************************************************/
bool UART_SendBuffer (uint8_t  *data, uint16_t size);
/*******************************************************************************
** FUNCTION NAME:   UART_DataReiceved
** DESCRIPTION:   Return if there is data in the Receive buffer
** INPUTS:          None
** OUTPUTS:         None
** RETURN:          Received Char
*******************************************************************************/
u8 UART_IsDataDataAvailable( void );
/*******************************************************************************
** FUNCTION NAME:   UART_SendChar
** DESCRIPTION:     Transmit a character by USART communication
** INPUTS:          char c - char to send to UART
** OUTPUTS:         None
** RETURN:          true if success false if fail
*******************************************************************************/
bool UART_SendChar (uint8_t  c);
/*******************************************************************************
** @Function   : UART_StartCommunication
** @Description:
** @Inputs     :
** @Outputs    :
*******************************************************************************/
void UART_StartCommunication( void );
/*******************************************************************************
** @Function   : UART_DataProcessing
** @Description: Processing Receiving data after interrupt
** @Inputs     :
** @Outputs    :
*******************************************************************************/
//void UART_DataProcessing(void);

#endif //DRV_UART_H

