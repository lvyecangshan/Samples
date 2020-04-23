#include "StdAfx.h"
#include "Elevator.h"


CElevator::CElevator(CRunProcess *pParent)
{
	m_pParent = pParent;
}

CElevator::CElevator()
{
	m_pParent = NULL;
	m_pElevator = NULL;
}

//设置父句柄
void CElevator::SetParent(CRunProcess *pParent)
{
	m_pParent = pParent;

}


CElevator::~CElevator(void)
{
	if(m_pElevator)
	{
		delete m_pElevator;
		m_pElevator = NULL;
	}
	
}



BOOL CElevator::AddElevator(CString strILineSafeUp,CString strILineSafeDown,CString strIElevSafe,
							CString strIStopBackPush,CString strIStopBackReturn,
							CString strIHasPlate1,CString strIHasPlate2,
							CString strOStopBackReturn,CString strOForwardRun,CString strOReverseRun)
{
	ASSERT(m_pParent != NULL);

	if(m_pElevator != NULL)
	{
		return FALSE;
	}
	m_pElevator = new tagElevatorInfo();
	
	
	m_pElevator->strILineSafeUp		= strILineSafeUp;
	m_pElevator->strILineSafeDown		= strILineSafeDown;
	m_pElevator->strIElevSafe			= strIElevSafe;
	m_pElevator->strIStopBackPush		= strIStopBackPush;	
	m_pElevator->strIStopBackReturn	= strIStopBackReturn;
	m_pElevator->strIHasPlate1			= strIHasPlate1;
	m_pElevator->strIHasPlate2			= strIHasPlate2;
	m_pElevator->strOStopBackReturn	= strOStopBackReturn;	
	m_pElevator->strOForwardRun		= strOForwardRun;
	m_pElevator->strOReverseRun		= strOReverseRun;
	

	m_pParent->APP_Add_Input(&(m_pElevator->ILineSafeUp),m_pElevator->strILineSafeUp);
	m_pParent->APP_Add_Input(&(m_pElevator->ILineSafeDown),m_pElevator->strILineSafeDown);
	m_pParent->APP_Add_Input(&(m_pElevator->IElevSafe),m_pElevator->strIElevSafe);
	m_pParent->APP_Add_Input(&(m_pElevator->IStopBackPush),m_pElevator->strIStopBackPush);
	m_pParent->APP_Add_Input(&(m_pElevator->IStopBackReturn),m_pElevator->strIStopBackReturn);
	m_pParent->APP_Add_Input(&(m_pElevator->IHasPlate1),m_pElevator->strIHasPlate1);
	m_pParent->APP_Add_Input(&(m_pElevator->IHasPlate2),m_pElevator->strIHasPlate2);

	m_pParent->APP_Add_Output(&(m_pElevator->OStopBackReturn),m_pElevator->strOStopBackReturn);
	m_pParent->APP_Add_Output(&(m_pElevator->OForwardRun),m_pElevator->strOForwardRun);
	m_pParent->APP_Add_Output(&(m_pElevator->OReverseRun),m_pElevator->strOReverseRun);

	return TRUE;
}


//确认是否安全
BOOL CElevator::CheckWorkSafe()
{
	ASSERT(m_pParent != NULL && m_pElevator != NULL);
	if(IsNoHardware() || m_pParent->m_bDryRun)
	{
		return TRUE;
	}
	if(m_pParent->APP_INPUTS_IsOff(m_pElevator->ILineSafeUp)  ||     //线体料盘安全感应
		m_pParent->APP_INPUTS_IsOff(m_pElevator->ILineSafeDown) ||    //回流线体料盘安全感应
		m_pParent->APP_INPUTS_IsOff(m_pElevator->IElevSafe))       //升降料盘安全感应
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CElevator::HasPlate()
{
	ASSERT(m_pParent != NULL && m_pElevator != NULL);
	if(IsNoHardware() || m_pParent->m_bDryRun)
	{
		return TRUE;
	}
	if(m_pParent->APP_INPUTS_IsOff(m_pElevator->IHasPlate1)  ||     //线体料盘安全感应
		m_pParent->APP_INPUTS_IsOff(m_pElevator->IHasPlate2) ||    //回流线体料盘安全感应
		m_pParent->APP_INPUTS_IsOff(m_pElevator->IElevSafe))       //升降料盘安全感应
	{
		return FALSE;

	}
	return TRUE;

}
BOOL CElevator::NoPlate()
{
	ASSERT(m_pParent != NULL && m_pElevator != NULL);
	if(IsNoHardware() || m_pParent->m_bDryRun)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CElevator::ForwardRun()
{
	return m_pParent->APP_OUTPUTS_On(m_pElevator->OForwardRun);
}

BOOL CElevator::ForwardStop()
{
	return m_pParent->APP_OUTPUTS_Off(m_pElevator->OForwardRun);
}

BOOL CElevator::ReverseRun()
{
	return m_pParent->APP_OUTPUTS_On(m_pElevator->OReverseRun);
}

BOOL CElevator::ReverseStop()
{
	return m_pParent->APP_OUTPUTS_Off(m_pElevator->OReverseRun);
}