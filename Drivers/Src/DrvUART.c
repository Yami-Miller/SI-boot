/**************************************************************************************
** FILE NAME:                 DrvUART.c
**
** DESCRIPTION:               UART Communication Driver
**
** DEVELOPMENT ENVIRONMENT:   Keil
**
** NOTES:
**************************************************************************************/


/*==============================================================================
==                               Includes
==============================================================================*/
#include "DrvUART.h"
#include "Communication.h"
#include "Counters.h"
//#include "OPFW.h"
/*==============================================================================
==                               Defines
==============================================================================*/
#define BYTE_SIZE 1
#define UART_HEAD_LEN 4
#define UART_MSG_LEN_INDEX 3
#define UART_TIME_OUT_TO_RECEIVE 800 // Ms
#define UART_TIME_OUT_TO_SEND    1000 // Ms
#define UART_CHECK_SUM_LEN       1
#define UART_ATTEMPTS            3
#define UART_TEMP_BUFFER_SIZE    50

#define UART_CH            huart3

#define UART_MAX_TRANSMIT_RETRY 10
/*==============================================================================
==                             Type Definitions
==============================================================================*/

/*==============================================================================
==                                 Enums
==============================================================================*/

/*=============================================================================
==                               Structures
==============================================================================*/
typedef union
{
   uint16_t Var;
   uint8_t data[sizeof(uint16_t)];
}TwoBytes;

/*==============================================================================
==                            Local/Private Constants
==============================================================================*/

/*==============================================================================
==                           Local/Private Variables
==============================================================================*/

struct buf_st_r rbuf = { 0, 0, };
#define SIO_RBUFLEN ((unsigned short)(rbuf.in - rbuf.out))

struct buf_st_t tbuf = { 0, 0, };
#define SIO_TBUFLEN ((unsigned short)(tbuf.in - tbuf.out))

unsigned int tx_restart = 1;               // NZ if TX restart is required

uint8_t uart_TempBuffer[UART_TEMP_BUFFER_SIZE];

bool uart_ContinueRecMsg = false;
uint8_t uart_SizeMsgToRec = 0;

volatile HAL_StatusTypeDef new_receive_is_busy = HAL_OK;
volatile bool   uart_transmit_done = true;

/*==============================================================================
==                        Local/Private Function Protoypes
==============================================================================*/
static void uart_BufferInit(void);

/*==============================================================================
==                          External/Public Constants
==============================================================================*/

/*==============================================================================
==                          External/Public Variables
==============================================================================*/
extern bool com_flgReceiveTimeout;
extern UART_HandleTypeDef huart3;

/*==============================================================================
==                      External/Public Function Protoypes
==============================================================================*/

/*==============================================================================
==                           Function Definitions
==============================================================================*/

u8 UART_GetCharFromRxBuffer(void)
{
    struct buf_st_r *p = &rbuf;
    return (u8)(p->buf [(p->out++) & (RBUF_SIZE - 1)]);
}

bool UART_AddByteToRxBuffer(u8 data)
{
    struct buf_st_r *Rx = &rbuf;

    if ( ((Rx->in - Rx->out) != RBUF_SIZE))
    {
        Rx->buf [Rx->in & (RBUF_SIZE-1)] = data;
        Rx->in++;
        return true;
    }
    else
    {
        return false;
    }
}

