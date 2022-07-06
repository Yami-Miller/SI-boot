#include "MainManager.h"
#include "DrvUART.h"
#include "Communication.h"
#include <cmsis_os2.h>



/**
 * @brief MainManagerTask task
 * 
 * @param arg 
 */
void MainManagerTask(void *arg)
{
	(void)arg;
	
	init_filesystem();
  UART_StartCommunication();
	/* Infinite loop */
  
  while (1)
  {
		COM_CommunicationManagement();
  }
}



/**
 * @brief init file system and mount it
 * 
 */
void init_filesystem (void) {
  fsStatus stat;
	fsDriveInfo info; 

	NVM_WRITE_PROTECT();
	osDelay(10);
	NVM_WRITE_UNPROTECT();
	osDelay(100);

  printf ("Initializing and mounting enabled drives...\n\n");

	/* Initialize and mount drive "N0" */
  stat = finit ("N0:");
  if (stat == fsOK) {
    stat = fmount ("N0:");
    if (stat == fsOK) {
      printf ("Drive N0 ready!\n");
    }
    else if (stat == fsNoFileSystem) {
      /* Format the drive */
      printf ("Drive N0 not formatted!\n");
    }
    else {
      printf ("Drive N0 mount failed with error code %d\n", stat);
    }
  }
  else {
    printf ("Drive N0 initialization failed!\n");
  }
	
	
	finfo("N0:", &info);
	stat = fcheck("N0:");
	printf ("fcheck is %d\r\n", stat);
	printf ("FS ID read %08X\r\n",fs_ioc_get_id("N0:"));
  printf ("\nDone!\n");
}
