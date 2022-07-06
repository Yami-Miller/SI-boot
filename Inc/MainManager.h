#ifndef MAIN_MANAGER_H
#define MAIN_MANAGER_H

#include "DrvNand.h"
#include "rl_fs.h"                      // Keil.MDK-Pro::File System:CORE


/**
 * @brief MainManagerTask task
 * 
 * @param arg 
 */
void MainManagerTask(void *arg);
/**
 * @brief init file system and mount it
 * 
 */
void init_filesystem (void); 

#endif
