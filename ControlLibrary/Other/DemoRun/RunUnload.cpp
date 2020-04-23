#include "StdAfx.h"
#include "RunUnload.h"
#include "GeneralHelper.h"
#include "BLMessageBox.h"
CRunUnload::CRunUnload(void) : CRun(RUN_UNLOAD)
{
	PowerUpRestart();
	m_strName = _T("RunUnload");
}

CRunUnload::~CRunUnload(void)
{
}

void CRunUnload::PowerUpRestart()
{
	CRun::PowerUpRestart();

	EVENTS[EVT_UNLOAD_READY]->Reset();

	m_nNextPowerUpStep = PU_RUN_HOME;
	m_nNextAutoStep = AUTO_MOVE_UP;
}

void CRunUnload::PowerUpOperation(void)
{
	switch(m_nNextPowerUpStep)
	{
		case PU_RUN_HOME:
			{
				m_strMsg = _T("Home Starting...");
				if(IoAction(INPUTS[I_UNLOAD_MAG_LOCK],OFF,OUTPUTS[O_UNLOAD_MAG_LOCK_CLY],OFF) != WAIT_OK)
					break;
				int nResult = IoMotorMove(INPUTS[I_UNLOAD_DOWN_LIMIT],OUTPUTS[O_UNLOAD_MOVE_DOWN],ON,NULL,OUTPUTS[O_UNLOAD_BREAKER]);
				if(nResult != WAIT_OK)
				{
					if(nResult != WAIT_STOP)
						BLMessageBox(LoadWarnMessage(MSG_UNLOAD_MOVE_DOWN_TIMEOUT), MB_ICONSTOP);
					break;
				}
				m_nNextPowerUpStep = PU_FINISH;
				CRun::PowerUpOperation();
				STATUS(_T("Homing Done..."));
				break;
			}
		case PU_FINISH:
			break;
		default:
			ASSERT(0);
			break;
	}
	return;
}

