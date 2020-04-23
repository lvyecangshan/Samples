#include "StdAfx.h"
#include "MachineMonitor.h"

CMachineMonitor::CMachineMonitor()
{
	m_bCheckStart = TRUE;
	m_bCheckStop = TRUE;
	m_bCheckAlarm = TRUE;
}

CMachineMonitor::~CMachineMonitor()
{
}

void CMachineMonitor::MonitorAndAction()
{
	int nMcStatus = -1;
	switch (MACHINE.m_pRunCtrl->GetMcState())
	{
	case MC_BEGIN:
	case MC_STOP_INIT:
	case MC_INITIALIZED:
	case MC_STOP_RUN:
		{
			nMcStatus = MACHINE_STOP;
			break;
		}
	case MC_INITIALIZING:
	case MC_RUNNING:
		{
			MACHINE.m_bMachineAlarm = FALSE;
			nMcStatus = MACHINE_RUN;
			break;
		}
	case MC_INIT_ERR:
	case MC_RUN_JAM:
		{
			MACHINE.m_bMachineAlarm = TRUE;
			nMcStatus = MACHINE_ALARM;
			break;
		}
	default:
		{
			break;
		}
	}

	MACHINE.DeleteMesData();
	MACHINE.DeleteMesInLog();
	MACHINE.DeleteMesOutLog();

	if(nMcStatus == MACHINE_ALARM && !MACHINE.m_bManualAuto)
	{
		MACHINE.m_bManualAuto = TRUE;
	}

	if(nMcStatus == MACHINE_RUN)
	{
		if(MACHINE.m_nIStop > -1 && INPUTS[MACHINE.m_nIStop]->IsOn())
		{
			MACHINE.m_bManualAuto = TRUE;
		}

		for (int i = 0; i < MACHINE.m_numModules; i++)
		{
			if (MACHINE.m_pModules[i] != NULL)
			{
				if (MACHINE.m_pModules[i]->m_strProcess == _T("MES"))
				{
					((CRunProcessMES*)MACHINE.m_pModules[i])->CheckSafeDoor();
				}
			}
		}
	}


	if (MACHINE_ALARM == nMcStatus)
	{
		if (MACHINE.m_bEnableAndon)
		{
			m_bCheckStart = TRUE;
			m_bCheckStop = TRUE;

			if(m_bCheckAlarm)
			{
				m_bCheckAlarm = FALSE;
				MACHINE.AndonLight(MACHINE.m_strAndonResource,_T("R"));
				MACHINE.AndonMachineStatus(MACHINE.m_strAndonResource,11);
				MACHINE.m_DlgAndon.ShowWindow(SW_SHOW);
				MACHINE.m_dwAndonShowTime = GetTickCount();
			}
		}
	}

	if(nMcStatus == MACHINE_STOP)
	{
		if (MACHINE.m_bEnableAndon)
		{
			m_bCheckStart = TRUE;
			m_bCheckAlarm = TRUE;
			if(m_bCheckStop)
			{
				m_bCheckStop = FALSE;
				MACHINE.AndonLight(MACHINE.m_strAndonResource,_T("Y"));
				MACHINE.AndonMachineStatus(MACHINE.m_strAndonResource,45);
				MACHINE.AndonMachineTotal(MACHINE.m_strAndonResource,MACHINE.m_nOnloadCount,MACHINE.m_nOffloadCount,MACHINE.m_nOnloadCount-MACHINE.m_nNgCount,MACHINE.m_nNgCount);
				MACHINE.m_DlgAndon.ShowWindow(SW_SHOW);
				MACHINE.m_dwAndonShowTime = GetTickCount();
			}
		}
	}

	if(nMcStatus == MACHINE_RUN)
	{
		if (MACHINE.m_bEnableAndon)
		{
			m_bCheckStop = TRUE;
			m_bCheckAlarm = TRUE;
			if(m_bCheckStart)
			{
				m_bCheckStart = FALSE;
				MACHINE.AndonLight(MACHINE.m_strAndonResource,_T("G"));
				MACHINE.AndonMachineStatus(MACHINE.m_strAndonResource,100);
			}
		}
	}

	if (MACHINE.m_nGreenLight > -1 && MACHINE.m_nRedLight > -1 && MACHINE.m_nYellowLight > -1 &&MACHINE.m_nLighthouseBuzzer > -1) 
	{
		if (MACHINE.m_bBuzzer && nMcStatus!=MACHINE_ALARM)  //三色灯闪烁提示
		{
			OUTPUTS[MACHINE.m_nRedLight]->IsOff()?OUTPUTS[MACHINE.m_nRedLight]->On():OUTPUTS[MACHINE.m_nRedLight]->Off();
			OUTPUTS[MACHINE.m_nGreenLight]->IsOff()?OUTPUTS[MACHINE.m_nGreenLight]->On():OUTPUTS[MACHINE.m_nGreenLight]->Off();
			OUTPUTS[MACHINE.m_nLighthouseBuzzer]->IsOn()?OUTPUTS[MACHINE.m_nLighthouseBuzzer]->Off():OUTPUTS[MACHINE.m_nLighthouseBuzzer]->On();
		}
	}

	if (MACHINE.m_nTowerGreen > -1 && MACHINE.m_nTowerRed > -1 && MACHINE.m_nTowerBuzzer > -1)
	{
		if (MACHINE.m_bBuzzer && nMcStatus!=MACHINE_ALARM)  //三色灯闪烁提示
		{
			OUTPUTS[MACHINE.m_nTowerRed]->IsOff()?OUTPUTS[MACHINE.m_nTowerRed]->On():OUTPUTS[MACHINE.m_nTowerRed]->Off();
			OUTPUTS[MACHINE.m_nTowerGreen]->IsOff()?OUTPUTS[MACHINE.m_nTowerGreen]->On():OUTPUTS[MACHINE.m_nTowerGreen]->Off();
			OUTPUTS[MACHINE.m_nTowerBuzzer]->IsOn()?OUTPUTS[MACHINE.m_nTowerBuzzer]->Off():OUTPUTS[MACHINE.m_nTowerBuzzer]->On();
			Sleep(50);
		}
	}

	if (MACHINE.m_pRobotOffload!=NULL)
	{
		MACHINE.m_pRobotOffload->NomalRunEvent();  //给后段正常运行信号
	}
}