/****************************************************************************************
����������������๩��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "stdafx.h"
#include "RunProcessRunProcessLongEdgeFeeder.h"

CRunProcessRunProcessLongEdgeFeeder::CRunProcessRunProcessLongEdgeFeeder(int nID) : CRunProcess(nID)
{
    PowerUpRestart();
	InitIO();
 }
CRunProcessRunProcessLongEdgeFeeder::~CRunProcessRunProcessLongEdgeFeeder(void)
{

 }

BOOL CRunProcessRunProcessLongEdgeFeeder::InitIO()
{
	CRunProcess::InitIO();

	for (int i=0;i<2;i++)
	{
		CString Key = _T("");

		m_nMotorR[i] = -1;
		Key.Format(_T("MotorR%d"),i);
		APP_Add_Motor(&m_nMotorR[i],Key);

		m_nIHaveMaterial[i] = -1;
		Key.Format(_T("IHaveMaterial%d"),i);
		APP_Add_Input(&m_nIHaveMaterial[i],Key);

		m_nINOMaterial[2] = -1;
		Key.Format(_T("INOMaterial%d"),i);
		APP_Add_Input(&m_nINOMaterial[i],Key);

		m_nOGushAlcohol[i] = -1;
		Key.Format(_T("OGushAlcohol%d"),i);
		APP_Add_Output(&m_nOGushAlcohol[i],Key);

		m_nOblow[i] = -1;
		Key.Format(_T("Oblow%d"),i);
		APP_Add_Output(&m_nOblow[i],Key);
	}

	return TRUE;
}

void CRunProcessRunProcessLongEdgeFeeder::PowerUpRestart()
{
    CRun::PowerUpRestart();
	m_bForNextReady = FALSE;
    m_nNextPowerUpStep = PU_CLOSE_GUSHALCOHOL;
    m_nNextAutoStep = AUTO_GUSHALCOHOL_OPEN;
}

void CRunProcessRunProcessLongEdgeFeeder::PowerUpOperation(void)
{
    switch(m_nNextPowerUpStep)
    {
	case PU_CLOSE_GUSHALCOHOL:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�رվƾ�����");

			if (GushAlcoholCtrl(FALSE))
			{
				m_nNextPowerUpStep = PU_CHECK_HAVE_MATERIAL;
			}
			break;
		}

	case PU_CHECK_HAVE_MATERIAL:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("��������಼");

			if (m_bDryRun || CheckHaveMaterial())
			{
				m_nNextPowerUpStep = PU_MOTOR_ENABLE_ON;
			}
			else
			{
				APP_BLMessageBox(_T("û�м�⵽��಼��"),3);
			}
			break;
		}

	case PU_MOTOR_ENABLE_ON:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�����ʹ��");

			if (IsNoHardware() || m_bDryRun || MotorSvOn())
			{
				Sleep(100);
				m_nNextPowerUpStep = PU_MOTOR_POS_MAKE_ZERO;
			}
			else
			{
				APP_BLMessageBox(_T("�����ʹ��ʧ�ܣ����飡"),3);
			}
			break;
		}

	case PU_MOTOR_POS_MAKE_ZERO:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�ѵ��λ������Ϊ��");

			if (IsNoHardware() || m_bDryRun || SetCurrentPos(0.0))
			{
				m_nNextPowerUpStep = PU_FINISH;
			}
			break;
		}

	case PU_FINISH:
		{
			CRun::PowerUpOperation();
			m_strMsg=m_bLangulageEng?_T(""):_T("��ʼ�����");

			m_nNextAutoStep = AUTO_GUSHALCOHOL_OPEN;
			break;
		}
    default:
        ASSERT(0);
        break;
    }
    return;
 }
void CRunProcessRunProcessLongEdgeFeeder::AutoOperation()
{
    switch(m_nNextAutoStep)
    {

	case AUTO_GUSHALCOHOL_OPEN:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�ƾ���������");

			if (GushAlcoholCtrl(TRUE))
			{
				m_nNextAutoStep = AUTO_MOTOR_MOVE_WORK_POS;
			}
			break;
		}

	case AUTO_MOTOR_MOVE_WORK_POS:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("����ƶ�������λ");

			if (IsNoHardware() || MotorsMoveToWorkLos())
			{
				Sleep(100);
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_OTOR_POS_MAKE_ZERO;
			}
			break;
		}


	case AUTO_OTOR_POS_MAKE_ZERO:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�ѵ��λ�ù���");

			if (IsNoHardware() || SetCurrentPos(0.0))
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_GUSHALCOHOL_CLOSE;
			}
			break;
		}


	case AUTO_GUSHALCOHOL_CLOSE:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�ƾ������ر�");

			if (GushAlcoholCtrl(FALSE))
			{
				m_nNextAutoStep = AUTO_CHECK_HAVE_MATERIAL1;
			}
			break;
		}

	case AUTO_CHECK_HAVE_MATERIAL1:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("����޷Ĳ��Ƿ�����");

			if (IsNoHardware() || CheckHaveMaterial())
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_SEND_BEGIN_WORK_EVENT;
			}
			else
			{
				m_nNextAutoStep = AUTO_MANUAL_CHANGE_MATERIAL;
			}
			break;
		}


	case AUTO_SEND_BEGIN_WORK_EVENT:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�����ģ�鷢�Ϳ�ʼ�����ź�");

			if (MACHINE.m_pLongEdgeClean[0] != NULL && !MACHINE.m_pLongEdgeClean[0]->m_bForNextReady 
				&&MACHINE.m_pLongEdgeClean[1] != NULL &&!MACHINE.m_pLongEdgeClean[1]->m_bForNextReady)
			{
				MACHINE.m_pLongEdgeClean[0]->m_bForNextReady = TRUE;
				MACHINE.m_pLongEdgeClean[1]->m_bForNextReady = TRUE;
				m_nNextAutoStep = AUTO_WAIT_WORK_FINISH;
			}
			break;
		}

	case AUTO_WAIT_WORK_FINISH:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�ȴ��������ź�");  

			 //�������ģ�鹲��һ�����޷Ĳ�ģ��
			if (MACHINE.m_pLongEdgeClean[0] != NULL && !MACHINE.m_pLongEdgeClean[0]->m_bForNextReady
				&&MACHINE.m_pLongEdgeClean[1] != NULL && !MACHINE.m_pLongEdgeClean[1]->m_bForNextReady)
			{
				m_nNextAutoStep = AUTO_CHECK_HAVE_MATERIAL2;
			}

			break;
		}

	case AUTO_CHECK_HAVE_MATERIAL2:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("����޷Ĳ��Ƿ�����");

			if (IsNoHardware()|| CheckHaveMaterial())
			{
				m_nNextAutoStep = AUTO_WORK_END;
			}
			else
			{
				m_nNextAutoStep = AUTO_MANUAL_CHANGE_MATERIAL;
			}
			break;
		}

	case AUTO_MANUAL_CHANGE_MATERIAL:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�ֶ������޷Ĳ�ȷ��");

			if (BLMessageBox(_T("��⵽�޷Ĳ�������,�Ƿ��Ѿ�������ɣ�"),MB_YESNO) == IDYES)
			{
				m_nNextAutoStep = AUTO_CHECK_HAVE_MATERIAL2;
			}
			break;
		}

	case AUTO_WORK_END:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�������");

			m_nNextAutoStep = AUTO_GUSHALCOHOL_OPEN;
			break;
		}
    default:
        ASSERT(0);
        break;
    }
 }
BOOL CRunProcessRunProcessLongEdgeFeeder::InitializeConfig(CString strProcessModule)
{
    if (!CRunProcess::InitializeConfig(strProcessModule))
        return FALSE;
    return TRUE;
}

BOOL CRunProcessRunProcessLongEdgeFeeder::CheckOutputCanActive(COutput *pOutput)
{
    return TRUE;
 }
BOOL CRunProcessRunProcessLongEdgeFeeder::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
    return TRUE;
 }

BOOL CRunProcessRunProcessLongEdgeFeeder::GushAlcoholCtrl(BOOL bOpen)
{
	for (int i=0;i<2;i++)
	{
		bOpen?APP_OUTPUTS_On(m_nOblow[i]):APP_OUTPUTS_Off(m_nOGushAlcohol[i]);
	}

	Sleep(100);

	for (int i=0;i<2;i++)
	{
		bOpen?APP_OUTPUTS_On(m_nOGushAlcohol[i]):APP_OUTPUTS_Off(m_nOblow[i]);
	}

	return TRUE;
}

BOOL CRunProcessRunProcessLongEdgeFeeder::CheckHaveMaterial()
{
	if (APP_INPUTS_IsOn(m_nIHaveMaterial[0]) && APP_INPUTS_IsOn(m_nIHaveMaterial[1]))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CRunProcessRunProcessLongEdgeFeeder::CheckNoMaterial()
{
	if (APP_INPUTS_IsOn(m_nINOMaterial[0]) || APP_INPUTS_IsOn(m_nINOMaterial[1]))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CRunProcessRunProcessLongEdgeFeeder::MotorSvOn()
{
	for (int i=0;i<2;i++)
	{
		if (MOTORS[m_nMotorR[i]]->SetSvOn(ON))
		{
			continue;
		}
		return FALSE;
	}

	return TRUE;
}

BOOL CRunProcessRunProcessLongEdgeFeeder::SetCurrentPos(double dPos)
{
	for (int i=0;i<2;i++)
	{
		if (MOTORS[m_nMotorR[i]]->SetCurPos(dPos))
		{
			continue;
		}
		return FALSE;
	}

	return TRUE;
}

BOOL CRunProcessRunProcessLongEdgeFeeder::MotorsMoveToWorkLos()
{
	int RPos[2] = {R1_WORK_POS,R2_WORK_POS};

	return MotorsMove(GetCurPorductID(MACHINE.m_nDefaultProduct),m_nMotorR,RPos,2);
}