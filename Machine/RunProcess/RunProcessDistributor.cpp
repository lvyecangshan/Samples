/****************************************************************************************
�ļ�����RunRunProcessDistributor.h
��������������������
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessDistributor.h"
#include "../MachineCtrl.h"

CRunProcessDistributor::CRunProcessDistributor(int nID) : CRunProcess(nID)
{
	PowerUpRestart();
	InitIO();
	m_Motor.InitMotorThread();
}

CRunProcessDistributor::~CRunProcessDistributor(void)
{

}

BOOL CRunProcessDistributor::InitIO()
{

	m_Distriutor.SetParent(this);
	m_Distriutor.AddDistributor(_T("IDistributorCylPush0"),_T("IDistributorCylReturn0"),_T("ODistributorCylReturn0"),_T("IHasMaterial0"),_T("ICheckDirection0"),_T("IFullCheck0"));
	m_Distriutor.AddDistributor(_T("IDistributorCylPush1"),_T("IDistributorCylReturn1"),_T("ODistributorCylReturn1"),_T("IHasMaterial1"),_T("ICheckDirection1"),_T("IFullCheck1"));
	
	m_Motor.SetParent(this);
	m_Motor.AddMotor(_T("OLinemotor0"),_T("OLinemotor1"),_T("OLinemotor2"));
	
	return  TRUE;

}

void CRunProcessDistributor::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_Motor.PowerUpRestart();
	m_nNextPowerUpStep = PU_SET_STOP_STATUS;
	m_nNextAutoStep = AUTO_SEND_STAR_SIGNAL;
	m_bFeedingMaterial = FALSE;
	m_bRequirePallet = FALSE;
}

void CRunProcessDistributor::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		return;
	}
	APP_NoHardware_Sleep();
	switch(m_nNextPowerUpStep)
	{
	case PU_SET_STOP_STATUS:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�����赲����");
			if(IsNoHardware() || (m_Distriutor.Return(0) && m_Distriutor.Push(1)))
			{
				m_nNextPowerUpStep = PU_FINISH;
			}
 			
			break;
		}
		//��ʼ�����
	case PU_FINISH:
		{
			CRun::PowerUpOperation();
			m_strMsg = m_bLangulageEng?_T(""):_T("��ʼ�����");
			m_nNextAutoStep = AUTO_SEND_STAR_SIGNAL;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

void CRunProcessDistributor::AutoOperation()
{
	if (!IsModuleEnable())
	{
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		Sleep(500);
		return;
	}

	APP_NoHardware_Sleep();
	
	switch(m_nNextAutoStep)
	{
		
	case AUTO_SEND_STAR_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("��ʼ�����ź�");
			APP_Wait_Signal_Sleep();
			if(IsNoHardware() || (m_Distriutor.Return(0) && m_Distriutor.Push(1)))
			{
				SetNeedSingal(TRUE);
				m_nNextAutoStep = AUTO_SEND_WAIT_PLATE;
			}
			break;
		}
	case AUTO_SEND_WAIT_PLATE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�ȴ����̵���");
			if(IsNoHardware() || m_bDryRun || CheckHasPlate())
			{
				if(IsNoHardware() || m_Distriutor.Push(0))
				{
					m_nNextAutoStep = AUTO_SEND_READY_SINGAL;
				}
				
			}
			break;
		}
	case AUTO_SEND_READY_SINGAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�ȴ������ź�");
			if(!m_Distriutor.IsMaterialFull() && GetNextNeedSingal())
			{
				if(m_Distriutor.Return(1))
				{
					m_nNextAutoStep = AUTO_WAIT_FEEDING_END;
				}
				
			}
			break;
		}
	case AUTO_WAIT_FEEDING_END:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("���ڷ���");
			if(IsNoHardware() || m_bDryRun || CheckNoPlate())
			{
				if(IsNoHardware() || m_Distriutor.Push(1))
				{
					m_nNextAutoStep = AUTO_WORK_END;
				}				
			}
			break;
		}
		//�������
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�������");
			APP_Wait_Signal_Sleep();
			m_nNextAutoStep = AUTO_SEND_STAR_SIGNAL;
			break;
		}

	default:
		{
			ASSERT(0);
			break;
		}
	}
}

BOOL CRunProcessDistributor::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessDistributor::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessDistributor::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessDistributor::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}


//����
BOOL CRunProcessDistributor::DistributorPlate()
{
	BOOL bRet = TRUE;

	return bRet;
}



//�����Ƿ�����
BOOL CRunProcessDistributor::CheckLineFull()
{
	BOOL bRet = APP_INPUTS_IsOn(m_ICheckLineFull);
	return bRet;
}
