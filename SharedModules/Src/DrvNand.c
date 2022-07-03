#include "DrvNand.h"
#include "SIMemoryMap.h"


void getFileNameByAddress(uint32_t address, uint8_t* fileName, uint32_t *fileAddr);


/**
 * @brief Read Buffer
 * This function reads a buffer from a file in the flash memory.
 * @param address: The address for reading the buffer from there - the file name is calculated according to this address.
 * @param pData: Pointer to the data buffer read.
 * @param PacketSize: Number of bytes to read. 
 * @return HAL_StatusTypeDef: Success or fail.
 */
HAL_StatusTypeDef DAT_ReadBuffer(uint32_t address, uint8_t *pData, uint32_t PacketSize )
{
	uint8_t fileName[32] = "";
	uint32_t fileAddr, readResult;
	HAL_StatusTypeDef status = HAL_OK;
	char* p;
	
	getFileNameByAddress(address, fileName, &fileAddr);
	if(strlen((char*)fileName) == 0){
		PRINTF ("\nInvalid address %d!\n", address);
		return HAL_ERROR;	//invalid address
	}
	PRINTF("Reading from %s file\n", fileName);
	FILE* f = fopen((char*)fileName, "rb");
	if (f == NULL) {
		f = fopen((char*)fileName, "wb+");
	}
	if (f == NULL) {
		PRINTF ("\nCan not open %s file!\n", fileName);  /* error when trying to open file     */
		return HAL_ERROR;
	}
	
	if(fseek(f, address-fileAddr, SEEK_SET) != 0){
		PRINTF ("\nFailed moving pointer in %s\n", fileName);
		status = HAL_ERROR;
	}
		//p = fgets((char*)pData, PacketSize, f);
		readResult = fread(pData, PacketSize, 1, f);
	if(readResult != 1)
	{
		if(ferror(f))
		{
			printf("error reading\n");
			status = HAL_ERROR;
		}
		if(feof(f))
		{
			printf("end of file\n");
		}
	}
//	if(strlen((char*)pData) < PacketSize){	//or PacketSize-1?
//		PRINTF ("\nCan not read from %s file!\n", fileName);
//		status = HAL_ERROR;
//	}
	
	if(fclose(f) != 0){
		PRINTF ("\nCan not close %s file!\n", fileName);
		status = HAL_ERROR;
	}
	
	return status;
}

/**
 * @brief Write Buffer
 * This function writes a buffer to file in the flash memory.
 * @param address: The address for writing the buffer to there - the file name is calculated according to this address.
 * @param pData: Pointer to the data buffer write.
 * @param PacketSize: Number of bytes to write. 
 * @param append: If destroy the content. (true it means to appending content and not destroying)
 * @return HAL_StatusTypeDef: Success or fail.
 */
HAL_StatusTypeDef DAT_WriteBuffer(uint32_t address, uint8_t *pData, uint32_t PacketSize, bool append )
{
	uint8_t fileName[32] = "";
	uint32_t fileAddr, writeResult/*, offset=0*/;
	HAL_StatusTypeDef status = HAL_OK;
	FILE *f, *offset_f; 
	
	getFileNameByAddress(address, fileName, &fileAddr);
	if(strlen((char*)fileName) == 0){
		PRINTF ("\nInvalid address %d!\n", address);
		return HAL_ERROR;	//invalid address
	}
	PRINTF("Writing to %s file\n", fileName);
	if(append == true){
		/*'ab' just appends the data to the end of the file, even if set pos by seek function
		so we can't used that*/
		f = fopen((char*)fileName, "rb+");
		if (f == NULL) {
			f = fopen((char*)fileName, "wb");
		}
	}
	else{
		f = fopen((char*)fileName, "wb"); /*destroy the content*/
	}
	if (f == NULL) {
		PRINTF ("\nCan not open %s file!\n", fileName);  /* error when trying to open file     */
		return HAL_ERROR;
	}

	if(fseek(f, address-fileAddr, SEEK_SET) != 0){
		PRINTF ("\nFailed moving pointer in %s\n", fileName);
		status = HAL_ERROR;
	}
	else{ 
		writeResult = fwrite(pData, PacketSize, 1, f);
		if(writeResult == EOF || writeResult != 1){
			PRINTF ("\nCan not write properly to %s file!\n", fileName);
			status = HAL_ERROR;
		}
	}
	fflush(stdout);
	if(fclose(f) != 0){
		PRINTF ("\nCan not close %s file!\n", fileName);
		status = HAL_ERROR;
	}
	
	return status;
}

