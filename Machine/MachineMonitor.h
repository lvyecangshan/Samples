#pragma once
#include "monitor.h"

class CMachineMonitor : public CMonitor
{
	enum MACHINE_STATUS
	{
		MACHINE_STOP = 0,
		MACHINE_RUN,
		MACHINE_ALARM
	};
public:
	CMachineMonitor();
	~CMachineMonitor();

protected:
	void MonitorAndAction();

private:
	BOOL m_bCheckStart;
	BOOL m_bCheckStop;
	BOOL m_bCheckAlarm;
};
