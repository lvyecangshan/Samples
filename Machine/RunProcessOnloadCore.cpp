#include "StdAfx.h"
#include "RunProcessOnloadCore.h"

UINT OnloadCoreThread(LPVOID	 lpvoid)
{
	CRunProcessOnloadCore* pRun = (CRunProcessOnloadCore*)lpvoid;

	if (pRun != NULL)
	{
		pRun->m_bThreadStart = TRUE;
		pRun->m_bThreadEnd = FALSE;
		while(pRun->m_bThreadStart)
		{
			if(!IsNoHardware())
				pRun->EmgStop();
			Sleep(100);
		}
		pRun->m_bThreadEnd = TRUE;
	}
	return 0;
}

CRunProcessOnloadCore::CRunProcessOnloadCore(int nID) : CRunProcess(nID)
{
	CString strKey;

	m_strScanIp = _T("127.0.0.1");
	m_nScanPort = 8008;
	m_strScanCmd = _T("Read\r\n");

	PowerUpRestart();
	
	for(int i = 0; i < 4; i++)
	{
		//��צ
		strKey.Format(_T("IFingerOpen%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IFingerOpen[i], strKey));
		strKey.Format(_T("IFingerClose%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IFingerClose[i], strKey));

		//ѹ��
		strKey.Format(_T("IPressPush%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IPressPush[i], strKey));
		strKey.Format(_T("IPressReturn%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IPressReturn[i], strKey));

		//ѹ�ϼ��
		strKey.Format(_T("IZCheckAbnormal%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IZCheckAbnormal[i], strKey));
	}

	for(int i = 0; i < 2; i++)
	{
		strKey.Format(_T("OFingerClose%d"), i);
		m_OutputMap.insert(pair<int*, CString>(&m_OFingerClose[i], strKey));
		strKey.Format(_T("OFingerOpen%d"), i);
		m_OutputMap.insert(pair<int*, CString>(&m_OFingerOpen[i], strKey));

		//�Ƴ�
		strKey.Format(_T("OPressPush%d"), i);
		m_OutputMap.insert(pair<int*, CString>(&m_OPressPush[i], strKey));

		//����
		strKey.Format(_T("OPressReturn%d"), i);
		m_OutputMap.insert(pair<int*, CString>(&m_OPressReturn[i], strKey));

		strKey.Format(_T("IHasCore%d"), i);//��צ���ϼ��
		m_InputMap.insert(pair<int*, CString>(&m_IHasCore[i], strKey));

		strKey.Format(_T("IHasCore%d"), i + 2);//�������ϼ��
		m_InputMap.insert(pair<int*, CString>(&m_IHasCore[i + 2], strKey));
	}

	m_MotorMap.insert(pair<int*, CString>(&m_nMotorY, _T("MotorY")));
	m_MotorMap.insert(pair<int*, CString>(&m_nMotorZ, _T("MotorZ")));

	//����
	m_InputMap.insert(pair<int*, CString>(&m_IBufferCylPush,_T("IBufferCylPush")));
	m_InputMap.insert(pair<int*, CString>(&m_IBufferCylReturn,_T("IBufferCylReturn")));
	m_OutputMap.insert(pair<int*, CString>(&m_OBufferCylPush,_T("OBufferCylPush")));
	m_OutputMap.insert(pair<int*, CString>(&m_OBufferCylReturn, _T("OBufferCylReturn")));

	//�������Խ��ź� Yao  5.6
	m_IReadyOnloadSignal = -1;
	m_InputMap.insert(pair<int*, CString>(&m_IReadyOnloadSignal,_T("IReadyOnloadSignal")));

	m_IPLCRunningSignal = -1;
	m_InputMap.insert(pair<int*, CString>(&m_IPLCRunningSignal,_T("IPLCRunningSignal")));

	m_ORequiredOnloadSignal = -1;
	m_OutputMap.insert(pair<int*, CString>(&m_ORequiredOnloadSignal,_T("ORequiredOnloadSignal")));

	m_OPLCRunningSignal = -1;
	m_OutputMap.insert(pair<int*, CString>(&m_OPLCRunningSignal,_T("OPLCRunningSignal")));
}

CRunProcessOnloadCore::~CRunProcessOnloadCore(void)
{
	ThreadRelease();
}

void CRunProcessOnloadCore::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng ? _T("Ready") : _T("׼����");

	m_nStartWithPalletData = -1;
	m_nBufferWithPalletData = -1;
	m_nWaitTime = 0;
	m_nWorkIndex = -1;
	m_bNextRequire = FALSE;
	m_bForNextReady = FALSE;
	m_bHomeMoveSafePos  = FALSE;
	m_bEnableDocking = FALSE;

	m_pTempPallet[0].Release();
	m_pTempPallet[1].Release();
	m_pBufferPallet.Release();


	m_nNextPowerUpStep = PU_PRESS_CYL_PUSH;
	m_nNextAutoStep = AUTO_MOVE_Y_TO_ONLOAD_POS;
	
}

void CRunProcessOnloadCore::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng ? _T("Moudle UnEnable") : _T("ģ�����");
		return;
	}

	switch(m_nNextPowerUpStep)
	{
		//ѹ��о�Ƴ�
	case PU_PRESS_CYL_PUSH:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Open"):_T("ѹ��о�Ƴ�");

			//if (IsNoHardware() || PressPush(-1, TRUE))
			{
				m_nNextPowerUpStep = PU_MOVE_Z_TO_HOME;
			}
			break;
		}

		//��λZ����
	case PU_MOVE_Z_TO_HOME:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Home"):_T("��λZ����");

			if(m_nMotorZ > -1 && HomeMotor(MOTORS[m_nMotorZ]))
			{
				m_bHomeMoveSafePos = TRUE;
				m_nNextPowerUpStep = PU_MOVE_Y_TO_HOME;
			}
			break;
		}
		//Y�����
	case PU_MOVE_Y_TO_HOME:
		{
			m_strMsg = m_bLangulageEng?_T("Motor X home"):_T("��λY����");

			if (m_nMotorY > -1 && HomeMotor(MOTORS[m_nMotorY]))
			{
				m_nNextPowerUpStep = PU_JUDGE_IF_WITH_PALLET;
			}
			break;
		}
		//�ж��Ƿ����
	case PU_JUDGE_IF_WITH_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Product"):_T("�ж��Ƿ����");

			if(MACHINE.m_bReCoverMachine)
			{
				ReadStepInfo();
				m_nNextPowerUpStep = PU_CHECK_BUFFER_PALLET_DATA;
			}
			else
			{
				//�������
				m_nWorkIndex = -1;
				m_bNextRequire = FALSE;
				m_bForNextReady = FALSE;
				m_pTempPallet[0].Release();
				m_pTempPallet[1].Release();
				m_pBufferPallet.Release();

				m_nNextPowerUpStep = PU_CHECK_WITHOUT_PRODUCT;
			}
			break;
		}

		//����޲�Ʒ
	case PU_CHECK_WITHOUT_PRODUCT:
		{
			m_strMsg = m_bLangulageEng?_T("Check Without Product"):_T("����޲�Ʒ");

			if (IsNoHardware() || CheckHasVaild(-1, FALSE))
			{
				m_nNextPowerUpStep = PU_PRESS_CYL_RETURN;		 							 
			}
			break;
		}

		//ѹ��о����
	case PU_PRESS_CYL_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Open"):_T("ѹ��о����");

			if (IsNoHardware() || PressPush(-1, FALSE))
			{
				m_nNextPowerUpStep = PU_FINGER_CYL_OPEN;
			}
			break;
		}

		//��צ��
	case PU_FINGER_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Open"):_T("��צ��");

			if (IsNoHardware() || FingerClose(-1, FALSE))
			{
				m_nNextPowerUpStep = PU_ROTATE_CYL_RETURN;
			}
			break;
		}
		//��ת���׻���
	case PU_ROTATE_CYL_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Open"):_T("��ת���׻���");

			/*if (IsNoHardware() || RotatePush(-1, FALSE))*/
			{
				m_nNextPowerUpStep = PU_MOVE_Z_TO_SAFE_POS;
			}
			break;
		}

		//�ƶ�Z�ᵽ��ȫλ
	case PU_MOVE_Z_TO_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Safe Pos"):_T("�ƶ�Z�ᵽ��ȫλ");

			if(m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_SAFE_LOC))
			{
				m_nNextPowerUpStep = PU_MOVE_Y_TO_ONLOAD_POS;		 
			}
			break;
		}	

		//�ƶ�Y�ᵽȡ��λ
	case PU_MOVE_Y_TO_ONLOAD_POS:
		{ 
			m_strMsg = m_bLangulageEng?_T("Move X To Onload Pos"):_T("�ƶ�Y�ᵽȡ��λ");

			if (m_nMotorY > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorY, Y_MOTOR_PICK_LOC)) 
			{
				m_nNextPowerUpStep = PU_BUFFER_CYL_BACK;
			}
			break;
		}

		//�ݴ����׻���
	case PU_BUFFER_CYL_BACK:
		{
			m_strMsg = m_bLangulageEng?_T("Reserve Cyl Back"):_T("�ݴ����׻���");
			if(IsNoHardware() || BufferPush(FALSE))
			{
				m_nNextAutoStep = AUTO_MOVE_Y_TO_ONLOAD_POS;
				m_nNextPowerUpStep = PU_FINISH;
			}
			break;
		}

		//��⻺��������ʵ���Ƿ�һ��
	case PU_CHECK_BUFFER_PALLET_DATA:
		{
			m_strMsg = m_bLangulageEng?_T("Move XY To Onload Pos"):_T("��⻺��������ʵ���Ƿ�һ��");
			if (CheckStartWithPalletDataSameForBuffer())
			{
				if (!m_pBufferPallet.m_bIsValid)
				{
					m_pBufferPallet.Release();
				}
				m_nNextAutoStep = PU_CHECK_PALLET_DATA;
			}
			break;
		}

		//����צ������ʵ���Ƿ�һ��
	case PU_CHECK_PALLET_DATA:
		{
			m_strMsg = m_bLangulageEng?_T("Move XY To Onload Pos"):_T("�������������ʵ���Ƿ�һ��");
			
			if(CheckStartWithPalletDataSame())
			{
				if(m_nStartWithPalletData == 0)
				{
					m_pTempPallet[0].Release();
					m_pTempPallet[1].Release();

					m_nNextPowerUpStep = PU_CHECK_WITHOUT_PRODUCT;
					m_nNextAutoStep = AUTO_MOVE_Y_TO_ONLOAD_POS;
				}
				else if(m_nStartWithPalletData == 1)//12��צ����
				{
					m_nNextPowerUpStep = PU_PRESS_CLY_ACTION_WITH_PALLET;
					m_nNextAutoStep = AUTO_FINGER_CYL_CLOSE;
				}
				else if(m_nStartWithPalletData == 2)  //1��צ����
				{
					m_pTempPallet[0].Release();
					m_nNextPowerUpStep = PU_PRESS_CLY_ACTION_WITH_PALLET;
					m_nNextAutoStep = AUTO_JUDGE_HAVE_NG;
				}
				else if(m_nStartWithPalletData == 3)//2��צ����
				{
					m_pTempPallet[1].Release();
					m_nNextPowerUpStep = PU_PRESS_CLY_ACTION_WITH_PALLET;
					m_nNextAutoStep = AUTO_JUDGE_HAVE_NG;
				}
				else
				{
					ASSERT(0);
				}
			}
			break;
		}

		//ѹ��о״̬��λ
	case PU_PRESS_CLY_ACTION_WITH_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Press Cly Return"):_T("ѹ��о״̬��λ");

			if(m_nStartWithPalletData == 1)
			{
				if (IsNoHardware() || PressPush(-1,TRUE))
				{
					m_nNextPowerUpStep = PU_JUDGE_BARCODE_EXIST;
				}
			}
			if(m_nStartWithPalletData == 2)
			{
				if (IsNoHardware() || PressPush(0,TRUE) && PressPush(1,FALSE))
				{
					m_nNextPowerUpStep = PU_JUDGE_BARCODE_EXIST;
				}
			}

			if(m_nStartWithPalletData == 3)
			{
				if (IsNoHardware() || PressPush(0,FALSE) && PressPush(1,TRUE))
				{
					m_nNextPowerUpStep = PU_JUDGE_BARCODE_EXIST;
				}
			}
			break;
		}

		//�ж������Ƿ����
	case PU_JUDGE_BARCODE_EXIST:
		{
			m_strMsg = m_bLangulageEng?_T("Judge BarCode Exist"):_T("�ж������Ƿ����");

			if (m_pTempPallet[0].m_strBarCode.IsEmpty())
			{
				m_pTempPallet[0].m_bIsNg = TRUE;
			}
			if (m_pTempPallet[1].m_strBarCode.IsEmpty())
			{
				m_pTempPallet[1].m_bIsNg = TRUE;
			}
			if (m_pBufferPallet.m_strBarCode.IsEmpty())
			{
				m_pBufferPallet.m_bIsNg = TRUE;
			}
			m_nNextPowerUpStep = PU_FINISH;
			break;
		}

		//��ʼ�����
	case PU_FINISH:
		{
			CRun::PowerUpOperation();
			m_strMsg = m_bLangulageEng?_T("Power up finished"):_T("��ʼ�����");

			if (m_ORequiredOnloadSignal > -1)
			{
				OUTPUTS[m_ORequiredOnloadSignal]->Off();//�ر���ǻ�?��������ź� Yao 5.9
			}
			
			if (m_OPLCRunningSignal > -1)
			{
				OUTPUTS[m_OPLCRunningSignal]->On();//�������߷����������ź� Yao 5.9
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

void CRunProcessOnloadCore::AutoOperation()
{
	if (!IsModuleEnable())
	{
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		Sleep(500);
		return;
	}

	if (IsNoHardware())
	{
		Sleep(500);
	}

	if (m_nSaveLastStep == -1)
	{
		//��һ�μ�¼����Ϳ�ʼʱ��
		m_nSaveLastStep = m_nNextAutoStep;
		m_nStepStartTime = GetTickCount();
	}
	if (m_nSaveLastStep != m_nNextAutoStep)
	{
		//��һ����¼��ʼ�ָ���
		if (m_nSaveLastStep == AUTO_MOVE_Y_TO_ONLOAD_POS)
		{
			WriteCTLog(_T("----------��ʼ----------"));
		}
		//ÿ�л�һ�β����¼һ��CT
		DWORD nStepEndTime = GetTickCount() - m_nStepStartTime;
		CString strText = _T("");
		strText.Format(_T("%s-��ʱ��%d"),m_strMsg.m_strRunInfo,nStepEndTime);
		WriteCTLog (strText);
		m_nTotalTime += nStepEndTime;
		//���һ����¼��ʱ��
		if (m_nSaveLastStep == AUTO_WORK_END)
		{
			CString str = _T("");
			str.Format(_T("��ʱ�䣺%d"),m_nTotalTime);
			WriteCTLog (str);
			WriteCTLog (_T("----------����----------"));
			m_nTotalTime = 0;
		}
		//�л���������¼�¼�µĲ���Ϳ�ʼʱ��
		m_nSaveLastStep = m_nNextAutoStep;
		m_nStepStartTime = GetTickCount();
	}

	switch(m_nNextAutoStep)
	{	
		//�ƶ�Y�ᵽȡ��λ
	case AUTO_MOVE_Y_TO_ONLOAD_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move X To Onload Pos"):_T("�ƶ�Y�ᵽȡ��λ");

			if(m_nMotorY > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorY, Y_MOTOR_PICK_LOC))
			{
				m_nNextAutoStep = AUTO_ROTATE_CLY_RETURN_INIT;
			}
			break;
		}
		//��ת���׻���
	case AUTO_ROTATE_CLY_RETURN_INIT:
		{
			m_strMsg = m_bLangulageEng?_T("Rotate Cly Return"):_T("��ת���׻���");

			/*if(IsNoHardware() || RotatePush(-1, FALSE))*/
			{
				m_nNextAutoStep = AUTO_PRESS_CLY_RETURN_INIT;
			}
			break;
		}
		//ѹ��о����
	case AUTO_PRESS_CLY_RETURN_INIT:
		{
			m_strMsg = m_bLangulageEng?_T("Press Cly Return"):_T("ѹ��о����");

			if(IsNoHardware() || PressPush(-1, FALSE))
			{
				m_nNextAutoStep = AUTO_FINER_CLY_OPEN_INIT;
			}
			break;
		}
		//��צ��
	case AUTO_FINER_CLY_OPEN_INIT:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Open"):_T("��צ��");

			if(IsNoHardware() || FingerClose(-1, FALSE))
			{
				m_nWaitTime = GetTickCount();
				//m_nNextAutoStep = AUTO_WAIT_START_EVENT_LINE;
				m_nNextAutoStep = AUTO_WAIT_READY_ONLOAD_SIGNAL;   //�ȴ�������׼���������ź� Yao  5.9
			}
			break;
		}

		//�ȴ�������׼���������ź�   Yao  5.9
	case AUTO_WAIT_READY_ONLOAD_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Ready Oload Sigal"):_T("�ȴ�������׼���������ź�");

			if(IsNoHardware() || m_bDryRun || MACHINE.m_bUnEnableLineSingle || m_IPLCRunningSignal < 0 || INPUTS[m_IPLCRunningSignal]->IsOn()
				&&m_IReadyOnloadSignal < 0 ||  INPUTS[m_IReadyOnloadSignal]->IsOn())
			{
				if(m_bDryRun)
				{
					//Sleep(2000);
					MACHINE.m_pReadCode[0]->m_bCanStarScaleCode = TRUE; 
					MACHINE.m_pReadCode[1]->m_bCanStarScaleCode = TRUE; 
					m_nNextAutoStep = AUTO_WAIT_START_EVENT_LINE;
				}
				else
				{
					if (MACHINE.m_pBarCodeNgLine != NULL && INPUTS[MACHINE.m_pBarCodeNgLine->m_IManualBtn]->IsOn())  // //����֮��Ҫ���ֶ������ź�
					{
						Sleep(3000);
						if (MACHINE.m_pBarCodeNgLine != NULL && INPUTS[MACHINE.m_pBarCodeNgLine->m_IManualBtn]->IsOn())
						{
							MACHINE.m_pReadCode[0]->m_bCanStarScaleCode = TRUE; 
							MACHINE.m_pReadCode[1]->m_bCanStarScaleCode = TRUE; 
							m_nNextAutoStep = AUTO_WAIT_START_EVENT_LINE;
						}
					}

				}
			}
			break;
		}

		//�ȴ���ʼ�ź�
	case AUTO_WAIT_START_EVENT_LINE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"):_T("�ȴ�ɨ�빤λ��ʼ�ź�");

			if (m_bDryRun || MACHINE.m_pReadCode[0] != NULL && (MACHINE.m_pReadCode[0]->m_bRequirePallet || !MACHINE.m_pReadCode[0]->IsModuleEnable())
				&& MACHINE.m_pReadCode[1] != NULL && (MACHINE.m_pReadCode[1]->m_bRequirePallet || !MACHINE.m_pReadCode[1]->IsModuleEnable()))
			{
				if (!MACHINE.m_pReadCode[0]->IsModuleEnable() && !MACHINE.m_pReadCode[1]->IsModuleEnable())
				{
					m_pTempPallet[0].m_strBarCode.Format(_T("%d"),GetTickCount());
					m_pTempPallet[1].m_strBarCode.Format(_T("%d"),GetTickCount());
				}
				MACHINE.m_bWaitMaterial = FALSE;
				m_nNextAutoStep = AUTO_MOVE_Z_TO_ONLOAD_POS;


				//�رտ�ɨ���ź�  Yao  5.9
				MACHINE.m_pReadCode[0]->m_bCanStarScaleCode = FALSE; 
				MACHINE.m_pReadCode[1]->m_bCanStarScaleCode = FALSE; 
				
			}
			if (GetTickCount() - m_nWaitTime > 10000)
			{
				MACHINE.m_bWaitMaterial = TRUE;
			}

			break;
		}

		//�ƶ�Z�ᵽȡ��λ
	case AUTO_MOVE_Z_TO_ONLOAD_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Pick Pos"):_T("�ƶ�Z�ᵽȡ��λ");
			if(m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_PICK_LOC))
			{
				m_nNextAutoStep = AUTO_CHECK_AXIS_STATE;
			}
			break;
		}
		//�����״̬
	case AUTO_CHECK_AXIS_STATE:
		{
			m_strMsg = m_bLangulageEng?_T("Check Axis Stste"):_T("�����״̬");
			if (CheckZSafe())
			{
				m_nNextAutoStep = AUTO_FINGER_CYL_CLOSE;
			}
			break;
		}

		//��ץ���׼н�
	case AUTO_FINGER_CYL_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Close"):_T("��ץ���׼н�");
			if(IsNoHardware() || FingerClose(-1, TRUE))
			{
				//��ȡ����������
				if (MACHINE.m_pReadCode[0] != NULL && MACHINE.m_pReadCode[1] != NULL)
				{
					
					m_pTempPallet[0] = MACHINE.m_pReadCode[0]->m_Pallet;
					m_pTempPallet[1] = MACHINE.m_pReadCode[1]->m_Pallet;

					if (!MACHINE.m_pReadCode[0]->IsModuleEnable() || !MACHINE.m_pReadCode[1]->IsModuleEnable())//����ʱû�ж���ģ�飬ǿ�Ƹ�ֵ
					{
						m_pTempPallet[0].m_bIsValid = TRUE;    
						m_pTempPallet[1].m_bIsValid = TRUE;
					}
					MACHINE.m_pReadCode[0]->m_Pallet.Release();
					MACHINE.m_pReadCode[1]->m_Pallet.Release();
				}
				MACHINE.m_nOnloadCount += 2;

				m_nNextAutoStep = AUTO_PRESS_CYL_PUSH;
				SaveStepInfo(WRITE_TYPE_BATTERY);
			}
			break;
		}

		//ѹ��о�����Ƴ�
	case AUTO_PRESS_CYL_PUSH:
		{
			m_strMsg = m_bLangulageEng?_T("Press Cyl Push"):_T("ѹ��о�����Ƴ�");
			if(IsNoHardware() || PressPush(-1, TRUE))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_TO_SAFE_POS;
			}
			break;
		}

		//�ƶ�Z�ᵽ��ȫλ
	case AUTO_MOVE_Z_TO_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Safe Pos"):_T("�ƶ�Z�ᵽ��ȫλ");

			if(m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_SAFE_LOC))
			{
				m_nNextAutoStep = AUTO_CHECK_HAVE_PRODUCT;
			}
			break;
		}

		//����Ƿ�����
	case AUTO_CHECK_HAVE_PRODUCT:
		{
			m_strMsg = m_bLangulageEng?_T("check Finger Status"):_T("����Ƿ�����");

			if (m_bDryRun || (CheckHasVaild(0, TRUE) && CheckHasVaild(1, TRUE)))                         
			{
				m_nNextAutoStep = AUTO_SEND_ONLOAD_FINISH;
			}
			break;
		}

		/*�����������*/
	case AUTO_SEND_ONLOAD_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Send Onload Finish"):_T("�����������");

			//������������ź�IO �ڰ�ȫλ����
			if (MACHINE.m_pReadCode[0] != NULL && MACHINE.m_pReadCode[0]->m_bRequirePallet
				&& MACHINE.m_pReadCode[1] != NULL && MACHINE.m_pReadCode[1]->m_bRequirePallet)
			{
				MACHINE.m_pReadCode[0]->m_bRequirePallet = FALSE;
				MACHINE.m_pReadCode[1]->m_bRequirePallet = FALSE;
			}

			//�������߷���ȡ��ɨ�뼰ȡ������ź�  Yao  5.9
			if(m_ORequiredOnloadSignal > -1 )
			{
				OUTPUTS[m_ORequiredOnloadSignal]->On();
			}

			/*m_nNextAutoStep = AUTO_ROTATE_CYL_PUSH;*/

			m_nNextAutoStep = AUTO_WAIT_REPLY_SIGNAL;//�ȴ������߷�������ź�  Yao  5.9
			break;
		}

		//�ȴ������߷�������ź� Yao 5.9
	case AUTO_WAIT_REPLY_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T("Rotate Cly Return"):_T("�ȴ������߷�������ź�");

			//if (m_IReadyOnloadSignal > -1 && INPUTS[m_IReadyOnloadSignal]->IsOff())
			//{
			//	if (m_ORequiredOnloadSignal > -1)
			//	{
			//		OUTPUTS[m_ORequiredOnloadSignal]->Off();//�ر���������ź�
			//	}

			//}

			m_nNextAutoStep = AUTO_ROTATE_CYL_PUSH;
			break;
		}



		//��ת�����Ƴ�
	case AUTO_ROTATE_CYL_PUSH:
		{
			m_strMsg = m_bLangulageEng?_T("Rotate Cly Return"):_T("��ת�����Ƴ�");

			/*if(IsNoHardware() || RotatePush(-1, TRUE))*/
			{
				//m_nWorkIndex = -1;                       //���Բ�����
				/*	m_pTempPallet[0].m_bIsNg =FALSE;
				m_pTempPallet[1].m_bIsNg=FALSE;
				m_pTempPallet[0].m_bIsValid = TRUE;
				m_pTempPallet[1].m_bIsValid = TRUE;*/
				m_nNextAutoStep = AUTO_JUDGE_HAVE_NG;
			}
			break;
		}

		//�ж�����NG
	case AUTO_JUDGE_HAVE_NG:
		{
			m_strMsg = m_bLangulageEng?_T("Judge Have Ng"):_T("�ж�����NG");
			if(m_bDryRun)
			{
				m_nWorkIndex = -1;
				m_nNextAutoStep = AUTO_MOVE_Y_TO_OFFLOAD_POS;
				break;
			}

			if (m_pTempPallet[0].m_bIsValid && m_pTempPallet[1].m_bIsValid)
			{
				if(!m_pTempPallet[0].m_bIsNg && !m_pTempPallet[1].m_bIsNg)
				{
					m_nWorkIndex = -1;
					m_nNextAutoStep = AUTO_MOVE_Y_TO_OFFLOAD_POS;
				}
				else
				{
					if(m_pTempPallet[0].m_bIsNg && m_pTempPallet[1].m_bIsNg)
						m_nWorkIndex = -1;
					else if(m_pTempPallet[0].m_bIsNg)
						m_nWorkIndex = 0;
					else if(m_pTempPallet[1].m_bIsNg)
						m_nWorkIndex = 1;
					else
						ASSERT(0);
					m_nNextAutoStep = AUTO_MOVE_Y_TO_NG_POS;

				}
			}
			else if(m_pTempPallet[0].m_bIsValid || m_pTempPallet[1].m_bIsValid)
			{
				if(m_pTempPallet[0].m_bIsNg || m_pTempPallet[1].m_bIsNg)
				{
					if(m_pTempPallet[0].m_bIsNg)
						m_nWorkIndex = 0;
					else
						m_nWorkIndex = 1;
					m_nNextAutoStep = AUTO_MOVE_Y_TO_NG_POS;
				}
				else
				{
					if(!m_pBufferPallet.m_bIsValid)
					{// ���ݴ�
						if(m_pTempPallet[0].m_bIsValid)
							m_nWorkIndex = 0;
						else
							m_nWorkIndex = 1;
						m_nNextAutoStep = AUTO_MOVE_Y_TO_BUFFER_POS;
					}
					else
					{// ȡ�ݴ�
						if(!m_pTempPallet[0].m_bIsValid)
							m_nWorkIndex = 0;
						else
							m_nWorkIndex = 1;
						m_nNextAutoStep = AUTO_MOVE_Y_TO_BUFFER_POS;
					}
				}	
			}
			else
			{
				m_nNextAutoStep = AUTO_MOVE_Y_TO_OFFLOAD_POS;
				/*m_nNextAutoStep = AUTO_WORK_END;*/
			}
			break;
		}

		//offload
		//�ƶ�Y�ᵽ����λ
	case AUTO_MOVE_Y_TO_OFFLOAD_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move X To Offlaod Pos"):_T("�ƶ�Y�ᵽ����λ");

			if (m_nMotorY > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorY, Y_MOTOR_PNP_OFFLOAD_LOC))
			{
				m_nNextAutoStep = AUTO_WAIT_NEXT_MOUDLE_REQUIRE;                         
			}		
			break;
		}

		//�ȴ���һģ��Ҫ���ź�
	case AUTO_WAIT_NEXT_MOUDLE_REQUIRE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Next Module Require"):_T("�ȴ���һģ��Ҫ���ź�");
		
			if(MACHINE.m_pBatteryCore[0] != NULL && MACHINE.m_pBatteryCore[0]->m_bRequireCore 
				&& MACHINE.m_pBatteryCore[1] != NULL && MACHINE.m_pBatteryCore[1]->m_bRequireCore)
			{
				m_nNextAutoStep = AUTO_MOVE_Z_TO_OFFLOAD_POS;
			}
			break;
		}

		//�ƶ�Z�ᵽ����Ʒλ
	case AUTO_MOVE_Z_TO_OFFLOAD_POS: 
		{
			m_strMsg = m_bLangulageEng?_T("Motor Z To Put Ok Pos"):_T("�ƶ�Z�ᵽ����Ʒλ");

			if(m_nMotorZ> -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_PUT_OK_LOC))
			{
				m_nNextAutoStep = AUTO_PRESS_CYL_RETURN_OK;
			}
			break;
		}

			//ѹ��о����
	case AUTO_PRESS_CYL_RETURN_OK:
		{
			m_strMsg = m_bLangulageEng?_T("Press Cly Return"):_T("ѹ��о����");
			if(IsNoHardware() || PressPush(-1, FALSE))
			{
				m_nNextAutoStep = AUTO_FINGER_CYL_OPEN_OK;
			}
			break;
		}


		//��צ��
	case AUTO_FINGER_CYL_OPEN_OK:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Open"):_T("��צ��");
			if(MACHINE.m_pBatteryCore[0]  !=  NULL  && MACHINE.m_pBatteryCore[0]->ColumnCylClose(TRUE)
		      && MACHINE.m_pBatteryCore[1]  !=  NULL  && MACHINE.m_pBatteryCore[1]->ColumnCylClose(TRUE))
			{
			if(IsNoHardware() || FingerClose(-1, FALSE))
			{
				m_nNextAutoStep = AUTO_SEND_DATE;
			}
			}
			break;
		}

		//���Ƶ�о��λ��������
	case AUTO_SEND_DATE:
		{
			m_strMsg = m_bLangulageEng?_T("Send Date"):_T("���Ƶ�о��λ��������");

			if(MACHINE.m_pBatteryCore[0] != NULL && (MACHINE.m_pBatteryCore[0]->m_bRequireCore || !MACHINE.m_pBatteryCore[0]->IsModuleEnable())
				&& MACHINE.m_pBatteryCore[1] != NULL && (MACHINE.m_pBatteryCore[1]->m_bRequireCore || !MACHINE.m_pBatteryCore[1]->IsModuleEnable()))
			{
				for(int i = 0; i < 2; i++)
				{
					MACHINE.m_pBatteryCore[i]->m_Pallet = m_pTempPallet[i];
					m_pTempPallet[i].Release();
					
					MACHINE.m_pBatteryCore[i]->SaveStepInfo(WRITE_TYPE_BATTERY);
				}
				m_nNextAutoStep = AUTO_MOVE_Z_TO_SAFE_POS_OK;
				SaveStepInfo(WRITE_TYPE_BATTERY);
			}
			break;
		}

		//�ƶ�Z�ᵽȡ��λ
	case AUTO_MOVE_Z_TO_SAFE_POS_OK:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Pick Pos"):_T("�ƶ�Z�ᵽ��ȫλ");

			if(m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_SAFE_LOC))
			{
				if(MACHINE.m_pBatteryCore[0] != NULL && MACHINE.m_pBatteryCore[1] != NULL)
				{//������������ź�
					MACHINE.m_pBatteryCore[0]->m_bRequireCore =FALSE;
					MACHINE.m_pBatteryCore[1]->m_bRequireCore =FALSE;
				}
				m_nNextAutoStep = AUTO_JUDGE_BATTERY_ALL_PUT_FINISH;
			}
			break;
		}

		//ng
		//�ƶ�Y�ᵽNGλ
	case AUTO_MOVE_Y_TO_NG_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move X To Ng Pos"):_T("�ƶ�Y�ᵽNGλ");
			

			if (m_nMotorY > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorY, Y_MOTOR_NG_POS))
			{
				m_nNextAutoStep = AUTO_WAIT_NG_MODULE_REQUIRE;
			}			
			break;
		}

		//�ȴ�NG��Ҫ���ź�
	case AUTO_WAIT_NG_MODULE_REQUIRE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Ng Module Require"):_T("�ȴ�NG��Ҫ���ź�");

			if(MACHINE.m_pBarCodeNgLine != NULL 
				&& (MACHINE.m_pBarCodeNgLine->m_bRequireNgBattery || !MACHINE.m_pBarCodeNgLine->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_TO_NG_POS;
			}
			break;
		}

		//�ƶ�Z�ᵽ��NGλ
	case AUTO_MOVE_Z_TO_NG_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Put Ng Pos"):_T("�ƶ�Z�ᵽ��NGλ");

			if(m_nMotorZ> -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_PUT_NG_LOC))
			{
				m_nNextAutoStep = AUTO_PRESS_CYL_RETURN_NG;
			}
			break;
		}

		//ѹ��о����
	case AUTO_PRESS_CYL_RETURN_NG:
		{
			m_strMsg = m_bLangulageEng?_T("Press Cly Return"):_T("ѹ��о����");

			if(IsNoHardware() || PressPush(m_nWorkIndex, FALSE))
			{
				m_nNextAutoStep = AUTO_FINGER_OPEN_NG;
			}
			break;
		}

		//�ڷ�NGλ��ץ��
	case AUTO_FINGER_OPEN_NG:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Open for Put Ng"):_T("�ڷ�NGλ��ץ��");

			if(IsNoHardware() || FingerClose(m_nWorkIndex, FALSE))
			{
				if(0 <= m_nWorkIndex && m_nWorkIndex <= 1)
				{
					m_pTempPallet[m_nWorkIndex].Release();
					//MACHINE.m_nNgCount++;    //ɨ�벻�㲻��
					MACHINE.m_nScanNgCount++;
					//MACHINE.m_ReportFile.NgCount();
				}
				else if(m_nWorkIndex == -1)
				{
					m_pTempPallet[0].Release();
					m_pTempPallet[1].Release();
					//MACHINE.m_nNgCount += 2;
					MACHINE.m_nScanNgCount += 2;
					//MACHINE.m_ReportFile.NgCount();
					//MACHINE.m_ReportFile.NgCount();
				}
				else
				{
					ASSERT(0);
				}
				m_nNextAutoStep = AUTO_MOVE_Z_TO_SAFE_PO_NG;
				SaveStepInfo(WRITE_TYPE_BATTERY);
			}
			break;
		}

		//����NG��Z��ص���ȫλ
	case AUTO_MOVE_Z_TO_SAFE_PO_NG:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Safe Pos After Pug Ng"):_T("����NG��Z��ص���ȫλ");

			if(m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_SAFE_LOC))
			{
				m_nNextAutoStep = AUTO_RESET_NG_LINE_REQUIRE;
			}
			break;
		}

		//��NG�߷�������ź�
	case AUTO_RESET_NG_LINE_REQUIRE:
		{
			m_strMsg = m_bLangulageEng?_T("Reset Ng Line Require"):_T("��NG�߷�������ź�");

			if(MACHINE.m_pBarCodeNgLine != NULL 
				&& (MACHINE.m_pBarCodeNgLine->m_bRequireNgBattery || !MACHINE.m_pBarCodeNgLine->IsModuleEnable()))
			{
				MACHINE.m_pBarCodeNgLine->m_bRequireNgBattery = FALSE;
				m_nNextAutoStep = AUTO_JUDGE_BATTERY_ALL_PUT_FINISH;
			}
			break;
		}

		// buffer
	case AUTO_MOVE_Y_TO_BUFFER_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Y To Buffer Pos"):_T("�ƶ�Y��ȡ����λ");

			if(m_nMotorY > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorY, Y_MOTOR_BUFFER_POS))
			{
				m_nNextAutoStep = AUTO_BUFFER_CYL_ACTION;
			}
			break;
		}

		//����λ���׶���
	case AUTO_BUFFER_CYL_ACTION:
		{
			m_strMsg = m_bLangulageEng?_T("Reserve Cyl Action"):_T("����λ���׶���");
			// �˴�����ʵ�ʵ����޸�
			if (m_pBufferPallet.m_bIsValid)
			{
				if(IsNoHardware() || BufferPush(m_nWorkIndex == 0?FALSE:TRUE))
				{
					m_nNextAutoStep = AUTO_CHECK_HAVE_PRODUCT_BUFFER;
				}
			}
			else
			{
				if(IsNoHardware() || BufferPush(m_nWorkIndex == 0?TRUE:FALSE))
				{
					m_nNextAutoStep = AUTO_CHECK_HAVE_PRODUCT_BUFFER;
				}
			}
			break;
		}

		//����λ���׶���
	case AUTO_CHECK_HAVE_PRODUCT_BUFFER:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Product Buffer"):_T("��⻺���о״̬");
			// �˴�����ʵ�ʵ����޸�
			if (IsNoHardware() || m_bDryRun || CheckHasVaild((m_nWorkIndex+2), m_pBufferPallet.m_bIsValid))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_TO_BUFFER_POS;
			}
			break;
		}

		//�ƶ�Z�ᵽ����λ
	case AUTO_MOVE_Z_TO_BUFFER_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Buffer Pos"):_T("�ƶ�Z�ᵽ����λ");

			if(m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_BUFFER_LOC))
			{
				m_nNextAutoStep = AUTO_PRESS_CYL_RETURN_BUF_EX;
			}
			break;
		}

		//�ڻ���λѹ��о����
	case AUTO_PRESS_CYL_RETURN_BUF_EX:
		{
			m_strMsg = m_bLangulageEng?_T("Press Cly Return"):_T("�ڻ���λѹ��о����");

			if (m_pBufferPallet.m_bIsValid)   //����������ϣ�ѹ�����ײ�����
			{
				m_nNextAutoStep = AUTO_FINGER_CYL_OPEN_BUF;
				break;
			}
			if(IsNoHardware() || PressPush(1-m_nWorkIndex, FALSE))
			{
				m_nNextAutoStep = AUTO_FINGER_CYL_OPEN_BUF;
			}
			break;
		}

		//�ڻ���λ��ץ����
	case AUTO_FINGER_CYL_OPEN_BUF:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl For Buffer Pos"):_T("�ڻ���λ��ץ����");

			ASSERT(0 <= m_nWorkIndex && m_nWorkIndex <= 1);
			int index;
			if (m_pBufferPallet.m_bIsValid)
			{
				index = m_nWorkIndex;
			}
			else
			{
				index = 1 - m_nWorkIndex;
			}
			if(IsNoHardware() || FingerClose(index, m_pBufferPallet.m_bIsValid))
			{
				if (m_pBufferPallet.m_bIsValid)
				{
					m_pTempPallet[m_nWorkIndex] = m_pBufferPallet;
					m_pBufferPallet.Release();
				}
				else
				{
					m_pBufferPallet = m_pTempPallet[1-m_nWorkIndex];
					m_pTempPallet[1-m_nWorkIndex].Release();
				}
				m_nNextAutoStep =AUTO_PRESS_CYL_RETURN_BUF;
				SaveStepInfo(WRITE_TYPE_BATTERY);
			}
			break;
		}

		//�ڻ���λѹ��о����
	case AUTO_PRESS_CYL_RETURN_BUF:
		{
			m_strMsg = m_bLangulageEng?_T("Press Cly Return"):_T("�ڻ���λѹ��о����");

			if (m_pBufferPallet.m_bIsValid)   //����������ϣ�ѹ�����׶���
			{
				m_nNextAutoStep = AUTO_MOVE_Z_TO_SAFE_POS_BUF;
				break;
			}
			if(IsNoHardware() || PressPush(1-m_nWorkIndex, TRUE))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_TO_SAFE_POS_BUF;
			}
			break;
		}

	case AUTO_MOVE_Z_TO_SAFE_POS_BUF:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Safe Pos For Buffer"):_T("�ڻ���λ�ƶ�Z�ᵽ��ȫλ");

			if(m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_SAFE_LOC))
			{
				m_nNextAutoStep = AUTO_CHECK_WITHOUT_PRODUCT_BUF;
			}
			break;
		}

		//����Ƿ�����
	case AUTO_CHECK_WITHOUT_PRODUCT_BUF:
		{
			m_strMsg = m_bLangulageEng?_T("check Finger Status"):_T("����ȡ/������ɣ�����צ�ͻ�����״̬");

			int Index = m_nWorkIndex;
			if (m_pBufferPallet.m_bIsValid)
			{
				Index = 1 - m_nWorkIndex;
			}
			if (m_bDryRun || (CheckHasVaild(Index, m_pTempPallet[Index].m_bIsValid) && CheckHasVaild(Index + 2, m_pBufferPallet.m_bIsValid)))                        
			{
				m_nNextAutoStep = AUTO_JUDGE_BATTERY_ALL_PUT_FINISH;
			}
			break;
		}

		//�ж����е�ط����
	case AUTO_JUDGE_BATTERY_ALL_PUT_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Judeg Put Ok All Finish"):_T("�ж����е�ط����");
			
			if (m_pTempPallet[0].m_bIsValid && m_pTempPallet[1].m_bIsValid)
			{
				m_nNextAutoStep = AUTO_MOVE_Y_TO_OFFLOAD_POS;
			}
			else if (!m_pTempPallet[0].m_bIsValid && !m_pTempPallet[1].m_bIsValid)
			{
				m_nNextAutoStep = AUTO_WORK_END;
			}
			else
			{
				m_nNextAutoStep = AUTO_MOVE_Y_TO_BUFFER_POS;
			}
			break;
		}
		//�������
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("�������");
		
			m_nNextAutoStep = AUTO_MOVE_Y_TO_ONLOAD_POS;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}

	if(MACHINE.m_bCreateCTLog && m_nNextAutoStep != m_nAutoNext)
	{
		m_dwModuleStepCT = GetTickCount() - m_dwModuleStrt;
		m_dwModuleCT += m_dwModuleStepCT;

		if(m_nNextAutoStep == AUTO_MOVE_Y_TO_ONLOAD_POS)
		{
			m_strModuleCTtemp.Format(_T("��%s������CT��%.2f\r\n\r\n-----------------------------��%s������ѭ��CT��%.2f-----------------------------\r\n"), m_strMsg.GetInfo(), (double)(m_dwModuleStepCT/1000.0), m_strName, (double)(m_dwModuleCT/1000.0));
			m_dwModuleCT = 0;
		}
		else
		{
			m_strModuleCTtemp.Format(_T("��%s������CT��%.2f\r\n"), m_strMsg.GetInfo(), (double)(m_dwModuleStepCT/1000.0));
		}
		WriteLog(m_strModuleCTtemp);
	}
}

