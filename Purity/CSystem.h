#ifndef __CSYSTEM_H__
#define __CSYSTEM_H__

#include "Purity.h"
#ifdef ENABLE_SYSTEM

class CSystem
{
private:
	THandler<CSystem>* m_sysSpecs;
	unsigned __int64 CycleCount();

public:
	CSystem(CHandler* m_chand);
	void specs(const msgBuffer *args);
	void __cpuid (int *CPUInfo, int InfoType);
	char *TimeStats(int iTime);
	void GetCPUInfo(char *CPUBrandString);
	int GetCpuSpeed();
};

#endif
#endif