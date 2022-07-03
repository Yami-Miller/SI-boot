#ifndef SI_MEMORY_MAP_H
#define SI_MEMORY_MAP_H

//------------ Non Volatile Memory Map ---------------------------
//       Item                             Start Address

// Parameter of sensor interface
//using for flash signature:
#define DAT_SI_PARAMETERS_ADDR         	(0x00)
#define DAT_SI_PARAMETERS_SIZE         	(NVM_BLOCK_SIZE)

// Parameter of sensor  
#define DAT_Sensor_ADDR                 (DAT_SI_PARAMETERS_ADDR + DAT_SI_PARAMETERS_SIZE)
#define DAT_Sensor_SIZE                 (NVM_BLOCK_SIZE)

//Patient ID & start Monitoring time 
#define DAT_PATEINT_INFO_ADDR					  (DAT_Sensor_ADDR + DAT_Sensor_SIZE)
#define DAT_PATIENT_INFO_SIZE					  (NVM_BLOCK_SIZE)

#define DAT_PULSES_FLOW_ADDR            (DAT_PATEINT_INFO_ADDR + DAT_PATIENT_INFO_SIZE) 
#define DAT_PULSES_FLOW_SIZE            (NVM_BLOCK_SIZE)

// Patient Data Log                                
#define DAT_STREAM_LOG_ADDR            (DAT_PULSES_FLOW_ADDR + DAT_PULSES_FLOW_SIZE)
#define DAT_STREAM_LOG_SIZE            (NVM_BLOCK_SIZE * 2040)

#define DAT_FW_IMAGES_INFO_ADDR         (DAT_STREAM_LOG_ADDR + DAT_STREAM_LOG_SIZE)
#define DAT_FW_IMAGES_INFO_SIZE         (NVM_BLOCK_SIZE)
//Firmware update images
#define DAT_FW_IMAGE_ADDR               (DAT_FW_IMAGES_INFO_ADDR + DAT_FW_IMAGES_INFO_SIZE)
#define DAT_FW_IMAGE_SIZE               (NVM_BLOCK_SIZE * 2)
#define DAT_NUM_OF_FW_IMAGES            (2)

#define EXT_FLS_REPLACE_DEFECT_BLOCK_AREA_START_PLANE 1
#define EXT_FLS_REPLACE_DEFECT_BLOCK_AREA_START_BLOCK 1900

#define DAT_FW_IMAGES_INFO							DAT_FW_IMAGES_INFO_ADDR


#define DAT_SI_PARAMETERS_FILE_NAME 		"si_prm.bin"
#define DAT_SENSOR_FILE_NAME 						"sns_prm.bin"
#define DAT_PATIENT_INFO_FILE_NAME		 	"patient_inf.bin"
#define DAT_PULSES_FLOW_FILE_NAME 			"flw_pul_dat.bin"
#define DAT_STREAM_LOG_FILE_NAME 				"strm_log_dat.bin"
#define DAT_FW_IMAGES_INFO_FILE_NAME		"images_inf.bin"
#define DAT_MAIN_IMAGE_FILE_NAME 				"main_img.bin"
#define DAT_SI_IMAGE_FILE_NAME 					"si_img.bin"

#define CYCLIC_POINTER_FILE_NAME 				"cyclic_p.bin"

#endif //SI_MEMORY_MAP_H