/**
 * @brief Erase File
 * This function removes all content of the given file
 * @param file_name: The name of the file to remove
 * @return true: Successed remove the file's content
 * @return false: Failed remove the file's content 
 */
HAL_StatusTypeDef DAT_EraseFile(char *file_name){
	FILE *f = fopen(file_name, "wb");/*Destroy the existing  content*/
	if(f == NULL)
	{
		printf ("\nCan not open file: %s!\n", file_name);  /* error when trying to open file     */
		return HAL_ERROR;
	}		
	if(fclose(f) != 0){
		PRINTF ("\nCan not close %s file!\n", file_name);
		return HAL_ERROR;
	}
	return HAL_OK;
}

/**
 * @brief Get Filename By Address
 * This function gives the fileName and fileAddr by a given address.
 * @param address: The address.
 * @param fileName: Pointer to the fileName. 
 * @param fileAddr: Pointer to the fileAddr. 
 */
void getFileNameByAddress(uint32_t address, uint8_t* fileName, uint32_t *fileAddr){
	if(address >= DAT_SI_PARAMETERS_ADDR && address < DAT_SI_PARAMETERS_ADDR+DAT_SI_PARAMETERS_SIZE)	{
		strcpy((char*)fileName, DAT_SI_PARAMETERS_FILE_NAME);
		*fileAddr = DAT_SI_PARAMETERS_ADDR;
	}
	else if(address >= DAT_Sensor_ADDR && address < DAT_Sensor_ADDR+DAT_Sensor_SIZE)	{
		strcpy((char*)fileName, DAT_SENSOR_FILE_NAME);
		*fileAddr = DAT_Sensor_ADDR;
	}
	else if(address >= DAT_PATEINT_INFO_ADDR && address < DAT_PATEINT_INFO_ADDR+DAT_PATIENT_INFO_SIZE)	{
		strcpy((char*)fileName, DAT_PATIENT_INFO_FILE_NAME);
		*fileAddr = DAT_PATEINT_INFO_ADDR;
	}
	else if(address >= DAT_PULSES_FLOW_ADDR && address < DAT_PULSES_FLOW_ADDR+DAT_PULSES_FLOW_SIZE)	{
		strcpy((char*)fileName, DAT_PULSES_FLOW_FILE_NAME);
		*fileAddr = DAT_PULSES_FLOW_ADDR;
	}
	else if(address >= DAT_STREAM_LOG_ADDR && address < DAT_STREAM_LOG_ADDR+DAT_STREAM_LOG_SIZE)	{
		strcpy((char*)fileName, DAT_STREAM_LOG_FILE_NAME);
		*fileAddr = DAT_STREAM_LOG_ADDR;
	}
	else if(address >= DAT_FW_IMAGES_INFO_ADDR && address < DAT_FW_IMAGES_INFO_ADDR+NVM_BLOCK_SIZE)	{
		strcpy((char*)fileName, DAT_FW_IMAGES_INFO_FILE_NAME);
		*fileAddr = DAT_FW_IMAGES_INFO_ADDR;
	}
	else if(address >= DAT_FW_IMAGE_ADDR && address < DAT_FW_IMAGE_ADDR+NVM_BLOCK_SIZE)	{
		strcpy((char*)fileName, DAT_MAIN_IMAGE_FILE_NAME);
		*fileAddr = DAT_FW_IMAGE_ADDR;
	}
	else if(address >= DAT_FW_IMAGE_ADDR + NVM_BLOCK_SIZE && address < DAT_FW_IMAGE_ADDR + NVM_BLOCK_SIZE*2)	{
		strcpy((char*)fileName, DAT_SI_IMAGE_FILE_NAME);
		*fileAddr = DAT_FW_IMAGE_ADDR + NVM_BLOCK_SIZE;
	}
	else{
		*fileName = '\0';
	}
}
