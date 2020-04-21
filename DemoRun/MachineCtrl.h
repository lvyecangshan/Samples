#pragma once
#include "EasyLibdef.h"
#include "CentralControl.h"
#include "LogWrite.h"
#include "MachineDef.h"
class CMachineCtrl : public CCentralControl
{
public:
	CMachineCtrl(void);
	~CMachineCtrl(void);
public:
	static CMachineCtrl * GetInstance();
	static void Destroy();
	BOOL Initialize(HWND hMsgProcessWnd, int numMotor, int numInput, int numOutput, int numEvent, int numMachineTime);
public:
	//CUserManager		m_user;
	static BOOL     TestStart();
protected:
	static CMachineCtrl	*m_pInstance;
    BOOL InitializeRunThreads(HWND hMsgProcessWnd);
};
#define MACHINE (*CMachineCtrl::GetInstance())