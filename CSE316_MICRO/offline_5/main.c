/*
 * offline1_code.c
 *
 * Created: 14-Apr-21 12:53:21 AM
 * Author : User
 */ 

#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>
#include <avr/interrupt.h>

#include "BinaryUtils.h"

int mode = 0;
int clck = 0;
const int DELAY = 5;
const int PERIOD = 28;

byte matrix[8][8] = {
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 1, 1, 1, 1, 1},
	{1, 0, 0, 1, 1, 1, 1, 1},
	{1, 0, 0, 1, 1, 1, 1, 1},
	{1, 0, 0, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1}
};

ISR(INT2_vect){
	mode ^= 1;
}

int main(void)
{
	byte curCol = 0;
	byte shift = 0;
	byte maskC = 0;
	byte maskD = 0;
	
	// enabling C for I/O
	MCUCSR = (1<<JTD);
	MCUCSR = (1<<JTD);
	
	// setting I/O enable
	DDRC = 0b11111111;
	DDRD = 0b11111111;
	DDRB = resetBit(DDRB, PINB2);
	
	// enabling interrupts
	GICR = oneBit(INT2);
	MCUCSR = setBit(MCUCSR, ISC2);
	sei();
	
    while (1) {
		// setting row pins
		maskC = oneBit(curCol);
		
		// setting column pins
		for(byte i = 0; i<8; i++){
			byte j = (curCol + shift);
			if(j >= 8) j -= 8;
			byte bit = matrix[i][j];
			
			maskD = assignBit(maskD, i, bit);
		}
		
		// using mask;
		PORTC = maskC;
		PORTD = maskD;
		
		// delay
		_delay_ms(DELAY);
		
		
		if(mode == 1) {	// dynamic mode
			++clck;
			if(clck == PERIOD){ // rotate
				clck = 0;
				
				if(shift == 0) 
					shift = 7;
				else 
					--shift;
			}
		}
		
		// scanning
		++curCol;
		if(curCol == 8){
			curCol = 0;
		}
    }
}

