// TryCatchLib.cpp : 定义 DLL 应用程序的导出函数。
//

#include "TryCatchLib.h"


// 这是导出变量的一个示例
TRYCATCHLIB_API int nTryCatchLib=0;

// 这是导出函数的一个示例。
TRYCATCHLIB_API int fnTryCatchLib(void)
{
    return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 TryCatchLib.h
CTryCatchLib::CTryCatchLib()
{
    return;
}


TRYCATCHLIB_API void divideZero()
{
	static int zero = 0;
	static int ret = 0;
	static int divide = 100;
	
	ret = divide / zero;
}
