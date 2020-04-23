#include "StdAfx.h"
#include "RunOnload.h"
#include "BLMessageBox.h"
#include "GeneralHelper.h"
CRunOnload::CRunOnload(void) : CRun(MACHINE_RUNS::RUN_ONLOAD)
{
	PowerUpRestart();
	m_strName = _T("RunOnload");
}

CRunOnload::~CRunOnload(void)
{
}

void CRunOnload::PowerUpRestart()
{
	CRun::PowerUpRestart();

	EVENTS[EVT_ONLOAD_READY]->Reset();

	m_nNextPowerUpStep = PU_RUN_HOME;
	m_nNextAutoStep = AUTO_MOVE_UP;
}

void  CRunOnload::PowerUpOperation(void)
{
	switch(m_nNextPowerUpStep)
	{
		case PU_RUN_HOME:
			{
				m_strMsg = _T("Home Starting...");
				if(IoAction(INPUTS[I_ONLOAD_MAG_LOCK],OFF,OUTPUTS[O_ONLOAD_MAG_LOCK_CLY],OFF) != WAIT_OK)
					break;
				int nResult = IoMotorMove(INPUTS[I_ONLOAD_DOWN_LIMIT],OUTPUTS[O_ONLOAD_MOVE_DOWN],ON,NULL,OUTPUTS[O_ONLOAD_BREAKER]);
				if(nResult != WAIT_OK)
				{
					if(nResult != WAIT_STOP)
						BLMessageBox(LoadWarnMessage(MSG_ONLOAD_MOVE_DOWN_TIMEOUT), MB_ICONSTOP);
					break;
				}
				m_bMagFirstMoveUp = TRUE;
				m_nNextPowerUpStep = PU_FINISH;
				CRun::PowerUpOperation();
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

void CRunOnload::AutoOperation()
{
	switch(m_nNextAutoStep)
	{
	case AUTO_MOVE_UP:
		{
			m_strMsg = _T("Up moving...");
			if(IoAction(INPUTS[I_ONLOAD_MAG_LOCK],ON,OUTPUTS[O_ONLOAD_MAG_LOCK_CLY],ON) != WAIT_OK)
					break;
			int nOldTimeout = INPUTS[I_ONLOAD_STRIP_PRESENT]->m_dwTimeout;
			if(m_bMagFirstMoveUp)
				INPUTS[I_ONLOAD_STRIP_PRESENT]->m_dwTimeout = MACHINE_TIME[TIME_ONLOAD_UP];
			int nResult = IoMotorMove(INPUTS[I_ONLOAD_STRIP_PRESENT],
				OUTPUTS[O_ONLOAD_MOVE_UP],ON,NULL,OUTPUTS[O_ONLOAD_BREAKER],INPUTS[I_ONLOAD_UP_LIMIT]);
			if(nResult != WAIT_OK)
			{
				if(nResult != WAIT_STOP)
					BLMessageBox(LoadWarnMessage(MSG_ONLOAD_MOVE_UP_TIMEOUT), MB_ICONSTOP);
				break;
			}
			INPUTS[I_ONLOAD_STRIP_PRESENT]->m_dwTimeout = nOldTimeout;
			if(INPUTS[I_ONLOAD_STRIP_PRESENT]->IsOff() && INPUTS[I_ONLOAD_UP_LIMIT]->IsOn())
				m_nNextAutoStep = AUTO_SUPPLY_FINISHED;
			else
			{
				EVENTS[EVT_ONLOAD_READY]->Set();
				m_nNextAutoStep = AUTO_WAIT_PICK_END;
			}
			break;
		}
	case AUTO_WAIT_PICK_END:
		m_strMsg = _T("wait for pick end...");
		m_bMagFirstMoveUp = FALSE;
		if(WaitEvent(EVENTS[EVT_PICK_END]) != WAIT_OK)
			break;
		EVENTS[EVT_PICK_END]->Reset();
		m_nNextAutoStep = AUTO_MOVE_UP;
		break;
	case AUTO_SUPPLY_FINISHED:
		{
			m_strMsg = _T("Onload supply end.");
			int nResult = IoMotorMove(INPUTS[I_ONLOAD_DOWN_LIMIT],
				OUTPUTS[O_ONLOAD_MOVE_DOWN],ON,NULL,OUTPUTS[O_ONLOAD_BREAKER]);
			if(nResult != WAIT_OK)
			{
				if(nResult != WAIT_STOP)
					BLMessageBox(LoadWarnMessage(MSG_ONLOAD_MOVE_DOWN_TIMEOUT), MB_ICONSTOP);
				break;
			}
			m_nNextAutoStep = AUTO_EXCHANGE_MAG;
			break;
		}
	case AUTO_EXCHANGE_MAG:
		m_strMsg = _T("Onload exchange mag...");
		if(IoAction(INPUTS[I_ONLOAD_MAG_LOCK],OFF,OUTPUTS[O_ONLOAD_MAG_LOCK_CLY],OFF) != WAIT_OK)
			break;
		m_nNextAutoStep = AUTO_MOVE_UP;
		m_bMagFirstMoveUp = TRUE;
		break;
		default:
			ASSERT(0);
			break;
	}

	return;
}


