#include <stdint.h>
#include <stdbool.h>

void buf_shift_left(uint16_t buf[8]);
void buf_shift_right(uint16_t buf[8]);
void buf_shift_up(uint16_t buf[8]);
void buf_shift_down(uint16_t buf[8]);

void write_to_buf(uint16_t buf1[8], uint16_t buf2[8]); // buf2 is target, buf1 is source
void erase_to_buf(uint16_t buf1[8], uint16_t buf2[8]); // buf2 is target, buf1 is source
void clear_buf(uint16_t buf[8]);

bool buf_eq(uint16_t buf1[8], uint16_t buf2[8]);
bool buf_isempty(uint16_t buf[8]);

void buf_move_left(uint16_t buf[8]);

void display_health(int hp, uint16_t buf[8]);
