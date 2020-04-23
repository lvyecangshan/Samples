/****************************************************************************************
�ļ�����RunprocessRunprocessShortEdgeclean.h
���ߣ�chenshengjin					�汾��A1						���ڣ�2019/7/12
��������: �����������λ
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "stdafx.h"
#include "RunProcessCleanOnLoad.h"

CRunProcessCleanOnLoad::CRunProcessCleanOnLoad(int nID) : CRunProcess(nID)
{
    PowerUpRestart();

	m_OnLoad.SetParent(this);
	m_OnLoad.AddOnLoadPlate(_T("IDamperPush"),_T("IDamperReturn"),_T("IJackingPush"),_T("IJackingReturn"),
		                     _T("IHasPlate"),_T("IHasCore"),
							 _T("ODamperReturn"),_T("OJackingPush"),_T("OJackingReturn"),
							 _T(""));
	m_pOnLoad2 = NULL;
 }
CRunProcessCleanOnLoad::~CRunProcessCleanOnLoad(void)
{

 }
void CRunProcessCleanOnLoad::PowerUpRestart()
{
    CRun::PowerUpRestart();
	m_bRequirePallet = FALSE;
	m_bForNextReady = FALSE;
	Count = 0;
	m_Pallet.Release();
    m_nNextPowerUpStep = PU_CHECK_HAVE_CORE;
    m_nNextAutoStep = AUTO_SEND_NEED_PLATE_EVEND;
}

void CRunProcessCleanOnLoad::PowerUpOperation(void)
{
    switch(m_nNextPowerUpStep)
    {
    case PU_CHECK_HAVE_CORE:
        {
            m_strMsg=m_bLangulageEng?_T(""):_T("����Ƿ��е�о");

			if (IsNoHardware() || !m_OnLoad.HasCore(0))
			{
				m_nNextPowerUpStep = PU_JACKING_CY_DOWN;
			}
			else
			{
				APP_BLMessageBox(_T("�ѵ�о���ߣ�"),3);
			}
            break;
         }

	case PU_JACKING_CY_DOWN:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("���������½�");

			if (IsNoHardware() || m_OnLoad.JackingReturn(0))
			{
				APP_NoHardware_Sleep();
				m_nNextPowerUpStep = PU_CHECK_HAVE_PlATE;
			}
			break;
		}

	case PU_CHECK_HAVE_PlATE:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("����Ƿ�������");

			if (IsNoHardware() || !m_OnLoad.HasPlate(0))
			{
				m_nNextPowerUpStep = PU_STOPPER_UP;
			}
			else
			{
				CString Mess;
				Mess.Format(_T("���%s��������"),this->m_strName);
				ShowMessage(Mess,2);
			}
			break;
		}

	case PU_STOPPER_UP:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�赲��������");

			if (IsNoHardware() || m_OnLoad.DamperPush(0))
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

            m_nNextAutoStep = AUTO_SEND_NEED_PLATE_EVEND;
            break;
        }
    default:
        ASSERT(0);
        break;
    }
    return;
 }
void CRunProcessCleanOnLoad::AutoOperation()
{
    switch(m_nNextAutoStep)
    {

	case AUTO_SEND_NEED_PLATE_EVEND:
        {
            m_strMsg=m_bLangulageEng?_T(""):_T("����Ҫ���ź�");

			if (!m_bRequirePallet)
			{
				m_bRequirePallet = TRUE;
				m_nNextAutoStep = AUTO_WAIT_GIVE_PLATI_EVEND;
			}
            break;
        }

	case AUTO_WAIT_GIVE_PLATI_EVEND:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�ȴ������ź�");

			if (!m_bRequirePallet)
			{
				m_nNextAutoStep = AUTO_BEGING_RECEIVE_PLATE;
			}
			break;
		}

	case AUTO_BEGING_RECEIVE_PLATE:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("��ʼ������");

			if (ReceivePlate())
			{
				if (m_strProcess ==_T("ShortCleanOnLoad01") || m_strProcess ==_T("LongCleanOnLoad01"))
				{
					m_Pallet.m_bIsValid = TRUE;   //��ֵ���ں����ж�
				} 
				m_nNextAutoStep = AUTO_JUDGE_WORK_STATION;
			}
			else
			{
				CString Mess;
				Mess.Format(_T("%s�������̳�ʱ��"),this->m_strName);
				ShowMessage(Mess,2);
			}
			break;
		}

	case AUTO_JUDGE_WORK_STATION:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�жϹ�����ʽ");

			if(m_Pallet.m_bIsClean)  //�Ѿ������ĵ��
			{
				m_Pallet.Release();    //��ǰ�������ͷ�
				m_nNextAutoStep = AUTO_STOPPER_DOWN;
			}
			 //���ȸ�����λ2����
			else if (m_pOnLoad2 != NULL && (m_pOnLoad2->m_bRequirePallet || !m_pOnLoad2->m_Pallet.m_bIsValid || m_pOnLoad2->m_Pallet.m_bIsClean))
			{
				m_nNextAutoStep = AUTO_JUDGE_NEXT_STATION_NEED_PLATE_EVENT;
			}
			else
			{
				m_nNextAutoStep = AUTO_JACKING_CY_UP;
			}
			break;
		}

	case AUTO_JACKING_CY_UP:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�������׶���");

			if (IsNoHardware() || m_OnLoad.JackingPush())
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_JUDGE_HAVE_CORE;
			}
			break;
		}

	case AUTO_JUDGE_HAVE_CORE:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("����о��Ӧ��");

			if (m_OnLoad.HasCore(0))
			{
				m_nNextAutoStep = AUTO_SEND_GIVE_PLATE_EVEND;
			}
			break;
		}

	case AUTO_SEND_GIVE_PLATE_EVEND:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�����ģ�鷢��׼�����ź�");

			if (!m_bForNextReady)
			{
				m_bForNextReady = TRUE;
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_WAIT_CLEAN_WORK_FINISH;
			}
			break;
		}

	case AUTO_WAIT_CLEAN_WORK_FINISH:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�ȴ����ģ�鹤�����");

			if (!m_bForNextReady)
			{
				m_nNextAutoStep = AUTO_JACKING_CY_DOWN;
			}
			break;
		}

	case AUTO_JACKING_CY_DOWN:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("���������½�");

			if (IsNoHardware() || m_OnLoad.JackingReturn(0))
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_JUDGE_NEXT_STATION_NEED_PLATE_EVENT;
			}
			break;
		}

	case AUTO_JUDGE_NEXT_STATION_NEED_PLATE_EVENT:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�ȴ���һ��λ�Ƿ�Ҫ���ź�");

			if (m_pOnLoad2 != NULL && m_pOnLoad2->m_bRequirePallet)
			{
				m_nNextAutoStep = AUTO_STOPPER_DOWN;
			}
			else if (m_strProcess==_T("ShortCleanOnLoad02") || m_strProcess==_T("LongCleanOnLoad02")) //����λ2��ָ��û��
			{
				m_nNextAutoStep = AUTO_STOPPER_DOWN;
			}
			break;
		}

	case AUTO_STOPPER_DOWN:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�赲�����½�");

			if (m_OnLoad.DamperReturn(0))
			{
				if (m_pOnLoad2 != NULL)   //����λ1
				{
					m_pOnLoad2->m_Pallet = m_Pallet;
					m_pOnLoad2->m_bRequirePallet = FALSE;
				}
				m_nNextAutoStep = AUTO_SEND_PLATE;
			}
			break;
		}

	case AUTO_SEND_PLATE:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("��������");

			if (SendPlate())
			{
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_STOPPER_UP;
			}
			break;
		}

	case AUTO_STOPPER_UP:
		{
			m_strMsg=m_bLangulageEng?_T(""):_T("�赲���׶���");

			if (m_OnLoad.DamperPush(0))
			{
				m_Pallet.Release();
				APP_NoHardware_Sleep();
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}

    case AUTO_WORK_END:
        {
            m_strMsg=m_bLangulageEng?_T(""):_T("�������");

            m_nNextAutoStep = AUTO_SEND_NEED_PLATE_EVEND;
            break;
        }
    default:
        ASSERT(0);
        break;
    }
 }
BOOL CRunProcessCleanOnLoad::InitializeConfig(CString strProcessModule)
{
    if (!CRunProcess::InitializeConfig(strProcessModule))
        return FALSE;
    return TRUE;
}
void CRunProcessCleanOnLoad::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
    CRunProcess::GetHardwareConfig(pInputs,pOutputs,pMotors);
}
BOOL CRunProcessCleanOnLoad::CheckOutputCanActive(COutput *pOutput)
{
    return TRUE;
 }
BOOL CRunProcessCleanOnLoad::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
  
    return TRUE;
 }

BOOL CRunProcessCleanOnLoad::ReceivePlate()
{

	BOOL Result =FALSE;
	Count = GetTickCount();

	while(1)
	{
		if (m_OnLoad.HasMaterial_IsOn(0))
		{
			Result = TRUE;
			Sleep(200);
		}

		if (Result && m_OnLoad.HasMaterial_IsOn(0))
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

BOOL CRunProcessCleanOnLoad::SendPlate()
{

	Count = GetTickCount();
	BOOL Result =FALSE;

	while(1)
	{
		if(m_OnLoad.HasMaterial_IsOff(0))
		{
			Result = TRUE;
			Sleep(200);
		}

		if (Result && m_OnLoad.HasMaterial_IsOff(0))
		{
			return TRUE;
		}
		else if(GetTickCount() - Count > 3000)
		{
			break;
		}
	}

	if (m_strProcess == _T("ShortCleanOnLoad01") || m_strProcess == _T("LongCleanOnLoad01"))  //����λ1
	{
		APP_ShowMessage(_T("�������̳�ʱ"),2);
	}

	return FALSE;
}