/**************************************************************************************
** FILE NAME:
** DESCRIPTION:
** DEVELOPMENT ENVIRONMENT:
** NOTES:
**************************************************************************************/
#ifndef COUNTERS_H
#define COUNTERS_H

/*==============================================================================
==                              Includes
==============================================================================*/
//#include "main.h"
#include "Globaldef.h"
/*==============================================================================
==                              Defines
==============================================================================*/

typedef enum
{
    SECONDS_CNT_1,
    SECONDS_CNT_STATUS_RECEIVE,
    SECONDS_CNT_TRANSMIT_DONE,
    SECONDS_CNT_KEEP_ALIVE,
    SECONDS_CNT_HOT_SENSOR_DELAY,
    SECONDS_CNT_NUMBER //must be last
} e_s_counter;
typedef enum
{
    MS_CNT_1     = 0,
    MS_CNT_I2C,
    MS_CNT_ADC_TIMEOUT,
    MS_CNT_CHECK_SENSOR,
    MS_CNT_SET_VOLT,
    MS_CNT_ADC2_CHANNEL_READY,
    MS_CNT_TEMPERATURE_READ,
    MS_CNT_HIGH_PRIORITY_CHECK,
    MS_CNT_NUMBER //must be last
} e_ms_counter;


#define SECONDS_CNT_NOT_RUN   0xFFFFFFFF

#define TIMER_FREE 0
#define TIMER_BUSY (!TIMER_FREE)

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
** FILE NAME:       Counters.c
** FUNCTION NAME:  CNT_MsInterruptHandler
** DESCRIPTION:    Start one of SECONDS_TIMERS_NUMBER seconds counters
** INPUTS:          u8 CounterNumber. Possible values 0...(SECONDS_TIMERS_NUMBER-1)
** OUTPUTS:         None
** RETURN:          None
** NOTES:
*
*******************************************************************************/
void CNT_MsInterruptHandler( void );



/*******************************************************************************
** FILE NAME:       Counters.c
** FUNCTION NAME:  get_SecCounter
** DESCRIPTION:    return the current seconds counter
** INPUTS:
** OUTPUTS:         None
** RETURN:          seconds counter
** NOTES:
*
*******************************************************************************/
u32 Get_SecCounter(void);

/*******************************************************************************
** FILE NAME:      Counters.c
**
** FUNCTION NAME:  CNT_SecondsCntrStart
**
** DESCRIPTION:    Start one of SECONDS_TIMERS_NUMBER seconds counters
**
** INPUTS:          u8 CounterNumber. Possible values 0...(SECONDS_TIMERS_NUMBER-1)
**
** OUTPUTS:         None
**
** RETURN:          None
**
** NOTES:
**
*******************************************************************************/
void CNT_SecondsCntrStart( u8 CounterNumber );

/*******************************************************************************
** FILE NAME:	   Counters.c
**
** FUNCTION NAME:  CNT_SecondsCntrStop
**
** DESCRIPTION:    Stop one of SECONDS_TIMERS_NUMBER seconds counters
**
** INPUTS: 			u8 CounterNumber. Possible values 0...(SECONDS_TIMERS_NUMBER-1)
**
** OUTPUTS:			None
**
** RETURN:	  	    None
**
** NOTES:
**
*******************************************************************************/
void CNT_SecondsCntrStop( u8 CounterNumber );

/*******************************************************************************
** FILE NAME:      Counters.c
**
** FUNCTION NAME:  IS_CNT_sencondsActive
**
** DESCRIPTION:    check if the counter is active
**
** INPUTS:          u8 CounterNumber. Possible values 0...(SECONDS_TIMERS_NUMBER-1)
**
** OUTPUTS:         None
**
** RETURN:          true if active, false if not
**
** NOTES:
**
*******************************************************************************/
bool IS_CNT_SencondsActive( u8 CounterNumber );

/*******************************************************************************
** FILE NAME:      Counters.c
**
** FUNCTION NAME:  CNT_SecondsCntrGet
**
** DESCRIPTION:    Read the seconds counter timer
**
** INPUTS:          u8 CounterNumber. Possible values 0...(SECONDS_TIMERS_NUMBER-1)
**
** OUTPUTS:         None
**
** RETURN:          Counter value( seconds number from moment when timer starts).
**                  May be from 0 to SECONDS_IN_DAY-1
**
** NOTES:
**
*******************************************************************************/
u32 CNT_SecondsCntrGet( u8 CounterNumber );

/*******************************************************************************
** FILE NAME:      Counters.c
**
** FUNCTION NAME:  CNT_MsInit
**
** DESCRIPTION:    Start Timer2 as ms counter
**
** INPUTS:          u8 CounterNumber. Possible values 0...(MS_TIMERS_NUMBER-1)
**
** OUTPUTS:         None
**
** RETURN:          None
**
** NOTES:           This function is calling once when progam starts run. User never calls it.
**
*******************************************************************************/
void CNT_MsInit( void );

/*******************************************************************************
** FILE NAME:      Counters.c
**
** FUNCTION NAME:  CNT_MsStart
**
** DESCRIPTION:    Start one of MS_TIMERS_NUMBER ms counters
**
** INPUTS:          u8 CounterNumber. Possible values 0...(MS_TIMERS_NUMBER-1)
**
** OUTPUTS:         None
**
** RETURN:          None
**
** NOTES:
**
*******************************************************************************/
void CNT_MsStart( u8 TimerNo );

/*******************************************************************************
** FILE NAME:      Counters.c
**
** FUNCTION NAME:  CNT_MsCntrGet
**
** DESCRIPTION:    Read the ms counter timer
**
** INPUTS:          u8 CounterNumber. Possible values 0...(MS_TIMERS_NUMBER-1)
**
** OUTPUTS:         None
**
** RETURN:          Counter value( miliseconds number from moment when timer starts).
**                  May be from 0 to USHRT_MAX/2-1  (counts up to 32.768s)
**
** NOTES:
**
*******************************************************************************/
u32 CNT_MsCntrGet( u8 TimerNo );

#endif //COUNTERS_H

