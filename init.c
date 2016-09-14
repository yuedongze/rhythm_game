#include "init.h"

void initpins(void)
{
	/* Ungate port clocks */
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; //Enable the clock to port B
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; //Enable the clock to port E
	
	/* Configure LED */
	PORTB->PCR[22] = PORT_PCR_MUX(001); //Set up PTB22 as GPIO
	PORTE->PCR[26] = PORT_PCR_MUX(001); //Set up PTE26 as GPIO
	PORTB->PCR[21] = PORT_PCR_MUX(001); //Set up PTB21 as GPIO
	// Set data direction
	PTB->PDDR |= (1<<21)|(1<<22);
	PTE->PDDR |= (1<<26);
	// Initialize value
	PTB->PSOR |= (1<<21)|(1<<22);
	PTE->PSOR |= (1<<26);
}


void LED_RED_INIT(void);
void LED_RED_ON(void){
	PTB->PCOR |= (1<<22);
}
void LED_RED_OFF(void){
	PTB->PSOR |= (1<<22);
}
void LED_RED_TOGGLE(void){
	PTB->PTOR |= (1<<22);
}

void LED_GREEN_INIT(void);
void LED_GREEN_ON(void){
	PTE->PCOR |= (1<<26);
}
void LED_GREEN_OFF(void){
	PTE->PSOR |= (1<<26);
}
void LED_GREEN_TOGGLE(void){
	PTE->PTOR |= (1<<26);
}

void LED_BLUE_INIT(void);
void LED_BLUE_ON(void){
	PTB->PCOR |= (1<<21);
}
void LED_BLUE_OFF(void){
	PTB->PSOR |= (1<<21);
}
void LED_BLUE_TOGGLE(void){
	PTB->PTOR |= (1<<21);
}
