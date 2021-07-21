/*
 * Counter.c
 *
 *  Created on: Dec 12, 2020
 *      Author: Mahmoud.Amin
 */

#include "Counter.h"

unsigned char counter_val;
unsigned char temp_Scale_Timer2 = 0;

static volatile void(*g_CallBack_T0_N)(char) = NULL;
static volatile void(*g_CallBack_T0_C)(char) = NULL;

static volatile void(*g_CallBack_T2_N)(char) = NULL;
static volatile void(*g_CallBack_T2_C)(char) = NULL;

static volatile void(*g_CallBack_T1_N)(char) = NULL;
static volatile void(*g_CallBack_T1_C)(char) = NULL;


ISR(TIMER0_OVF_vect)
{
	if((*g_CallBack_T0_N) != NULL)
	{
		(*g_CallBack_T0_N)(counter_val);
	}
}

ISR(TIMER0_COMP_vect)
{
	if((*g_CallBack_T0_C) != NULL)
	{
		(*g_CallBack_T0_C)(counter_val);
	}
}

ISR(TIMER2_OVF_vect)
{
	if((*g_CallBack_T2_N) != NULL)
	{
		(*g_CallBack_T2_N)(counter_val);
	}
}

ISR(TIMER2_COMP_vect)
{
	if((*g_CallBack_T2_C) != NULL)
	{
		(*g_CallBack_T2_C)(counter_val);
	}
}

ISR(TIMER1_OVF_vect)
{
	if((*g_CallBack_T1_N) != NULL)
	{
		(*g_CallBack_T1_N)(counter_val);
	}
}

ISR(TIMER1_COMPA_vect)
{
	if((*g_CallBack_T1_C) != NULL)
	{
		(*g_CallBack_T1_C)(counter_val);
	}
}


void set_counter_value(char val)
{
	counter_val = val;
}


void toggle()
{
	PORTC ^= (1<<PC0);
}


void Counter_Set_CallBack(void(*a_ptr)(char),timer_Type timer, Counter_Mode mode)
{
	if(timer == Timer0 && mode == Normal_Mode)
	{
		g_CallBack_T0_N = a_ptr;
	}

	else if(timer == Timer0 && mode == Compare_Mode)
	{
		g_CallBack_T0_C = a_ptr;
	}
	else if(timer == Timer2 && mode == Normal_Mode)
	{
		g_CallBack_T2_N = a_ptr;
	}
	else if(timer == Timer2 && mode == Compare_Mode)
	{
		g_CallBack_T2_C = a_ptr;
	}
	else if(timer == Timer1 && mode == Normal_Mode)
	{
		g_CallBack_T1_N = a_ptr;
	}
	else if(timer == Timer1 && mode == Compare_Mode)
	{
		g_CallBack_T1_C = a_ptr;
	}

}

void counter_ini(Counter_Config *ptr)
{
	SREG |= (1<<7);

	if(ptr -> timer == Timer0)
	{
		TCNT0 = ptr -> start_value;
		TCCR0 = (TCCR0 & 0xF8) | (ptr ->scale);
		if(ptr -> mode == Normal_Mode)
		{
			TIMSK |= (1<<TOIE0);
		}

		else if(ptr -> mode == Compare_Mode)
		{
			TIMSK |= (1<<OCIE0);
			TCCR0 |= (1<<WGM01);
			OCR0 = ptr -> compare_value;
		}
	}

	else if(ptr -> timer == Timer1)
	{
		TCNT1 = ptr ->start_value;
		TCCR1B = (TCCR1B & 0xF8) | (ptr -> scale);
		if(ptr -> mode == Normal_Mode)
		{
			TIMSK |= (1<<TOIE1);
		}
		else if(ptr -> mode == Compare_Mode)
		{
			TCCR1B |= (1<<WGM12);
			TIMSK |= (1<<OCF1A);
			OCR1A = ptr -> compare_value;
		}

	}

	else if(ptr -> timer == Timer2)
	{
		TCNT2 = ptr -> start_value;

		if(ptr -> scale == scale_64)
		{
			temp_Scale_Timer2 = (ptr -> scale)+1;
		}
		else if(ptr -> scale == scale_256 || ptr -> scale == scale_1024)
		{
			temp_Scale_Timer2 = (ptr -> scale)+2;
		}
		else
		{
			temp_Scale_Timer2 = ptr -> scale;

		}


		TCCR2 = (TCCR2 & 0xF8) | (temp_Scale_Timer2);

		if(ptr -> mode == Normal_Mode)
		{
			TIMSK |= (1<<TOIE2);
		}

		else if(ptr -> mode == Compare_Mode)
		{
			TCCR2 |= (1<<WGM21);
			TIMSK |= (1<<OCIE2);
			OCR2 = ptr -> compare_value;
		}

	}
}

void counter_Stop()
{
	TCCR0 = 0x00;
	TCCR1A = 0x00;
	TCCR1B = 0x00;
	TCCR2 = 0x00;
}
