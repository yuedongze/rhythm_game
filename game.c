#include "game.h"
#include "matrix.h"


// variables
uint16_t dispbuf[8];
uint8_t controller_input = 0;
int game_state = 0; // init the game state

extern int data[];
extern int data_len;

uint16_t keybuf[8] = {0,0,0,0,0,0,0,0};
uint16_t blocktmpbuf[8]= {0,0,0,0,0,0,0,0};
uint16_t scorebuf[8] = {0,0,0,0,0,0,0,0};

int health;

long threshold = 0;
long max = 0;
bool reload_sw = false;
bool end_flag = false;
bool game_end = false;

volatile timer_t current_time = {0,0};
Hashtable task;
Linkedlist update;

// initializes the game
void game_init(void){

	init_delay();
	i2c_init();
	controller_init();
	
	
	game_state = GAME_INIT;
	reload_sw = false;
	
	i2c_setaddr(0x70);
	i2c_sendbyte(0x21);
	i2c_sendbyte(HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (0 << 1));
	i2c_sendbyte(HT16K33_CMD_BRIGHTNESS | 9);
	clear_disp();
}

void loadbuffer(const int *d, bool reload){
	static int i = 0;
	long j, k;
	k = 0;
	if (reload) i = 0;
	
	//PRINTF("Loading game data from local buffer to scheduled hashtable...\r\n");
	
	while(i < data_len){
		j = d[i]*1000+d[i+1] - LAG;
		if (task->n < 12){
			k = j;
		 HTInsert(task, j, d[i+2], d[i+3]);
			if (j+(d[i+2]+16)*FALL_SPD+END_DELAY > max) {
				max = j+(d[i+2]+16)*FALL_SPD+END_DELAY;
			}
		} else {break;}
		i += 4;
	}
	threshold = k;
	if (i >= data_len) end_flag = true;
}
/*
void loadbuf(bool reload){
	int b[4] = {0,0,0,0};
	long j;
	LED_RED_ON();
	while(task->n < 16){
		if(!read_data(b)){
			j = b[0]*1000 + b[1] - LAG;
			PRINTF("%d,%d,%d,%d\r\n",b[0],b[1],b[2],b[3]);
			HTInsert(task, j, b[2], b[3]);	
			threshold=j;
			if (j+(b[2]+16)*FALL_SPD+END_DELAY > max) {
				max = j+(b[2]+16)*FALL_SPD+END_DELAY;
			}
		}else{
			end_flag = true;
			break;
		}
	}
	LED_RED_OFF();
}
*/
void game_run(void){
	game_end = false;
	// main game implemented here
	while(!game_end){
		switch(game_state){
			case GAME_INIT:
				task = HTCreate();
				loadbuffer(data, reload_sw);
			PRINTF("Game:Init\r\n");
				reload_sw = false;
				LED_BLUE_OFF();
				poll_key(KEY_START);
				game_state=GAME_START;
				health = 16;
			PRINTF("Game:Start\r\n");
				clock_start();
				break;
			case GAME_START:
				game_update();
				break;
			case GAME_PAUSE:
				clear_disp();
			PRINTF("Game:Paused\r\n");
			PRINTF("Press START to restart.");
				clock_stop();
				end_flag = false;
				clear_buf(blocktmpbuf);
				HTDestroy(task);
				task = 0;
				reload_sw = true;
				clear_timer(&current_time);
				poll_key(KEY_START);
				game_state=GAME_INIT;
				break;
			case GAME_END:
				LED_GREEN_ON();
			PRINTF("Game:End, Press START to Return to Menu.\r\n");
				clock_stop();
				end_flag = false;
				clear_buf(blocktmpbuf);
				HTDestroy(task);
				task = 0;
				reload_sw = true;
				clear_timer(&current_time);
				poll_key(KEY_START);
				LED_GREEN_OFF();
				game_end = true;
				break;
			default:
				break;
		}
	}
}



void game_update(void){
	uint16_t db[8] = {0,0,0,0,0,0,0,0};
	static uint8_t controller_prev = 0;
	static bool hit_a = false;
	static bool hit_b = false;
	controller_input = controller_read();
	
	
	// basically, at each update cycle, do garbage collection and display
	// the movement of block is implement in the timer interrupt
	if ((controller_input != controller_prev)){
	clear_buf(keybuf);
		switch(controller_input){
		case KEY_A:
			write_to_buf(buf_a, keybuf);
			hit_a = true;
			break;
		case KEY_B:
			write_to_buf(buf_b, keybuf);
			hit_b = true;
			break;
		case KEY_A+KEY_B:
			write_to_buf(buf_b, keybuf);
			write_to_buf(buf_a, keybuf);
			hit_a = true;
			hit_b = true;
			break;
		case KEY_SELECT:
			game_state = GAME_PAUSE;
		default:
			erase_to_buf(buf_a, keybuf);
			erase_to_buf(buf_b, keybuf);
			hit_a = false;
			hit_b = false;
			break;
		}
	}
	if (hit_a){
		if ((blocktmpbuf[2] >> 14) & 3) health += 2; else if ((blocktmpbuf[2] >> 12) & 3) health += 1;
	} 
	if (hit_b){
		if ((blocktmpbuf[5] >> 14) & 3) health += 2; else if ((blocktmpbuf[5] >> 12) & 3) health += 1;
	}
	
	if (health > 16) health = 16;
	write_to_buf(keybuf, db);
	write_to_buf(blocktmpbuf, db);
	display_health(health, scorebuf);
	write_to_buf(scorebuf,db);
	display_update(db);
	controller_prev = controller_input;
}



void display_update(uint16_t buf[8]){
	if (!buf_eq(buf, dispbuf)){
		clear_buf(dispbuf);
		write_to_buf(buf, dispbuf);
		i2c_display(dispbuf);
	}
}

// main game update implemented below
// at every ms, update current_time
// check for blocks that needs to be released from task to update
// also updates display buffer and recycles garbage from the linked list
void PIT1_IRQHandler(void)
{
	current_time.msec++;
	if (current_time.msec == 1000) {
		current_time.sec++;
		current_time.msec -= 1000;
	}
	
    block_queue_release_start(task, val(&current_time), &update);
    if (val(&current_time) % FALL_SPD == 0){
      block_queue_move_block(blocktmpbuf);  
			block_queue_update_display(update, blocktmpbuf);
        block_queue_recycle(&update);
		}
		
		if (end_flag) {
			if (val(&current_time) == max){
				game_state = GAME_END;
			}
		}
		
		if (val(&current_time) == threshold) {
			//PRINTF("Timing threshold met at %d s %d ms.\r\nRehashing hashtable before loading new data...\r\n", current_time.sec, current_time.msec);
			HTRehash(task, threshold);
			loadbuffer(data, reload_sw);
		}
		
		if (val(&current_time) % 140 == 0 && val(&current_time) != 0) {
			health--;
			if (health < 0) health = 0;
		}
		
	PIT->CHANNEL[1].TFLG = 1; //Clear timer 1 interrupt flag
}
