#ifndef UTIL_H
#define	UTIL_H
#include <stdint.h>

#define PORT    COM5

int test(void);
int serial_init(void);
char serial_read(void);
void serial_close(void);
void send_one_int8(char tx);
int16_t rec_one_int16_nb();

#endif

