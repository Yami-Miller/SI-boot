#ifndef COM_OPCODES_H
#define COM_OPCODES_H

// Messages enumerator define
typedef enum _MessageOpcode
{
    COM_ACK_OPCODE                                  = 1,
    COM_START_MONITORING_OPCODE                     = 2,
    COM_STOP_MONITORING_OPCODE                      = 3,
    COM_PERIODIC_STATUS_OPCODE                      = 4,
    COM_FW_VERSION_OPCODE                           = 5,
    COM_PATIENT_DATA_LOG_OPCODE                     = 6,
    COM_GET_PULSE_DATA_LOG_PACKET_OPCODE            = 7,
    COM_SENSOR_DATA_REQUEST_OPCODE                  = 8,
    COM_CLEAR_PATIENT_DATA_OPCODE                   = 9,
    COM_CLEAR_NOTE_OPCODE                           = 10,
    COM_START_HANDSHAKE_OPCODE                      = 11,
    COM_SHUT_DOWN_OPCODE                            = 12,
    COM_SET_THERMISTOR_VOLT_OPCODE                  = 13,
    COM_GET_LAST_THERM_VOLT_OPCODE                  = 14,
    COM_RESUME_MONITORING_OPCODE                    = 15,
    COM_RESUME_MONITORING_New_Sensor_OPCODE         = 16,
    COM_REQUST_ZERO_FLOW_OPCODE                     = 17,
    COM_SET_MCP_CONNECTION_STATUS                   = 18,
    COM_SET_PATIENT_DATA                            = 19,
    COM_SI_Set_DateAndTime_OPCODE                   = 20,
    COM_GET_PULSE_INFO                              = 21,
    COM_GET_HEATER_AND_STICK                        = 22,
    COM_SAVE_SENSOR_DETAILES_AND_PATIENT_ID_OPCODE  = 23,
    COM_CLEAR_ALL_OPCODE                            = 24,
    COM_WHITE_BOX_OPCODE                            = 25,
    COM_GET_SYSTEM_LOG                              = 26,
    COM_GET_LAST_FLOW_PARAMS                        = 27,
    COM_SI_Get_CheckSum_OPCODE                      = 28,

    COM_KeepAlive                                   = 50,
    COM_RESET_SI_OPCODE                             = 51,
    COM_START_PROGRAM                               = 52,
    COM_FW_IMAGE_DATA                               = 53,//28
    COM_PERFORM_UPDATE                              = 54,//53
    COM_ROLLBACK_TO_GOLDEN                          = 55,//54
    COM_SET_CURRENT_VERSION_AS_GOLDEN               = 56,//55
    COM_FW_PREPARE_FOR_UPDATE                       = 57, //30
    COM_FW_IMAGE_TRANSMIT_DONE                      = 58, //29

    COM_LAST_OPCODE
}MessageOpcode_t;


#endif //COM_OPCODES_H

