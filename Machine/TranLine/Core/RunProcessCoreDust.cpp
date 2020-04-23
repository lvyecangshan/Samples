/****************************************************************************************
�ļ�����CRunProcessCoreDust.h
������������о����
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessCoreDust.h"
#include "../../MachineCtrl.h"

CRunProcessCoreDust::CRunProcessCoreDust(int nID) : CRunProcessOnLoadOrOffPlate(nID)
{
	PowerUpRestart();
	
}


CRunProcessCoreDust::~CRunProcessCoreDust(void)
{
	
}
BOOL CRunProcessCoreDust::InitIO()
{	
	return TRUE;
}

void CRunProcessCoreDust::PowerUpRestart()
{
	CRunProcessOnLoadOrOffPlate::PowerUpRestart();
	
}

void CRunProcessCoreDust::PowerUpOperation(void)
{
	CRunProcessOnLoadOrOffPlate::PowerUpOperation();
}

void CRunProcessCoreDust::AutoOperation()
{
	return CRunProcessOnLoadOrOffPlate::AutoOperation();
}

BOOL CRunProcessCoreDust::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessCoreDust::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessCoreDust::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessCoreDust::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}


void CRunProcessCoreDust::AutoOperationFeed()
{
	BOOL b = m_bFeedPlate;
	switch (m_nNextAutoStepEx)
	{
	case AUTO_WORK_STAR:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("��ʼ����");
			m_dwStarDust = GetTickCount();

			m_nNextAutoStepEx = AUTO_WAIT_DUST_END;
			break;
		}
	case AUTO_WAIT_DUST_END:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("���ڳ���");
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

BOOL CRunProcessCoreDust::GetNextNeedSingal()
{
	//�������ϸ�Ӧ�����Ƿ�Ҫ��
	return TRUE;
}



