/*
 * UART.h
 *
 *  Created on: Nov 9, 2020
 *      Author: MRM
 */

#ifndef UART_H_
#define UART_H_



#include "common_macros.h"
#include "std_types.h"
#include "micro_config.h"

//#define USART_BAUDRATE 9600


typedef enum
{
	rate_2400 = 416 , rate_4800 = 207, rate_9600 = 103 , rate_14400 = 68 , rate_19200 = 51, rate_28800 = 34, rate_57600 = 16
}baudrate;


typedef enum
{
	disabled=0,evenParity=2,oddParity=3
}parity_Mode;

typedef enum
{
	one,two
}stop_Bits;

typedef enum
{
	five_bits,six_bits,seven_bits,eight_bits
}char_Size;

typedef struct
{
	baudrate rate;
	stop_Bits s_bits;
	char_Size bits_num;
	parity_Mode parity;

}uart_ConfigType;


void UART_init(const uart_ConfigType * Config_Ptr);
void UART_sendByte(uint8 data);
uint8 UART_receiveByte();
void UART_sendString(uint8 *str);
void UART_receiveString();


#endif /* UART_H_ */