BOOL CRunProcessOnloadCore::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
	{
		return FALSE;
	}

	AfxBeginThread((AFX_THREADPROC)OnloadCoreThread, this);
	return TRUE;
}

void CRunProcessOnloadCore::GetHardwareConfig(CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs, pOutputs, pMotors);
}

BOOL CRunProcessOnloadCore::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
#ifdef _TEST_
	return TRUE;
#endif
	ASSERT(m_nMotorY > -1 && m_nMotorZ > -1);
	if (pMotor == MOTORS[m_nMotorY])
	{
		if (!MOTORS[m_nMotorZ]->IsOriRefKnown())
		{
			BLMessageBox(_T("��M07����о����ץ��Z��û�и�λ��Y�᲻���ƶ�"), MB_ICONWARNING);
			return FALSE;
		}
		
		double dPosZ0 = MOTORS[m_nMotorZ]->GetCurPos();
		double dPosZ1 = GetMotorPos(MACHINE.m_nDefaultProduct, m_nMotorZ, Z_MOTOR_SAFE_LOC);

		if (fabs(dPosZ0 - dPosZ1) > 0.5)
		{
			BLMessageBox(_T("(M07)��о����ץ��Z�᲻�ڰ�ȫλ��Y�᲻���ƶ�"), MB_ICONWARNING);
			return FALSE;
		}
	}

	if (pMotor == MOTORS[m_nMotorZ])
	{
		if (iLoc == Z_MOTOR_PICK_LOC)
		{
			double pos = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct),m_nMotorY,Y_MOTOR_PICK_LOC);
			if (fabs(MOTORS[m_nMotorY]->GetCurPos() - pos) > 0.5)
			{
				BLMessageBox(_T("(M06)��о����ץ��Y�᲻��ȡ��λ��Z�᲻���ƶ���ȡ��λ"), MB_ICONWARNING);
				return FALSE;
			}

			/*for (int i = 0;i < 2 ; i++)
			{
			ASSERT(m_IRotatePush[i] > -1 && m_IRotateReturn[i] > -1);
			ASSERT(m_IPressPush[i] > -1 && m_IPressReturn[i] >  -1);

			if (INPUTS[m_IRotateReturn[i]]->IsOff() || INPUTS[m_IRotatePush[i]]->IsOn()
			|| INPUTS[m_IPressReturn[i]]->IsOff() || INPUTS[m_IPressPush[i]]->IsOn())
			{
			CString strKey;
			strKey.Format(_T("��о����ץ��%d��ת/ѹ������״̬���ԣ�Z�᲻���ƶ���ȡ��λ"),i);
			BLMessageBox(strKey, MB_ICONWARNING);
			return FALSE;
			}
			}*/

			for (int i = 0; i < 2; i++)
			{
				ASSERT(m_IFingerClose[i] > -1 && m_IFingerOpen[i] > -1);

				if (INPUTS[m_IFingerClose[i]]->IsOn() || INPUTS[m_IFingerOpen[i]]->IsOff())
				{
					CString strKey;
					strKey.Format(_T("��о����ץ��%d�ӽ���Z�᲻���ƶ���ȡ��λ"), i/2);
					BLMessageBox(strKey, MB_ICONWARNING);
					return FALSE;
				}
			}
		}

		if (iLoc == Z_MOTOR_PUT_OK_LOC)
		{
			if (m_nMotorY > -1)
			{
				double pos = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct),m_nMotorY,Y_MOTOR_PNP_OFFLOAD_LOC);
				if (fabs(MOTORS[m_nMotorY]->GetCurPos() - pos) > 0.5)
				{
					BLMessageBox(_T("(M06)��о����ץ��Y�᲻�ڷ���λ��Z�᲻���ƶ�������λ"), MB_ICONWARNING);
					return FALSE;
				}
			}

			if (MACHINE.m_pBatteryCore[m_nGroupID] == NULL)
			{
				BLMessageBox(_T("ģ�����"), MB_ICONWARNING);
				return FALSE;
			}

			ASSERT(MACHINE.m_pBatteryCore[0] != NULL && MACHINE.m_pBatteryCore[1] != NULL);

			int IHaveCore[2] = {MACHINE.m_pBatteryCore[0]->m_IHaveCore, MACHINE.m_pBatteryCore[1]->m_IHaveCore};
			ASSERT(IHaveCore[0] > -1 && IHaveCore[1] > -1);
			ASSERT(m_IHasCore[0] > -1 && m_IHasCore[1] > -1);

			if ((INPUTS[m_IHasCore[0]]->IsOn() || INPUTS[m_IHasCore[1]]->IsOn()) 
				&& (INPUTS[IHaveCore[0]]->IsOn() || INPUTS[IHaveCore[1]]->IsOn()))
			{
				BLMessageBox(_T("��о����ģ�������ϣ��Ƶ�о�������ϣ�Z�᲻���ƶ�������λ"), MB_ICONWARNING);
				return FALSE;
			}
		}

		if (iLoc == Z_MOTOR_PUT_NG_LOC)
		{
			double pos = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct),m_nMotorY,Y_MOTOR_NG_POS);
			if (fabs(MOTORS[m_nMotorY]->GetCurPos() - pos) > 0.5)
			{
				BLMessageBox(_T("(M06)��о����ץ��Y�᲻��NGλ��Z�᲻���ƶ���NGλ"), MB_ICONWARNING);
				return FALSE;
			}

			int ICheckBattery[2] = {MACHINE.m_pBarCodeNgLine->m_ICheckBattery[0], MACHINE.m_pBarCodeNgLine->m_ICheckBattery[1]};
			ASSERT(ICheckBattery[0] > -1 && ICheckBattery[1] > -1);
			ASSERT(m_IHasCore[0] > -1 && m_IHasCore[1] > -1);

			if ((INPUTS[m_IHasCore[0]]->IsOn() || INPUTS[m_IHasCore[1]]->IsOn()) 
				&& (INPUTS[ICheckBattery[0]]->IsOn() || INPUTS[ICheckBattery[1]]->IsOn()))
			{
				BLMessageBox(_T("��о����ģ�������ϣ�ɨ��NGģ�������ϣ�Z�᲻���ƶ�����NGλ"), MB_ICONWARNING);
				return FALSE;
			}
		}
		if (iLoc == Z_MOTOR_BUFFER_LOC)
		{
			if (m_nMotorY > -1)
			{
				double pos = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct),m_nMotorY,Y_MOTOR_BUFFER_POS);
				if (fabs(MOTORS[m_nMotorY]->GetCurPos() - pos) > 0.5)
				{
					BLMessageBox(_T("(M06)��о����ץ��Y�᲻�ڻ���λ��Z�᲻���ƶ�������λ"), MB_ICONWARNING);
					return FALSE;
				}
			}


			/*ASSERT(m_IBufferCylPush > -1 && m_IBufferCylReturn > -1);

			if (INPUTS[m_IBufferCylPush]->IsOn() || INPUTS[m_IBufferCylReturn]->IsOff())
			{
			CString strKey;
			strKey.Format(_T("��о����ץ��%d��������״̬���ԣ�Z�᲻���ƶ�������λ"));
			BLMessageBox(strKey,MB_ICONWARNING);
			return FALSE;
			}	*/
		}
	}

	return TRUE;
}

