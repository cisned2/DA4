
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int temp; //variable to hold ADC value
	
	ISR(ADC_vect){
		ADCSRA |= (1 << ADIF); //set ADIF 1
		temp = ADCH; //read conversion from ADCH register
		//PORTD = temp; //output ADC value
		//OCR1A = temp;
	}

int main(void)
{
	DDRD = 0b00001111; //set PIND0-PIND3 to output
	
	//set to CTC mode
	TCCR1A = 0; //set WGM11 = 0, WGM10 = 0
	TCCR1B = 13; //set WGM12 = 1, WGM13 = 0, prescale = 1024
	OCR1A = 23000; //set Output Compare Register to calculated count(compared

	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADSC) | (1 << ADATE) | (1 << ADIE);//enable ADC interrupt set channel to take input for ADC0(changed to ADC1),left adjust,AVcc with external cap at AREF (You can ignore connecting external capacitor at AREF)
		
	ADMUX |= (1 << ADLAR) | (1 << REFS0) | (1 << MUX0); //ADC prescaler 64, enable ADC,Start conversion,enable auto trigger (free running mode)
	
	sei();
	while (1)
	{

		//if((TIFR1 & (1 << OCF1A)) == 0){ //check if flag is set
			for(int i=0;i<temp;i++)
				_delay_ms(1);
			PORTD = 1; //black
			for(int i=0;i<temp;i++)
				_delay_ms(1);		
			PORTD = 2; //red
			for(int i=0;i<temp;i++)
				_delay_ms(1);
			PORTD = 4; //green
			for(int i=0;i<temp;i++)
				_delay_ms(1);
			PORTD = 8; //blue
				TIFR1 |= (1 << OCF1A); //clear flag
	}

}