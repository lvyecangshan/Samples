/****************************************************************************************
功能描述：长边清洁供料
历史修改记录：
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
			m_strMsg=m_bLangulageEng?_T(""):_T("关闭酒精喷洒");

			if (GushAlcoholCtrl(FALSE))
			{
				m_nNextPowerUpStep = PU_CHECK_HAVE_MATERIAL;
			}
			break;
		}

	case PU_CHECK_HAVE_MATERIAL:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("检测否有清洁布");

			if (m_bDryRun || CheckHaveMaterial())
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

			if (IsNoHardware() || m_bDryRun || MotorSvOn())
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

			if (IsNoHardware() || m_bDryRun || SetCurrentPos(0.0))
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
void CRunProcessRunProcessLongEdgeFeeder::AutoOperation()
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
			m_strMsg=m_bLangulageEng?_T(""):_T("把电机位置归零");

			if (IsNoHardware() || SetCurrentPos(0.0))
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_GUSHALCOHOL_CLOSE;
			}
			break;
		}


	case AUTO_GUSHALCOHOL_CLOSE:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("酒精喷洒关闭");

			if (GushAlcoholCtrl(FALSE))
			{
				m_nNextAutoStep = AUTO_CHECK_HAVE_MATERIAL1;
			}
			break;
		}

	case AUTO_CHECK_HAVE_MATERIAL1:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("检测无纺布是否用完");

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
			m_strMsg=m_bLangulageEng?_T(""):_T("给清洁模组发送开始工作信号");

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
			m_strMsg=m_bLangulageEng?_T(""):_T("等待清洁完成信号");  

			 //两个清洁模组共用一个供无纺布模组
			if (MACHINE.m_pLongEdgeClean[0] != NULL && !MACHINE.m_pLongEdgeClean[0]->m_bForNextReady
				&&MACHINE.m_pLongEdgeClean[1] != NULL && !MACHINE.m_pLongEdgeClean[1]->m_bForNextReady)
			{
				m_nNextAutoStep = AUTO_CHECK_HAVE_MATERIAL2;
			}

			break;
		}

	case AUTO_CHECK_HAVE_MATERIAL2:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("检测无纺布是否用完");

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