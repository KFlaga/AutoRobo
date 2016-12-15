/*
 * Core.c
 *
 *  Created on: Aug 21, 2015
 *      Author: Kamil
 */


#include "Core.h"

bool comp_mem(uint8_t* tab1, uint8_t* tab2, uint16_t sizeInBytes)
{
	uint16_t i;
	for(i = 0; i < sizeInBytes; i++)
	{
		if(tab1[i] != tab2[i])
			return false;
	}
	return true;
}


void uint_to_uchar(uint32_t src, uchar* dst)
{
		uint32_t srccpy = src;
		uint32_t helper;
		uint8_t pos = 0;
		uint32_t div = 10;
		while( src*10 >= div )
		{
			helper = srccpy - (srccpy/div)*div;
			dst[pos] = (uchar) ((helper*10)/div) + '0';
			srccpy =  srccpy - helper;
			pos++;
			div *= 10;
		}
		uint8_t i = 0;
		while(i < pos/2)
		{
			helper = dst[pos-i-1];
			dst[pos-i-1] = dst[i];
			dst[i] = helper;
			i++;
		}
}
