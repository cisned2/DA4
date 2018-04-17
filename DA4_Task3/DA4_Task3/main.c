#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include  <avr/interrupt.h>

int temp=0;

void Wait()
{
	uint8_t i;
	for(i=0;i<1;i++)
	{
		_delay_loop_2(0);
		_delay_loop_2(0);
		_delay_loop_2(0);
	}
}

int main(void)
{
	DDRB |= (1 << PINB1); //OCR1A as output to servo
	
	TCCR1A = 0x82; //non inverted PWM
	TCCR1B = 0x1A; //prescale 8 Fast PWM mode

	ICR1 = 19999; //(8Mhz/(8*50Hz)-1) = 20ms period
	
	ADCSRA |= (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADSC) | (1 << ADATE) ;//enable ADC interrupt set channel to take input for ADC0(changed to ADC1),left adjust,AVcc with external cap at AREF (You can ignore connecting external capacitor at AREF)
	ADMUX |= (1 << REFS0) | (1 << MUX0); //ADC prescaler 8, enable ADC,Start conversion,enable auto trigger (free running mode)
	sei();

while (1){
	while ((ADCSRA & (1 << ADIF)) == 0); //check if ADIF is 1, if not stay here
	ADCSRA |= (1 << ADIF); //set ADIF 1
	temp = ADC; //read conversion from ADCH register
	//divide = 1/1023 = .000977517;
	temp = 1800*(1-(0.000977517*temp));
	OCR1A = temp+400;
	Wait();
}
}