#ifndef __SYSTEM_METRICS_H__
#define __SYSTEM_METRICS_H__

#include <Windows.h>

struct SysMetricItem
{
	int iIndex;
	TCHAR* szLabel;
	TCHAR* szDesc;
}; 

class SysMetricItemManager
{
public:
	SysMetricItemManager();

	static SysMetricItemManager* GetInstance();
	
	int GetCount() const;
	const SysMetricItem& GetItem(int index) const;

private:
	// don't allow to copy
	SysMetricItemManager(const SysMetricItemManager&);
	SysMetricItemManager& operator=(const SysMetricItemManager&);
};

#endif//__SYSTEM_METRICS_H__
