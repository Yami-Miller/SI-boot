/**************************************************************************************
** FILE NAME:
** DESCRIPTION:
** DEVELOPMENT ENVIRONMENT:
** NOTES:
**************************************************************************************/
#ifndef GLOBAL_DEF_H
#define GLOBAL_DEF_H

/*==============================================================================
==                              Includes
==============================================================================*/
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "string.h"
/*==============================================================================
==                              Defines
==============================================================================*/

#define SHORT_PULSE_5_5     /* Comment when use 120/30 algorithmm currently FW uses pulse 5/5 algorithm */
// Version             107

#ifdef USE_ERASE_EXT_FLASH
    #define SI_VERSION      99999
#else
    #ifdef USE_ADJUST_CALIBRATION_FOR_USA
        #define SI_VERSION      20211
    #else
        #define SI_VERSION      20214
    #endif // USE_ADJUST_CALIBRATION_FOR_USA
#endif  // USE_ERASE_EXT_FLASH

#define FIRMWARE_YEAR ((((__DATE__ [7] - '0') * 10 + (__DATE__ [8] - '0')) * 10 \
+ (__DATE__ [9] - '0')) * 10 + (__DATE__ [10] - '0'))

#define FIRMWARE_MONTH ( 1 + (__DATE__ [2] == 'n' ? (__DATE__ [1] == 'a' ? 0 : 5) \
: __DATE__ [2] == 'b' ? 1 \
: __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 2 : 3) \
: __DATE__ [2] == 'y' ? 4 \
: __DATE__ [2] == 'l' ? 6 \
: __DATE__ [2] == 'g' ? 7 \
: __DATE__ [2] == 'p' ? 8 \
: __DATE__ [2] == 't' ? 9 \
: __DATE__ [2] == 'v' ? 10 : 11))

#define FIRMWARE_DAY ((__DATE__ [4] == ' ' ? 0 : __DATE__ [4] - '0') * 10 \
+ (__DATE__ [5] - '0'))

#define SI_FW_DATE              ((FIRMWARE_DAY << 24) | (FIRMWARE_MONTH << 16) | FIRMWARE_YEAR)



// Enable Elments
//#define CHECKSUM_ENABLE 1
//#define DEBUG_PRINT_EN  1

#ifndef NULL
#define NULL ((void *)0)
#endif

#define IN
#define INOUT
#define OUT

#if DEBUG
#define ERR_PRINT(...)  printf("ERROR ! ! ! file: %s, func: %s , line: %d\n", __FILE__, __func__, __LINE__)
#else
#define ERR_PRINT(...)
#endif

#define SAMPLING_DATA_LEN     30
#define PATIENT_ID_SIZE       16
#define SI_SENSOR_ID_EEPROM_SIZE  7 // Note: this constant is identical to SI_SENSOR_ID_EE_SIZE defined in DrvSensorEprom.h
#define SENSOR_CALIBRATIN_DATA_SIZE 20

//#define PULSE_PERIOD_IN_SEC       90 //1.5 minutes
#define PULSE_PERIOD_IN_SEC     150 //2.5 minutes

#define SAMPLES_IN_PULSE        (PULSE_PERIOD_IN_SEC*30)

#if PULSE_PERIOD_IN_SEC == 90 // 1.5 minutes

#define ON_PULSE_TIME         30   // seconds
#define OFF_PULSE_TIME        60   // Seconds
#define PULSE_PACKETS_SIZE    25

#elif PULSE_PERIOD_IN_SEC == 150 // 2.5 minutes

#define ON_PULSE_TIME         120   // seconds
#define OFF_PULSE_TIME        30   // Seconds
#define PULSE_PACKETS_SIZE    42
#endif

#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#if DEBUG
    #define ENABLE_WATCHDOG     0
#else
    #define ENABLE_WATCHDOG     1
#endif
#define WATCHDOG_TIMEOUT        10 //seconds


#define PULSE_DATA_IN_PACKET  108
#define SENSOR_CONN_MASK               0x01
#define DATA_START            2
// Macros
#define MyStrCmp(s1,s2)      strcmp((char*)s1, (char*)s2)
typedef enum _pulseStatus
{
    FLOW_OK = 0,
    AIR_IN_SENSOR = 1,
    SENSOR_ERROR = 2,
    HEATER_ERROR = 3
}E_pulseStatus;

