#pragma once

//
// includes
//
#include "RunTask.h"
#include "Input.h"
#include "Output.h"

//
// CCheckSwitch: check system switch thread class
//  only check inputs on/off edge, no process should be put here, should all be processed by CRunCtrl
//
class CCheckSwitch : public CRunTask  
{
// message send or not record struct
	struct stMSG_SEND
	{
		BOOL	bOnSend;
		BOOL	bOffSend;
	};

// constructor & destructors
public:
	CCheckSwitch(PVOID pRunCtrl);
	virtual ~CCheckSwitch();

// public interface
public:
	// assign the input pointer to a perticular switch
	void SetSystemInput(int nSwitchNo, CInput *cipInputpt);

	// for other class (especially runctrl) to get a perticular input's pointer
	CInput* GetSystemInput(int nSwitchNo);

	//	Set door switch, curtain and engineering switch warn to not send before, so that
	//   if their waring(like door open, curtain still blocked) can be send again
	void ResetSwitchWarning();

// overrides
public:
	UINT RunThread(void);

private:
	void CheckSwitch(int nSwitchNo, CInput *cipSwitch, stMSG_SEND &stMsgSend/*BOOL &bSwitchCurrentState*/, BOOL &bSendBefore);
	void CheckSwitch(int nSwitchNo, CInput *cipSwitch,  stMSG_SEND &stMsgSend);//BOOL &bSwitchCurrentState);

// private members
private:
	// inputs pointers
	CInput	*m_pInputs[NUM_OF_SYSTEM_INPUT];
	
	// flags to record inputs' last status
	stMSG_SEND	m_stMsgSend[NUM_OF_SYSTEM_INPUT];

	// pointer to runctrl, which will receive the switch down/up message and do processing
	VOID *m_pRunCtrl;
};