bool UART_SendBuffer (uint8_t  *data, uint16_t size)
{
    uint8_t number_of_uart_transmit_rtry = 5;
    HAL_StatusTypeDef Err = HAL_OK;
    uart_transmit_done = false;//new transfer - flag for transfer done.

    do
    {
        Err = HAL_UART_Transmit_IT( &UART_CH, data, size);

        if( number_of_uart_transmit_rtry >= UART_MAX_TRANSMIT_RETRY)
        {
            PRINTF("\n----------------------------\n");
            PRINTF("!!! uart error trnasfer !!!\n");
            PRINTF("----------------------------\n");

            return false;
        }

        number_of_uart_transmit_rtry++;
    }
    while(Err != HAL_OK);

    CNT_SecondsCntrStart( SECONDS_CNT_TRANSMIT_DONE );
    while(new_receive_is_busy)//if receive fail
    {   //restart receive
        new_receive_is_busy = HAL_UART_Receive_IT( &UART_CH, uart_TempBuffer, 1 );

        if(CNT_SecondsCntrGet( SECONDS_CNT_TRANSMIT_DONE ) > 5 )
        {
            PRINTF("\n----------------------------\n");
            PRINTF("!!! ERRO uart  transmit  !!!\n");
            PRINTF("----------------------------\n");
            return false;
        }
    }

    //wait for transmit to end:
    CNT_SecondsCntrStart( SECONDS_CNT_TRANSMIT_DONE );
    do
    {
        if(CNT_SecondsCntrGet( SECONDS_CNT_TRANSMIT_DONE ) > 8 )
        {
            PRINTF("\n----------------------------\n");
            PRINTF("!!! ERRO uart  transmit  !!!\n");
            PRINTF("----------------------------\n");
            break;
        }
    }
    while(uart_transmit_done == false);

    return true;

}
/*******************************************************************************
** FUNCTION NAME:   UART_DataReiceved
** DESCRIPTION:   Return if there is data in the Receive buffer
** INPUTS:          None
** OUTPUTS:         None
** RETURN:          Received Char
*******************************************************************************/
u8 UART_IsDataDataAvailable( void )
{
    if( SIO_RBUFLEN > 0 ){
        return 1;
    }else{
        return 0;
    }
}

/*******************************************************************************
** @Function   : UART_StartCommunication
** @Description:
** @Inputs     :
** @Outputs    :
*******************************************************************************/
void UART_StartCommunication( void )
{
   HAL_StatusTypeDef status;
    
   status = HAL_UART_Receive_IT( &UART_CH, uart_TempBuffer, 1);
   if (status == HAL_ERROR)
   {
       ERR_PRINT();
   }

   uart_BufferInit();

}
/*******************************************************************************
** @Function   : HAL_UART_RxCpltCallback
** @Description:
** @Inputs     :
** @Outputs    :
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
   if( huart == &UART_CH )
   {
        bool result = UART_AddByteToRxBuffer( *uart_TempBuffer );
           
        if (result != true)
        {
            ERR_PRINT("UART AddByteToRxBuffer failed. *uart_TempBuffer=0x%02X\n", *uart_TempBuffer);
        }

        *uart_TempBuffer = 0;
        COM_ParseData();
        new_receive_is_busy = HAL_UART_Receive_IT( &UART_CH, uart_TempBuffer, 1 );
   }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    uart_transmit_done = true;
}
/*******************************************************************************
** @Function   : HAL_UART_ErrorCallback
** @Description:
** @Inputs     :
** @Outputs    :
*******************************************************************************/
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
   HAL_StatusTypeDef status;
   if( huart == &UART_CH )
   {
      //HAL_UART_Receive_IT( &UART_CH, uart_TempBuffer, UART_HEAD_LEN );
      status = HAL_UART_Receive_IT( &UART_CH, uart_TempBuffer, 1 );
      if (status == HAL_ERROR)
      {
          ERR_PRINT();
      }

   }
}

/*******************************************************************************
** FUNCTION NAME:   UART_buffer_Init
** DESCRIPTION:   Initialize the USART RX / TX buffers
** INPUTS:          None
** OUTPUTS:         None
** RETURN:          None
*******************************************************************************/
static void uart_BufferInit(void)
{
    tbuf.in = 0;                                        // Clear com buffer indexes
    tbuf.out = 0;
    tx_restart = 1;
    rbuf.in = 0;                                        // Clear com buffer indexes
    rbuf.out = 0;
}
