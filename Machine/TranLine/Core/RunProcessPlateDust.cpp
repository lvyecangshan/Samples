/****************************************************************************************
文件名：CRunProcessPlateDust.h
功能描述：电芯除尘
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessPlateDust.h"
#include "../../MachineCtrl.h"

CRunProcessPlateDust::CRunProcessPlateDust(int nID) : CRunProcessOnLoadOrOffPlate(nID)
{
	PowerUpRestart();
	InitIO();
}

CRunProcessPlateDust::~CRunProcessPlateDust(void)
{
	
}
BOOL CRunProcessPlateDust::InitIO()
{
	m_IDamperPush = -1;	
	m_IDamperReturn = -1;
	m_IJackingPush = -1;
	m_IJackingReturn = -1;
	m_IHasPlate01 = -1;
	m_IHasPlate02 = -1;
	m_ODamperReturn = -1;
	m_OJackingPush = -1;
	m_OJackingReturn = -1;
	m_ODustOpen=-1;


	APP_Add_Input(&m_IDamperPush,_T("IDamperPush"));
	APP_Add_Input(&m_IDamperReturn,_T("IDamperReturn"));
	APP_Add_Input(&m_IJackingPush,_T("IJackingPush"));
	APP_Add_Input(&m_IJackingReturn,_T("IJackingReturn"));
	APP_Add_Input(&m_IHasPlate01,_T("IHasPlate0"));
	APP_Add_Input(&m_IHasPlate02,_T("IHasPlate1"));

	APP_Add_Output(&m_ODamperReturn,_T("ODamperReturn"));
	APP_Add_Output(&m_OJackingPush,_T("OJackingPush"));
	APP_Add_Output(&m_OJackingReturn,_T("OJackingReturn"));
	APP_Add_Output(&m_ODustOpen,_T("ODustOpen"));

	return TRUE;
}




void CRunProcessPlateDust::PowerUpRestart()
{
	CRunProcessOnLoadOrOffPlate::PowerUpRestart();
	
}

void CRunProcessPlateDust::PowerUpOperation(void)
{

	CRunProcessOnLoadOrOffPlate::PowerUpOperation();
	
}

void CRunProcessPlateDust::AutoOperation()
{
	CRunProcessOnLoadOrOffPlate::AutoOperation();
}

BOOL CRunProcessPlateDust::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessPlateDust::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessPlateDust::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessPlateDust::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

void CRunProcessPlateDust::AutoOperationFeed()
{
	BOOL b = m_bFeedPlate;
	switch (m_nNextAutoStepEx)
	{
	case AUTO_WORK_STAR:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("开始除尘");
			m_dwStarDust = GetTickCount();

			m_nNextAutoStepEx = AUTO_WAIT_DUST_END;
			break;
		}
	case AUTO_WAIT_DUST_END:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("正在除尘");
			if(GetTickCount() - m_dwStarDust > 5000)
			{
				m_nNextAutoStepEx = AUTO_WORK_END;
			}
			break;
		}
	case AUTO_WORK_END:
		{
			SetFeedSingal(FALSE);
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

BOOL CRunProcessPlateDust::GetNextNeedSingal()
{
	//根据满料感应决定是否要料
	return TRUE;
}

