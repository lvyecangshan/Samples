/****************************************************************************************
�ļ�����RunProcessOnLoadOrOffPlate.h
���������������Ϲ�λ
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessOnLoadOrOffPlate.h"

CRunProcessOnLoadOrOffPlate::CRunProcessOnLoadOrOffPlate(int nID) : CRunProcess(nID)
{
	PowerUpRestart();
	m_bOnLoadMode = TRUE;
	InitIO();
	m_Motor.InitMotorThread(); //��ʼ������߳�
}

CRunProcessOnLoadOrOffPlate::~CRunProcessOnLoadOrOffPlate(void)
{
	
}

//IO��ʼ��
BOOL CRunProcessOnLoadOrOffPlate::InitIO()
{	
	m_Plate.SetParent(this);
    m_Plate.AddOnOrOffLoadPlate(_T("IDamperPush"),_T("IDamperReturn"),_T("IJackingPush"),_T("IJackingReturn"),_T("IHasPlate"),_T("IHasCore"),_T("ICheckDirection"),
		                        _T("ODamperReturn"),_T("OJackingPush"),_T("OJackingReturn"),_T("IHasPlate2"),_T("IHasCore2"),_T("ICheckDirection2"),
								_T("IFullCheck0"),_T("IFullCheck1"));
	m_Motor.SetParent(this);
	m_Motor.AddMotor(_T("OLinemotor0"),_T("OLinemotor1"),_T("OLinemotor2"));

	return TRUE;
}

//��������ģʽ
void CRunProcessOnLoadOrOffPlate::SetOnLoadMode()
{
	m_bOnLoadMode = TRUE;
}
BOOL CRunProcessOnLoadOrOffPlate::IsOnLoadMode()
{
	return m_bOnLoadMode;
}
//��������ģʽ
void CRunProcessOnLoadOrOffPlate::SetOffLoadMode()
{
	m_bOnLoadMode = FALSE;
}
BOOL CRunProcessOnLoadOrOffPlate::IsOffLoadMode()
{
	return !m_bOnLoadMode;
}



void CRunProcessOnLoadOrOffPlate::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_Motor.PowerUpRestart();
	m_bRequirePallet = FALSE;
	m_nNextPowerUpStep = PU_WAIT_RESET;
	m_nNextAutoStep = AUTO_WORK_STAR;	
	m_bNeedPlate = FALSE;
	m_bFeedPlate = FALSE; 
	m_bFeedEnd   = TRUE;   
}

void CRunProcessOnLoadOrOffPlate::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		return;
	}
	APP_NoHardware_Sleep();
	if(IsOnLoadMode())
	{
		PowerUpOperationOnLoad();
	}
	else if(IsOffLoadMode())
	{
		PowerUpOperationOffLoad();
	}
}

void CRunProcessOnLoadOrOffPlate::PowerUpOperationOnLoad(void)
{
	switch(m_nNextPowerUpStep)
	{
		//�ȴ���ʼ�����ź�
	case PU_WAIT_RESET:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("��ʼ��ʼ��");
			if(m_Plate.DamperPush() && m_Plate.JackingReturn())
			{
				if(!m_Plate.NoPlate(0))
				{
					APP_BLMessageBox(_T("����λ���ϣ�������"));					
				}
				else
				{
					m_nNextPowerUpStep = PU_FINISH;
				}				
			}			
			break;
		}	
		//��ʼ�����
	case PU_FINISH:
		{
			CRun::PowerUpOperation();
			m_strMsg = m_bLangulageEng?_T(""):_T("��ʼ�����");
			m_nNextAutoStep = AUTO_WORK_STAR;
			break;
		}
	default:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("δ���嶯��");
			break;
		}
	}

}
void CRunProcessOnLoadOrOffPlate::PowerUpOperationOffLoad(void)
{
	switch(m_nNextPowerUpStep)
	{
		//�ȴ���ʼ�����ź�
	case PU_WAIT_RESET:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("��ʼ��ʼ��");
			if(m_Plate.DamperPush() && m_Plate.JackingReturn())
			{
				if(!m_Plate.NoCore(0))
				{
					APP_BLMessageBox(_T("����λ�е�о��������"));					
				}
				else
				{
					m_nNextPowerUpStep = PU_FINISH;
				}				
			}			
			break;
		}	
		//��ʼ�����
	case PU_FINISH:
		{
			CRun::PowerUpOperation();
			m_strMsg = m_bLangulageEng?_T(""):_T("��ʼ�����");
			m_nNextAutoStep = AUTO_WORK_STAR;
			break;
		}
	default:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("δ���嶯��");
			break;
		}
	}

}

void CRunProcessOnLoadOrOffPlate::AutoOperation()
{
	if (!IsModuleEnable())
	{
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		Sleep(500);
		return;
	}
	APP_NoHardware_Sleep();
	if(IsOnLoadMode())
	{
		AutoOperationOnLoad();
	}
	else if(IsOffLoadMode())
	{
		AutoOperationOffLoad();
	}	
}
void CRunProcessOnLoadOrOffPlate::AutoOperationOnLoad()
{
	switch(m_nNextAutoStep)
	{
		//��ʼ����
	case AUTO_WORK_STAR:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("��ʼ����");

			if(IsNoHardware() || m_Plate.DamperPush() && m_Plate.JackingReturn())
			{
				if(IsNoHardware() || m_Plate.GetCurPlateCount(0) < m_Plate.GetPlateCount(0))
				{
					//����Ҫ���ź�
					if(SetNeedSingal(TRUE))
					{
						m_nNextAutoStep = AUTO_WAIT_PLATE_ARRIVED;
					}				
				}
				else
				{
					m_nNextAutoStep = AUTO_CHECK_HAS_CORE;
				}
			}			
			break;
		}
	case AUTO_WAIT_PLATE_ARRIVED:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�ȴ����̵���");
			
			if(IsNoHardware() || m_bDryRun)
			{
				Sleep(1000);
			}
			
			if(IsNoHardware() || m_bDryRun || m_Plate.HasPlate(0))
			{
				SetNeedSingal(FALSE);
				m_nNextAutoStep = AUTO_CHECK_HAS_CORE;
			}
			break;
		}
	case AUTO_CHECK_HAS_CORE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("����Ƿ��е�о");
			if(IsNoHardware() || m_bDryRun || m_Plate.HasCore(0))
			{
				m_nNextAutoStep = AUTO_JACKING_PLATE;
			}
			else
			{
				m_nNextAutoStep = ATUO_WAIT_NEXT_NEED_PLATE_SINGAL;
			}
			break;
		}
	case AUTO_JACKING_PLATE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("��������");
			if(IsNoHardware() || m_Plate.JackingPush(0))
			{
				//����׼�����ź�
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
			if(m_bShowWaitFeedSingal)
			{
				m_strMsg = m_bLangulageEng?_T(""):_T("�ȴ��������");	
			}
					
			AutoOperationFeed();
			if(!GetFeedSingal())
			{
				SetFeedSingal(FALSE);
				m_nNextAutoStep = ATUO_WAIT_NEXT_NEED_PLATE_SINGAL;
			}
			break;
		}
	case ATUO_WAIT_NEXT_NEED_PLATE_SINGAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�ȴ���һģ��Ҫ���ź�");
			if(!m_Plate.IsMaterialFull() && GetNextNeedSingal())
			{
				if(IsNoHardware() || (m_Plate.JackingReturn(0) && m_Plate.DamperReturn(0)))
				{
					m_nNextAutoStep = ATUO_WAIT_FOLLOW_PLATE;
				}
			}
			break;
		}
	case ATUO_WAIT_FOLLOW_PLATE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�����뿪");
			if(IsNoHardware() || m_bDryRun || m_Plate.NoPlate(0))
			{
				OnPlateLeaved();
				if(IsNoHardware() || m_Plate.DamperPush(0))
				{
					m_nNextAutoStep = AUTO_WORK_END;
				}
			}
		}
		//�������
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�������");
			m_nNextAutoStep = AUTO_WORK_STAR;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}
void CRunProcessOnLoadOrOffPlate::AutoOperationOffLoad()
{
	switch(m_nNextAutoStep)
	{
		//��ʼ����
	case AUTO_WORK_STAR:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("��ʼ����");
			if(IsNoHardware() && !m_Plate.NoCore(0))  //�е�о��ֱ��������һģ��
			{
				m_nNextAutoStep = ATUO_WAIT_NEXT_NEED_PLATE_SINGAL;
				break;
			}
			if(IsNoHardware() || (m_Plate.DamperPush() && m_Plate.JackingReturn()))
			{
				if(IsNoHardware() || m_bDryRun || (m_Plate.GetCurPlateCount(0) < m_Plate.GetPlateCount(0)))
				{
					//����Ҫ�������ź�
					if(SetNeedSingal(TRUE))
					{
						m_nNextAutoStep = AUTO_WAIT_PLATE_ARRIVED;
					}				
				}
				else
				{
					m_nNextAutoStep = AUTO_CHECK_NO_CORE;
				}
			}			
			break;
		}
	case AUTO_WAIT_PLATE_ARRIVED:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�ȴ������̵���");
			if(IsNoHardware() || m_bDryRun || m_Plate.HasPlate(0))
			{
				SetNeedSingal(FALSE);
				m_nNextAutoStep = AUTO_CHECK_NO_CORE;
			}			
			break;
		}
	case AUTO_CHECK_NO_CORE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("����޵�о");
			if(IsNoHardware() || m_bDryRun || m_Plate.NoCore(0))
			{
				m_nNextAutoStep = AUTO_JACKING_PLATE;
			}
			else
			{
				m_nNextAutoStep = ATUO_WAIT_NEXT_NEED_PLATE_SINGAL;
			}
			break;
		}
	case AUTO_JACKING_PLATE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("��������");
			if(IsNoHardware() || m_Plate.JackingPush(0))
			{
				//����׼�����ź�
				if(SetFeedSingal(TRUE))
				{
					m_nNextAutoStep = AUTO_WAIT_FEED_END_SINGAL;
				}
			}
			break;
		}
	case AUTO_WAIT_FEED_END_SINGAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�ȴ��������");
			m_nNextAutoStepEx = 0;
			AutoOperationFeed();
			if(!GetFeedSingal())
			{
				m_nNextAutoStep = ATUO_WAIT_NEXT_NEED_PLATE_SINGAL;
			}
			break;
		}
	case ATUO_WAIT_NEXT_NEED_PLATE_SINGAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�ȴ���һģ��Ҫ���ź�");
			if(GetNextNeedSingal())
			{
				if(IsNoHardware() || (m_Plate.JackingReturn(0) && m_Plate.DamperReturn(0)))
				{
					m_nNextAutoStep = ATUO_WAIT_FOLLOW_PLATE;
				}
			}
			break;
		}
	case ATUO_WAIT_FOLLOW_PLATE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�����뿪");
			APP_Wait_Signal_Sleep();
			if(IsNoHardware() || m_bDryRun || m_Plate.NoPlate(0))
			{
				if(IsNoHardware() || m_Plate.DamperPush(0))
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
			m_nNextAutoStep = AUTO_WORK_STAR;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

BOOL CRunProcessOnLoadOrOffPlate::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessOnLoadOrOffPlate::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessOnLoadOrOffPlate::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessOnLoadOrOffPlate::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}
//���ϣ�����Ҫ��������ģ�飩
void CRunProcessOnLoadOrOffPlate::AutoOperationFeed()
{
	return ;
}
BOOL CRunProcessOnLoadOrOffPlate::HasCore()
{
	if(m_Plate.HasCore(-1))
	{
		return TRUE;
	}
	return FALSE;
}
void CRunProcessOnLoadOrOffPlate::OnPlateLeaved()
{

}

void CRunProcessOnLoadOrOffPlate::MotorRun()
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
