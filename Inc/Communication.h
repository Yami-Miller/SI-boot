/**************************************************************************************
** FILE NAME:
** DESCRIPTION:
** DEVELOPMENT ENVIRONMENT:
** NOTES:
**************************************************************************************/
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#ifdef __cplusplus
extern "C" {
#endif
/*==============================================================================
==                              Includes
==============================================================================*/
#include "Globaldef.h"
#include <stdbool.h>
/*==============================================================================
==                              Defines
==============================================================================*/
#if (DEBUG)
#define ERR_PRINT(...)  printf("ERROR ! ! ! file: %s, func: %s , line: %d\n", __FILE__, __func__, __LINE__)
#else
#define ERR_PRINT(...)
#endif

#define COM_START_OF_MSG1         0xA5     // First byte header
#define COM_START_OF_MSG2         0x5A     // Second byte header
#define COM_RECEIV_TIMEOUT       100//500      // Receive timeout in Ms
#define COM_HEAD_OF_MESSAGE_LEN 2        // the head of message length
#define COM_SI_CheckSum_SIZE     2
#define MAGIC_EEPROM_NUMBER 0xDE, 0xAD, 0xBA, 0xBE
//
#define ACK_MESSAGE_LENGTH             2
#define SENSOR_DATA_RESPONSE_MSG_LEN   4

#define COM_SENSOR_DATA_RESPONSE_SIZE  19

#define BOOT_COMMANDS_BASE_OPCODE 50

/*==============================================================================
==                            Type Definitions
==============================================================================*/
/*==============================================================================
==                                Enums
==============================================================================*/

typedef enum __ProgramExecutionMode{
    BOOT_MODE,
    PROGRAM_MODE
}ProgramExecutionMode_t;

typedef enum
{
    SENSOR_NOT_DEFINED = 0xFF,
    SENSOR_DISATTACH   = 0x1
}sensor_disattach;


typedef enum AckResults{
    ACK_Ok = 1,
    ACK_BadCommunication,
    ACK_BadExecution,
    ACK_BadOpcode,
    ACK_TimeOut
} AckResults_t;

typedef enum
{
    MCP_CONNECTED = 0,
    MCP_DISCONNECTED
}COM_mcp_status;

/*=============================================================================
==                              Structures
==============================================================================*/
typedef void( *FuncPtr_t)( u8 *Data );
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
** @Function   : COM_CommunicationManagement
** @Description: This function manageing the communication between sensor
**               Interface and the console.
** @Inputs     : None
** @Outputs    : None
** @Return     : None
*******************************************************************************/
void COM_CommunicationManagement( void );
/*******************************************************************************
** @Function   : COM_ParseData
** @Description: Should be called from the uart callback. Parses the data received by uart to the desired msg fields.
** @Inputs     : None
** @Outputs    : None
** @Return     : None
*******************************************************************************/
void COM_ParseData( void );


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif //COMMUNICATION_H

