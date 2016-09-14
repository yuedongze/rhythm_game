#include "data.h"
#include "map.h"

int data[12];
int data_len = 12;

void read_data_to_buffer(void){
	int b[4] = {0,0,0,0};
	int ptr = 0;
	
	open_file("/maps/snow_e2.txt");
	
	data_len = read_maplength();
	PRINTF("Total length of Map is %d lines.\r\n", data_len);
	data_len = data_len*4;
	
	
	PRINTF("Total length of Data is %d.\r\n", sizeof(data));
	
	while(!read_data(b)){
		data[ptr] = b[0];
		data[ptr+1] = b[1];
		data[ptr+2] = b[2];
		data[ptr+3] = b[3];
		PRINTF("%d\r\n", data[ptr+1]);
		ptr +=4;
	}
	close_file();
}

