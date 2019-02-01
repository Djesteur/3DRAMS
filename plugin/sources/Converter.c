#include "Converter.h"

uint8_t charToHexa(const char number) {

	uint8_t newNumber = 255;

	if(number >= 48 && number <= 57) { newNumber = number - 48; } 	//0 to 9
	if(number >= 65 && number <= 70) { newNumber = number - 55; }	//A to F
	if(number >= 97 && number <= 102) { newNumber = number - 87; }	//a to f

	return newNumber;
}

char hexaToChar(const uint8_t number) {

	char result = 0;

	if(number >= 0 && number <= 9) { result = 48 + number; }
	if(number >= 10 && number <= 15) { result = 55 + number; }

	return result;
}



uint32_t stringToHexaNumber(const char number[8]) {

	uint32_t result = 0;

	for(unsigned int i = 0; i < 8; i++) {

		result += charToHexa(number[i]);
		if(i != 7) { result <<= 4; }
	}
	
	return result;
}

void hexaNumberToString(const uint32_t number, char string[8]) {

	uint32_t currentNumber = 0;

	for(unsigned int i = 0; i < 8; i++) {

		currentNumber = number << i*4;
		currentNumber >>= 28;

		string[i] = hexaToChar((uint8_t)(currentNumber));
	}
}



uint32_t parametersToNumber(const uint8_t number[4]) {

	uint32_t result = 0;

	for(unsigned int i = 0; i < 4; i++) {

		result += number[i];
		if(i != 3) { result <<= 8; }
	}
	
	return result;
}