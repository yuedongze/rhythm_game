#include "start.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "i2c.h"
#include "display.h"
#include "definition.h"
#include "controller.h"
#include "delay.h"

uint16_t sb1[8] = {0x0,0xF6DB,0x5451,0x4000,0x4000,0x4451,0xC6DB,0x0};
uint16_t sb2[8] = {0x0,0xD99B,0x8A51,0x80,0x100,0x8A51,0xD99B,0x0};
uint16_t sb3[8] = {0x0,0xDB66,0x8A29,0x8,0x8,0x8A29,0xDB66,0x0};
uint16_t sb1_c[8] = {0x0,0xF757,0x5575,0x4555,0x4777,0x4001,0xC001,0x0};
uint16_t sb2_c[8] = {0x0,0x9E0,0x1C10,0x2A10,0x854,0x838,0x790,0x0};
uint16_t dy[8] = {0,0,0xEEEE,0xA2A2,0xEEEE,0x2AA8,0x2AEE,0};
uint16_t yl[8] = {0,0,0xEB97,0x2894,0xEB97,0xAA11,0xABB7,0};
uint16_t ece[8] = {0,0,0x39B8,0x2220,0x3A38,0x2220,0x39B8,0};
uint16_t ece2[8] = {0XCE26,0X2429,0X2429,0XC4ED,0X24AB,0X2CA9,0XC4A6,0};

uint8_t start_controller_input = 0;
bool selected = false;
int s_state = 0;

void start_update(void){
	static uint8_t controller_prev = 0;
	static int ss_prev = 0;
	start_controller_input = controller_read();

	if ((start_controller_input != controller_prev)){
		switch(controller_prev){
		case KEY_RIGHT:
			s_state++;
			if(s_state>2) s_state=2;
			break;
		case KEY_LEFT:
			s_state--;
			if(s_state<0) s_state=0;
			break;
		case KEY_A:
			selected = true;
			break;
		default:
			break;
		}
		
		if (ss_prev != s_state){
		switch(s_state){
			case 0:
				i2c_display(sb1);
			PRINTF("\r>>> Load Map File <<<\n");
				break;
			case 1:
				i2c_display(sb2);
			PRINTF("\r>>>>> Play Game <<<<<\n");
				break;
			case 2:
				i2c_display(sb3);
			PRINTF("\r>>>>>> Credits <<<<<<\n");
				break;
			default:
				break;
		}
	}
		ss_prev = s_state;
	}
	controller_prev = start_controller_input;
}

int start_screen(void) {
	s_state = 0;
	selected=false;
	i2c_display(sb1);
	PRINTF("\r>>> Load Map File <<<\n");
	while(1) {
		if (selected) break;
		start_update();
	}
	switch(s_state){
			case 0:
				i2c_display(sb1_c);
				break;
			case 1:
				i2c_display(sb2_c);
				break;
			case 2:
				i2c_display(dy);
				delay_us(800000);
				i2c_display(yl);
				delay_us(800000);
				i2c_display(ece);
				delay_us(800000);
				i2c_display(ece2);
				delay_us(800000);
				break;
			default:
				break;
		}
	return s_state;
}

