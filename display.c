#include "display.h"
#include "game.h"
#include "i2c.h"

extern uint16_t dispbuf[8];
extern uint8_t controller_input;

void clear_disp(void){
	int i;
	for (i=0; i<8; i++){
		dispbuf[i] = 0;
	}
	i2c_display(dispbuf);
}

void buf_move_left(uint16_t buf[8]){
	int i;
	for (i=0; i<8; i++){
		buf[i] = buf[i] << 1;
	}
}

void buf_shift_left(uint16_t buf[8]){
	int i, j;
	for (i=0; i<8; i++){
		j = (buf[i] >> 15) & 1;
		buf[i] = buf[i] << 1;
		buf[i] |= j;
	}
}
void buf_shift_right(uint16_t buf[8]){
	int i, j;
	for (i=0; i<8; i++){
		j = (buf[i] & 1) << 15;
		buf[i] = buf[i] >> 1;
		buf[i] |= j;
	}
}
void buf_shift_up(uint16_t buf[8]){
	int i, j;
	j = buf[7];
	for (i=7; i>0; i--){
		buf[i] = buf[i-1];
	}
	buf[0] = j;
}
void buf_shift_down(uint16_t buf[8]){
	int i, j;
	j = buf[0];
	for (i=0; i<7; i++){
		buf[i] = buf[i+1];
	}
	buf[7] = j;
}

void write_to_buf(uint16_t buf1[8], uint16_t buf2[8]){
	int i;
	for (i=0; i<8; i++){
		buf2[i] |= buf1[i];
	}
}

void erase_to_buf(uint16_t buf1[8], uint16_t buf2[8]){
	int i;
	for (i=0; i<8; i++){
		buf2[i] &= ~(buf1[i]);
	}
}

void clear_buf(uint16_t buf[8]){
	int i;
	for (i=0; i<8; i++){
		buf[i] = 0;
	}
}

bool buf_eq(uint16_t buf1[8], uint16_t buf2[8]){
	bool res = true;
	int i;
	for (i=0; i<8; i++){
		if (buf1[i] != buf2[i]) {res = false;}
	}
	return res;
}

bool buf_isempty(uint16_t buf[8]){
	int i;
	for (i=0; i<8; i++){
		if (buf[i] != 0){return false;}
	}
	return true;
}

void display_health(int hp, uint16_t buf[8]){
	hp = 16-hp;
	buf[0] = (((((0xFFFF) >> hp) << hp)<<hp)&0xFFFF)>>hp;
	buf[7] = (((((0xFFFF) >> hp) << hp)<<hp)&0xFFFF)>>hp;
}

