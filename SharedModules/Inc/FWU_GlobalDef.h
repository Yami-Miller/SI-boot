#ifndef FWU_GLOBAL_DEF_H
#define FWU_GLOBAL_DEF_H

#include <stdint.h>
/*==============================================================================
                                    Enums
==============================================================================*/
#define APP_ADDRESS                         (0x08008000)

typedef enum __BootMode{
    BOOT_NORMAL, //Just load the program.
    BOOT_UPDATE, //Update the program and the starts
    BOOT_GOLDEN, //Load the golden version
    BOOT_OLD,    //Load the old version
    BOOT_MODE_LAST //must be last
}BootMode_t;

typedef enum __FWUImage{
    FWU_UPDATE,
    FWU_GOLDEN,
    FWU_OLD,
    FWU_NO_IMAGE
}FWUImage_t;

typedef enum __FWUImageDevice{
    MAIN_IMAGE,
    SI_IMAGE,
}FWUImageDevice_t;

/*==============================================================================
                                Structures
==============================================================================*/  
#pragma pack(push, 1)
typedef struct __FirmwareImageInfo
{
    uint32_t m_checkSum;
    uint32_t m_imageAddress;
    uint32_t m_imageSize;
    FWUImage_t m_imageType;
}FirmwareImageInfo_t;
#pragma pack(pop)


#endif //FWU_GLOBAL_DEF_H

