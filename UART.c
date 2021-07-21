/*
 * UART.c
 *
 *  Created on: Nov 9, 2020
 *      Author: MRM
 */



#include "UART.h"



void UART_init(const uart_ConfigType * Config_Ptr)
{
	SET_BIT(UCSRB, RXEN);
	SET_BIT(UCSRB,TXEN);
	SET_BIT(UCSRA,U2X);

	SET_BIT(UCSRC,URSEL);
	CLEAR_BIT(UCSRC,UMSEL);

	UCSRC = (UCSRC & 0xCF) | ((Config_Ptr ->parity)<<4);   //to configure the parity check method
	UCSRC = (UCSRC & 0xF7) | ((Config_Ptr ->s_bits)<<3);   //to configure the number of stop bits
	UCSRC = (UCSRC & 0xF9) | ((Config_Ptr ->bits_num)<<1);    // this to configure the number of bits inside the data frame

	UBRRH = (Config_Ptr -> rate) >> 8;
	UBRRL = Config_Ptr -> rate;

}
void UART_sendByte(uint8 data)
{


	/*

	UDR = data;
	while(bit_is_clear(UCSRA, TXC));
	SET_BIT(UCSRA,TXC);

	*/


	while(bit_is_clear(UCSRA,UDRE));
	UDR = data;
}



uint8 UART_receiveByte()
{
	while(bit_is_clear(UCSRA,RXC));
	return UDR;
}


void UART_sendString(uint8 *str)
{
	while(*(str) != '\0')
	{
		UART_sendByte(*str);
		str++;
	}
}


void UART_receiveString(uint8 *str)
{

	*str = UART_receiveByte();
	while(*str != '#')
	{
		str++;
		*str = UART_receiveByte();
	}
	*str = '\0';
}



