/****************************************************************************************
功能描述：清洁前分料
历史修改记录：
*****************************************************************************************/
#include "stdafx.h"
#include "RunProcessCleanFrontDistributor.h"

CRunProcessCleanFrontDistributor::CRunProcessCleanFrontDistributor(int nID) : CRunProcess(nID)
{
    PowerUpRestart();
	Stopper.SetParent(this);
	Stopper.AddStopper(_T("IStopperCylPush0"),_T("IStopperCylReturn0"),_T("OStopperCylReturn0"),_T("IHasMaterial"));
	Stopper.AddStopper(_T("IStopperCylPush1"),_T("IStopperCylReturn1"),_T("OStopperCylReturn1"),_T(""));
	m_pClaenOnLoad1 = NULL;
 }

CRunProcessCleanFrontDistributor::~CRunProcessCleanFrontDistributor(void)
{

 }

void CRunProcessCleanFrontDistributor::PowerUpRestart()
{
    CRun::PowerUpRestart();
	m_bRequirePallet = FALSE;
	Count = 0;
    m_nNextPowerUpStep = PU_CHECK_HAVE_PlATE;
    m_nNextAutoStep = AUTO_JUDGE__BEGING_RECEIVE_PLATE_EVENT;
}

void CRunProcessCleanFrontDistributor::PowerUpOperation(void)
{
    switch(m_nNextPowerUpStep)
    {
    case PU_CHECK_HAVE_PlATE:
        {
            m_strMsg=m_bLangulageEng?_T(""):_T("检测有没有料盘");

			if (IsNoHardware() || Stopper.HasMaterial(0))
			{
				 APP_NoHardware_Sleep();
				 m_nNextPowerUpStep = PU_STOPPER1_CYDER_DOWN;
			}
			else
			{
				CString Mess;
				Mess.Format(_T("请把%s料盘拿走"),this->m_strName);
				ShowMessage(Mess,2);
			}
            break;
         }

	case PU_STOPPER1_CYDER_DOWN:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("阻挡1气缸下降");

			if (IsNoHardware() || Stopper.CylinderDown(0))
			{
				APP_NoHardware_Sleep();
				m_nNextPowerUpStep = PU_STOPPER2_CYDER_UP;
			}
			break;
		}

	case PU_STOPPER2_CYDER_UP:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("阻挡2气缸上升");

			if (IsNoHardware() || Stopper.CylinderDown(1))
			{
				APP_NoHardware_Sleep();
				m_nNextPowerUpStep = PU_FINISH;
			}
			break;
		}

    case PU_FINISH:
        {
            CRun::PowerUpOperation();
            m_strMsg=m_bLangulageEng?_T(""):_T("初始化完成");
			APP_NoHardware_Sleep();
            m_nNextAutoStep = AUTO_JUDGE__BEGING_RECEIVE_PLATE_EVENT;
            break;
        }
    default:
        ASSERT(0);
        break;
    }
    return;
 }
void CRunProcessCleanFrontDistributor::AutoOperation()
{
    switch(m_nNextAutoStep)
    {

	case AUTO_JUDGE__BEGING_RECEIVE_PLATE_EVENT:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("判断是否有盘接收");

			if (m_bDryRun || HavePlateReceive())
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_BEGING_RECEIVE_PLATE;
			}
			break;
		}

	case AUTO_BEGING_RECEIVE_PLATE:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("接收料盘");

			if (m_bDryRun || ReceivePlate())
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_JUDGE_NEXT_STATION_NEED_PLATE_EVENT;
			}
			else
			{
				//接收失败后返回重新判断
				m_nNextAutoStep = AUTO_JUDGE__BEGING_RECEIVE_PLATE_EVENT;
			}
			break;
		}

	case AUTO_JUDGE_NEXT_STATION_NEED_PLATE_EVENT:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("判断下一个模组是否要料");

			if (m_pClaenOnLoad1 != NULL && m_pClaenOnLoad1->m_bRequirePallet)
			{
				m_pClaenOnLoad1->m_bRequirePallet = FALSE;
				m_nNextAutoStep = AUTO_STOPPER_CY_WORK;
			}
			break;
		}

	case AUTO_STOPPER_CY_WORK:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("阻挡气缸分料");

			if (Stopper.CylinderUp(0) && Stopper.CylinderDown(1))
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_WAIT_NEXT_STATION_RECEIVE_SUCCESS_EVENT;
			}
			break;
		}

	case AUTO_WAIT_NEXT_STATION_RECEIVE_SUCCESS_EVENT:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("发送料盘");

			if (SendPlate())
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_STOPPER_CY_REGRESS;
			}
			else
			{
				CString Mess;
				Mess.Format(_T("%s发送料盘超时"),this->m_strName);
				ShowMessage(Mess,2);
			}
			break;
		}

	case AUTO_STOPPER_CY_REGRESS:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("阻挡气缸恢复");

			if (Stopper.CylinderUp(1) && Stopper.CylinderDown(0))
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_WORK_FINISH;
			}
			break;
		}

    case AUTO_WORK_FINISH:
        {
            m_strMsg=m_bLangulageEng?_T(""):_T("工作完成");

            m_nNextAutoStep = AUTO_JUDGE__BEGING_RECEIVE_PLATE_EVENT;
            break;
        }
    default:
        ASSERT(0);
        break;
    }
 }
BOOL CRunProcessCleanFrontDistributor::InitializeConfig(CString strProcessModule)
{
    if (!CRunProcess::InitializeConfig(strProcessModule))
        return FALSE;
    return TRUE;
}
void CRunProcessCleanFrontDistributor::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
    CRunProcess::GetHardwareConfig(pInputs,pOutputs,pMotors);
}
BOOL CRunProcessCleanFrontDistributor::CheckOutputCanActive(COutput *pOutput)
{
    if (!CRunProcess::CheckOutputCanActive(pOutput))
    {
        return FALSE;
    }
    return TRUE;
 }
BOOL CRunProcessCleanFrontDistributor::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
    return TRUE;
 }

BOOL CRunProcessCleanFrontDistributor::ReceivePlate()
{

    BOOL Result =FALSE;
	Count = GetTickCount();

	while(1)
	{
		if (Stopper.HasMaterial_IsOn(0))
		{
			Result = TRUE;
			Sleep(100);
		}

		if (Result && Stopper.HasMaterial_IsOn(0))
		{
			return TRUE;
		}
		else if (GetTickCount() - Count > 3000 )
		{
			break;
		}
	}

	return FALSE;
}

BOOL CRunProcessCleanFrontDistributor::SendPlate()
{
	Count = GetTickCount();
	BOOL Result =FALSE;

	while(1)
	{
		if(Stopper.HasMaterial_IsOff(0))
		{
			Result = TRUE;
			Sleep(100);
		}

		if (Result && Stopper.HasMaterial_IsOff(0))
		{
			return TRUE;
		}
		else if(GetTickCount() - Count > 3000 )
		{
			break;
		}
	}

	return FALSE;
}

BOOL CRunProcessCleanFrontDistributor::HavePlateReceive()
{
	BOOL Result = FALSE;

	if (Stopper.HasMaterial_IsOn(0))
	{
		Result = TRUE;
		Sleep(200);
	}

	if (Result && Stopper.HasMaterial_IsOn(0))
	{
		return TRUE;
	}

	return FALSE;
}