BOOL CRunProcessOnloadCore::CheckOutputCanActive(COutput *pOutput)
{
#ifdef _TEST_
	return TRUE;
#endif
	ASSERT(m_OBufferCylPush > -1 && m_OBufferCylReturn > -1);

	ASSERT(m_nMotorZ > -1);

	double dPosZ0 = MOTORS[m_nMotorZ]->GetCurPos();
	double dPosZ1 = GetMotorPos(MACHINE.m_nDefaultProduct, m_nMotorZ, Z_MOTOR_SAFE_LOC);

	if (fabs(dPosZ0 - dPosZ1) > 0.5)
	{
		if (pOutput == OUTPUTS[m_OBufferCylPush] || pOutput == OUTPUTS[m_OBufferCylReturn])
		{
			BLMessageBox(_T("(M07)��о����ץ��Z�᲻�ڰ�ȫλ���������ײ����ƶ�"), MB_ICONWARNING);
			return FALSE;
		}

	}
	return  TRUE;
}

/****************************************************************************************
������������צ���׶���
���������nIndex:-1�������м�צ���ף�Ϊ0-1ʱ�ֱ�����צ����1��2
bClose:TRUE:��צ���׼н� FALSE:��צ�����ɿ�
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessOnloadCore::FingerClose(int nIndex, BOOL bClose)
{
	ASSERT(-1 <= nIndex && nIndex <= 1);
	ASSERT(m_IFingerClose[0] > -1 && m_IFingerClose[1] > -1 
		&& m_IFingerOpen[0] > -1 && m_IFingerOpen[1] > -1 
		&& m_OFingerClose[0] > -1 && m_OFingerClose[1] > -1 
		&& m_OFingerOpen[0] > -1 && m_OFingerOpen[1] > -1);

	BOOL bRes = FALSE;
	if(nIndex > -1 && nIndex < 2)
	{
		bClose?OUTPUTS[m_OFingerClose[nIndex]]->On():OUTPUTS[m_OFingerClose[nIndex]]->Off();
		bClose?OUTPUTS[m_OFingerOpen[nIndex]]->Off():OUTPUTS[m_OFingerOpen[nIndex]]->On();

		if(m_bDryRun)
			Sleep(300);

		if((m_bDryRun && bClose)
			|| WAIT_OK == WaitInput(INPUTS[m_IFingerClose[nIndex]], bClose?ON:OFF, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IFingerOpen[nIndex]], bClose?OFF:ON, MSG_SHOW_DEFAULT))
		{
			bRes = TRUE;
		}
	}
	else
	{
		bClose?OUTPUTS[m_OFingerClose[0]]->On():OUTPUTS[m_OFingerClose[0]]->Off();
		bClose?OUTPUTS[m_OFingerClose[1]]->On():OUTPUTS[m_OFingerClose[1]]->Off();
		bClose?OUTPUTS[m_OFingerOpen[0]]->Off():OUTPUTS[m_OFingerOpen[0]]->On();
		bClose?OUTPUTS[m_OFingerOpen[1]]->Off():OUTPUTS[m_OFingerOpen[1]]->On();
		
		if(m_bDryRun)
			Sleep(300);

		if((m_bDryRun && bClose)
			||  WAIT_OK == WaitInput(INPUTS[m_IFingerClose[0]], bClose?ON:OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IFingerClose[1]], bClose?ON:OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IFingerOpen[0]], bClose?OFF:ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IFingerOpen[1]], bClose?OFF:ON, MSG_SHOW_DEFAULT))
		{
			bRes = TRUE;
		}
	}
	return bRes;
}

/****************************************************************************************
����������ѹ�����׶���
���������nIndex:-1�������м�צ���ף�Ϊ0-1ʱ�ֱ�����צ����1��2
bPush:TRUE:ѹ�������Ƴ� FALSE:ѹ�����׻���
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessOnloadCore::PressPush(int nIndex,BOOL bPush)
{
	ASSERT(-1 <= nIndex && nIndex < 2);
	if (m_IPressPush[0] < 0 || m_IPressPush[1] < 0 || m_IPressPush[2] < 0 || m_IPressPush[3] < 0
		|| m_IPressReturn[0] < 0 || m_IPressReturn[1] < 0 || m_IPressReturn[2] < 0 || m_IPressReturn[3] < 0
		|| m_OPressPush[0] < 0 || m_OPressPush[1] < 0)
	{
		return TRUE;
	}
	
	if(nIndex == -1)
	{
		for (int i=0;i<2;i++)
		{
			bPush?OUTPUTS[m_OPressPush[i]]->On():OUTPUTS[m_OPressPush[i]]->Off();
			bPush?OUTPUTS[m_OPressReturn[i]]->Off():OUTPUTS[m_OPressReturn[i]]->On();
		}

		if((m_bDryRun && bPush)
			|| (WAIT_OK == WaitInput(INPUTS[m_IPressPush[0]], bPush?ON:OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IPressPush[1]], bPush?ON:OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IPressPush[2]], bPush?ON:OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IPressPush[3]], bPush?ON:OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IPressReturn[0]], bPush?OFF:ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IPressReturn[1]], bPush?OFF:ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IPressReturn[2]], bPush?OFF:ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IPressReturn[3]], bPush?OFF:ON, MSG_SHOW_DEFAULT)))
		{
			 return TRUE;
		}
	}
	else
	{
		bPush?OUTPUTS[m_OPressReturn[nIndex]]->Off():OUTPUTS[m_OPressReturn[nIndex]]->On();
		bPush?OUTPUTS[m_OPressPush[nIndex]]->On():OUTPUTS[m_OPressPush[nIndex]]->Off();

		if((m_bDryRun && bPush)
			|| (WAIT_OK == WaitInput(INPUTS[m_IPressPush[nIndex+nIndex]], bPush?ON:OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IPressPush[nIndex+nIndex+1]], bPush?ON:OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IPressReturn[nIndex+nIndex]], bPush?OFF:ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IPressReturn[nIndex+nIndex+1]], bPush?OFF:ON, MSG_SHOW_DEFAULT)))
		{
			return TRUE;
		}
	}

	return FALSE;
}


/****************************************************************************************
�����������������׶���
���������
bPush:TRUE:��ת�����Ƴ� FALSE:��ת���׻���
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessOnloadCore::BufferPush(BOOL bPush )
{
	BOOL bRes = FALSE;
	ASSERT(m_IBufferCylPush > -1 && m_IBufferCylReturn > -1 && m_OBufferCylPush > -1 && m_OBufferCylReturn > -1);

	bPush?OUTPUTS[m_OBufferCylPush]->On():OUTPUTS[m_OBufferCylPush]->Off();
	bPush?OUTPUTS[m_OBufferCylReturn]->Off():OUTPUTS[m_OBufferCylReturn]->On();

	if(WAIT_OK == WaitInput(INPUTS[m_IBufferCylPush], bPush?ON:OFF, MSG_SHOW_DEFAULT) 
		&& WAIT_OK == WaitInput(INPUTS[m_IBufferCylReturn], bPush?OFF:ON, MSG_SHOW_DEFAULT))
	{
		bRes = TRUE;
	}
	return bRes;
}



/****************************************************************************************
��������������Ƿ��в�Ʒ
���������nIndex:-1�������м�צ���ף�Ϊ0-1ʱ�ֱ�����צ1��2   Ϊ2-3ʱ�ֱ�Ϊ�������ϼ�� 
bHave:��TRUE:�������    FALSE:���û����
�������أ�TRUE:��bHaveһ�£�FALSE:��bHave�෴
*****************************************************************************************/
BOOL CRunProcessOnloadCore::CheckHasVaild(int nIndex, BOOL bHave)
{
	ASSERT(-1 <= nIndex && nIndex <= 3 && m_IHasCore[0] > -1 && m_IHasCore[1] > -1 && m_IHasCore[2] > -1 && m_IHasCore[3] > -1);

	BOOL bRes = FALSE;

	if(nIndex < 0)
	{
		for (int i = 0; i < 4; i++)
		{
			if(WAIT_OK == WaitInput(INPUTS[m_IHasCore[i]], bHave?ON:OFF, MSG_SHOW_DEFAULT))
			{
				bRes = TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}
	else
	{
		if(WAIT_OK == CheckInput(INPUTS[m_IHasCore[nIndex]], bHave?ON:OFF, MSG_SHOW_DEFAULT))
		{
			Sleep(100);
			if(WAIT_OK == CheckInput(INPUTS[m_IHasCore[nIndex]], bHave?ON:OFF, MSG_SHOW_DEFAULT))
			{
				bRes = TRUE;
			}
		}
	}
	return bRes;
}

/****************************************************************************************
�������������Z���Ƿ�ȫ
���������nIndex:Ϊ-1ʱ�������Z���Ƿ�ѹ��    Ϊ0-1ʱ�ֱ����Z��1��2
�������أ�TRUE:��צδѹ�ϣ�FALSE:��צѹ��
*****************************************************************************************/
BOOL CRunProcessOnloadCore::CheckZSafe()
{
	if(::IsNoHardware() || m_IZCheckAbnormal[0] < 0 || m_IZCheckAbnormal[1] < 0)
	{
		return TRUE;
	}
	BOOL bCheckSafe = FALSE;
	for (int i = 0 ; i < 2; i++)
	{
		if(CheckInput(INPUTS[m_IZCheckAbnormal[i]], OFF))
		{
			bCheckSafe = TRUE;
		}
		else
		{
			bCheckSafe = FALSE;
		}
		if (!bCheckSafe)
		{
			break;
		}
	}
	if (bCheckSafe)
	{
		return TRUE;
	}
	return FALSE;
}

/****************************************************************************************
������������ѹ�ϼ�ͣ
���������
�������أ�TRUE:��צ��ȫ��FALSE:��צѹ��
*****************************************************************************************/
void CRunProcessOnloadCore::EmgStop()
{
	if(MACHINE.m_pRunCtrl == NULL)
	{
		return;
	}
	for (int i = 0; i < 2; i++)
	{
		ASSERT(m_IZCheckAbnormal[i] > -1);
	}
	
	if (INPUTS[m_IZCheckAbnormal[0]]->IsOn() || INPUTS[m_IZCheckAbnormal[1]]->IsOn())
	{
		Sleep(30);
		if (INPUTS[m_IZCheckAbnormal[0]]->IsOn() || INPUTS[m_IZCheckAbnormal[1]]->IsOn())
		{
			if(m_nMotorZ > -1)                         
			{
				MOTORS[m_nMotorZ]->Stop(2);
			}

			if(m_nMotorY > -1)
			{
				MOTORS[m_nMotorY]->Stop(2);
			}
			if (MOTORS[m_nMotorZ]->GetCurPos() > 10)
			{
				HomeMotor(MOTORS[m_nMotorZ]);
			}

			MACHINE.m_pRunCtrl->Stop();
			////ȡ�ϻ�е�����Ի�����ײ��Ӧ�����������˹����ȷ��
			ShowMessage(_T("ȡ�ϻ�е�����Ի�����ײ��Ӧ�����������˹����ȷ��"),3);
			//ShowMessage(MSG_ONLOAD_CORE_PNP_PICK_COLLIDE);
		}	
	}
	return;
}
/****************************************************************************************
������������ѹ�ϼ�ͣ�߳��ͷ�
���������
�������أ�TRUE:��צ��ȫ��FALSE:��צѹ��
*****************************************************************************************/
void CRunProcessOnloadCore::ThreadRelease()
{
	m_bThreadStart = FALSE;
	DWORD dwTime = GetTickCount();

	while(!m_bThreadEnd)
	{
		if (GetTickCount() - dwTime > 10000)
		{
			break;
		}
		Sleep(1);
	}
}

void CRunProcessOnloadCore::ReadStepInfo()				//��ȡ������������
{
	CString strPath = GetAbsolutePathName(MODULE_RUN_FOLDER) + m_strName + _T(".cfg");

	m_nNextAutoStep = IniReadBoolean(_T("Data"), _T("m_nNextAutoStep"), 0, strPath);
	m_nWorkIndex = IniReadInt(_T("Data"), _T("m_nWorkIndex"), -1, strPath);

	m_pBufferPallet.ReadCfg(_T("BufferPallet"), strPath);
	m_pTempPallet[0].ReadCfg(_T("TempPallet0"), strPath);
	m_pTempPallet[1].ReadCfg(_T("TempPallet1"), strPath);

	return;
}

BOOL CRunProcessOnloadCore::SaveStepInfo(int nWriteType)   	//���������������
{
	CString strPath = GetAbsolutePathName(MODULE_RUN_FOLDER) + m_strName + _T(".cfg");

	if(nWriteType & WRITE_TYPE_AUTO_STEP)
	{
		IniWriteBoolean(_T("Data"), _T("m_nNextAutoStep"), m_nNextAutoStep, strPath);
	}

	if(nWriteType & WRITE_TYPE_VARIABLES)
	{
		IniWriteInt(_T("Data"), _T("m_nWorkIndex"), m_nWorkIndex, strPath);
	}

	if(nWriteType & WRITE_TYPE_BATTERY)
	{
		m_pBufferPallet.WriteCfg(_T("BufferPallet"), strPath);
		m_pTempPallet[0].WriteCfg(_T("TempPallet0"), strPath);
		m_pTempPallet[1].WriteCfg(_T("TempPallet1"), strPath);
	}
	return TRUE;
}

//�������������ʵ�ʼ������һ����,nCheckResultΪ�������0��ץû���ϣ�1��ץ�����ϣ�2����ס1���ϼ�ץ2û�ϣ�3����ס1���ϼ�ץ2����
BOOL CRunProcessOnloadCore::CheckStartWithPalletDataSame()
{
	ASSERT(m_IHasCore[0] > -1 && m_IHasCore[1] > -1);

	CString strMsg;
	for(int i = 0; i < 2; i++)
	{
		if(m_pTempPallet[i].m_bIsValid )
		{
			if((m_IHasCore[i] > -1 && INPUTS[m_IHasCore[i]]->IsOff()))
			{
				strMsg.Format(_T("�����������ϵ�о���ػ�е��#%d���ϸ�Ӧ��ʵ�����ݲ�һ��,��������,��Ӧ����,��ȷ��"), i + 1);
				ShowMessageEX(strMsg);
				strMsg.Format(_T("ȷ���ϵ�о���ػ�е��#%d���������ϵ�YES�����ϵ�NO"), i + 1);
				if(BLMessageBox(strMsg,MB_YESNO) == IDYES)
				{
					m_pTempPallet[i].m_bIsValid = TRUE;
				}
				else
				{
					m_pTempPallet[i].m_bIsValid = FALSE;
				}
			}
		}
		else
		{
			if((m_IHasCore[i] > -1 && INPUTS[m_IHasCore[i]]->IsOn()))
			{
				strMsg.Format(_T("���������ϵ�о���ػ�е��#%d���ϸ�Ӧ��ʵ�����ݲ�һ��,��������,��Ӧ����,��ȷ��"), i + 1);
				ShowMessageEX(strMsg);
				strMsg.Format(_T("ȷ���ϵ�о���ػ�е��#%d���������ϵ�YES�����ϵ�NO"), i + 1);
				if(BLMessageBox(strMsg,MB_YESNO) == IDYES)
				{
					m_pTempPallet[i].m_bIsValid = TRUE;
				}
				else
				{
					m_pTempPallet[i].m_bIsValid = FALSE;
				}
			}
		}
	}

	if(!m_pTempPallet[0].m_bIsValid && !m_pTempPallet[1].m_bIsValid)
	{
		m_nStartWithPalletData = 0;
	}
	else if(m_pTempPallet[0].m_bIsValid && m_pTempPallet[1].m_bIsValid)
	{
		m_nStartWithPalletData = 1;
	}
	else if(m_pTempPallet[0].m_bIsValid && !m_pTempPallet[1].m_bIsValid)
	{
		m_nStartWithPalletData = 2;
	}
	else if(!m_pTempPallet[0].m_bIsValid && m_pTempPallet[1].m_bIsValid)
	{
		m_nStartWithPalletData = 3;
	}
	return TRUE;
}

BOOL CRunProcessOnloadCore::CheckStartWithPalletDataSameForBuffer()
{
	CString strMsg = _T("");
	if(m_pBufferPallet.m_bIsValid)
	{
		if(m_IHasCore[2] > -1 && INPUTS[m_IHasCore[2]]->IsOff()
			&& m_IHasCore[3] > -1 && INPUTS[m_IHasCore[3]]->IsOff())
		{
			strMsg.Format(_T("��������,�ϵ�о���ػ�е�ֻ������ϸ�Ӧ��ʵ�����ݲ�һ��,��������,��Ӧ����,��ȷ��"));
			ShowMessageEX(strMsg);
			strMsg.Format(_T("ȷ���ϵ�о���ػ�е�ֻ������������ϵ�YES�����ϵ�NO"));
			if(BLMessageBox(strMsg,MB_YESNO) == IDYES)
			{
				m_pBufferPallet.m_bIsValid = TRUE;
			}
			else
			{
				m_pBufferPallet.m_bIsValid = FALSE;
			}
		}
	}
	else
	{
		if(m_IHasCore[2] > -1 && INPUTS[m_IHasCore[2]]->IsOn()
			|| m_IHasCore[3] > -1 && INPUTS[m_IHasCore[3]]->IsOn())
		{
			strMsg.Format(_T("��������,�ϵ�о���ػ�е�ֻ������ϸ�Ӧ��ʵ�����ݲ�һ��,��������,��Ӧ����,��ȷ��"));
			ShowMessageEX(strMsg);
			strMsg.Format(_T("ȷ���ϵ�о���ػ�е�ֻ������������ϵ�YES�����ϵ�NO"));
			if(BLMessageBox(strMsg,MB_YESNO) == IDYES)
			{
				m_pBufferPallet.m_bIsValid = TRUE;
			}
			else
			{
				m_pBufferPallet.m_bIsValid = FALSE;
			}
		}
	}
	return TRUE;
}


/****************************************************************************************
�������������Z���Ƿ��ڰ�ȫλ
���������
�������أ�TRUE:��ȫ��FALSE:����ȫ
*****************************************************************************************/

double CRunProcessOnloadCore::GetMotorZSafePos()
{
	ASSERT(m_nMotorZ > -1);
	
	double dPos1 = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_SAFE_LOC);

	return dPos1;
}
