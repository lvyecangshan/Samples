/****************************************************************************************
功能描述：短边清洁供料
历史修改记录：
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
			m_strMsg=m_bLangulageEng?_T(""):_T("关闭酒精喷洒");

			if (GushAlcoholCtrl(FALSE))
			{
				m_nNextPowerUpStep = PU_CHECK_HAVE_MATERIAL;
			}
			break;
		}

    case PU_CHECK_HAVE_MATERIAL:
        {
            m_strMsg=m_bLangulageEng?_T(""):_T("检测是否有清洁布");

			if (CheckHaveNOMaterial(TRUE))
			{
				 m_nNextPowerUpStep = PU_MOTOR_ENABLE_ON;
			}
			else
			{
				APP_BLMessageBox(_T("没有检测到清洁布！"),3);
			}
            break;
         }

	case PU_MOTOR_ENABLE_ON:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("电机上使能");

			if (MOTORS[m_nMotorR]->SetSvOn(ON))
			{
				Sleep(100);
				m_nNextPowerUpStep = PU_MOTOR_POS_MAKE_ZERO;
			}
			else
			{
				APP_BLMessageBox(_T("电机上使能失败，请检查！"),3);
			}
			break;
		}

	case PU_MOTOR_POS_MAKE_ZERO:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("把电机位置设置为零");

			if (MOTORS[m_nMotorR]->SetCurPos(0.0))
			{
				m_nNextPowerUpStep = PU_FINISH;
			}
			break;
		}

    case PU_FINISH:
        {
            CRun::PowerUpOperation();
            m_strMsg=m_bLangulageEng?_T(""):_T("初始化完成");

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
            m_strMsg=m_bLangulageEng?_T(""):_T("酒精喷洒开启");

			if (GushAlcoholCtrl(TRUE))
			{
				m_nNextAutoStep = AUTO_MOTOR_MOVE_WORK_POS;
			}
            break;
        }

	case AUTO_MOTOR_MOVE_WORK_POS:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("电机移动到工作位");

			if (IsNoHardware() || APP_MotorMove(m_nMotorR,R_WORK_POS))
			{
				Sleep(100);
				m_nNextAutoStep = AUTO_OTOR_POS_MAKE_ZERO;
			}
			break;
		}


	case AUTO_OTOR_POS_MAKE_ZERO:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("把电机位置归零");

			if (IsNoHardware() || MOTORS[m_nMotorR]->SetCurPos(0.0))
			{
				m_nNextAutoStep = AUTO_GUSHALCOHOL_CLOSE;
			}
			break;
		}


	case AUTO_GUSHALCOHOL_CLOSE:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("酒精喷洒关闭");

			if (GushAlcoholCtrl(FALSE))
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_CHECK_HAVE_MATERIAL1;
			}
			break;
		}

	case AUTO_CHECK_HAVE_MATERIAL1:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("检测无纺布是否用完");

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
			m_strMsg=m_bLangulageEng?_T(""):_T("发送开始工作信号");

			if (m_pCleanStation != NULL)
			{
				m_pCleanStation->m_bForNextReady =TRUE;
				m_nNextAutoStep = AUTO_WAIT_WORK_FINISH;
			}
			break;
		}

	case AUTO_WAIT_WORK_FINISH:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("等待清洁完成信号");

			if (m_pCleanStation != NULL && !m_pCleanStation->m_bForNextReady)
			{
				m_nNextAutoStep = AUTO_CHECK_HAVE_MATERIAL2;
			}
			break;
		}

	case AUTO_CHECK_HAVE_MATERIAL2:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("检测无纺布是否用完");

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
			m_strMsg=m_bLangulageEng?_T(""):_T("手动更换无纺布确认");

			if (BLMessageBox(_T("检测到无纺布已用完,是否已经更换完成？"),MB_YESNO) == IDYES)
			{
				m_nNextAutoStep = AUTO_CHECK_HAVE_MATERIAL2;
			}
			break;
		}

    case AUTO_WORK_END:
        {
            m_strMsg=m_bLangulageEng?_T(""):_T("工作完成");

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