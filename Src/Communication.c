/**************************************************************************************
** FILE NAME:
**
** DESCRIPTION:
**
** DEVELOPMENT ENVIRONMENT:
**
** NOTES:
**
** Functions : ------------------------------------
**             void COM_StartMonitoring( u8 *data );
**             void COM_StopMonitoring( u8 *data );
**             void COM_SensorDataRequest( u8 *data );
**             void COM_SetSensorData( u8 *data );
**             void COM_Status(void);
**             void COM_StatusRequest_ack( u8 *data );

**
****************************************************************************************/


/*==============================================================================
==                               Includes
==============================================================================*/
#include "Communication.h"
#include "DrvUART.h"
#include <string.h>
#include "boot_loader.h"
#include "FWUImagesInfoHandler.h"
#include "ComOpcodes.h"

/*==============================================================================
==                               Defines
==============================================================================*/
#define COM_BOOT_PROTOCOL_VERSION        1

#define COM_COMMAND_SIZE        60     // the long of command buffer
#define STATUS_RECEIVE_TIMEOUT  10
#define KEEP_ALIVE_TIMEOUT      30

/*==============================================================================
==                             Type Definitions
==============================================================================*/

/*==============================================================================
==                                 Enums
==============================================================================*/
enum{
    WaitForStart1 = 0,
    WaitForStart2,
    GettingOpcode,
    GettingLengthOfMsg,
    GettingBodyOfMsg,
    GettingChecksum
}ParseSteps;

/*=============================================================================
==                               Structures
==============================================================================*/

/*==============================================================================
==                        Local/Private Function Protoypes
==============================================================================*/
extern volatile bool    uart_transmit_done;

/*==============================================================================
==                           Function Definitions
==============================================================================*/
static void com_StartProgram( u8 *data );
static void com_ResetSI(u8* data);
static void com_PerformMessageRequest( u8 *command );
static bool com_TestCheckSum( u8 *data, u16 size );
static void com_PerformUpdate(u8* data);
static FuncPtr_t com_GetCommandToExecute(uint8_t opCode);
void com_InitComResponse(u8* data);
static void com_RollbackToGolden(u8* data);
void com_SendAck( u8 Op, u8 Res );
bool com_SendBuffer( u8 *buffer, u8 size );
/*==============================================================================
==                            Local/Private Constants
==============================================================================*/
const FuncPtr_t g_bootModeCommands[ ] =
{
/*  Method                              Opcode */
/*  ------------------------------------------ */
    com_InitComResponse, //50
    com_ResetSI, //51
    com_StartProgram, //52
    NULL,
    com_PerformUpdate, //53
    com_RollbackToGolden //54
};


/*==============================================================================
==                           Local/Private Variables
==============================================================================*/

// Indicate which step in parsing performed
u8 com_ParsingStep = WaitForStart1;

// Buffer of received command
u8 com_ReceivedCommand[COM_COMMAND_SIZE ];

// Counter to indicate the receive time out
u16 com_ReceivTimeout = 0;

// Counter of the recevied data
u16 com_ReceivedDataCounter = 0;

// Temp variant to hold the length of message
u8 com_TmpLenMsg;

// The received check sum
u8 com_ReceivedCheckSum;

u32 com_StatusIndex = 0;

u8 TransmitBuffer[255] = {0};

uint16_t temperature_out = 0;

// bool console_request_message = false;

/*==============================================================================
==                          External/Public Constants
==============================================================================*/

// The phrepheral that used to communicate with the EEPROM I2C2

extern uint16_t SI_FlowSum15Min;

extern BootParams_t g_siBootInfo;
/*==============================================================================
==                          External/Public Variables
==============================================================================*/
// Global flag to indicate when command was recevied
bool COM_flgCommandWasRecevied = false;
/*==============================================================================
==                      External/Public Function Protoypes
==============================================================================*/

