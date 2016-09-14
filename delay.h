#include <stdbool.h>

typedef struct {
	long sec;
	long msec;
} timer_t;

void delay_us(int us);
void init_delay(void);
void delay_ns(int ns);

void clock_start(void);
void clock_stop(void);

extern volatile bool pitIsrFlag;
extern volatile timer_t current_time;

long val(volatile timer_t* time);
void clear_timer(volatile timer_t* time);
