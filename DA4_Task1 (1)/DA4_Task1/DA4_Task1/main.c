/*
 * DA4_Task1.c
 *
 * Created: 4/11/2018 5:07:49 PM
 * Author : Damian Cisneros
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
int temp=0;

ISR(INT0_vect){
	EIFR |= (INTF0 << 1); // clear INT0 flag
	_delay_ms(10);
	//toggle on and off
	if(temp == 0)
		temp = 1;
	else if(temp == 1)
		temp = 0;
}

int main(void)
{
	DDRB |= (1 << PINB1); //use PINB1 as output for input of motor driver
	DDRD |= (1 << PIND0); //use PIND0 as output for input of driver enable

	EIMSK = (1 << INT0); //activate external interrupt
	EICRA = (1 << ISC01 ) | (1 << ISC00); //activate external interrupt on rising edge
	
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (0 << CS11) | (1 << CS10); //prescale to 1024
	TCCR1A |= (1 << COM1B1) | (1 << COM1A1) | (1 << WGM11) | (1 << WGM10); //set mode to PWM with TOP 1024
	
	//set channel to take input for ADC0(changed to ADC1),left adjust,AVcc with external cap at AREF
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADSC) | (1 << ADATE);

	ADMUX |= (1 << MUX0); //ADC prescaler 64,	enable ADC,Start conversion,enable auto trigger (free running mode)
	sei();
	
	while (1)
	{
		volatile int ad;
		while ((ADCSRA & (1 << ADIF)) == 0); //check if ADIF is 1, if not stay here
		ADCSRA |= (1 << ADIF); //set ADIF 1
		ad = ADC << 2; //read conversion from ADC register
		OCR1A = ad;
		if(temp == 1) //if on turn on enable
			PORTD = 1;
		else if(temp == 0) //if off turn off enable
			PORTD = 0;
	}
}