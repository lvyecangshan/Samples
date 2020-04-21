#include "StdAfx.h"
#include "RunPnPArm.h"

CRunPnPArm::CRunPnPArm(void) : CRun(RUN_PNP_ARM)
{
	PowerUpRestart();
	m_strName = _T("RunPnPArm");
}

CRunPnPArm::~CRunPnPArm(void)
{
}

void CRunPnPArm::PowerUpRestart()
{
	CRun::PowerUpRestart();

	m_nNextPowerUpStep = PU_ARM_Z_HOME;
	m_nNextAutoStep = AUTO_ARM_X_TO_ONLOAD;
}

void CRunPnPArm::PowerUpOperation(void)
{
	switch(m_nNextPowerUpStep)
	{
		case PU_ARM_Z_HOME:
			m_strMsg = _T("PnP Z axis Home Starting...");
			if(!HomeMotor(MOTORS[MOTOR_ARM_Z_AXIS]))
				break;
			m_nNextPowerUpStep = PU_ARM_X_HOME;
			break;
		case PU_ARM_X_HOME:
			m_strMsg = _T("PnP X axis Home Starting...");
			if(!HomeMotor(MOTORS[MOTOR_ARM_X_AXIS]))
				break;
            m_nNextPowerUpStep = PU_FINISH;
			break;
		case PU_FINISH:
			CRun::PowerUpOperation();
			m_strMsg = _T("Homing Done...");
			break;
		default:
			ASSERT(0);
			break;
	}
	return;
}

void CRunPnPArm::AutoOperation()
{
	switch(m_nNextAutoStep)
	{
	case AUTO_ARM_X_TO_ONLOAD:
		m_strMsg = _T("PnP arm to onload position...");
		if(!MoveMotor(MOTORS[MOTOR_ARM_X_AXIS],LOC_ARM_AT_ONLOAD))
				break;
		m_nNextAutoStep = AUTO_WAIT_FOR_ONLOAD;
		break;
	case AUTO_WAIT_FOR_ONLOAD:
		m_strMsg = _T("PnP arm wait onload ready...");
		if(WaitEvent(EVENTS[EVT_ONLOAD_READY]) != WAIT_OK)
			break;
		EVENTS[EVT_ONLOAD_READY]->Reset();
		m_nNextAutoStep = AUTO_ARM_Z_PICK_DOWN;
		break;
	case AUTO_ARM_Z_PICK_DOWN:
		m_strMsg = _T("PnP arm to pick down...");
		if(!MoveMotor(MOTORS[MOTOR_ARM_Z_AXIS],LOC_ARM_AT_DOWN))
				break;
		m_nNextAutoStep = AUTO_ARM_PICK_STRIP;
		break;
	case AUTO_ARM_PICK_STRIP:
		m_strMsg = _T("PnP arm pick strip...");
		if(IoAction(INPUTS[I_VACUUM_ON],ON,OUTPUTS[O_VACUUM_VALVE],ON) != WAIT_OK)
			break;
		m_nNextAutoStep = AUTO_ARM_Z_PICK_UP;
		break;
	case AUTO_ARM_Z_PICK_UP:
		m_strMsg = _T("PnP arm to pick up...");
		if(!MoveMotor(MOTORS[MOTOR_ARM_Z_AXIS],LOC_ARM_AT_UP))
				break;
		EVENTS[EVT_PICK_END]->Set();
		m_nNextAutoStep = AUTO_ARM_X_TO_UNLOAD;
		break;
	case AUTO_ARM_X_TO_UNLOAD:
		m_strMsg = _T("PnP arm to unload position...");
		if(!MoveMotor(MOTORS[MOTOR_ARM_X_AXIS],LOC_ARM_AT_UNLOAD))
				break;
		m_nNextAutoStep = AUTO_WAIT_FOR_UNLOAD;
		break;
	case AUTO_WAIT_FOR_UNLOAD:
		m_strMsg = _T("PnP arm wait unload ready...");
		if(WaitEvent(EVENTS[EVT_UNLOAD_READY]) != WAIT_OK)
			break;
		EVENTS[EVT_UNLOAD_READY]->Reset();
		m_nNextAutoStep = AUTO_ARM_Z_PLACE_DOWN;
		break;
		case AUTO_ARM_Z_PLACE_DOWN:
		m_strMsg = _T("PnP arm to place down...");
		if(!MoveMotor(MOTORS[MOTOR_ARM_Z_AXIS],LOC_ARM_AT_DOWN))
				break;
		m_nNextAutoStep = AUTO_ARM_RELASE_STRIP;
		break;
	case AUTO_ARM_RELASE_STRIP:
		m_strMsg = _T("PnP arm place strip...");
		if(IoAction(INPUTS[I_VACUUM_ON],OFF,OUTPUTS[O_VACUUM_VALVE],OFF) != WAIT_OK)
			break;
		m_nNextAutoStep = AUTO_ARM_Z_BACK_UP;
		break;
	case AUTO_ARM_Z_BACK_UP:
		m_strMsg = _T("PnP arm to back up...");
		if(!MoveMotor(MOTORS[MOTOR_ARM_Z_AXIS],LOC_ARM_AT_UP))
				break;
		m_nNextAutoStep = AUTO_ARM_X_TO_ONLOAD;
		EVENTS[EVT_PLACE_END]->Set();
		break;
		default:
			ASSERT(0);
			break;
	}

	return;
}