/*******************************************************************************
** @Function   : COM_CommunicationManagement
** @Description: This function manageing the communication between sensor
**               Interface and the console.
** @Inputs     : None
** @Outputs    : None
** @Return     : None
*******************************************************************************/
void COM_CommunicationManagement( void )
{
    //reply back console side: if no transfer in progress:
    if(( COM_flgCommandWasRecevied == true ) && (uart_transmit_done == true))
    {        
        // clear the flag - enable to si send status:
        COM_flgCommandWasRecevied = false;

        // Execute the command
        com_PerformMessageRequest( com_ReceivedCommand );
    }
}

/*******************************************************************************
** @Function   : com_PerformMessageRequest
** @Description: This function performing the message request
** @Inputs     : opcode - message opcode
** @Outputs    : None
** @Return     : TRUE or FALSE
*******************************************************************************/
static void com_PerformMessageRequest( u8 *msg )
{
    bool Csum;
    FuncPtr_t commandToExecute = {0};
    
    Csum = com_TestCheckSum( com_ReceivedCommand, com_TmpLenMsg + COM_HEAD_OF_MESSAGE_LEN  );
    if( Csum == true )
    {
      if( *msg > 0 && *msg <= COM_LAST_OPCODE )
      {
				 printf("Get message: %d\n", (char)*msg);
         commandToExecute = com_GetCommandToExecute(*msg);
         if( commandToExecute != NULL )
         {
           commandToExecute( msg );

         }
         else
         {
            //Send NACK
            com_SendAck( *msg, ACK_BadOpcode );
         }
      }
      else
      {
         //Send NACK
         com_SendAck( *msg, ACK_BadOpcode );
      }
    }
    //else
    else
    {
        //Send NACK
        com_SendAck( *msg, ACK_BadCommunication );
    }
}
/*******************************************************************************
** @Function   : COM_ParseData
** @Description: This function parsing the received data in the buffer
** @Inputs     : None
** @Outputs    : None
** @Return     : None
*******************************************************************************/
void COM_ParseData( void )
{
   u8 tmpData;
   while( UART_IsDataDataAvailable() == true )
   {
      switch( com_ParsingStep )
      {
         case WaitForStart1:     // Wait for the first Header
         {
            tmpData = UART_GetCharFromRxBuffer();
            if( tmpData == COM_START_OF_MSG1 )
            {
               com_ReceivTimeout = COM_RECEIV_TIMEOUT;
               com_ParsingStep = WaitForStart2;
            }
         }
         break;
         case WaitForStart2:       // Wait for the second Header
         {
            tmpData = UART_GetCharFromRxBuffer();
            if( tmpData == COM_START_OF_MSG2 )
            {
               com_ParsingStep = GettingOpcode;
               com_ReceivedDataCounter = 0;
            }
            else
            {
               com_ParsingStep = WaitForStart1;
               com_ReceivedDataCounter = 0;
            }
         }
         break;
         case GettingOpcode:       // Receiving the opcode
         {
            com_ReceivedCommand[ com_ReceivedDataCounter++ ] = UART_GetCharFromRxBuffer();
            com_ParsingStep =  GettingLengthOfMsg;
         }
         break;
         case GettingLengthOfMsg:   // Receiving the lenght of message
         {
            com_ReceivedCommand[ com_ReceivedDataCounter++ ] = com_TmpLenMsg = UART_GetCharFromRxBuffer();
            if(com_TmpLenMsg == 0)
            {
                com_ParsingStep = GettingChecksum;
            }
            else
            {
                com_ParsingStep = GettingBodyOfMsg;
            }
         }
         break;
         case GettingBodyOfMsg:     // Receiving the body of message
         {
            if( com_ReceivedDataCounter == (com_TmpLenMsg + COM_HEAD_OF_MESSAGE_LEN ))
            {
               com_ParsingStep = GettingChecksum;
            }
            else
            {
               com_ReceivedCommand[ com_ReceivedDataCounter++ ] = UART_GetCharFromRxBuffer();
            }
         }
         break;
         case GettingChecksum:     // Receiving the checksum
         {
            // Get the check sum
            com_ReceivedCheckSum = UART_GetCharFromRxBuffer();

            // command was recevied
            com_ParsingStep = WaitForStart1;       // Wait for the next command
            com_ReceivedDataCounter =0;            // reset the recevied data counter
            com_ReceivTimeout = 0;                 // Reset the timeout timer
             
            COM_flgCommandWasRecevied = true;      // Set the flag that command received
         }
         break;

         default:
         {
             ERR_PRINT();
         }
         break;
      }
   }

}
/*******************************************************************************
** @Function   : COM_CalcCheckSum
** @Description: This function calculate the checksum of the buffer
** @Inputs     : data - data to calculate the checksum
**               size   - the size of the data
** @Outputs    : None
** @Return     : The calculated checksum
*******************************************************************************/
u8 COM_CalcCheckSum( u8 const *data, u16 size )
{
    u16 index;
    u8 tmpCsum = 0;
    for( index = 0; index < size; index++ )
    {
        tmpCsum += data[index];
    }

    return (u8)( tmpCsum );
}
/*******************************************************************************
** @Function   : com_TestCheckSum
** @Description: This function the chechsum in the buffer
** @Inputs     : data - data to calculate the checksum
**               size   - the size of the data
** @Outputs    : None
** @Return     : The calculated checksum
*******************************************************************************/
static bool com_TestCheckSum( u8 *data, u16 size )
{
   u8 calcCsum;
   calcCsum = COM_CalcCheckSum( data, size);
   if( calcCsum == com_ReceivedCheckSum /* data[size]*/ )
   {
      return true;
   }
   else
   {
      return false;
   }
}

