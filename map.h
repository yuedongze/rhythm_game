#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fsl_uart.h"
#include "fsl_gpio.h"
#include "fsl_debug_console.h"
#include "ff.h"
#include "diskio.h"
#include "board.h"

#define BUFFER_SIZE (16U)
#define DATA_SIZE (1800)

void poll_SDinsert(void);
int read_data(int b[4]);

void open_file(TCHAR* filename);
void list_dir(TCHAR* dirname);
void close_file(void);
void resetptr(void);
void load_map_to_buffer(TCHAR* filename);

extern int data[];
extern int data_len;

int f_browse(void);
