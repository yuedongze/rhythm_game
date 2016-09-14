#include "map.h"
#include "start.h"
#include "controller.h"
#include "definition.h"

static char buf[BUFFER_SIZE];

static FATFS g_fileSystem; /* File system object */
static FIL g_fileObject;   /* File object */ 
static char buf[BUFFER_SIZE];

int data[DATA_SIZE];
int data_len=0;

void poll_SDinsert(void){
	PRINTF("Please insert a card into board.\r\n");
	while (!(GPIO_ReadPinInput(BOARD_SDHC_CD_GPIO_BASE, BOARD_SDHC_CD_GPIO_PIN)))
    {
    }
		PRINTF("Detected SD card inserted.\r\n");
}


void open_file(TCHAR* filename){
	const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
	
		f_mount(&g_fileSystem, driverNumberBuffer, 0U);
		f_chdrive((char const *)&driverNumberBuffer[0U]);
	PRINTF("Opening File: %s\r\n", filename);
		f_open(&g_fileObject, filename, (FA_WRITE | FA_READ));
}

void resetptr(void){
	f_lseek(&g_fileObject, 0U);
}

int read_data(int b[4]){
	char* buffer;
	int i = 0;
	
		f_lseek(&g_fileObject, f_tell(&g_fileObject));
		if (f_eof(&g_fileObject)) return 1;
				
		f_gets(buf, BUFFER_SIZE, &g_fileObject);
		buffer = strtok(buf, ",");
		while (buffer) {
			b[i] = atol(buffer);
			buffer = strtok(NULL, ",");
			while (buffer && *buffer == '\040') buffer++;
			i++;
		}
		free(buffer);
	return 0;
}


void close_file(void){
		f_close(&g_fileObject);
}

void load_map_to_buffer(TCHAR* filename){
	int b[4] = {0,0,0,0};
	
	open_file(filename);
	
	PRINTF("Loading game data to the local buffer...\r\n");
	
	while(!read_data(b)){
		if (data_len<DATA_SIZE){
			data[data_len] = b[0];
			data[data_len+1] = b[1];
			data[data_len+2] = b[2];
			data[data_len+3] = b[3];
			data_len += 4;
		}
	}
	PRINTF("Loading complete...\r\n");
	close_file();
}

int f_browse(void){
	int state = 0; // default init state, 1 is active state, 2 is reloading state, 3 is end
	DIR directory;
	FRESULT error;
	FILINFO fileInformation;
	char res[15] = "/maps/";
	bool exit_status = false;
	const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
	
	while(!exit_status){
	
	switch(state){
		case 0:
			f_mount(&g_fileSystem, driverNumberBuffer, 0U);
			f_chdrive((char const *)&driverNumberBuffer[0U]);
			f_opendir(&directory, _T("/maps"));
			state = 1;
			break;
		case 1:
			error = f_readdir(&directory, &fileInformation);
			if ((error != FR_OK) || (fileInformation.fname[0U] == 0U))
       {
					state = 2;
					break;
			 }
       if (fileInformation.fname[0] == '.')
       {
					break;
       }
       if (fileInformation.fattrib & AM_DIR)
       {
					break;
       }
       else
       {
					PRINTF("\rCurrent map selecting : %s.\n", fileInformation.fname);
					if (poll_2k_release(KEY_DOWN, KEY_A) == 2){
						state = 3;
					}
       }
			break;
		case 2:
			f_closedir(&directory);
			state = 0;
			break;
		case 3:
			f_closedir(&directory);
			exit_status = true;
			break;
		default:
			break;
	}
	}
	PRINTF("\r\n\r\nAll files loaded and ready\r\n");
	strcat(res, fileInformation.fname);
	PRINTF("\r\nThe current map is : %s.\r\n", res);
	load_map_to_buffer(res);
	return 0;
}