/*******************************************************************************
** @Function   : com_ClearCommand
** @Description: This function clear the command from the buffer
** @Inputs     : buffer - data to send
**               size   - the size of the data to send in bytes
** @Outputs    : None
** @Return     : TRUE or FALSE
*******************************************************************************/
void com_ClearCommand( void )
{
    memset( com_ReceivedCommand, 0, COM_COMMAND_SIZE );
}
/*******************************************************************************
** @Function   : com_SendAck
** @Description: This function clear the command from the buffer
** @Inputs     : buffer - data to send
**               size   - the size of the data to send in bytes
** @Outputs    : None
** @Return     : TRUE or FALSE
*******************************************************************************/
void com_SendAck( u8 Op, u8 Res )
{
    AckMsg  response;
    bool result;
    //--- perpare the Message ---------------------
    response.Msg.Opcode = COM_ACK_OPCODE;
    response.Msg.Length = sizeof(AckMsg)- COM_HEAD_OF_MESSAGE_LEN;
    response.Msg.AckOpcode = Op;
    response.Msg.AckRes = Res;
    //-----------------------------------------------
    result = com_SendBuffer( response.Data, sizeof(AckMsg) );
    if (result == false)
    {
        ERR_PRINT();
    }
}

/*******************************************************************************
** @Function   : com_SendBuffer
** @Description: This function sending data buffer by the RS232 communication
**               to console.
** @Inputs     : buffer - data to send
**               size   - the size of the data to send in bytes
** @Outputs    : None
** @Return     : TRUE or FALSE
*******************************************************************************/
bool com_SendBuffer( u8 *buffer, u8 size )
{
    u8 *tx = TransmitBuffer;
    u8 checkSum;
    //--- Send the header ----------------------------------------
   tx[0] = COM_START_OF_MSG1;
   tx[1] = COM_START_OF_MSG2;
    //--- Send the Message ---------------------------------------
   memcpy(&tx[2], buffer, size );
    //--- Send the Check sum -------------------------------------
   checkSum = COM_CalcCheckSum( buffer, size );
   tx[ 2 + size] = checkSum;
  
   return UART_SendBuffer( tx, 3 + size );

}

