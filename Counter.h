/*
 * Counter.h
 *
 *  Created on: Dec 12, 2020
 *      Author: Mahmoud.Amin
 */

#ifndef COUNTER_H_
#define COUNTER_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

typedef enum
{
	scale_1 = 1, scale_8 = 2, scale_64 = 3, scale_256 = 4, scale_1024 =5
}prescaler;

typedef enum
{
	Normal_Mode,Compare_Mode
}Counter_Mode;

typedef enum
{
	Timer0,Timer1,Timer2
}timer_Type;

typedef struct
{
	timer_Type timer;
	Counter_Mode mode;
	int start_value;
	int compare_value;
	prescaler scale;

}Counter_Config;

void set_counter_value(char val);

void counter_ini(Counter_Config *ptr);
void Counter_Set_CallBack(void(*a_ptr)(char),timer_Type timer, Counter_Mode mode);
void toggle();
void counter_Stop();

#endif /* COUNTER_H_ */
