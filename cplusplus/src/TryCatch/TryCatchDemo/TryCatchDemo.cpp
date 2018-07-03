// TryCatchDemo.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>

#include "..\TryCatchLib\TryCatchLib.h"

#include <excpt.h>
#include <Windows.h>

int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep) {

	puts("in filter.");

	switch (code) {
	case EXCEPTION_ACCESS_VIOLATION:
		puts("caught access_violation as expected.");
		break;
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		puts("caught datatype misalignment as expected.");
		break;
	case EXCEPTION_BREAKPOINT:
		puts("caught breakpoint as expected.");
		break;
	case EXCEPTION_SINGLE_STEP:
		puts("caught single step as expected.");
		break;
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		puts("caught array_bounds_exceeded as expected.");
		break;
	case EXCEPTION_FLT_DENORMAL_OPERAND:
		puts("caught float_denormal_operand as expected.");
		break;
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		puts("caught float_divide_by_zero as expected.");
		break;
	case EXCEPTION_FLT_INEXACT_RESULT:
		puts("caught float_inexact_result as expected.");
		break;
	case EXCEPTION_FLT_INVALID_OPERATION:
		puts("caught float_invalid_operation as expected.");
		break;
	case EXCEPTION_FLT_OVERFLOW:
		puts("caught float_overflow as expected.");
		break;
	case EXCEPTION_FLT_STACK_CHECK:
		puts("caught float_stack_check as expected.");
		break;
	case EXCEPTION_FLT_UNDERFLOW:
		puts("caught float_underflow as expected.");
		break;
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		puts("caught integer_divide_by_zero as expected.");
		break;
	case EXCEPTION_INT_OVERFLOW:
		puts("caught integer_overflow as expected.");
		break;
	case EXCEPTION_PRIV_INSTRUCTION:
		puts("caught privileged_instruction as expected.");
		break;
	case EXCEPTION_IN_PAGE_ERROR:
		puts("caught in_page_error as expected.");
		break;
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		puts("caught illegal_instruction as expected.");
		break;
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		puts("caught noncontinuable_exception as expected.");
		break;
	case EXCEPTION_STACK_OVERFLOW:
		puts("caught stack_overflow as expected.");
		break;
	case EXCEPTION_INVALID_DISPOSITION:
		puts("caught invalid_disposition as expected.");
		break;
	case EXCEPTION_GUARD_PAGE:
		puts("caught guard_page_violation as expected.");
		break;
	case EXCEPTION_INVALID_HANDLE:
		puts("caught invalid_handle as expected.");
		break;
	//case EXCEPTION_POSSIBLE_DEADLOCK:
	//	puts("caught possible_deadlock as expected.");
	//	break;
	default:
		puts("caught unkown expected.");
		break;
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

int main()
{
	__try
	{
		divideZero();
	} 
	__except(filter(GetExceptionCode(), GetExceptionInformation()))
	{
		puts("in except.");
	}

	getchar();

    return 0;
}

