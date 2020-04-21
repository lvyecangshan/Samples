#pragma once
#include "run.h"

#define RUN_MONTIOR_ID 1001

class __declspec(dllexport) CMonitor : public CRun
{
public:
	CMonitor(void);
	~CMonitor(void);

protected:
	virtual void MonitorAndAction();
	UINT RunThread();
};
