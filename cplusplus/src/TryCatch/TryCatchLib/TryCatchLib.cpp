// TryCatchLib.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "TryCatchLib.h"


// ���ǵ���������һ��ʾ��
TRYCATCHLIB_API int nTryCatchLib=0;

// ���ǵ���������һ��ʾ����
TRYCATCHLIB_API int fnTryCatchLib(void)
{
    return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� TryCatchLib.h
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
