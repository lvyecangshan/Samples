/****************************************************************************************
�����������̱���๩��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "stdafx.h"
#include "RunProcessRunProcessShortEdgeFeeder.h"

CRunProcessRunProcessShortEdgeFeeder::CRunProcessRunProcessShortEdgeFeeder(int nID) : CRunProcess(nID)
{
    PowerUpRestart();
	InitIO();

	m_pCleanStation = NULL;
 }

CRunProcessRunProcessShortEdgeFeeder::~CRunProcessRunProcessShortEdgeFeeder(void)
{
	m_pCleanStation = NULL;
 }

BOOL CRunProcessRunProcessShortEdgeFeeder::InitIO()
{
	CRunProcess::InitIO();

	m_nMotorR = -1;
	APP_Add_Motor(&m_nMotorR,_T("MotorR"));

	m_nIHaveMaterial = -1;
	APP_Add_Input(&m_nIHaveMaterial,_T("IHaveMaterial"));

	m_nINOMaterial = -1;
	APP_Add_Input(&m_nINOMaterial,_T("INOMaterial"));

	m_nOGushAlcohol = -1;
	APP_Add_Output(&m_nOGushAlcohol,_T("OGushAlcohol"));

	m_nOblow = -1;
	APP_Add_Output(&m_nOblow,_T("Oblow"));

	return TRUE;
}

void CRunProcessRunProcessShortEdgeFeeder::PowerUpRestart()
{
    CRun::PowerUpRestart();
    m_nNextPowerUpStep = PU_CLOSE_GUSHALCOHOL;
    m_nNextAutoStep = AUTO_GUSHALCOHOL_OPEN;
}

void CRunProcessRunProcessShortEdgeFeeder::PowerUpOperation(void)
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
            m_strMsg=m_bLangulageEng?_T(""):_T("����Ƿ�����಼");

			if (CheckHaveNOMaterial(TRUE))
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

			if (MOTORS[m_nMotorR]->SetSvOn(ON))
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

			if (MOTORS[m_nMotorR]->SetCurPos(0.0))
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

void CRunProcessRunProcessShortEdgeFeeder::AutoOperation()
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

			if (IsNoHardware() || APP_MotorMove(m_nMotorR,R_WORK_POS))
			{
				Sleep(100);
				m_nNextAutoStep = AUTO_OTOR_POS_MAKE_ZERO;
			}
			break;
		}


	case AUTO_OTOR_POS_MAKE_ZERO:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�ѵ��λ�ù���");

			if (IsNoHardware() || MOTORS[m_nMotorR]->SetCurPos(0.0))
			{
				m_nNextAutoStep = AUTO_GUSHALCOHOL_CLOSE;
			}
			break;
		}


	case AUTO_GUSHALCOHOL_CLOSE:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�ƾ������ر�");

			if (GushAlcoholCtrl(FALSE))
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_CHECK_HAVE_MATERIAL1;
			}
			break;
		}

	case AUTO_CHECK_HAVE_MATERIAL1:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("����޷Ĳ��Ƿ�����");

			if (IsNoHardware()||!CheckHaveNOMaterial(FALSE))
			{
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
			m_strMsg=m_bLangulageEng?_T(""):_T("���Ϳ�ʼ�����ź�");

			if (m_pCleanStation != NULL)
			{
				m_pCleanStation->m_bForNextReady =TRUE;
				m_nNextAutoStep = AUTO_WAIT_WORK_FINISH;
			}
			break;
		}

	case AUTO_WAIT_WORK_FINISH:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�ȴ��������ź�");

			if (m_pCleanStation != NULL && !m_pCleanStation->m_bForNextReady)
			{
				m_nNextAutoStep = AUTO_CHECK_HAVE_MATERIAL2;
			}
			break;
		}

	case AUTO_CHECK_HAVE_MATERIAL2:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("����޷Ĳ��Ƿ�����");

			if (IsNoHardware()|| !CheckHaveNOMaterial(FALSE))
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
BOOL CRunProcessRunProcessShortEdgeFeeder::InitializeConfig(CString strProcessModule)
{
    if (!CRunProcess::InitializeConfig(strProcessModule))
        return FALSE;
    return TRUE;
}

BOOL CRunProcessRunProcessShortEdgeFeeder::CheckOutputCanActive(COutput *pOutput)
{
    return TRUE;
 }
BOOL CRunProcessRunProcessShortEdgeFeeder::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
    return TRUE;
 }

BOOL CRunProcessRunProcessShortEdgeFeeder::GushAlcoholCtrl(BOOL bOpen)
{
	if (bOpen)
	{
		APP_OUTPUTS_On(m_nOblow);
		Sleep(100);
		APP_OUTPUTS_On(m_nOGushAlcohol);
	}
	else
	{
		APP_OUTPUTS_Off(m_nOGushAlcohol);
		Sleep(100);
		APP_OUTPUTS_Off(m_nOblow);
	}

	return TRUE;
}

BOOL CRunProcessRunProcessShortEdgeFeeder::CheckHaveNOMaterial(BOOL bHaveNo)
{
	if (bHaveNo)
	{
		return APP_INPUTS_IsOn(m_nIHaveMaterial);
	}
	else
	{
		return APP_INPUTS_IsOn(m_nINOMaterial);
	}
}

void CRunProcessRunProcessShortEdgeFeeder::SetCleanStation(CRunProcess* CleanStation)
{
	m_pCleanStation = CleanStation;
}