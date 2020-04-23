/****************************************************************************************
�ļ�����RunProcessElevator.h
�����������������ͻ�
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessElevator.h"

CRunProcessElevator::CRunProcessElevator(int nID) : CRunProcess(nID)
{
	PowerUpRestart();
	InitIO();

	m_Motor.InitMotorThread();
}

CRunProcessElevator::~CRunProcessElevator(void)
{
	
}

//IO��ʼ��
BOOL CRunProcessElevator::InitIO()
{

	m_Elevator.SetParent(this);
	m_Elevator.AddElevator(_T("ILineSafeUp"),_T("ILineSafeDown"),_T("IElevSafe"),
							_T("IStopBackPush"),_T("IStopBackReturn"),_T("IHasPlate0"),_T("IHasPlate1"),
							_T("OStopBackReturn"),_T("OForwardRun"),_T("OReverseRun"));
	m_Motor.SetParent(this);
	m_Motor.AddMotor(_T("OLinemotor0"),_T("OLinemotor1"),_T("OLinemotor2"));

	//���
	APP_Add_Motor(&m_nMotorZ,_T("MotorZ"));
	
	
	return TRUE;
}

void CRunProcessElevator::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_Motor.PowerUpRestart();
	m_nNextPowerUpStep = PU_WAIT_STAR_SIGNAL;
	m_nNextAutoStep = AUTO_SEND_REQUIRE_SIGNAL;
	m_bPlateStatus[0] = FALSE;
	m_bPlateStatus[1] = FALSE;
	
}

void CRunProcessElevator::PowerUpOperation(void)
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
		//�ȴ���ʼ�����ź�
	case PU_WAIT_STAR_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�ȴ���ʼ�����ź�");
 			m_nNextPowerUpStep = PU_STOP_BELT;
			break;
		}
		//ֹͣƤ��ת��
	case PU_STOP_BELT:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("ֹͣƤ��ת��");
			if(IsNoHardware() || (StopReceiveMotor() && StopFeedingMotor()))  //���²�Ƥ��ֹͣ����
			{
				m_nNextPowerUpStep = PU_CHECK_SAFE;
			}
			break;
		}
		//����Ƿ�ȫ
	case PU_CHECK_SAFE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("����Ƿ�ȫ");
			if(IsNoHardware() || m_Elevator.CheckWorkSafe())  //ȷ�������ȫ
			{
				m_nNextPowerUpStep = PU_OPERATION_BEFORE_MOVE;
			}
			break;
		}
	case PU_OPERATION_BEFORE_MOVE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("ֹ�������Ƴ�");
			if(IsNoHardware() || CylinderPush())
			{
				m_nNextPowerUpStep = PU_TO_RECEIVE_MATERIAL_POSTION;
			}
			
			break;
		}
		//Z�����ƶ�������λ
	case PU_TO_RECEIVE_MATERIAL_POSTION:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("Z�����ƶ�������λ");
			APP_Wait_Signal_Sleep();
			if(IsNoHardware())
			{
				m_nNextPowerUpStep = PU_FINISH;
			}
			else 
			{
				if(CylinderReturn() && MoveToReceiveMaterialPostion())
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
			m_nNextAutoStep = AUTO_CHECK_WORK_SAFE;
			break;
		}
	default:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("δ���嶯��");
			break;
		}
	}

}

void CRunProcessElevator::AutoOperation()
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
		//��ⰲȫ
	case AUTO_CHECK_WORK_SAFE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("��ⰲȫ");
			APP_Wait_Signal_Sleep();
			if(IsNoHardware())
			{
				m_nNextAutoStep = AUTO_TO_RECEIVE_MATERIAL_POSTION;
			}
			else
			{
				if(StartReceiveMotor() && StopFeedingMotor()
					&& m_Elevator.CheckWorkSafe() && CylinderPush())  
				{
					m_nNextAutoStep = AUTO_TO_RECEIVE_MATERIAL_POSTION;
				}
			}
			break;
		}
		//Z���ƶ�������λ
	case AUTO_TO_RECEIVE_MATERIAL_POSTION:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("Z���ƶ�������λ");
			APP_Wait_Signal_Sleep();
			if(IsNoHardware())
			{
				m_nNextAutoStep = AUTO_SEND_REQUIRE_SIGNAL;
			}
			else if(MoveToReceiveMaterialPostion() && CylinderReturn())
			{
				m_nNextAutoStep = AUTO_SEND_REQUIRE_SIGNAL;
			}
			if(IsNoHardware() || m_bDryRun)
			{
				m_bSendNeedCount = 2 ;
			}
			else
			{
				m_bSendNeedCount = 2 - GetCurPlateCount() ;
			}
			
			break;
		}
		//����Ҫ���ź�
	case AUTO_SEND_REQUIRE_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("����Ҫ���ź�");
			APP_Wait_Signal_Sleep();
			int nPlateCount = GetCurPlateCount();
			if(IsNoHardware() || m_bDryRun)  
			{
				if(m_bSendNeedCount <= 0)  //����
				{
					SetNeedSingal(FALSE);
					if(IsNoHardware() || CylinderPush())
					{
						m_nNextAutoStep = AUTO_SEND_RECEIVE_END;
					}
				}
				else 
				{
					if(SetNeedSingal(TRUE))
					{	
						//�������
						if(IsNoHardware() || StartReceiveMotor())
						{
							m_nNextAutoStep = AUTO_START_RECEIVE_PLATE;
						}
					}
				}			
			}
			else   //��������
			{
				if(nPlateCount < 2)
				{
					if(SetNeedSingal(TRUE))
					{	
						//�������
						if(IsNoHardware() || StartReceiveMotor())
						{
							m_nNextAutoStep = AUTO_START_RECEIVE_PLATE;
						}
					}
				}
				else
				{
					m_nNextAutoStep = AUTO_SEND_RECEIVE_END;
				}

			}
			
			
			break;
		}
	case AUTO_START_RECEIVE_PLATE:
		{			
			if(IsNoHardware() || m_bDryRun)
			{
				m_strMsg = m_bLangulageEng?_T(""):_T("��ʼ��������");
				if(!GetNeedSingal())
				{
					m_nNextAutoStep = AUTO_SEND_REQUIRE_SIGNAL;
				}
				else
				{
					m_bSendNeedCount --;
				}
			}
			else
			{
				if(GetCurPlateCount() < 2)   //�ȴ����̵���
				{
					m_nNextAutoStep = AUTO_SEND_REQUIRE_SIGNAL;
				}	
				else
				{
					m_nNextAutoStep = AUTO_SEND_RECEIVE_END;
				}
			}			
			
			break;
		}
		//���̽������
	case  AUTO_SEND_RECEIVE_END:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("���̽������");
			APP_Wait_Signal_Sleep();
			if(IsNoHardware())
			{
				m_nNextAutoStep = AUTO_TO_FEEDING_POSITION;
				SendReceiveEnd();
			}
			else
			{
				if(m_Elevator.CheckWorkSafe() && CylinderPush() && StopReceiveMotor())
				{
					m_nNextAutoStep = AUTO_TO_FEEDING_POSITION;
					SendReceiveEnd();
				}
			}

			
			break;
		}
	case  AUTO_TO_FEEDING_POSITION:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("Z���ƶ�������λ");
			APP_Wait_Signal_Sleep();
			APP_Wait_Signal_Sleep();
			if(IsNoHardware() || MoveToFeedingMaterialPostion())
			{
				m_nNextAutoStep = AUTO_WAIT_FEEDING_SIGNAL;
			}
			break;
		}
	case AUTO_WAIT_FEEDING_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�ȴ������ź�");						
			if(GetNextNeedSingal())
			{
				m_nNextAutoStep = AUTO_FEEDING_MATERIAL;
			}
			break;
		}
	case AUTO_FEEDING_MATERIAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("��ʼ����");
			if(CheckCanFeedingMaterial())
			{
				if(IsNoHardware() || StartFeedingMotor())
				{
					m_nNextAutoStep = AUTO_WAIT_FEEDING_END;
				}
				
			}
		}
	case AUTO_WAIT_FEEDING_END:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�ȴ��������");
			APP_Wait_Signal_Sleep();
			m_bPlateStatus[0]=FALSE;
			m_bPlateStatus[1]=FALSE;
			if(CheckFeedingMaterialEnd())
			{
				
				if(IsNoHardware() || StopFeedingMotor())
				{
					m_nNextAutoStep = AUTO_WORK_END;
				}
				
			}
		}
		//�������
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�������");
			APP_Wait_Signal_Sleep();
			m_nNextAutoStep = AUTO_CHECK_WORK_SAFE;
			
			break;
		}

	default:
		{
			ASSERT(0);
			break;
		}
	}
}

BOOL CRunProcessElevator::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessElevator::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessElevator::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessElevator::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

//�ƶ�������λ
BOOL CRunProcessElevator::MoveToReceiveMaterialPostion()
{
	return FALSE;
}
//�ƶ�������λ
BOOL CRunProcessElevator::MoveToFeedingMaterialPostion()
{
	return FALSE;
}

//��ⰲȫ


//�ж��Ƿ���Ը���һ��������
BOOL CRunProcessElevator::CheckCanFeedingMaterial()
{
	return FALSE;
}

//��ȡ��ǰ����������������
int CRunProcessElevator::GetCurPlateCount()
{
	int nCurPlateCount = 0;
	if(IsNoHardware() || m_bDryRun)
	{
		if(m_bPlateStatus[0])
		{
			nCurPlateCount++;
		}
		if(m_bPlateStatus[1])
		{
			nCurPlateCount++;
		}
	}
	else
	{
		nCurPlateCount = 0;
		if(APP_INPUTS_IsOn(m_IHasPlate1))  //��������1��λ��Ӧ
		{
			nCurPlateCount ++;
		}
		if(APP_INPUTS_IsOn(m_IHasPlate2))  //��������2��λ��Ӧ
		{
			nCurPlateCount ++;
		}
	}	
	return nCurPlateCount;
}

//�ж������Ƿ����
BOOL CRunProcessElevator::CheckFeedingMaterialEnd()
{
	if(IsNoHardware() || m_bDryRun)
	{
		if(!m_bPlateStatus[0] && !m_bPlateStatus[1])
		{
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		if(!APP_INPUTS_IsOn(m_IHasPlate1) && !APP_INPUTS_IsOn(m_IHasPlate2))
		{
			return m_Elevator.CheckWorkSafe();
		}
	}
	return FALSE;
	
	
}


//�������ϵ��
BOOL CRunProcessElevator::StartReceiveMotor()
{
	BOOL bRet = m_Elevator.ReverseRun();
	return bRet;
}
//ֹͣ���ϵ��
BOOL CRunProcessElevator::StopReceiveMotor()
{
	BOOL bRet = m_Elevator.ReverseStop();
	return bRet;
}
//�������ϵ��
BOOL CRunProcessElevator::StartFeedingMotor()
{
	BOOL bRet = m_Elevator.ForwardRun();
	return bRet;
}
//ֹͣ���ϵ��
BOOL CRunProcessElevator::StopFeedingMotor()
{
	BOOL bRet = m_Elevator.ForwardStop();
	return bRet;
}

//ֹ�������Ƴ�
BOOL CRunProcessElevator::CylinderPush()
{
	BOOL bRet = APP_OUTPUTS_On(m_OCheckCylinder);
	return bRet;
}
//ֹ�����׻���
BOOL CRunProcessElevator::CylinderReturn()
{
	BOOL bRet = APP_OUTPUTS_Off(m_OCheckCylinder);
	return bRet;
}

BOOL CRunProcessElevator::SendReceiveEnd()
{
	return FALSE;
}

void CRunProcessElevator::MotorRun()
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
