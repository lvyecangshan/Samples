#include "StdAfx.h"
#include <locale.h>
#include <imagehlp.h>
#include "RunProcessMES.h"

BOOL CRunProcessMES::m_bSaveFileLock = FALSE;

DWORD WINAPI OnloadMachineStateThreadProc(void* pParam)
{
	CRunProcessMES* pThread = (CRunProcessMES*)pParam;
	if (pThread == NULL)
	{
		return 0;
	}

	while(pThread->m_bRunning)
	{
		pThread->RunWhile();
		Sleep(10000);   //10��ˢһ��״̬
	}

	return 0;
}

CRunProcessMES::CRunProcessMES(int nID):CRunProcess(nID)
{
	PowerUpRestart( );
	CString strKey;
	for (int k = 0; k < 21; k++)
	{
		strKey.Format(_T("ISafeDoor%d"),k);
		m_InputMap.insert(pair<int*, CString>(&m_ISafeDoor[k], strKey));
	}
	m_InputMap.insert(pair<int*, CString>(&m_INegativePressure, _T("INegativePressure")));
	m_InputMap.insert(pair<int*, CString>(&m_IAirPressCheck, _T("IAirPressCheck")));

	m_hThread = NULL;
	m_dwThreadId = 0;
	m_bRunning = FALSE;
	m_nCount = 0;
}


CRunProcessMES::~CRunProcessMES(void)
{
}

void CRunProcessMES::PowerUpRestart( void )
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng?_T("Ready"):_T("׼����");
	nRecode = 0;
	m_nCode = 0;
	m_bUploadMesStatue = FALSE;
	m_bNextRequire = FALSE;
	strErr = _T("");
	m_nNextPowerUpStep = PU_FINISH;
	m_nNextAutoStep = AUTO_SEND_REQUIRE_EVENT;
}

void CRunProcessMES::PowerUpOperation( void )
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		return;
	}

	switch (m_nNextPowerUpStep)
	{
	case PU_FINISH:
		{
			CRun::PowerUpOperation();
			m_strMsg = m_bLangulageEng?_T("Power up finished"):_T("��ʼ�����");
			break;
		}
	default:
		{
			ASSERT(0);
		}
	}
}

void CRunProcessMES::AutoOperation( void )
{
	if (!IsModuleEnable())
	{
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		return;
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
		if (m_nSaveLastStep == AUTO_SEND_REQUIRE_EVENT)
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
		//����Ҫ���ź�
	case AUTO_SEND_REQUIRE_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Send Require Event"):_T("����Ҫ���ź�");
			if(!m_bNextRequire)
			{
				m_bNextRequire = TRUE;
				m_nNextAutoStep =AUTO_WAIT_START_EVENT;
			}
			break;
		}
		//�ȴ���ʼ�ź�
	case AUTO_WAIT_START_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"):_T("�ȴ���ʼ�ź�");

			if(!m_bNextRequire)
			{
				m_nNextAutoStep =AUTO_SEND_DATA_TO_MES;
			}
			break;
		}
		// �ϴ����ݵ�������
	case AUTO_SEND_DATA_TO_MES:
		{
			m_strMsg = m_bLangulageEng?_T("Send Data To Mes"):_T("�ϴ����ݵ�������");

			X_BatteryData BatteryData;
			BatteryData.strBatteryCode = m_Pallet.m_strBarCode;
			CString strKey;
			strKey.Format(_T("%.3f"),m_Pallet.m_dLimitPressValue);
			BatteryData.strPressure = strKey;
			BatteryData.strSpeed = MACHINE.m_ValueSetting.strSpeed;
			if (MACHINE.m_bCheckMes)
			{
				if (MACHINE.m_objMes.X_UpdateData(BatteryData,MACHINE.m_ValueRange))
				{
					;
				}
			}
			m_nNextAutoStep = AUTO_WRITE_LOCAL_MES_FILE;
			
			break;
		}
		//�ϴ����ݵ�������
	case AUTO_WRITE_LOCAL_MES_FILE:
		{
			m_strMsg = m_bLangulageEng?_T("Write Local Mes File"):_T("д�����ļ�");
		
			if (CreateRecordFile(MACHINE.m_strMesDatePath))
			{
				m_nNextAutoStep = AUTO_WRITE_BACK_UP_FILE;
			}
			break;
		}

	case AUTO_WRITE_BACK_UP_FILE:
		{
			m_strMsg = m_bLangulageEng?_T("Write BackUp File"):_T("д�����ļ�");
			//if (CreateRecordFile(_T("D:\\CopyData\\")))
			{
				MACHINE.Release();
				m_nNextAutoStep =AUTO_WORK_END;
			}
			break;
		}

		//�������
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work End"):_T("�������");
			m_nNextAutoStep = AUTO_SEND_REQUIRE_EVENT;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

