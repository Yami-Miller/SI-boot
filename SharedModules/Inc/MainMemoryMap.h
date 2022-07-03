#ifndef MAIN_MEMORY_MAP_H
#define MAIN_MEMORY_MAP_H

//------------ Non Volatile Memory Map ---------------------------
//       Item                             Start Address
// Internal use and test                        // Sector 0
#define DM_SIGNITURE_ADDRESS                    0
#define DM_SIGNITURE_SIZE                       sizeof(uint16_t)

#define DM_NVM_TEST_ADDRESS                     (DM_SIGNITURE_ADDRESS + DM_SIGNITURE_SIZE)
#define DM_NVM_TEST_SIZE                        1

// Parameter of the console
#define DM_SYSTEM_PARAMETERS_ADDR               0
#define DM_SYSTEM_PARAMETERS_SIZE               (NVM_SECTOR_SIZE * 2)

// GUI Languages header       // start from sector 1
#define DM_LANGUAGES_PARAMS_ADDR                (DM_SYSTEM_PARAMETERS_ADDR + DM_SYSTEM_PARAMETERS_SIZE)
#define DM_LANGUAGES_PARMS_SIZE                 (NVM_SECTOR_SIZE)

#define DM_HL7_PARAMS_ADDR                      (DM_LANGUAGES_PARAMS_ADDR + DM_LANGUAGES_PARMS_SIZE)
#define DM_HL7_PARAMS_SIZE                      (NVM_SECTOR_SIZE)

// saline
#define DM_SALINE_ADDR                          (DM_HL7_PARAMS_ADDR + DM_HL7_PARAMS_SIZE)
#define DM_SALINE_SIZE                          (NVM_SECTOR_SIZE)

// System events and actions log
#define DM_SYSTEM_LOG_ADDR                      (DM_SALINE_ADDR + DM_SALINE_SIZE)
#define DM_SYSTEM_LOG_SIZE                      ((((256 * NVM_SECTOR_SIZE - DM_SYSTEM_LOG_ADDR) / 2)/(NVM_SECTOR_SIZE))*(NVM_SECTOR_SIZE)) // DG: just write 125!!!  -- 125 sector (each sector is 0x40000)

#define DM_USAGE_LOG_ADDR                       (DM_SYSTEM_LOG_ADDR + DM_SYSTEM_LOG_SIZE) //must be start of sector
#define DM_USAGE_LOG_SIZE                       (256 * NVM_SECTOR_SIZE - DM_USAGE_LOG_ADDR) //30.5Mb = 126 sector (each sector is 0x40000) // DG: just write 126!!!

#define DM_PATIENT_FLOW_LOG_ADDR                (0x0000)
#define DM_PATIENT_FLOW_LOG_SIZE                (NVM_SECTOR_SIZE * 203)

//Firmware Update
#define DM_MAIN_UPDATE_BASE_ADDRESS             (DM_PATIENT_FLOW_LOG_ADDR + DM_PATIENT_FLOW_LOG_SIZE)
#define DM_MAIN_MAX_IMAGE_SIZE                  (NVM_SECTOR_SIZE * 4)
#define DM_MAIN_IMAGES_IN_FLASH                 (2)
#define DM_SIZE_OF_MAIN_IMAGES_AREA             (DM_MAIN_MAX_IMAGE_SIZE * DM_MAIN_IMAGES_IN_FLASH)

#define DM_SI_UPDATE_ADDRESS                    (DM_MAIN_UPDATE_BASE_ADDRESS + DM_SIZE_OF_MAIN_IMAGES_AREA)
#define DM_SI_IMAGES_IN_FLASH                   (1)
#define DM_SI_MAX_IMAGE_SIZE                    (NVM_SECTOR_SIZE)

#define DM_BOOT_INFO_ADDRESS                    (DM_SI_UPDATE_ADDRESS + DM_SI_MAX_IMAGE_SIZE)
#define DM_BOOT_INFO_SIZE                       (NVM_SECTOR_SIZE)

//multilanguage map

//language size (per lagnuage):
#define DM_LANGUAGE_SIZE                        (NVM_SECTOR_SIZE * 2)
//keyboard size (per lagnuage):
#define DM_KEYBOARD_SIZE                        (NVM_SECTOR_SIZE * 2)

//language first sector:
#define DM_START_LANGUAGES_ADDRESS              (DM_BOOT_INFO_ADDRESS + DM_BOOT_INFO_SIZE)   // strings sector  0
#define DM_START_KEYBOARD_ADDRESS               (DM_START_LANGUAGES_ADDRESS + NVM_SECTOR_SIZE)  // keyboard layout 0

//|---------------------|
//|  strings sector  0  |
//|----------------------
//|  keyboard layout 0  |
//|----------------------
//|  strings sector  1  |
//|----------------------
//|  keyboard layout 1  |
//|----------------------
// ...
//|----------------------
//|  strings sector  n  |
//|----------------------
//|  keyboard layout n  |
//|----------------------

//font start sector:
#define DM_START_UNICODE_FONSTS_ADDRESS         (DM_START_LANGUAGES_ADDRESS + NUM_OF_LANGUAGES * DM_LANGUAGE_SIZE)

// font parameters:
#define MAX_CACHED_CHAR_UNICODE                 (0xff - 0x20)
#define MAX_CACHED_BITMAP_CHAR                  (1)


//x mean - number of sector
// size of small font = NVM_SECTOR_SIZE / 6 (42Kb)
// size of medium font = NVM_SECTOR_SIZE / 3 (84KB)
// size of large font = NVM_SECTOR_SIZE / 2 (128KB)
// in each sector: small_font adress is in start of sector
//                       medium_font adress is in start of (sector + (1/6)sector)
//                       large font adress is in start of (sector + (1/2) sector)
#define DM_FONT_OFFSET_TABLE_ADDR(sector, fontSize)         (DM_START_UNICODE_FONSTS_ADDRESS + \
                                                                                (sector) * NVM_SECTOR_SIZE + \
                                                                                ((fontSize == FONT_SMALL) ? 0 : \
                                                                                ((fontSize == FONT_MEDIUM) ? (((NVM_SECTOR_SIZE / 6) / NVM_PAGE_SIZE) * NVM_PAGE_SIZE) \
                                                                                : (NVM_SECTOR_SIZE / 2) ) ) )
#define DM_FONT_OFFSET_TABLE_SIZE                                   (0x2000) // size = 8kB
#define DM_FONT_WIDTH_TABLE_ADDR(sector, fontSize)              (DM_FONT_OFFSET_TABLE_ADDR(sector, fontSize) + DM_FONT_OFFSET_TABLE_SIZE)
#define DM_FONT_WIDTH_TABLE_SIZE                                        (0x1000) // size = 4kB
#define DM_FONT_DATA_TABLE_ADDR(sector, fontSize)               (DM_FONT_WIDTH_TABLE_ADDR(sector, fontSize) + DM_FONT_WIDTH_TABLE_SIZE)


#endif //MAIN_MEMORY_MAP_H