/*==============================================================================
==                            Type Definitions
==============================================================================*/
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

/*==============================================================================
==                                Enums
==============================================================================*/
// status enum
typedef enum _STATUS
{
    TRUE = 0,
   FALSE = 1
}STATUS;

//
typedef enum{
   NOTE_None = 0,
   NOTE_SensorAttached,
   NOTE_SensorConnected,
   NOTE_SensorDetached,
   NOTE_SensorNotConnected, /*at startup*/
   NOTE_FlowReady
}G_NotifcationsType;
/*=============================================================================
==                              Structures
==============================================================================*/
#pragma pack(1)
//--- Ack response message -----------------------
struct _AckMsg{
    u8  Opcode;
    u8  Length;
    u8  AckOpcode;
    u8  AckRes;
};
typedef union{
    struct _AckMsg  Msg;
    u8                  Data[ sizeof( struct _AckMsg ) ];
}AckMsg;



//--- whitebox message -----------------------
struct _WhiteBoxMsg{
    u8  Opcode;
    u8  Length;
    u8  whiteboxCommand;
};
typedef union{
    struct _WhiteBoxMsg Msg;
    u8                  Data[ sizeof( struct _WhiteBoxMsg ) ];
}WhiteBoxMsg;


//--- reset si message -----------------------
struct _ResetSiMsg{
    u8      Opcode;
    u8      Length;
    bool    ToEraseFlash;
};
typedef union{
    struct _ResetSiMsg     Msg;
    u8                      Data[ sizeof( struct _ResetSiMsg ) ];
}ResetSiMsg;


//--- Start Monitoring Message -----------------------
struct _StartMonitorMsg{
    u8  Opcode;
    u8  Length;
    u8  PatientId[PATIENT_ID_SIZE];
    u16     PatientWeight;
   u32    PatientBDate;
   u32   MonStartTime;
   u32   MonStartDate;
     u8     MonPulseDef;
};
typedef union{
    struct _StartMonitorMsg     Msg;
    u8                           Data[ sizeof( struct _StartMonitorMsg ) ];
}StartMonitorMsg;

//----set date and time ----
struct _DateAndTimeMsg{
    u32 shortTime;
    u32 shortDate;
};
typedef union{
    struct _DateAndTimeMsg  Msg;
    u8                           Data[ sizeof( struct _DateAndTimeMsg ) ];
} DateAndTimeMsg;

//--- set patient Message -----------------------
struct _SetPatientMsg{
    u8  Opcode;
    u8  Length;
    u8  PatientId[PATIENT_ID_SIZE];
    u16 SensorExpiration;     // used to be PatientWeight;
   u32    PatientBDate;
};
typedef union{
    struct _SetPatientMsg   Msg;
    u8                           Data[ sizeof( struct _SetPatientMsg ) ];
}SetPatientMsg;

//--- Sensor data response Message -----------------------
struct _SensorDataResponseMsg{
    u8  Opcode;
    u8  Length;
    u32     sID;
    u32     StartUseTime;
    u32     StartUseDate;
};
typedef union{
    struct _SensorDataResponseMsg   Msg;
    u8                                 Data[ sizeof( struct _SensorDataResponseMsg ) ];
}SensorDataResponseMsg;

//--- Set sensor data Message -----------------------
struct _SetSensorDataMsg{
    u8  Opcode;
    u8  Length;
    u8  Param;
    u32     Value;
};
typedef union{
    struct _SetSensorDataMsg    Msg;
    u8                           Data[ sizeof( struct _SetSensorDataMsg ) ];
}SetSensorDataMsg;

//--- Status data Response Message -----------------------
struct _StatusResponseMsg{
    u8  Opcode;
    u8  Length;
   u32   index;
    u8  status;
   u32    Error;
   u8    Notification;
   u32   PulseIndex;
   u16   CoreTemp;
   u16   UrineFlow;
   u32   expire_date;
    u8   SensorVer;
};
typedef union{
    struct _StatusResponseMsg   Msg;
    u8                           Data[ sizeof( struct _StatusResponseMsg ) ];
}StatusResponseMsg;

