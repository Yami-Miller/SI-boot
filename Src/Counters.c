/**************************************************************************************
** FILE NAME:
**
** DESCRIPTION:
**
** DEVELOPMENT ENVIRONMENT:
**
** NOTES:
**************************************************************************************/


/*==============================================================================
==                               Includes
==============================================================================*/
#include "Counters.h"

/*==============================================================================
==                               Defines
==============================================================================*/


//
#define MS_IN_SECOND                1000
//
/*==============================================================================
==                             Type Definitions
==============================================================================*/

/*==============================================================================
==                                 Enums
==============================================================================*/

/*=============================================================================
==                               Structures
==============================================================================*/


/*==============================================================================
==                            Local/Private Constants
==============================================================================*/


/*==============================================================================
==                           Local/Private Variables
==============================================================================*/
static bool Is_CNT_SecondsActive[SECONDS_CNT_NUMBER] = {false};
static u32 CNT_SecondsInitVal[ SECONDS_CNT_NUMBER ] ;

static u32 CNT_MsInitVal[ MS_CNT_NUMBER ];

static u32 CNT_MsCounter = 0;
static u32 CNT_SecCounter = 0;
u16 SecondCount = 0;


/*==============================================================================
==                        Local/Private Function Protoypes
==============================================================================*/

/*==============================================================================
==                          External/Public Constants
==============================================================================*/


/*==============================================================================
==                          External/Public Variables
==============================================================================*/
u8 CNT_SecondTimersFree[ SECONDS_CNT_NUMBER ];
u8 CNT_MsTimersFree[ MS_CNT_NUMBER ];

/*==============================================================================
==                      External/Public Function Protoypes
==============================================================================*/

/*==============================================================================
==                           Function Definitions
==============================================================================*/
/*******************************************************************************
** FILE NAME:	    Counters.c
** FUNCTION NAME:  CNT_MsInterruptHandler
** DESCRIPTION:    Start one of SECONDS_TIMERS_NUMBER seconds counters
** INPUTS: 			u8 CounterNumber. Possible values 0...(SECONDS_TIMERS_NUMBER-1)
** OUTPUTS:			None
** RETURN:	  	    None
** NOTES:
*
*******************************************************************************/
void CNT_MsInterruptHandler( void )
{
   CNT_MsCounter ++;  // Mili Second increment
   //
   if( ++SecondCount >= MS_IN_SECOND )
   {
      CNT_SecCounter++;

      SecondCount = 0;
   }
}


/*******************************************************************************
** FILE NAME:	    Counters.c
** FUNCTION NAME:  get_SecCounter
** DESCRIPTION:    return the current seconds counter
** INPUTS:
** OUTPUTS:			None
** RETURN:	  	    seconds counter
** NOTES:
*
*******************************************************************************/
u32 Get_SecCounter(void)
{
	return CNT_SecCounter;
}
/*******************************************************************************
** FILE NAME:	   Counters.c
**
** FUNCTION NAME:  CNT_SecondsCntrStart
**
** DESCRIPTION:    Start one of SECONDS_TIMERS_NUMBER seconds counters
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
void CNT_SecondsCntrStart( u8 CounterNumber )
{
    CNT_SecondsInitVal[	CounterNumber ] = CNT_SecCounter;
    Is_CNT_SecondsActive[CounterNumber] = true;
}
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
void CNT_SecondsCntrStop( u8 CounterNumber )
{
    Is_CNT_SecondsActive[CounterNumber] = false;
}
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
bool IS_CNT_SencondsActive( u8 CounterNumber )
{
    return Is_CNT_SecondsActive[CounterNumber];
}

/*******************************************************************************
** FILE NAME:	   Counters.c
**
** FUNCTION NAME:  CNT_SecondsCntrGet
**
** DESCRIPTION:    Read the seconds counter timer
**
** INPUTS: 			u8 CounterNumber. Possible values 0...(SECONDS_TIMERS_NUMBER-1)
**
** OUTPUTS:			None
**
** RETURN:	  	    Counter value( seconds number from moment when timer starts).
**                  May be from 0 to SECONDS_IN_DAY-1
**
** NOTES:
**
*******************************************************************************/
u32 CNT_SecondsCntrGet( u8 CounterNumber )
{

	// SecondsCntrVal must be unsigned int because if CNT_SecCounter zeroed we want that the value will be still the diffrence
	// example: 0 - 255 = -255 in signed int
	//				0 - 255 = 1 in unsinged int
  u32 SecondsCntrVal ;

  SecondsCntrVal = CNT_SecCounter -CNT_SecondsInitVal[ CounterNumber ];

  return SecondsCntrVal;
}


/*******************************************************************************
** FILE NAME:	   Counters.c
**
** FUNCTION NAME:  CNT_MsStart
**
** DESCRIPTION:    Start one of MS_TIMERS_NUMBER ms counters
**
** INPUTS: 			u8 CounterNumber. Possible values 0...(MS_TIMERS_NUMBER-1)
**
** OUTPUTS:			None
**
** RETURN:	  	    None
**
** NOTES:
**
*******************************************************************************/
void CNT_MsStart( u8 TimerNo )
{
  CNT_MsInitVal[ TimerNo ] = CNT_MsCounter;
}

/*******************************************************************************
** FILE NAME:	   Counters.c
**
** FUNCTION NAME:  CNT_MsCntrGet
**
** DESCRIPTION:    Read the ms counter timer
**
** INPUTS: 			u8 CounterNumber. Possible values 0...(MS_TIMERS_NUMBER-1)
**
** OUTPUTS:			None
**
** RETURN:	  	    Counter value( miliseconds number from moment when timer starts).
**                  May be from 0 to USHRT_MAX/2-1  (counts up to 24 days)
**
** NOTES:
**
*******************************************************************************/
u32 CNT_MsCntrGet( u8 TimerNo )
{
  return CNT_MsCounter - CNT_MsInitVal[ TimerNo ];
}
