/*  Author: Harry Hancock
 * Licence: This work is licensed under the Creative Commons Attribution License. 
 *           View this license at http://creativecommons.org/about/licenses/
 *   Notes: F_CPU must be defined to match the clock frequency
 */
 
// http://maxembedded.com/2011/06/the-adc-of-the-avr/

#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"

void init_adc(void)
{
	/* TODO: Initialisation code */

	//Select ADC Channel must be 0-7
	ADMUX = 1; //selected single conversion mode on
	//channel 0 on 3.1.
	
	ADCSRA |= _BV(ADEN); // Enable ADC
	
	//Prescalar value = 64
	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);
	
	//Prescalar value = 128
	//ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	
	//Prescalar value = 32
	//ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0);
}

uint16_t read_adc(void)
{
	/* TODO: Acquisition code */
	//Start Single conversion
	ADCSRA |= _BV(ADSC);
	return ADC;
}

int main(void)
{
	uint16_t result;
	double voltage;
	
	init_debug_uart0();
	init_adc();
	
	for (;;) 
	{
		/* TODO: Measurement code */
		read_adc(); //starts the conversion and waits till its finished.
		result = ADC;
		voltage = (result * 3.3)/ 1024;
		while(ADCSRA & _BV(ADSC));
		printf("%4d : %6.5fV\n", result, voltage);
		_delay_ms(1000);
	}
}