//--- Version response Message -----------------------
struct _VerResponseMsg{
    u8  Opcode;
    u8  Length;
    u32 FwVersion;
    u32 SiSerialNumber;
    u32 FwDate;
};
typedef union{
    struct _VerResponseMsg  Msg;
    u8                           Data[ sizeof( struct _VerResponseMsg ) ];
}VerResponseMsg;

typedef union
{
   uint16_t value;
   uint8_t  data[ sizeof(uint16_t)];
}Uint16Union;
typedef union
{
   uint32_t value;
   uint8_t  data[ sizeof(uint32_t)];
}Uint32Union;

//--- Get patient data log ----------------------------
struct _PatientDataLogRespopnse{
    u8  Opcode;
    u8  Length;
   u32   StartDate;
   u32   StartTime;
    u32   EndDate;
   u32   EndTime;
   u8    PatientID[PATIENT_ID_SIZE];
    u16 NumOfSamplesPerPulse;
   u32   NumOfPulses;
};
typedef union{
   struct   _PatientDataLogRespopnse Msg;
   u8       Data[ sizeof( struct _PatientDataLogRespopnse)];
}PatientDataLogRespopnse;
//--- Pulse Patient data log response ---------------------
struct _PatientPulseDataReq{
   u8   Opcode;
   u8   Length;
   u32   PulseNum;
   u8    PacketNum;
};
typedef union{
   struct _PatientPulseDataReq   Msg;
   u8                         Data[sizeof(struct _PatientPulseDataReq)];
}PatientPulseDataReq;

//--- Pulse Patient data log response ---------------------
struct _PatientPulseData{
   u8   Opcode;
   u8   Length;
   u32   PulseNum;
   u8    PacketNum;
   u16   PulseData[PULSE_DATA_IN_PACKET];
};
typedef union{
   struct _PatientPulseData   Msg;
   u8                         Data[sizeof(struct _PatientPulseData)];
}PatientPulseData;

struct _startUse{
      u16 year;
      u8  month;
      u8  day;
      u8  hour;
      u8  minute;
   };

typedef union
{
   struct _startUse FirstUse;
   u8 data[ sizeof(struct _startUse)];
}SI_StartUseDate;

//--- Sensor data response --------------------------------
struct _SensorDataResponse{
    u8                  Opcode;
    u8                  Length;
    u8                  SensorID[SI_SENSOR_ID_EEPROM_SIZE + 1];
    u8                  SensorVer;
    u8                  SKSpecialFlag;
    u8                  SKSpecialFlagParameter;
    SI_StartUseDate     StartUseDate;
    u32                 MnufDate;
    uint8_t             SensorCalibData[SENSOR_CALIBRATIN_DATA_SIZE]; //20
};
typedef union{
   struct _SensorDataResponse Msg;
   u8                         Data[sizeof(struct _SensorDataResponse)];
}SensorDataResponse;

typedef union
{
   struct _startHnadshake{
      uint8_t Opcode;
      uint8_t Length;
      SI_StartUseDate dateTime;
   }msg;
   uint8_t data[sizeof(struct _startHnadshake)];
}MsgStartHandShake;

typedef union
{
   struct MsgCheckSum_{
      uint8_t Opcode;
      uint8_t Length;
      uint16_t checksum;
   }msg;
   uint8_t data[sizeof(struct MsgCheckSum_)];
}MsgCheckSum;

typedef struct _LastPulseAndFlow
{
        uint16_t flow;
        uint32_t isAlreadyReset;
        uint32_t validPulsesIndex;
        uint32_t pulsesIndex;
        uint32_t date;
        uint32_t time;
        uint32_t magicNumber; /*must be last parameter in structure to ensure erasing block if adding parameter.
                                currently please pay attention to change NVM_SYSTEM_SIGNATURE in main also if adding parameter*/
} LastPulseAndFlow;

