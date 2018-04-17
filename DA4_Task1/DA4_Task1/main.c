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
int temp;

	ISR(ADC_vect){
		ADCSRA |= (1 << ADIF); //set ADIF 1
		temp = ADCH; //read conversion from ADCH register
		//PORTD = temp; //output ADC value
		//OCR1A = temp;
	}

int main(void)
{
	DDRD = (1 << PIND7); //PIND7=enable/disable driver
	DDRB = (1 << PINB1) | (1 << PINB6) | (1 << PINB7); //use PINB1 as output
	
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (0 << CS11) | (1 << CS10); //prescale to 1024
	TCCR1A |= (1 << COM1B1) | (1 << COM1A1) | (1 << WGM11) | (1 << WGM10); //set mode to PWM with TOP 1024
	
		ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADSC) | (1 << ADATE) | (1 << ADIE);//enable ADC interrupt set channel to take input for ADC0(changed to ADC1),left adjust,AVcc with external cap at AREF (You can ignore connecting external capacitor at AREF)
		
		ADMUX |= (1 << ADLAR) | (1 << REFS0) | (1 << MUX0); //ADC prescaler 64, enable ADC,Start conversion,enable auto trigger (free running mode)
		
		sei();
	
	PORTB = 0xFF;
	PORTD = 0xFF;

	while (1)
	{
		//increase speed until max (by increasing duty cycle)
		OCR1A = 1000;
	
		

		
	}
}