void CRunUnload::AutoOperation()
{
	switch(m_nNextAutoStep)
	{
	case AUTO_MOVE_UP:
		{
			m_strMsg = _T("Up moving...");
			if(IoAction(INPUTS[I_UNLOAD_MAG_LOCK],ON,OUTPUTS[O_UNLOAD_MAG_LOCK_CLY],ON) != WAIT_OK)
					break;
			int nOldTimeout = INPUTS[I_UNLOAD_STRIP_PRESENT]->m_dwTimeout;
				INPUTS[I_UNLOAD_STRIP_PRESENT]->m_dwTimeout = MACHINE_TIME[TIME_UNLOAD_UP];
			int nResult = IoMotorMove(INPUTS[I_UNLOAD_STRIP_PRESENT],
				OUTPUTS[O_UNLOAD_MOVE_UP],ON,NULL,OUTPUTS[O_UNLOAD_BREAKER],INPUTS[I_UNLOAD_UP_LIMIT]);
			if(nResult != WAIT_OK)
			{
				if(nResult != WAIT_STOP)
					BLMessageBox(LoadWarnMessage(MSG_UNLOAD_MOVE_UP_TIMEOUT), MB_ICONSTOP);
				break;
			}
			INPUTS[I_UNLOAD_STRIP_PRESENT]->m_dwTimeout = nOldTimeout;
			m_nNextAutoStep = AUTO_MOVE_DOWN_STEP;
			break;
		}
	case AUTO_WAIT_PUT_END:
		m_strMsg = _T("wait for place end...");
		if(WaitEvent(EVENTS[EVT_PLACE_END]) != WAIT_OK)
			break;
		EVENTS[EVT_PLACE_END]->Reset();
		m_nNextAutoStep = AUTO_CHECK_MOVE_DOWN;
		break;
	case AUTO_CHECK_MOVE_DOWN:
		if(!::IsNoHardware() && INPUTS[I_UNLOAD_DOWN_LIMIT]->IsOn())
			m_nNextAutoStep = AUTO_EXCHANGE_MAG;
		else if(INPUTS[I_UNLOAD_STRIP_PRESENT]->IsOn())
			m_nNextAutoStep = AUTO_MOVE_DOWN_STEP;
		else
		{
			EVENTS[EVT_UNLOAD_READY]->Set();
			m_nNextAutoStep = AUTO_WAIT_PUT_END;
		}
		break;
	case AUTO_MOVE_DOWN_STEP:
		{
			m_strMsg = _T("Unload step moving down...");
			if( IoMotorMove(INPUTS[I_UNLOAD_STRIP_PRESENT],
				OUTPUTS[O_UNLOAD_MOVE_DOWN],OFF,NULL,OUTPUTS[O_UNLOAD_BREAKER],INPUTS[I_UNLOAD_DOWN_LIMIT])
				!= WAIT_OK)
			{
				m_strMsg = _T("Unload step moving down failed.");
				break;
			}
			if(!::IsNoHardware() && INPUTS[I_UNLOAD_DOWN_LIMIT]->IsOn())// or unload count equal onload
			{
				m_nNextAutoStep = AUTO_UNLOAD_FINISHED;
			}
			else
			{
				EVENTS[EVT_UNLOAD_READY]->Set();
				m_nNextAutoStep = AUTO_WAIT_PUT_END;
			}
			break;
		}
	case AUTO_UNLOAD_FINISHED:
		{
			m_strMsg = _T("Unload supply end.");
			if(IoMotorMove(INPUTS[I_UNLOAD_DOWN_LIMIT],
				OUTPUTS[O_UNLOAD_MOVE_DOWN],ON,NULL,OUTPUTS[O_UNLOAD_BREAKER])
				!= WAIT_OK)
			{
				m_strMsg = _T("Unload supply move down failed.");
				BLMessageBox(LoadWarnMessage(MSG_UNLOAD_MOVE_DOWN_TIMEOUT), MB_ICONSTOP);
				break;
			}
			m_nNextAutoStep = AUTO_EXCHANGE_MAG;
			break;
		}
	case AUTO_EXCHANGE_MAG:
		m_strMsg = _T("Unload exchange mag...");
		if(IoAction(INPUTS[I_UNLOAD_MAG_LOCK],OFF,OUTPUTS[O_UNLOAD_MAG_LOCK_CLY],OFF) != WAIT_OK)
			break;
		m_nNextAutoStep = AUTO_MOVE_UP;
		break;
	default:
			ASSERT(0);
			break;
	}

	return;
}

//BOOL CRunUnload::MoveDown(BOOL bStep)
//{
//	if(::IsNoHardware())
//		return TRUE;
//	if(INPUTS[I_UNLOAD_STRIP_PRESENT]->IsOff() || INPUTS[I_UNLOAD_DOWN_LIMIT]->IsOn())
//		return TRUE;
//	DWORD nTimeOut = bStep ? 1000 : INPUTS[I_UNLOAD_STRIP_PRESENT]->m_dwTimeout;
//	OUTPUTS[O_UNLOAD_BREAKER]->On();
//	OUTPUTS[O_UNLOAD_MOVE_DOWN]->On();
//	DWORD dwTimeStat = GetTickCount();
//	while((GetTickCount()-dwTimeStat) < nTimeOut)
//	{
//		if(INPUTS[I_UNLOAD_STRIP_PRESENT]->IsOff() || INPUTS[I_UNLOAD_DOWN_LIMIT]->IsOn())
//			break;
//		if(m_evtMcStop == ON)
//			break;
//		Sleep(1);
//	}
//	if(INPUTS[I_UNLOAD_STRIP_PRESENT]->IsOff() || INPUTS[I_UNLOAD_DOWN_LIMIT]->IsOn())
//		return TRUE;
//	if((GetTickCount()-dwTimeStat) > nTimeOut && m_evtMcStop == OFF)
//	{
//		//ShowMessage(
//	}
//	return FALSE;
//}