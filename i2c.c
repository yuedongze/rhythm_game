#include "MK64F12.h"
#include "i2c.h"
#include "delay.h"

#define DELAYTIME    1250
#define WRITE       0U
#define READ        1U

static uint8_t slave_addr = (0x70 << 1);

// INPUT = 1, OUTPUT = 0

void scl_setdir(int dir){
	if (dir == 1){
		PTE->PDDR &= ~(1 << 24);
	} else {
		PTE->PDDR |= (1 << 24);
	}
}

void scl_setval(void){
	PTE->PSOR |= (1 << 24);
}

void scl_clrval(void){
	PTE->PCOR |= (1 << 24);
}

int scl_getdir(void){
	return ((PTE->PDDR) >> 24) & 1;
}

uint8_t scl_getval(void){
	uint8_t res;
	if (scl_getdir() == 0){
		res = ((PTE->PDIR)>>24) & 1;
	} else {
		res = ((PTE->PDOR)>>24) & 1;
	}
	return res;
}

void sda_setdir(int dir){
	if (dir == 1){
		PTE->PDDR &= ~(1 << 25);
	} else {
		PTE->PDDR |= (1 << 25);
	}
}

void sda_setval(void){
	PTE->PSOR |= (1 << 25);
}

void sda_clrval(void){
	PTE->PCOR |= (1 << 25);
}

int sda_getdir(void){
	return ((PTE->PDDR) >> 25) & 1;
}

uint8_t sda_getval(void){
	uint8_t res;
	if (sda_getdir() == 0){
		res = ((PTE->PDIR)>>25) & 1;
	} else {
		res = ((PTE->PDOR)>>25) & 1;
	}
	return res;
}

void i2c_init(void){
	SIM->SCGC5 |= (1<<13);
	
	PTE->PDDR &= ~((1<<25)|(1<<24)); // Set both pins at input
	
	PTE->PDOR |= (1<<25)|(1<<24); // Drive two pins high
	
	// Mux settings
	PORTE->PCR[24] = PORT_PCR_MUX(001);
	PORTE->PCR[25] = PORT_PCR_MUX(001);
}

void i2c_write(uint8_t data){
	uint8_t shift;
	uint8_t i;
	int timeout;
	
	shift = 0x80;
	for (i = 0x08; i != 0; i--){
		if (data & shift) {
			sda_setdir(1);
		} else {
			sda_setdir(0);
			sda_clrval();
		}
		delay_ns(1200);
		shift = (uint8_t)(shift >> 1);
		scl_setdir(1);
		delay_ns(1200);
		timeout = 65535;
		while((scl_getval()==0)&&(timeout!=0)){
			timeout--;
		}
		delay_ns(1200);
		scl_setdir(0);
		scl_clrval();
		delay_ns(1200);
	}
}

int i2c_getack(void){
	int timeout;
	
	sda_setdir(1);
	delay_ns(1200);
	scl_setdir(1);
	delay_ns(1200);
	timeout = 65535;
	while((scl_getval() == 0)&&(timeout!=0)){
		timeout--;
	}
	return sda_getval();
}

void i2c_sendbyte(uint8_t data){
	int trial;
	int acknowledge;
	int timeout;
	
	trial = 256;
	do{
		sda_setdir(1);
		scl_setdir(1);
		delay_ns(1200);
		
		timeout = 65535;
		while((scl_getval() == 0)&&(timeout!=0)){timeout--;}
		//delay_ns(1200);
		if (timeout == 0) {return;}
		
		sda_setdir(0);
		sda_clrval();
		delay_ns(1200);
		scl_setdir(0);
		scl_clrval();
		delay_ns(1200);
		
		i2c_write((uint8_t)(slave_addr + WRITE));
		acknowledge = i2c_getack();
		trial--;
	} while ((trial != 0) && (acknowledge));
	
	if (acknowledge){
		scl_setdir(0);
		scl_clrval();
		return;
	} else {
		scl_setdir(0);
		scl_clrval();
		delay_ns(1200);
	}
	
	i2c_write(data);
	if (i2c_getack() == 1){
		scl_setdir(0);
		scl_clrval();
		return;
	} else {
		scl_setdir(0);
		scl_clrval();
		delay_ns(1200);
	}
	
}