/*******************************************************************************
** @Function   : COM_GetDateAndTime
** @Description:
** @Inputs     : data - data command
** @Outputs    : None
*******************************************************************************/
static void com_StartProgram( u8 *data )
{
    PRINTF("Recived Start Program from main\n");
    
    com_SendAck( *data, ACK_Ok );

	/*wait for ack to transmit to console before reset SI*/
 		osDelay(100);
		
    PRINTF("Recived Start Program from main\n");
    BL_JumpToApplication(&g_siBootInfo);
}

/*******************************************************************************
** @Function   : com_ResetSI
** @Description:
** @Inputs     :
** @Outputs    : None
*******************************************************************************/
static void com_ResetSI(u8* data)
{
    PRINTF("reset SI recived\n");
    com_SendAck( *data, ACK_Ok );

    /*wait for acknoledge to transmit to console before reset SI*/
    HAL_Delay(100);
    //reset si:
    HAL_NVIC_SystemReset();
}

static void com_PerformUpdate(u8* data)
{
    bool status = false;
    AckResults_t response = ACK_Ok;
    FirmwareImageInfo_t* imageInfo = NULL;
    
    status = IMGH_InitFWUImagesInfo();
    if(!status)
    {
        com_SendAck(*data, ACK_BadExecution);
        return;
    }
    
    imageInfo = IMGH_FindImage(FWU_UPDATE);
    if(imageInfo == NULL)
    {
        com_SendAck(*data, ACK_BadExecution);
        return;
    }
    
    status = BL_LoadImageToInetnalFlash(&g_siBootInfo, imageInfo);
    if(!status)
    {
        //rollback to golden
        imageInfo = IMGH_FindImage(FWU_GOLDEN);
        status = BL_LoadImageToInetnalFlash(&g_siBootInfo, imageInfo);
        response = ACK_BadExecution;
        PRINTF("Failed to load update image to flash\n");
    }
    else
    {
        PRINTF("Loaded update image to flash\n");
    }
    com_SendAck(*data, response);
}

static void com_RollbackToGolden(u8* data)
{
    bool status = false;
    AckResults_t response = ACK_Ok;
    FirmwareImageInfo_t* goldenimageInfo = NULL;
    
    status = IMGH_InitFWUImagesInfo();
    if(!status)
    {
        response = ACK_BadExecution;
        com_SendAck(*data, response);
        return;
    }
    
    goldenimageInfo = IMGH_FindImage(FWU_GOLDEN);
    if(goldenimageInfo == NULL)
    {
        response = ACK_BadExecution;
        com_SendAck(*data, response);
        return;
    }
    
    status = BL_LoadImageToInetnalFlash(&g_siBootInfo, goldenimageInfo);
    if(!status)
    {
        response = ACK_BadExecution;
    }
    
    com_SendAck(*data, response);
}

void com_InitComResponse(u8* data)
{
    KeepAliveMsg_t request = {0};
    KeepAliveMsg_t response = {0};
    bool status;

    memcpy( request.m_data, data, sizeof( KeepAliveMsg_t ));
    
    //TODO: add check if diffrent versions
    
    response.m_msg.m_opcode = COM_KeepAlive;
    response.m_msg.m_length = sizeof(KeepAliveMsg_t) - COM_HEAD_OF_MESSAGE_LEN;
    response.m_msg.m_programModeVersion = 0;
    response.m_msg.m_bootModeVersion = COM_BOOT_PROTOCOL_VERSION;
    response.m_msg.m_programExecutionMode = BOOT_MODE;
    
    status = com_SendBuffer( response.m_data, sizeof( KeepAliveMsg_t ) );
    if (status == false)
    {
        ERR_PRINT();
    }
}

static FuncPtr_t com_GetCommandToExecute(uint8_t opCode)
{
    if(opCode >= BOOT_COMMANDS_BASE_OPCODE)
    {
        return g_bootModeCommands[opCode - BOOT_COMMANDS_BASE_OPCODE];
    }
    return NULL;
}

