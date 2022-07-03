/*==============================================================================
                                Includes
==============================================================================*/
#include "CommonFunctions.h"

/*==============================================================================
                           API/Public Function Implementation
==============================================================================*/
inline uint32_t CF_Min( uint32_t a, uint32_t b ) 
{
    return a < b ? a : b; 
}

uint32_t CF_CalcBufferSum(uint8_t* buffer, uint32_t bufferSize)
{
    uint32_t tmpCsum = 0;
    for(uint32_t i = 0; i < bufferSize; i++ )
    {
        tmpCsum += buffer[i];
    }

    return tmpCsum;
}
