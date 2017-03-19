#ifndef UTILS_H_
#define UTILS_H_

#include "stm32f4xx.h"

typedef unsigned char uchar;
typedef uint8_t bool;
#define TRUE 1
#define true 1
#define FALSE 0
#define false 0

#define CHAR_ARRUP '^'
#define CHAR_ARRDOWN 'v'
#define CHAR_ARRRIGHT 0x7E
#define CHAR_ARRLEFT 0x7F

#define COPY_MEM(src,dst,soff,doff,size) { uint8_t i = 0; \
	while(i<size) \
	{ \
		dst[i+doff] = src[i+soff]; \
		i++; } }

bool comp_mem(uint8_t* tab1, uint8_t* tab2, uint16_t sizeInBytes);
void uint_to_uchar(uint32_t src, uchar* dst);

#define STORE_IN_FLASH __attribute__ ((section(".rodata")))

#endif
