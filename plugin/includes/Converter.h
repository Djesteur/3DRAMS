#ifndef CONVERTER_H
#define CONVERTER_H

#include <stdint.h>

uint8_t charToHexa(const char number);
char hexaToChar(const uint8_t number);

uint32_t stringToHexaNumber(const char number[8]);
void hexaNumberToString(const uint32_t number, char string[8]);

uint32_t parametersToNumber(const uint8_t number[4]);

#endif