//--- Mcp status -----------------------
struct _McpConnectionStatus{
    u8  Opcode;
    u8  Length;
    u8      ConnectionStatus;
};
typedef union{
    struct _McpConnectionStatus     Msg;
    u8                           Data[ sizeof( struct _McpConnectionStatus ) ];
}McpConnectionStatus;

//--- pulse info request  ----------------------
struct _PulseInfoReq{
   u8   Opcode;
   u8   Length;
   u32   PulseNum;
};
typedef union{
   struct _PulseInfoReq   Msg;
   u8                         Data[sizeof(struct _PulseInfoReq)];
}PulseInfoReq;

//--- heater & stick value Message -----------------------
struct _HeaterAndStickResponseMsg{
    u8      Opcode;
    u8      Length;
    u16     heaterValue;
    float   stickValue;

};
typedef union{
    struct _HeaterAndStickResponseMsg   Msg;
    u8                           Data[ sizeof( struct _HeaterAndStickResponseMsg ) ];
}HeaterAndStickResponseMsg;

struct _LastFlowParamsResponseMsg{
    u8      Opcode;
    u8      Length;
    u16     flow;
    u32     validIndex;
    u32     date;
    u32     time;
};

typedef union{
    struct _LastFlowParamsResponseMsg   Msg;
    u8                           Data[ sizeof( struct _LastFlowParamsResponseMsg ) ];
}LastFlowParamsResponseMsg;

//--- system log resonse msg -----------------------
struct _SystemLogResonseMsg_t{
    #define SYSTEM_LOG_MAX_DATA_SIZE 200
    u8      Opcode;
    u8      Length;
    char    logBuffer[SYSTEM_LOG_MAX_DATA_SIZE];
};
typedef union{
    struct _SystemLogResonseMsg_t   Msg;
    u8                           Data[ sizeof( struct _SystemLogResonseMsg_t ) ];
}SystemLogResonseMsg_t;
//--- Pulse Info response ---------------------
struct _PulseInfo{
   u8       Opcode;
   u8       Length;
   uint32_t PulseNum;
    uint32_t    StartDate;
    uint32_t    StartTime;
    uint32_t EndDate;
    uint32_t EndTime;
    uint8_t SensorId[SI_SENSOR_ID_EEPROM_SIZE + 1];
    float       SensorCalibK;
    float   SensorCalibAlphaNus;
    float   SensorCalibAlphaNus2;
    float       SensorCalibR0;
    float   SensorCalibRc;
    uint8_t PatientId[PATIENT_ID_SIZE];
};
typedef union{
   struct _PulseInfo   Msg;
   u8                  Data[sizeof(struct _PulseInfo)];
}PulseInfo;

//--- data managment Pulse Info ---------------------
struct _DAT_PulseInfo{
   uint32_t PulseNum;
    uint32_t    StartDate;
    uint32_t    StartTime;
    uint32_t EndDate;
    uint32_t EndTime;
    uint8_t SensorId[SI_SENSOR_ID_EEPROM_SIZE + 1];
    float       SensorCalibK;
    float   SensorCalibAlphaNus;
    float   SensorCalibAlphaNus2;
    float       SensorCalibR0;
    float   SensorCalibRc;
    uint8_t PatientId[PATIENT_ID_SIZE];
};
typedef union{
   struct _DAT_PulseInfo   Msg;
   u8                  Data[sizeof(struct _DAT_PulseInfo)];
}DAT_PulseInfo;

struct _KeepAliveMsg{
    uint8_t m_opcode;
    uint8_t m_length;
    uint8_t m_bootModeVersion; //protcol version of boot
    uint8_t m_programModeVersion; //protcol version of program
    uint8_t m_programExecutionMode; //execution mode should be a ProgramExecutionMode_t; 
};
typedef union{
    struct _KeepAliveMsg m_msg;
    u8 m_data[ sizeof( struct _KeepAliveMsg) ];
} KeepAliveMsg_t;

#pragma pack(4)
/*==============================================================================
==                          External/Public Constants
==============================================================================*/
/*==============================================================================
==                          External/Public Variables
==============================================================================*/
/*==============================================================================
==                      External/Public Function Protoypes
==============================================================================*/
#endif //GLOBAL_DEF_H