void i2c_test(void){
	int trial;
	int acknowledge;
	int timeout;
	int i;
	
	trial = 256;
	do{
		sda_setdir(1);
		scl_setdir(1);
		delay_ns(1200);
		
		timeout = 65535;
		while((scl_getval() == 0)&&(timeout!=0)){timeout--;}
		delay_ns(1200);
		if (timeout == 0) {return;}
		
		sda_setdir(0);
		sda_clrval();
		delay_ns(1200);
		scl_setdir(0);
		scl_clrval();
		delay_ns(1200);
		
		i2c_write((uint8_t)(slave_addr + WRITE));
		acknowledge = i2c_getack();
		--trial;
	} while ((trial != 0) && (acknowledge!=0));
	if (acknowledge != 0){
		scl_setdir(0);
		scl_clrval();
		return;
	} else {
		scl_setdir(0);
		scl_clrval();
		delay_ns(1200);
	}
	
	i2c_write((uint8_t)0x00);
	acknowledge = i2c_getack();
	if (acknowledge != 0){
		scl_setdir(0);
		scl_clrval();
		return;
	} else {
		scl_setdir(0);
		scl_clrval();
		delay_ns(1200);
	}
	
	for (i = 0; i < 8; i++){
		i2c_write((uint8_t)0x55);
		if (i2c_getack() == 1){
			scl_setdir(0);
			scl_clrval();
			return;
		} else {
			scl_setdir(0);
			scl_clrval();
			delay_ns(1200);
		}
		
		i2c_write((uint8_t)0x55);
		if (i2c_getack() == 1){
			scl_setdir(0);
			scl_clrval();
			return;
		} else {
			scl_setdir(0);
			scl_clrval();
			delay_ns(1200);
		}
	}
	delay_ns(1200);
	timeout = 65535;
	while((scl_getval() == 0)&&(timeout!=0)){timeout--;}
}

void i2c_setaddr(uint8_t a){
	slave_addr = (uint8_t)(a<<1);
}

void i2c_display(uint16_t buf[8]){
	//MUST PASS IN A BUFFER WITH 8 ITEMS!!!
	int trial;
	int acknowledge;
	int timeout;
	int i;
	uint16_t current;
	
	trial = 256;
	do{
		sda_setdir(1);
		scl_setdir(1);
		delay_ns(1200);
		
		timeout = 65535;
		while((scl_getval() == 0)&&(timeout!=0)){timeout--;}
		delay_ns(1200);
		if (timeout == 0) {return;}
		
		sda_setdir(0);
		sda_clrval();
		delay_ns(1200);
		scl_setdir(0);
		scl_clrval();
		delay_ns(1200);
		
		i2c_write((uint8_t)(slave_addr + WRITE));
		acknowledge = i2c_getack();
		--trial;
	} while ((trial != 0) && (acknowledge!=0));
	if (acknowledge != 0){
		scl_setdir(0);
		scl_clrval();
		return;
	} else {
		scl_setdir(0);
		scl_clrval();
		delay_ns(1200);
	}
	
	i2c_write((uint8_t)0x00);
	acknowledge = i2c_getack();
	if (acknowledge != 0){
		scl_setdir(0);
		scl_clrval();
		return;
	} else {
		scl_setdir(0);
		scl_clrval();
		delay_ns(1200);
	}
	
	for (i = 0; i < 8; i++){
		current = buf[i];
		i2c_write((uint8_t)current&0xFF);
		if (i2c_getack() == 1){
			scl_setdir(0);
			scl_clrval();
			return;
		} else {
			scl_setdir(0);
			scl_clrval();
			delay_ns(1200);
		}
		
		current = current >> 8;
		i2c_write((uint8_t)current&0xFF);
		if (i2c_getack() == 1){
			scl_setdir(0);
			scl_clrval();
			return;
		} else {
			scl_setdir(0);
			scl_clrval();
			delay_ns(1200);
		}
	}
	delay_ns(1200);
	timeout = 65535;
	while((scl_getval() == 0)&&(timeout!=0)){timeout--;}
}
