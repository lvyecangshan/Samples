/****************************************************************************************
�ļ�����RunProcessDamper.h
�����������赲������
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessDamper.h"

CRunProcessDamper::CRunProcessDamper(int nID) : CRunProcess(nID)
{
	PowerUpRestart();
	InitIO();
	
	m_Motor.InitMotorThread();
}

CRunProcessDamper::~CRunProcessDamper(void)
{
	//m_Motor.ReleaseMotorThread();
}
BOOL CRunProcessDamper::InitIO()
{

	m_Damper.SetParent(this);
	m_Damper.AddDamper(_T("IDamperPush"),_T("IDamperReturn"),_T("ODamperReturn"),_T("IHasPlate"),_T("ICheckDirection"),_T("IFullCheck0"),_T("IFullCheck1"));
	
	m_Motor.SetParent(this);
	m_Motor.AddMotor(_T("OLinemotor0"),_T("OLinemotor1"),_T("OLinemotor2"));
	
	return  TRUE;
	
}

void CRunProcessDamper::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_Motor.PowerUpRestart();
	m_nNextPowerUpStep = PU_SET_STOP_STATUS;
	m_nNextAutoStep = AUTO_SET_STOP_STATUS;
	m_nNextAutoStepEx = 0;

	m_bNeedPlate = FALSE;
	m_bFeedPlate = FALSE; 
	m_nNextAutoStepEx = 0; 
	
}

void CRunProcessDamper::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		return;
	}

	switch(m_nNextPowerUpStep)
	{
	case PU_SET_STOP_STATUS:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�����赲");
			if(IsNoHardware() || m_Damper.Push())
			{
				m_nNextPowerUpStep = PU_FINISH;
			}
			break;
		}
		//��ʼ�����
	case PU_FINISH:
		{
			CRun::PowerUpOperation();
			m_strMsg = m_bLangulageEng?_T("Power up finished"):_T("��ʼ�����");
			m_nNextAutoStep = AUTO_SET_STOP_STATUS;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

void CRunProcessDamper::AutoOperation()
{
	if (!IsModuleEnable())
	{
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		Sleep(500);
		return;
	}

	APP_NoHardware_Sleep();
	static DWORD dwTime = 0;

	switch(m_nNextAutoStep)
	{
		//����Ҫ���ź�
	case AUTO_SET_STOP_STATUS:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�����赲");
			if(IsNoHardware() || m_Damper.Push())
			{
				SetNeedSingal(TRUE);
				dwTime = GetTickCount();
				m_nNextAutoStep = AUTO_WAIT_PLATE_ARRIVE;
			}
			break;
		}
	case AUTO_WAIT_PLATE_ARRIVE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�ȴ����̵���");
			if(IsNoHardware() || m_bDryRun)
			{
				if(GetTickCount() - dwTime > 1000)
				{
					SetNeedSingal(FALSE);					
					if(SetFeedSingal(TRUE))
					{
						m_nNextAutoStepEx = 0;
						m_nNextAutoStep = AUTO_WAIT_FEED_END_SINGAL;
					}					
				}
			}
			else if(m_Damper.HasMaterial(0))
			{	
				SetNeedSingal(FALSE);
				APP_Wait_Signal_Sleep();
				if(SetFeedSingal(TRUE))
				{
					m_nNextAutoStepEx = 0;
					m_nNextAutoStep = AUTO_WAIT_FEED_END_SINGAL;
				}
				
			}			
			break;
		}
	case AUTO_WAIT_FEED_END_SINGAL:
		{
			AutoOperationFeed();
			if(!GetFeedSingal())
			{
				m_nNextAutoStepEx = 0;
				m_nNextAutoStep = ATUO_WAIT_NEXT_NEED_PLATE_SINGAL;
			}
			break;
		}
	case ATUO_WAIT_NEXT_NEED_PLATE_SINGAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�ȴ���һģ��Ҫ���ź�");
			if(!m_Damper.IsMaterialFull() && GetNextNeedSingal())
			{
				if(IsNoHardware() || (m_Damper.Return()))
				{
					m_nNextAutoStep = ATUO_WAIT_FOLLOW_PLATE;
				}
			}			
			break;
		}
	case ATUO_WAIT_FOLLOW_PLATE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�����뿪");
			if(IsNoHardware() || m_bDryRun || !m_Damper.HasMaterial(0))
			{
				OnPlateLeaved();
				if(IsNoHardware() || m_Damper.Return(0))
				{
					
					m_nNextAutoStep = AUTO_WORK_END;
				}
			}
			break;
		}
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("�������");
			m_nNextAutoStep = AUTO_SET_STOP_STATUS;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

void CRunProcessDamper::OnPlateLeaved()
{

}
void CRunProcessDamper::AutoOperationFeed(void)
{
	SetFeedSingal(FALSE);
}
BOOL CRunProcessDamper::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessDamper::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessDamper::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessDamper::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

void CRunProcessDamper::MotorRun()
{
	if (m_Motor.GetMachinState()==MC_RUNNING)
	{
		m_Motor.AllMotorStart();

		while(m_Motor.GetMachinState()==MC_RUNNING)
		{
			//�������ж�
			Sleep(10);
		}
	}
	else if (m_Motor.GetMachinState() == MC_STOP_RUN && m_Motor.CountTime())   //��ʱ1000msֹͣ
	{
		m_Motor.AllMotorStop();
	}
}