// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� TRYCATCHLIB_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// TRYCATCHLIB_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef TRYCATCHLIB_EXPORTS
#define TRYCATCHLIB_API __declspec(dllexport)
#else
#define TRYCATCHLIB_API __declspec(dllimport)
#endif

// �����Ǵ� TryCatchLib.dll ������
class TRYCATCHLIB_API CTryCatchLib {
public:
	CTryCatchLib(void);
	// TODO:  �ڴ�������ķ�����
};

extern TRYCATCHLIB_API int nTryCatchLib;

TRYCATCHLIB_API int fnTryCatchLib(void);

TRYCATCHLIB_API void divideZero();
