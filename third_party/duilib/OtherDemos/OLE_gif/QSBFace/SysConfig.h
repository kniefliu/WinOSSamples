#ifndef _SYS_CONFIG_H
#define _SYS_CONFIG_H

#include "resource.h"

class SysConfig
{
public:
	SysConfig(){};
	~SysConfig(){};
public:
	static CDuiString GetSkinFolder()
	{
			#ifdef _DEBUG
					return _T("\\res\\QSBFaceRes\\");
			#else
					return _T("\\res\\");
			#endif  
	}
	static UILIB_RESOURCETYPE GetResourceType() 
	{
#ifdef _DEBUG
		return UILIB_FILE;
#else
		return UILIB_ZIPRESOURCE;
#endif
	}

	static LPCTSTR GetResourceID() 
	{
		return MAKEINTRESOURCE(IDR_ZIPRES);
	} 

	static  tString GetZIPFileName() 
	{
		return _T("QSBFaceRes.zip");
	} 

};

#endif