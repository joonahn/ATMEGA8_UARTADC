/*
 * RC_CAR.c
 *
 * Created: 2017-04-23 오후 11:33:03
 * Author : SEOKJOON
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#define ubrr 51
#define badu 9600

void uart_init();
unsigned char uart_receive(void);
void uart_transmit(unsigned char data);
int adc_read(int channel);

int main(void)
{
	volatile int data;
	uart_init();
    /* Replace with your application code */
    while (1) 
    {
		data = adc_read(0);
		if(data>800)
			uart_transmit('w');
		else if (data<300)
			uart_transmit('s');
		else
		{
			data = adc_read(1);
			if(data>800)
				uart_transmit('d');
			else if (data<300)
				uart_transmit('a');
			else
				uart_transmit('q');
		}
		_delay_ms(50);
    }
}


int adc_read(int channel)
{
	int analog;

	/* select channel */
	switch (channel) {
		case 0:
		ADMUX = 1 << REFS0 | 0 << MUX2 | 0 << MUX1 | 0 << MUX0;
		break;
		case 1:
		ADMUX = 1 << REFS0 | 0 << MUX2 | 0 << MUX1 | 1 << MUX0;
		break;
		case 2:
		ADMUX = 1 << REFS0 | 0 << MUX2 | 1 << MUX1 | 0 << MUX0;
		break;
		case 3:
		ADMUX = 1 << REFS0 | 0 << MUX2 | 1 << MUX1 | 1 << MUX0;
		break;
		case 4:
		ADMUX = 1 << REFS0 | 1 << MUX2 | 0 << MUX1 | 0 << MUX0;
		break;
		case 5:
		ADMUX = 1 << REFS0 | 1 << MUX2 | 0 << MUX1 | 1 << MUX0;
		break;
		case 6:
		ADMUX = 1 << REFS0 | 1 << MUX2 | 1 << MUX1 | 0 << MUX0;
		break;
		case 7:
		ADMUX = 1 << REFS0 | 1 << MUX2 | 1 << MUX1 | 1 << MUX0;
		break;
	}

	ADCSRA = 1 << ADEN | 1 << ADPS0 | 1 << ADPS1 | 1 << ADPS2;

	/*	read analog value */
	ADCSRA = 1 << ADSC | ADCSRA;
	while (ADCSRA & (1 << ADSC))
	;
	analog = ADCW;

	return analog;
}

void uart_init()
{
	UBRRH = (unsigned char)(ubrr >> 8);
	UBRRL = (unsigned char)ubrr;
	UCSRB = (1 << RXEN) | (1 << TXEN) | (1<<RXCIE);
	UCSRC = (1 << URSEL) | (0 << USBS) | (3 << UCSZ0);
}


void uart_transmit(unsigned char data)
{
	while (!(UCSRA & 0b00100000))
		;
	UDR = data;
}

unsigned char uart_receive(void)
{
	while (!(UCSRA) & (1<<RXC));
	return UDR;
}



