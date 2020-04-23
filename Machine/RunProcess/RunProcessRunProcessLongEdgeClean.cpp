/****************************************************************************************
�ļ�����RunProcessRunProcessLongEdgeClean.cpp
���ߣ�chenshengjin					�汾��A1						���ڣ�2019/7/12
��������: ���ǳ������
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "stdafx.h"
#include "RunProcessRunProcessLongEdgeClean.h"

CRunProcessRunProcessLongEdgeClean::CRunProcessRunProcessLongEdgeClean(int nID) :CRunProcessRunProcessShortEdgeClean(nID)
{
    PowerUpRestart();
	m_CleanCylinder.SetParent(this);
	m_CleanCylinder.AddPressCoreCylinder(_T("ICleanLeft1"),_T("ICleanRight1"),_T("ICleanLeft2"),_T("ICleanRight2"),
		                                 _T("OCleanLeft"),_T("OCleanRight"));

	m_pCleanOnLoad = NULL;

 }

CRunProcessRunProcessLongEdgeClean::~CRunProcessRunProcessLongEdgeClean(void)
{
	m_pCleanOnLoad = NULL;
 }
void CRunProcessRunProcessLongEdgeClean::PowerUpRestart()
{
    CRun::PowerUpRestart();
	m_bForNextReady = FALSE;
    m_nNextPowerUpStep = PU_CHECK_FINGER_HAVE_CORE;
    m_nNextAutoStep = AUTO_WAIT_BEGIN_WORK_EVEND;
}

void CRunProcessRunProcessLongEdgeClean::PowerUpOperation(void)
{
    switch(m_nNextPowerUpStep)
    {
	case PU_CHECK_FINGER_HAVE_CORE:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("����צ��û�е�о");

			if (m_Finger.HasNoMaterial(0))
			{
				m_nNextPowerUpStep = PU_FINGER_CY_OPEN;
			}
			else
			{
				CString Mess;
				Mess.Format(_T("%s��צ�е�о��������"),this->m_strName);
				ShowMessage(Mess,2);
			}
			break;
		}

	case PU_FINGER_CY_OPEN:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("��צ��");

			if (m_Finger.Open(0))
			{
				APP_NoHardware_Sleep();
				m_nNextPowerUpStep = PU_JACKING_CY_UP;
			}
			break;
		}

	case PU_JACKING_CY_UP:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("������������");

			if (m_JackingCylinder.Up(0))
			{
				APP_NoHardware_Sleep();
				m_nNextPowerUpStep = PU_RODLESS_CY_RETURN;
			}
			break;
		}

	case PU_RODLESS_CY_RETURN:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�޸����׻���");

			if (m_RodlessCylinder.SCtrolCyReturn(0))
			{
				APP_NoHardware_Sleep();
				m_nNextPowerUpStep = PU_CLEAN_CY_RETURN;
			}
			break;
		}

	case PU_CLEAN_CY_RETURN:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("������׻���");

			if (m_CleanPush.CylinderReturn(0))
			{
				APP_NoHardware_Sleep();
				m_nNextPowerUpStep = PU_CLEAN_SIDESWAY_CY_RETURN;
			}
			break;
		}

	case PU_CLEAN_SIDESWAY_CY_RETURN:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("���������׻���");

			if (m_CleanCylinder.CylinderReturn(0))
			{
				APP_NoHardware_Sleep();
				m_nNextPowerUpStep = PU_FINISH;
			}
			break;
		}

	case PU_FINISH:
		{
			CRun::PowerUpOperation();
			m_strMsg=m_bLangulageEng?_T(""):_T("��ʼ�����");

			m_nNextAutoStep = AUTO_WAIT_BEGIN_WORK_EVEND;
			break;
		}
    default:
        ASSERT(0);
        break;
    }
    return;
 }
void CRunProcessRunProcessLongEdgeClean::AutoOperation()
{
    switch(m_nNextAutoStep)
    {

	case AUTO_WAIT_BEGIN_WORK_EVEND:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�ȴ�����λ��ʼ�����ź�");

			if (m_pCleanOnLoad != NULL && m_pCleanOnLoad->m_bForNextReady)
			{
				m_nNextAutoStep = AUTO_CHECK_INDUCTOR;
			}
			break;
		}

	case AUTO_CHECK_INDUCTOR:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("������ϸ�Ӧ");

			if (((CRunProcessCleanOnLoad*)m_pCleanOnLoad)->m_OnLoad.HasCore(0))
			{
				m_nNextAutoStep = AUTO_JACKING_CY_DOWN;
			}
			break;
		}

	case AUTO_JACKING_CY_DOWN:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("���������½�");

			if (m_JackingCylinder.Down(0))
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_FINGER_CY_CLOSE;
			}
			break;
		}

	case AUTO_FINGER_CY_CLOSE:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("��צ�н�");

			if (m_Finger.Close(0))
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_JACKING_CY_UP;
			}
			break;
		}


	case AUTO_JACKING_CY_UP:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("������������");

			if (m_JackingCylinder.Up(0))
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_CHECK_HAVE_CORE;
			}
			break;
		}

	case AUTO_CHECK_HAVE_CORE:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�����û��ץ����о");

			if (m_Finger.CheckHasCore(0))
			{
				m_nNextAutoStep = AUTO_RODLESS_CY_PUSH;
			}
			break;
		}

	case AUTO_RODLESS_CY_PUSH:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�޸������Ƴ�");

			if (m_RodlessCylinder.SCtrolCyPush(0))
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_JUDGE_FEEDER_READY;
			}
			break;
		}

	case AUTO_JUDGE_FEEDER_READY:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�ж�����಼ģ�鷢��׼�����ź�");

			if (m_bForNextReady)
			{
				m_nNextAutoStep = AUTO_CLEAN_CY_PUSH;
			}
			break;
		}

	case AUTO_CLEAN_CY_PUSH:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("��������Ƴ�");

			if (m_CleanPush.CylinderPush(0))
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_CLEAN_SIDESWAY_CY_PUSH;
			}
			break;
		}

	case AUTO_CLEAN_SIDESWAY_CY_PUSH:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("���������Ƴ�");

			if (m_CleanCylinder.CylinderPush(0))
			{
				Sleep(200);
				m_nNextAutoStep = AUTO_CLEAN_SIDESWAY_CY_RETURN;
			}
			break;
		}

	case AUTO_CLEAN_SIDESWAY_CY_RETURN:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("���������Ƴ�");

			if (m_CleanCylinder.CylinderPush(0))
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_CLEAN_CY_RETURN;
			}
			break;
		}

	case AUTO_CLEAN_CY_RETURN:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("������׻���");

			if (m_CleanPush.CylinderReturn(0))
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_SEND_FEEDER_WORD_FINISH;
			}
			break;
		}

	case AUTO_SEND_FEEDER_WORD_FINISH:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("������಼ģ�鷢�͹�������ź�");

			m_bForNextReady = FALSE;     //����಼ģ��ֻ��һ��
			m_nNextAutoStep = AUTO_RODLESS_CY_RETURN;
			break;
		}

	case AUTO_RODLESS_CY_RETURN:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�޸����׻���");

			if (m_RodlessCylinder.SCtrolCyReturn(0))
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_JACKING_CY_REPEAT_DOWN;
			}
			break;
		}

	case AUTO_JACKING_CY_REPEAT_DOWN:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("���������½�");

			if (m_JackingCylinder.Down(0))
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_FINGER_CY_OPEN;
			}
			break;
		}

	case AUTO_FINGER_CY_OPEN:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("��צ���״�");

			if (m_Finger.Open(0))
			{
				m_pCleanOnLoad->m_Pallet.m_bIsClean = TRUE;  //�������
				m_nNextAutoStep = AUTO_SEND_CLEAN_FINISH_EVENT;
			}
			break;
		}

	case AUTO_SEND_CLEAN_FINISH_EVENT:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("���͹�������ź�");

			if (m_pCleanOnLoad != NULL)
			{
				m_pCleanOnLoad->m_bForNextReady = FALSE;
				m_nNextAutoStep = AUTO_JACKING_CY_REPEAT_UP;
			}
			break;
		}

	case AUTO_JACKING_CY_REPEAT_UP:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("������������");

			if (m_JackingCylinder.Up(0))
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}

	case AUTO_WORK_END:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�������");

			m_nNextAutoStep = AUTO_WAIT_BEGIN_WORK_EVEND;
			break;
		}
	
    default:
        ASSERT(0);
        break;
    }
 }
BOOL CRunProcessRunProcessLongEdgeClean::InitializeConfig(CString strProcessModule)
{
    if (!CRunProcess::InitializeConfig(strProcessModule))
        return FALSE;
    return TRUE;
}

BOOL CRunProcessRunProcessLongEdgeClean::CheckOutputCanActive(COutput *pOutput)
{

    return TRUE;
 }
BOOL CRunProcessRunProcessLongEdgeClean::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{

    return TRUE;
 }

void CRunProcessRunProcessLongEdgeClean::SetCleanOnLoad(CRunProcess* CleanOnLoad)
{
	m_pCleanOnLoad = CleanOnLoad;
}

void CRunProcessRunProcessLongEdgeClean::Anticollision()
{
	if (m_Finger.HasPressMaterial(0))
	{
		Sleep(50);
		if (m_Finger.HasPressMaterial(0))
		{
			APP_ShowMessage(_T("��צѹ�ϸ�Ӧ������,�����Ƿ�ӻ���"),3);
		}
	}
}