BOOL CRunProcessMES::InitializeConfig( CString strProcessModule )
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
	{
		return FALSE;
	}

	return TRUE;
}

void CRunProcessMES::GetHardwareConfig( CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors )
{
	CRunProcess::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessMES::CreateRecordFile(CString strMesLogPath)
{
	if(m_bSaveFileLock)
		return FALSE;

	m_bSaveFileLock = TRUE;

	CStdioFile file;
	CString strTime, strFilePath, strTemp, strText;

	SYSTEMTIME currentTime;
	char szMesDatePath[256];

	GetLocalTime(&currentTime);

	memset(szMesDatePath,0,256);
	for (int i = 0;i < strMesLogPath.GetLength();i++)
	{
		szMesDatePath[i] =(char) strMesLogPath.GetAt(i);
	}

	PCSTR AimPath=szMesDatePath;
	MakeSureDirectoryPathExists(AimPath);

	COleDateTime coletime = COleDateTime::GetCurrentTime(); 

	if(0 <= coletime.GetHour() && coletime.GetHour() < 8)
	{
		MACHINE.m_nEachPartOffload[8 + coletime.GetHour()/2]++;
	}
	
	if(8 <= coletime.GetHour() && coletime.GetHour() < 24)
	{
		MACHINE.m_nEachPartOffload[coletime.GetHour()/2 - 4]++;
	}

	strTime.Format(_T("\\%4d%02d%02d"),currentTime.wYear,currentTime.wMonth,currentTime.wDay);

	strFilePath = strMesLogPath + strTime + _T(".csv");


	if (!file.Open(strFilePath,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))
	{
		BLMessageBox(_T("����MESData�����ļ����򿪣���رգ�"));
		m_bSaveFileLock = FALSE;
		return FALSE;
	}

	file.ReadString(strText);
	file.SeekToEnd();

	char* old_locale = _strdup(setlocale(LC_CTYPE,NULL));
	setlocale(LC_CTYPE,"chs");

	if(strText.Find(_T("ʱ��")) < 0 && strText.IsEmpty())
	{
		strText = _T("��о����,����ʱ��,�ܽ��,MES״̬,MES����ֵ,���ѹ�����ֵ(N),���ѹ���趨��Сֵ(N),���ѹ���趨���ֵ(N),���ѹ�����,��ǹ�λ,������ID,���,������,�豸ID,�豸��λ,\n");
		file.WriteString(strText);
	}

	strText.Empty();

	strTemp.Format(_T("%s,"), m_Pallet.m_strBarCode);
	strText += strTemp;

	// ����ʱ��
	strTime.Format(_T("%4d-%02d-%02d-%02d:%02d:%02d,"),currentTime.wYear, currentTime.wMonth, currentTime.wDay, currentTime.wHour, currentTime.wMinute, currentTime.wSecond);
	strText += strTime;

	// �ܽ��
	strTemp = _T("OK,");
	if(m_Pallet.m_bIsNg)
	{
		strTemp = _T("NG(���ѹ��),");
	}
	strText += strTemp;

	// MES״̬
	if(!MACHINE.m_bCheckMes)
	{
		strTemp = _T("δ����,");
	}
	else if(m_bUploadMesStatue)
	{
		strTemp = _T("�ϴ��ɹ�,");
	}
	else
	{
		strTemp = _T("�ϴ�ʧ��,");
	}
	strText += strTemp;

	//MES����ֵ
	strTemp.Format(_T("%d,"), MACHINE.m_nMesInterfaceCode);
	strText += strTemp;

	//���ѹ�����ֵ(N)
	strText += MACHINE.m_strInprocess + _T(",");

	//���ѹ����Сֵ(N)
	strTemp.Format(_T("%.3f,"), m_Pallet.m_dPressNormValue[0]);
	strText += strTemp;

	//���ѹ�����ֵ(N)
	strTemp.Format(_T("%.3f,"), m_Pallet.m_dPressNormValue[1]);
	strText += strTemp;

	//��ǽ��
	strTemp.Format(_T("%s,"), m_Pallet.m_bIsNg?_T("NG"):_T("OK"));
	strText += strTemp;

	//��ǹ�λ
	strText += MACHINE.m_strInstation + _T(",");

	//������ID
	strText += _T("1000,");

	// ���
	if(coletime.GetHour() >= 8 && coletime.GetHour() < 20)
	{
		strTemp =_T("�װ�,");
	}
	else
	{
		strTemp =_T("���,");
	}
	strText += strTemp;

	// ������
	strText += _T("���ǰ��,");

	// �豸ID
	strText += _T("INSHELLID,");

	// �豸��λ
	strText += _T("�������,\n");

	file.WriteString(strText);
	setlocale(LC_CTYPE,old_locale);
	free(old_locale);

	if (file)
	{
		file.Close();
	}
	m_bSaveFileLock = FALSE;
	return TRUE;
}

void CRunProcessMES::CheckSafeDoor(void)
{
	if(!::IsNoHardware()  && MACHINE.m_pRunCtrl != NULL)   
	{
		int nStatue = MACHINE.m_pRunCtrl->GetMcState();
		if (nStatue == MC_RUNNING || nStatue == MC_INITIALIZING)
		{
			if (MACHINE.m_bUseSafeDoor)
			{
				for (int i = 0; i < 21; i++)
				{
					if (m_ISafeDoor[i] > -1 /*&& MACHINE.m_bUseSafeDoor*/)
					{
						if (CheckInput(INPUTS[m_ISafeDoor[i]],OFF))
						{
							//Sleep(500);
						}
						else
						{
							/*MACHINE.m_bUseSafeDoor = FALSE;*/
							Sleep(500);
						}
					}
				}
			}

			if (m_INegativePressure > -1) 
			{
				if(INPUTS[m_INegativePressure]->IsOn())
				{
					ShowMessage(MSG_FU_AIR_ALARM);  //����ѹ����������!
					Sleep(1000);
				}
			}
			if(m_IAirPressCheck > -1)
			{
				if(INPUTS[m_IAirPressCheck]->IsOn())
				{
					ShowMessage(MSG_AIR_ALARM);   //��ѹ����������!
					Sleep(1000);
				}
			}
		}
	}
	Sleep(10);
	return;
}

BOOL CRunProcessMES::InitClientThread()
{
	if (m_hThread != NULL)
	{
		return TRUE;
	}

	m_bRunning = TRUE;
	m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)OnloadMachineStateThreadProc, this, 0, &m_dwThreadId);

	if (m_hThread == NULL)
	{
		return FALSE;
	}
	return TRUE;
}

