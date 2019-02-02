#include <iostream>

#include <crtdefs.h>

#include <ctype.h>
#include <locale.h>

#include "CharType.h"

#include <Windows.h>

int main()
{
	CharType::initCharTypeArray();
	
	DWORD time1 = GetTickCount();
	for (int j = 0; j < 1000000; j++)
	for (int i = 0; i < 256; i++)  {
		CharType::isalpha(i);
	}
	DWORD time2 = GetTickCount();
	for (int j = 0; j < 1000000; j++)
	for (int i = 0; i < 256; i++)  {
		CharType::isAlpha(i);
	}
	DWORD time3 = GetTickCount();

	std::cout << "isalpha use time: " << (time2 - time1) << " ms" << std::endl;
	std::cout << "isAlpha use time: " << (time3 - time2) << " ms" << std::endl;

	getchar();

	return 0;
}
