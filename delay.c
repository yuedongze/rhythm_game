#include "delay.h"
#include "MK64F12.h"

volatile bool pitIsrFlag = false;
extern volatile timer_t current_time;

void setTimer(long time, int chnl){
	PIT->CHANNEL[chnl].TFLG = 1; //Clear timer 0 interrupt flag
	PIT->CHANNEL[chnl].LDVAL = time; //Set the load value of the zeroth PIT with variable 'time'
}

void init_delay(void){
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK; //Enable the clock to the PIT module
	PIT->MCR &= ~(1 << 1); //Enable the standard timers
}

void delay_us(int us){
	pitIsrFlag = false;
	
	setTimer(us*1000/8.333, 0);
	
	PIT->CHANNEL[0].TCTRL |= 1 << 1; //Enable interrupts for timer 0
	
	NVIC_EnableIRQ(PIT0_IRQn);
	
	PIT->CHANNEL[0].TCTRL |= 1; //Enable timer 0
	
	while(!pitIsrFlag);
	
	PIT->CHANNEL[0].TCTRL &= ~1; //Disable timer 0
	
	PIT->CHANNEL[0].TCTRL &= ~(1 << 1); //Diable Int
	
	NVIC_DisableIRQ(PIT0_IRQn);
	
	pitIsrFlag= false;
}

void delay_ns(int ns){
	pitIsrFlag = false;
	
	setTimer(ns/8.333, 0);
	
	PIT->CHANNEL[0].TCTRL |= 1 << 1; //Enable interrupts for timer 0
	
	NVIC_EnableIRQ(PIT0_IRQn);
	
	PIT->CHANNEL[0].TCTRL |= 1; //Enable timer 0
	
	while(!pitIsrFlag);
	
	PIT->CHANNEL[0].TCTRL &= ~1; //Disable timer 0
	
	PIT->CHANNEL[0].TCTRL &= ~(1 << 1); //Diable Int
	
	NVIC_DisableIRQ(PIT0_IRQn);
	
	pitIsrFlag= false;
}

void clock_start(void){
	setTimer(500000/8.33, 1);
	PIT->CHANNEL[1].TCTRL |= 1 << 1; //Enable interrupts for timer 1
	NVIC_EnableIRQ(PIT1_IRQn);
	PIT->CHANNEL[1].TCTRL |= 1; //Enable timer 0
}

void clock_stop(void){
	PIT->CHANNEL[1].TCTRL &= ~1; //Disable timer 1
	PIT->CHANNEL[1].TCTRL &= ~(1 << 1); //Diable Int
	NVIC_DisableIRQ(PIT1_IRQn);
}

long val(volatile timer_t* t){
	return t->msec + t->sec*1000;
}

void clear_timer(volatile timer_t* time){
	time->msec = 0;
	time->sec = 0;
}

void PIT0_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    pitIsrFlag = true;
		PIT->CHANNEL[0].TFLG = 1; //Clear timer 0 interrupt flag
}



