/**************************************************************************************
** FILE NAME: CommonFunctions.h
** DESCRIPTION: common genral functions for use across the project
** DEVELOPMENT ENVIRONMENT: Keil
** NOTES:
**************************************************************************************/
#ifndef COMMON_FUNCTION_H
#define COMMON_FUNCTION_H

/*==============================================================================
                                Includes
==============================================================================*/
#include <stdbool.h>
#include <stdint.h>

/*==============================================================================
                                MACROS
==============================================================================*/
#if (DEBUG)

#define start_timer()       *((volatile uint32_t*)0xE0001000) = 0x40000001  // Enable CYCCNT register
#define stop_timer()        *((volatile uint32_t*)0xE0001000) = 0x40000000  // Disable CYCCNT register
#define get_timer()         *((volatile uint32_t*)0xE0001004)               // Get value from CYCCNT register
    
#define DECLARE_TIMING(s)  uint32_t timeStart_##s; uint32_t timeDiff_##s; uint32_t timeTally_##s = 0; uint32_t countTally_##s = 0
#define EXTERN_TIMING(s)  extern uint32_t timeStart_##s; extern uint32_t timeDiff_##s; extern uint32_t timeTally_##s; extern uint32_t countTally_##s
#define START_TIMING(s)    timeStart_##s = get_timer(); start_timer();
#define STOP_TIMING(s)     timeDiff_##s = get_timer() - timeStart_##s ;stop_timer(); timeTally_##s += timeDiff_##s; countTally_##s++
#define GET_TIMING(s)      timeDiff_##s
#define GET_AVERAGE_TIMING(s)   timeTally_##s/ countTally_##s
#define CLEAR_AVERAGE_TIMING(s) timeTally_##s = 0; countTally_##s = 0

#endif //(DEBUG)

/*==============================================================================
                        External/Public Function Protoypes
==============================================================================*/
/*******************************************************************************
** @Function   : CF_Min
** @Description: returns the smaller number betweeen a and b
** @Inputs     : @a
                 @b
** @Outputs    : None
** @Return     : the samller number
*******************************************************************************/
uint32_t CF_Min( uint32_t a, uint32_t b );

/*******************************************************************************
** @Function   : CF_CalcBufferSum
** @Description: Returns the sum of the values in the buffer
** @Inputs     : @buffer - array of uint8_t 
                 @bufferSize - size of the array
** @Outputs    : None
** @Return     : the sum of the values in the array
*******************************************************************************/
uint32_t CF_CalcBufferSum(uint8_t* buffer, uint32_t bufferSize);

#endif //COMMON_FUNCTION_H
