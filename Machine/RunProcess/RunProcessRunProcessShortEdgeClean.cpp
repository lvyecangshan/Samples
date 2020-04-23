/****************************************************************************************
�ļ�����RunProcessRunProcessShortEdgeClean.cpp
���ߣ�chenshengjin					�汾��A1						���ڣ�2019/7/13
��������: ���Ƕ̱����
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "stdafx.h"
#include "RunProcessRunProcessShortEdgeClean.h"


CRunProcessRunProcessShortEdgeClean::CRunProcessRunProcessShortEdgeClean(int nID) : CRunProcess(nID)
{
    PowerUpRestart();
	m_PressCoreCy.SetParent(this);
	m_PressCoreCy.AddPressCoreCylinder(_T("IPressCoreCylPush1"),_T("IPressCoreCylReturn1"),
										_T("IPressCoreCylPush2"),_T("IPressCoreCylReturn2"),
										_T("OPressCoreCylPush"),_T(""));                //ѹ������
	m_JackingCylinder.SetParent(this);
	m_JackingCylinder.AddCylinder(_T("IJackingCylOpen"),_T("IJackingCylClose"),_T("OJackingCylOpen"),_T("OJackingCylClose"));  //��������

	m_Finger.SetParent(this);
	m_Finger.AddFinger(_T("IFingerCylOpen"),_T("IFingerCylClose"),_T("OFingerCylOpen"),_T("OFingerCylClose"),
		                _T("IHasMaterial"),_T("IPress"));            //��צ

	m_RodlessCylinder.SetParent(this);
	m_RodlessCylinder.AddSCtrolCylinder(_T("IRodlessCylOpen"),_T("IRodlessCyClose"),_T("ORodlessCylOpen"));   //�޸�����

	m_CleanPush.SetParent(this);
	m_CleanPush.AddPressCoreCylinder(_T("ICleanPush1"),_T("ICleanReturn1"),_T("ICleanPush2"),_T("ICleanReturn2"),
		                              _T("OCleanPush"),_T("OCleanReturn"));      //����Ƴ�

	m_pCleanOnLoad = NULL;

	m_Finger.InitMotorThread();
 }

CRunProcessRunProcessShortEdgeClean::~CRunProcessRunProcessShortEdgeClean(void)
{
	m_pCleanOnLoad = NULL;
 }
void CRunProcessRunProcessShortEdgeClean::PowerUpRestart()
{
    CRun::PowerUpRestart();
	m_bForNextReady = FALSE;
    m_nNextPowerUpStep = PU_CHECK_FINGER_HAVE_CORE;
    m_nNextAutoStep = AUTO_WAIT_BEGIN_WORK_EVEND;
}

void CRunProcessRunProcessShortEdgeClean::PowerUpOperation(void)
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
				m_nNextPowerUpStep = PU_JACKING_CY_UP;
			}
			break;
		}
		
	case PU_JACKING_CY_UP:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("������������");

			if (m_JackingCylinder.Up(0))
			{
				m_nNextPowerUpStep = PU_RODLESS_CY_RETURN;
			}
			break;
		}
		
	case PU_RODLESS_CY_RETURN:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�޸����׻���");

			if (m_RodlessCylinder.SCtrolCyReturn(0))
			{
				m_nNextPowerUpStep = PU_CLEAN_CY_RETURN;
			}
			break;
		}

	case PU_CLEAN_CY_RETURN:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("������׻���");

			if (m_CleanPush.CylinderReturn(0))
			{
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
void CRunProcessRunProcessShortEdgeClean::AutoOperation()
{
    switch(m_nNextAutoStep)
    {
	case AUTO_WAIT_BEGIN_WORK_EVEND:
        {
            m_strMsg=m_bLangulageEng?_T(""):_T("�ȴ�����λ��ʼ�����ź�");

			if (m_pCleanOnLoad->m_bForNextReady)
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
			m_strMsg=m_bLangulageEng?_T(""):_T("�ж�����಼ģ���Ƿ�׼����");

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
				Sleep(200);
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

			m_bForNextReady = FALSE;
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
BOOL CRunProcessRunProcessShortEdgeClean::InitializeConfig(CString strProcessModule)
{
    if (!CRunProcess::InitializeConfig(strProcessModule))
        return FALSE;
    return TRUE;
}

BOOL CRunProcessRunProcessShortEdgeClean::CheckOutputCanActive(COutput *pOutput)
{
    return TRUE;
 }
BOOL CRunProcessRunProcessShortEdgeClean::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
    return TRUE;
 }

void CRunProcessRunProcessShortEdgeClean::SetCleanOnLoad(CRunProcess* CleanOnLoad)
{
	m_pCleanOnLoad = CleanOnLoad;
}

void CRunProcessRunProcessShortEdgeClean::Anticollision()
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
