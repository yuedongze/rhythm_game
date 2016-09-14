#include "controller.h"
#include "MK64F12.h"
#include "delay.h"

void CTRL_CLK_HIGH(void){
	PTC->PSOR |= (1<<CTRL_CLK_GPIO_PIN);
}

void CTRL_CLK_LOW(void){
	PTC->PCOR |= (1<<CTRL_CLK_GPIO_PIN);
}

void CTRL_LAT_HIGH(void) {
	PTC->PSOR |= (1<<CTRL_LAT_GPIO_PIN);
}

void CTRL_LAT_LOW(void){
	PTC->PCOR |= (1<<CTRL_LAT_GPIO_PIN);
}

void controller_init(void){
	SIM->SCGC5 |= (1<<11); // Ungate port C
	SIM->SCGC5 |= (1<<9);
	
	PORTC->PCR[CTRL_CLK_GPIO_PIN] = PORT_PCR_MUX(001);
	PORTC->PCR[CTRL_LAT_GPIO_PIN] = PORT_PCR_MUX(001);
	PORTA->PCR[CTRL_DATA_GPIO_PIN] = PORT_PCR_MUX(001);

	PTC->PDDR |= (1<<CTRL_CLK_GPIO_PIN) | (1<<CTRL_LAT_GPIO_PIN);
	PTA->PDDR &= ~(1<<CTRL_DATA_GPIO_PIN);
	
	CTRL_CLK_HIGH();
	CTRL_LAT_HIGH();
}

int DATA_READ(void){
	return ((PTA->PDIR >> CTRL_DATA_GPIO_PIN) & 1);
}

/*
all negate

up 0x8
left 0x2
down 0x4
right 0x1
select 0x20
start 0x10
B 0x40
A 0x80

multiple keys pressed: add their values together
*/

uint8_t controller_read(void){
	uint8_t res = 0;
	int i = 0;
	
	CTRL_LAT_LOW();
	CTRL_CLK_LOW();
	
	CTRL_LAT_HIGH();
	delay_us(2);
	CTRL_LAT_LOW();
	
	res = DATA_READ();
	
	for(i = 1;i <= 7; i++)
  {
		CTRL_CLK_HIGH();
		delay_us(2);
		res = res << 1;
		res += DATA_READ();
		delay_us(4);
		CTRL_CLK_LOW();
  }
	
	return ~res;

}

void poll_key(uint8_t key){
	uint8_t d = 0;
	while (d != key) {d = controller_read();}
}

void poll_key_release(uint8_t key){
	uint8_t d = 0;
	uint8_t d_prev = 0;
	while (1) {
		d = controller_read();
		if ((d_prev == key) && (d != d_prev)) {break;}
		d_prev = d;
	}
}

int poll_2k_release(uint8_t key1, uint8_t key2){
	uint8_t d = 0;
	uint8_t d_prev = 0;
	while (1) {
		d = controller_read();
		if ((d_prev == key1) && (d != d_prev)) {return 1;}
		if ((d_prev == key2) && (d != d_prev)) {return 2;}
		d_prev = d;
	}
}
