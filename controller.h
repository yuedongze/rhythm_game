#include <stdint.h>

uint8_t controller_read(void);
void controller_init(void);

#define CTRL_CLK_GPIO GPIOC
#define CTRL_CLK_GPIO_PIN 3
#define CTRL_CLK_GPIO_PORT PORTC

#define CTRL_LAT_GPIO GPIOC
#define CTRL_LAT_GPIO_PIN 2
#define CTRL_LAT_GPIO_PORT PORTC

#define CTRL_DATA_GPIO GPIOA
#define CTRL_DATA_GPIO_PIN 2
#define CTRL_DATA_GPIO_PORT PORTA

void poll_key(uint8_t key);
void poll_key_release(uint8_t key);
int poll_2k_release(uint8_t key1, uint8_t key2);
