//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// central control module for hardware & system objects init/destroy
// last update: 2008/02/15
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once
#include "MachineTimeManager.h"
#include "ModuleManager.h"
#include "EventManager.h"
#include "RunCtrl.h"
#include "DeviceManager.h"
class __declspec(dllexport) CCentralControl
{
public:
	CCentralControl(void);
	virtual ~CCentralControl(void);

public:
	virtual BOOL Initialize(HWND hMsgProcessWnd,int nMotors, int nInputs, int nOutputs, int nEvents, int nMachineTimes,CString strModule = _T(""));
	virtual void UpdateLanguage();

	void ShowMaintenanceDialog(CWnd *pParentWnd, CRect rcDesired);

protected:
    virtual BOOL InitializeRunThreads(HWND hMsgProcessWnd);

public:
	// some main object
    CRunCtrl			*m_pRunCtrl;
	CMaintenance		*m_pMaintenance;
	CMonitor			*m_pMonitor;

protected:
	CString				m_strMachineStatus[NUM_MC_STATE];
};
