#ifndef BINARY_UTILS_H
#define BINARY_UTILS_H

typedef unsigned char byte;

byte getBit(byte x, byte pos){
	return ((x&(1<<pos)) != 0);
}

byte setBit(byte x, byte pos){
	return x | (1<<pos);
}

byte resetBit(byte x, byte pos){
	return x & ~(1<<pos);
}

byte flipBit(byte x, byte pos){
	return x ^ (1<<pos);
}

byte assignBit(byte x, byte pos, byte bit){
	x = resetBit(x, pos);
	if(bit == 1) 
		x = setBit(x, pos);
	return x;
}

byte oneBit(byte pos){
	return (1<<pos);
}

#endif