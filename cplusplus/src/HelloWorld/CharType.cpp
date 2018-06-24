#include "CharType.h"

#include <ctype.h>

/* set bit masks for the possible character types */

#define CharType_UPPER          0x1     /* upper case letter */
#define CharType_LOWER          0x2     /* lower case letter */
#define CharType_DIGIT          0x4     /* digit[0-9] */
#define CharType_SPACE          0x8     /* tab, carriage return, newline, */
                                /* vertical tab or form feed */
#define CharType_PUNCT          0x10    /* punctuation character */
#define CharType_CONTROL        0x20    /* control character */
#define CharType_BLANK          0x40    /* space char */
#define CharType_HEX            0x80    /* hexadecimal digit */

#define CharType_LEADBYTE       0x8000                  /* multibyte leadbyte */
#define CharType_ALPHA          (0x0100|CharType_UPPER|CharType_LOWER)  /* alphabetic character */


static char CharType_Array_[256] = {0};

static void initCharTypeArray() {
	for (int i = 0; i < 256; i++) {
		if (isdigit(i)) {
			CharType_Array_[i] |= CharType_DIGIT;
		}
		if (isupper(i)) {
			CharType_Array_[i] |= CharType_UPPER;
		}
		if (islower(i)) {
			CharType_Array_[i] |= CharType_LOWER;
		}
		if (isspace(i)) {
			CharType_Array_[i] |= CharType_SPACE;
		}
		if (ispunct(i)) {
			CharType_Array_[i] |= CharType_PUNCT;
		}
		if (iscntrl(i)) {
			CharType_Array_[i] |= CharType_CONTROL;
		}
		if (isxdigit(i)) {
			CharType_Array_[i] |= CharType_HEX;
		}
	}
}


void CharType::initCharTypeArray()
{
	::initCharTypeArray();
}
bool CharType::isalpha(int c)
{
	unsigned char index = (unsigned char)c;
	int char_type = CharType_Array_[index];
	if ((char_type & CharType_DIGIT) != 0 ||
		(char_type & CharType_UPPER) != 0 ||
		(char_type & CharType_LOWER) != 0) {
			return true;
	}
	return false;
}
bool CharType::isAlpha(int c)
{
	static char upper_min = 'A';
	static char upper_max = 'Z';
	static char lower_min = 'a';
	static char lower_max = 'z';
	static char digit_min = '0';
	static char digit_max = '9';

	char ch = (char)c;
	if (upper_min <= ch && ch <= upper_max)
		return true;
	if (lower_min <= ch && ch <= lower_max)
		return true;
	if (digit_min <= ch && ch <= digit_max)
		return true;

	return false;
}