void CRunProcessMES::ReleaseClientThread()
{
	m_bRunning = FALSE;
	if (m_hThread != NULL)
	{
		if(0 != WaitForSingleObject(m_hThread,INFINITE))
		{
			;
		}
		CloseHandle(m_hThread);
		m_hThread = NULL;
		m_dwThreadId = 0;
	}
}

void CRunProcessMES::RunWhile()
{
	if (!MACHINE.m_bCheckMes)
	{
		return;
	}

	m_nCount++;
	if (MACHINE.m_pRunCtrl != NULL)
	{
		int nStatu = MACHINE.m_pRunCtrl->GetMcState();

		if (MC_RUNNING == nStatu)
		{
			if (1 != MACHINE.m_objMes.X_isConnected())
			{
				MACHINE.m_pRunCtrl->Stop();
				MACHINE.SetBuzzerStatue(TRUE);
				BLMessageBox(_T("MESδ��¼,�豸��������"));
				MACHINE.SetBuzzerStatue(FALSE);
				return;
			}

			if (1 != MACHINE.m_objMes.X_IsMESOnLine())
			{
				MACHINE.m_pRunCtrl->Stop();
				MACHINE.SetBuzzerStatue(TRUE);
				BLMessageBox(_T("������MES��̨������,�豸��������"));
				MACHINE.SetBuzzerStatue(FALSE);
				return;
			}
		}
	}

	if (m_nCount == 60)  //ʮ�����ϴ�һ���豸��������
	{
		m_nCount = 0;
		CString str;
		str.Format(L"%d",MACHINE.m_nOffloadCount);
		MACHINE.m_MachineStatus.strQuantityGood = str;
		str.Format(L"%d",MACHINE.m_nNgCount);
		MACHINE.m_MachineStatus.strQuantityDefective = str;

		if(MACHINE.m_dwProduceTime - MACHINE.m_dwWaitMaterialTime == 0
			|| MACHINE.m_nOffloadCount/(1.0*(MACHINE.m_dwProduceTime - MACHINE.m_dwWaitMaterialTime)))
		{
			str = _T("0");
		}
		else
		{
			str.Format(_T("%.1f"),(60/(MACHINE.m_nOffloadCount/(1.0*(MACHINE.m_dwProduceTime - MACHINE.m_dwWaitMaterialTime)))));
		}
		MACHINE.m_MachineStatus.strCycleTime = str;
		MACHINE.m_objMes.X_UpdateProductData(MACHINE.m_MachineStatus);
	}
}