/*
 * code.c
 *
 * Created: 04-Jun-21 12:49:17 AM
 * Author : Md Sabbir Rahman
 */ 

#include <avr/io.h>

#define F_CPU 1000000
#include <util/delay.h>

#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7
#include "lcd.h"


#include <string.h>
#include <stdlib.h> 
#include "BinaryUtils.h"

// reference voltage and 
const float V_REF = 5.0f;
const int MAX_RESULT = 1024;

int main(void)
{
	// variables for processing
	char output[20] = "Voltage: ";
	byte position = strlen(output);
	float value;
	int ADC_result;
	
	// port C and D for output to LCD
	DDRC = 0b11111111;
	DDRD = 0b11111111;
	
	//    01 (ref voltage) 1 (right adj.) 00011 (ADC3)
	ADMUX = 0b01000011;
	
	//   1 (enable ADC) 0 (don't start conversion) 0 (no auto trigger) 0 (no interrupt flag) 0 (interrupt disable) 111 (scalar 128)
	ADCSRA = 0b10000111;
	
	// initialize lcd display
	Lcd4_Init();
	
    while (1) 
    {
		// start conversion
		ADCSRA = setBit(ADCSRA, ADSC);
		
		// wait for conversion finish
		while(getBit(ADCSRA, ADSC))
			;
		
		// read digital value from ADCH|ADCL
		ADC_result = ADCL;
		ADC_result |= (ADCH<<8);
		
		// calculate float value
		value = V_REF * ADC_result / MAX_RESULT;
		
		// put value of float to string
		dtostrf(value, 3, 2, output+position);
		
		// output to LCD
		Lcd4_Set_Cursor(1,1);
		Lcd4_Write_String(output);
		
		// Delay
		_delay_ms(500);
    }
}

