/****************************************************************************************
�ļ�����RunRunProcessOffLoad.h
��������������ģ��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessOffLoad.h"
#include "../../MachineCtrl.h"

CRunProcessOffLoad::CRunProcessOffLoad(int nID) : CRunProcess(nID)
{
	PowerUpRestart();
	InitIO();
}

CRunProcessOffLoad::~CRunProcessOffLoad(void)
{
	
}
BOOL CRunProcessOffLoad::InitIO()
{
	
	m_IDamperPush = -1;
	m_IDamperReturn=-1;
	m_IJackingPush=-1;
	m_IJackingReturn=-1;
	m_IHasPlate=-1;
	m_ICheckCore=-1;
	m_IHasCore=-1;
	m_ODamperReturn=-1;
	m_OJackingPush=-1;
	m_OJackingReturn=-1;

	APP_Add_Input(&m_IDamperPush,_T("IDamperPush"));
	APP_Add_Input(&m_IDamperReturn,_T("IDamperReturn"));
	APP_Add_Input(&m_IJackingPush,_T("IJackingPush"));
	APP_Add_Input(&m_IJackingReturn,_T("IJackingReturn"));
	APP_Add_Input(&m_IHasPlate,_T("IHasPlate"));
	APP_Add_Input(&m_ICheckCore,_T("ICheckCore"));
	APP_Add_Input(&m_IHasCore,_T("IHasCore"));

	APP_Add_Output(&m_ODamperReturn,_T("ODamperReturn"));
	APP_Add_Output(&m_OJackingPush,_T("OJackingPush"));
	APP_Add_Output(&m_OJackingReturn,_T("OJackingReturn"));

	return TRUE;
}

void CRunProcessOffLoad::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_nNextPowerUpStep = PU_SET_STOP_STATUS;
	m_nNextAutoStep = AUTO_SEND_STAR_SIGNAL;
	m_bRequirePallet = FALSE;
	
}

void CRunProcessOffLoad::PowerUpOperation(void)
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
			m_strMsg = m_bLangulageEng?_T("Wait Safe Signal"):_T("�赲����");
			if(IsNoHardware() || (DamperPush() && JackingReturn()))
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

void CRunProcessOffLoad::AutoOperation()
{
	if (!IsModuleEnable())
	{
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		APP_NoHardware_Sleep();
		return;
	}

	APP_NoHardware_Sleep();
	
	switch(m_nNextAutoStep)
	{
		
	case AUTO_SEND_STAR_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T("Send Require Signal"):_T("��ʼ�����ź�");
			APP_Wait_Signal_Sleep();
			if(IsNoHardware()  || (DamperPush() && JackingReturn()))
			{
				m_nNextAutoStep = AUTO_WAIT_PLATE_ARRIVE;
			}
			break;
		}
	case AUTO_WAIT_PLATE_ARRIVE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�ȴ����̵���");
			APP_Wait_Signal_Sleep();
			if(IsNoHardware() || m_bDryRun || CheckHasPlate())
			{
				if( CheckHasCore())   //�е�о
				{
					if(CheckNextRequirePlate())  //��һģ���Ƿ���Ҫ��
					{
						if(JackingReturn())
						{
							m_nNextAutoStep = AUTO_WAIT_MATERIAL_FLOW;
						}						
					}
				}
				else   //�޵�о
				{
					if(CheckPlatFormNeedPlate())  //��ǰ��λ��Ҫ����
					{
						if(IsNoHardware() || JackingPush())
						{
							m_nNextAutoStep = AUTO_SEND_MATERIAL_READY_SIGNAL;  //��������׼�����ź�
						}
					}
					else    //��ǰ��λ����Ҫ����
					{
						if(CheckNextRequirePlate())  //��һģ������Ҫ��
						{
							m_nNextAutoStep = AUTO_WAIT_MATERIAL_FLOW;  //������������һģ��
						}
						else
						{
							if(JackingPush())
							{
								m_nNextAutoStep = AUTO_SEND_MATERIAL_READY_SIGNAL;  //��������׼�����ź�
							}
						}

					}
				}
			}
			break;
		}
	case AUTO_WAIT_MATERIAL_FLOW:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�ȴ���������һ��ģ��");
			APP_Wait_Signal_Sleep();
			//����һģ�鷢�Ϳ������ź�
			if(IsNoHardware() || m_bDryRun )
			{
				m_nNextAutoStep = AUTO_WORK_END;
			}
			else 
			{
				if(JackingReturn())
				{
					if(!CheckHasPlate())
					{
						m_nNextAutoStep = AUTO_WORK_END;
					}
				}
			}
			break;
		}
	case AUTO_SEND_MATERIAL_READY_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("��������׼�����ź�");
			APP_Wait_Signal_Sleep();
			if(IsNoHardware() || SendPlateReadySignal(TRUE))
			{
				m_nNextAutoStep = AUTO_WAIT_FEEDING_END_SIGNAL;
			}
			break;
		}
	case AUTO_WAIT_FEEDING_END_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�ȴ�ȡ������ź�");

			APP_Wait_Signal_Sleep();
			if(IsNoHardware())
			{
				APP_Wait_Signal_Sleep();
				m_nNextAutoStep = AUTO_WAIT_MATERIAL_FLOW;
				break;
			}
			if(GetFeedingEndSignal())
			{
				m_nNextAutoStep = AUTO_WAIT_MATERIAL_FLOW;
			}
			break;
		}
	
		//�������
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("�������");
			APP_Wait_Signal_Sleep();
			if(IsNoHardware() || SendPlateReadySignal(FALSE))
			{
				if(SendRequiredSignal())
				{
					m_nNextAutoStep = AUTO_SEND_STAR_SIGNAL;
				}
			}
			
			break;
		}

	default:
		{
			ASSERT(0);
			break;
		}
	}
}

BOOL CRunProcessOffLoad::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessOffLoad::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessOffLoad::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessOffLoad::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

//�赲�Ƴ�
BOOL CRunProcessOffLoad::DamperPush()
{
	BOOL bRet = APP_OUTPUTS_Off(m_ODamperReturn);
	return bRet;
}
//�赲����
BOOL CRunProcessOffLoad::DamperReturn()
{
	BOOL bRet = APP_OUTPUTS_On(m_ODamperReturn);
	return bRet;
	
}

//���϶��������Ƴ�
BOOL CRunProcessOffLoad::JackingPush()
{
	BOOL bRet = APP_OUTPUTS_On(m_OJackingPush) && APP_OUTPUTS_Off(m_OJackingReturn);
	return bRet;
}
//���϶������׻���
BOOL CRunProcessOffLoad::JackingReturn()
{
	BOOL bRet = APP_OUTPUTS_Off(m_OJackingPush) && APP_OUTPUTS_On(m_OJackingReturn);
	return bRet;
	
}

//�ж��Ƿ�������
BOOL CRunProcessOffLoad::CheckHasPlate()
{
	return APP_INPUTS_IsOn(m_IHasPlate);
}

//�ж��Ƿ�������
BOOL CRunProcessOffLoad::CheckHasCore()
{
	if(IsNoHardware() || m_bDryRun)
	{
		return FALSE;
	}
	return APP_INPUTS_IsOn(m_IHasCore);
}
//�ж���һģ���Ƿ�Ҫ��
BOOL CRunProcessOffLoad::CheckNextRequirePlate()
{
	return FALSE;
}

//��������׼�����ź�
BOOL CRunProcessOffLoad::SendPlateReadySignal(BOOL bReady)
{
	BOOL bRet = FALSE;
	if(bReady)
	{
		bRet = APP_OUTPUTS_On(m_OReadPlateSignal);
	}
	else
	{
		bRet = APP_OUTPUTS_Off(m_OReadPlateSignal);
	}
	 
	return bRet;
}
//��ȡ��������ź�
BOOL CRunProcessOffLoad::GetFeedingEndSignal()
{	
	BOOL bRet = APP_INPUTS_IsOn(m_IFeedingPlateSignal);
	return bRet;
}

//��̨�Ƿ�Ҫ��
BOOL CRunProcessOffLoad::CheckPlatFormNeedPlate()
{
	BOOL bRet = APP_INPUTS_IsOn(m_IFeedingPlateSignal);
	return bRet;
}




