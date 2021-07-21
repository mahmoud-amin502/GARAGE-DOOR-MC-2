/*
 * Main.c
 *
 *  Created on: Nov 28, 2020
 *      Author: MRM
 */


#include "I2C.h"
#include "external_EEPROM.h"
#include "UART.h"
#include "Main_MC2.h"
#include "Counter.h"

/* function to count the required time */

void APP_Counter(char time_sec)
{
	count ++;
	if(count == (4000*time_sec))    // checks if the global variable count reached the desired time
	{
		count = 0;
	}
}

/*********************************************************************************************************************************************************************/

/* function to take the desired amount of time to be counted time */

void my_Delay(char val)
{
	set_counter_value(val);   // set the value to be passed by the app_counter function.
	APP_Counter(val);  // call the app_counter function passing the required time to be counted

	Counter_Set_CallBack(APP_Counter,Timer2,Normal_Mode);  // set the call back passing the application function name, the timer type and the mode
	Counter_Config ptr = {Timer2,Normal_Mode,0,256,scale_8}; //configure the desired counter
	counter_ini(&ptr);  // passing the configuration pointer
	while(count != 0 );  //checks if the counter finished counting the desired time
	counter_Stop();  // stops the counter
}




uint8 compare_Pass(uint8 * str1, uint8 *str2)
{

	for(int i=0;i<5;i++)
	{
		if(str1[i] != str2[i])
		{
			return 0;
		}
	}

	return 1;
}


void mem_Read_Pass(uint8 *pass_Read)
{
	uint8 temp;
	for( int i=0;i<6;i++)
	{
		EEPROM_readByte((address+i), &temp);
		_delay_ms(100);
		pass_Read[i] = temp;
	}
}

void mem_Write_Pass(uint8 *pass_Write)
{
	for( int i=0;i<6;i++)
	{
		EEPROM_writeByte((address+i), pass_Write[i]);
		_delay_ms(100);
	}
}

void set_Pass()
{
	UART_sendString(first_Time);
	UART_receiveString(rec_Pass);
	UART_sendString(pass_Again);
	UART_receiveString(rec_Pass_2nd);

	if(compare_Pass(rec_Pass, rec_Pass_2nd) == 0)
	{
		UART_sendString(wrong_Pass);
	}

	else
	{
		UART_sendString(success);
		mem_Write_Pass(rec_Pass_2nd);
	}

}

void door_Control()
{
	UART_sendString(open_Door);
	SET_BIT(PORTB,PB0);
	my_Delay(15);
	CLEAR_BIT(PORTB,PB0);

	my_Delay(3);

	UART_sendString(close_Door);
	SET_BIT(PORTB,PB1);
	my_Delay(15);
	CLEAR_BIT(PORTB,PB1);
}

void menu_Option1(uint8 option)
{
	if(menu_Option ==  1)
	{
		_delay_ms(1);
		UART_sendString(option1);
		UART_receiveString(rec_Pass);

		mem_Read_Pass(mem_Pass);

		if(compare_Pass(mem_Pass, rec_Pass) == 0)
		{
			UART_sendByte('0');
			UART_sendString(wrong_Pass);
		}

		else if(compare_Pass(mem_Pass, rec_Pass) == 1)
		{
			UART_sendByte(' ');
			UART_sendString(new_pass);
			UART_receiveString(rec_Pass);

			UART_sendString(pass_Again);
			UART_receiveString(rec_Pass_2nd);
			if(compare_Pass(rec_Pass, rec_Pass_2nd) == 0)
			{
				UART_sendString(wrong_Pass);
			}

			else
			{
				mem_Write_Pass(rec_Pass_2nd);
				UART_sendString(success);
			}

		}

	}
}


void menu_Option2(uint8 option)
{

	if(option == 2)
	{
		_delay_ms(10);
		UART_sendString(option1);
		UART_receiveString(rec_Pass);
		mem_Read_Pass(mem_Pass);
		if(compare_Pass(mem_Pass, rec_Pass) == 1)
		{
			UART_sendByte(yes);
			door_Control();
		}

		else if(compare_Pass(mem_Pass, rec_Pass) == 0)
		{
			UART_sendByte(no);
			_delay_ms(10);
			uint8 counter = 2;
			while(counter > 0)
			{
				UART_sendString(pass_Again);
				UART_receiveString(rec_Pass);
				counter --;

				if(compare_Pass(mem_Pass, rec_Pass) == 0)
				{
					UART_sendByte(no);
				}
				else
				{
					UART_sendByte(yes);
					counter = 0;
					door_Control();
					flag1 = 1;

				}
			}

			if(counter == 0 && flag1 == 0)
			{
				SET_BIT(PORTB,PB2);
				_delay_ms(3000);
				CLEAR_BIT(PORTB,PB2);
			}
		}
	}
}







int main(void)
{
	EEPROM_init();
	uart_ConfigType u1 = {rate_9600,two,eight_bits,disabled};
	UART_init(&u1);
	DDRB = 0x07;
	PORTB = 0x00;;
	while(1)
	{
		while(UART_receiveByte() != ready);
		EEPROM_readByte(address,&ch);
		_delay_ms(10);
		if(ch == 0xFF)
		{
			set_Pass();
		}

		else
		{
			UART_sendString(menu);
			menu_Option = UART_receiveByte();
			if(menu_Option == 1)
			{
				menu_Option1(menu_Option);
			}

			else if(menu_Option == 2)
			{
				menu_Option2(menu_Option);
			}
		}
	}
}
