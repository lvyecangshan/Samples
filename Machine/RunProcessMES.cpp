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
		Sleep(10000);   //10秒刷一次状态
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
	m_strMsg = m_bLangulageEng?_T("Ready"):_T("准备好");
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
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		return;
	}

	switch (m_nNextPowerUpStep)
	{
	case PU_FINISH:
		{
			CRun::PowerUpOperation();
			m_strMsg = m_bLangulageEng?_T("Power up finished"):_T("初始化完成");
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
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		return;
	}

	if (m_nSaveLastStep == -1)
	{
		//第一次记录步骤和开始时间
		m_nSaveLastStep = m_nNextAutoStep;
		m_nStepStartTime = GetTickCount();
	}
	if (m_nSaveLastStep != m_nNextAutoStep)
	{
		//第一步记录开始分隔行
		if (m_nSaveLastStep == AUTO_SEND_REQUIRE_EVENT)
		{
			WriteCTLog(_T("----------开始----------"));
		}
		//每切换一次步骤记录一次CT
		DWORD nStepEndTime = GetTickCount() - m_nStepStartTime;
		CString strText = _T("");
		strText.Format(_T("%s-耗时：%d"),m_strMsg.m_strRunInfo,nStepEndTime);
		WriteCTLog (strText);
		m_nTotalTime += nStepEndTime;
		//最后一步记录总时间
		if (m_nSaveLastStep == AUTO_WORK_END)
		{
			CString str = _T("");
			str.Format(_T("总时间：%d"),m_nTotalTime);
			WriteCTLog (str);
			WriteCTLog (_T("----------结束----------"));
			m_nTotalTime = 0;
		}
		//切换步骤后重新记录新的步骤和开始时间
		m_nSaveLastStep = m_nNextAutoStep;
		m_nStepStartTime = GetTickCount();
	}

	switch(m_nNextAutoStep)
	{
		//发送要料信号
	case AUTO_SEND_REQUIRE_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Send Require Event"):_T("发送要料信号");
			if(!m_bNextRequire)
			{
				m_bNextRequire = TRUE;
				m_nNextAutoStep =AUTO_WAIT_START_EVENT;
			}
			break;
		}
		//等待开始信号
	case AUTO_WAIT_START_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"):_T("等待开始信号");

			if(!m_bNextRequire)
			{
				m_nNextAutoStep =AUTO_SEND_DATA_TO_MES;
			}
			break;
		}
		// 上传数据到服务器
	case AUTO_SEND_DATA_TO_MES:
		{
			m_strMsg = m_bLangulageEng?_T("Send Data To Mes"):_T("上传数据到服务器");

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
		//上传数据到服务器
	case AUTO_WRITE_LOCAL_MES_FILE:
		{
			m_strMsg = m_bLangulageEng?_T("Write Local Mes File"):_T("写本地文件");
		
			if (CreateRecordFile(MACHINE.m_strMesDatePath))
			{
				m_nNextAutoStep = AUTO_WRITE_BACK_UP_FILE;
			}
			break;
		}

	case AUTO_WRITE_BACK_UP_FILE:
		{
			m_strMsg = m_bLangulageEng?_T("Write BackUp File"):_T("写备份文件");
			//if (CreateRecordFile(_T("D:\\CopyData\\")))
			{
				MACHINE.Release();
				m_nNextAutoStep =AUTO_WORK_END;
			}
			break;
		}

		//工作完成
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work End"):_T("工作完成");
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
		BLMessageBox(_T("本地MESData数据文件被打开，请关闭！"));
		m_bSaveFileLock = FALSE;
		return FALSE;
	}

	file.ReadString(strText);
	file.SeekToEnd();

	char* old_locale = _strdup(setlocale(LC_CTYPE,NULL));
	setlocale(LC_CTYPE,"chs");

	if(strText.Find(_T("时间")) < 0 && strText.IsEmpty())
	{
		strText = _T("电芯条码,生产时间,总结果,MES状态,MES返回值,入壳压力最大值(N),入壳压力设定最小值(N),入壳压力设定最大值(N),入壳压力结果,入壳工位,操作者ID,班次,工序名,设备ID,设备工位,\n");
		file.WriteString(strText);
	}

	strText.Empty();

	strTemp.Format(_T("%s,"), m_Pallet.m_strBarCode);
	strText += strTemp;

	// 生产时间
	strTime.Format(_T("%4d-%02d-%02d-%02d:%02d:%02d,"),currentTime.wYear, currentTime.wMonth, currentTime.wDay, currentTime.wHour, currentTime.wMinute, currentTime.wSecond);
	strText += strTime;

	// 总结果
	strTemp = _T("OK,");
	if(m_Pallet.m_bIsNg)
	{
		strTemp = _T("NG(入壳压力),");
	}
	strText += strTemp;

	// MES状态
	if(!MACHINE.m_bCheckMes)
	{
		strTemp = _T("未启用,");
	}
	else if(m_bUploadMesStatue)
	{
		strTemp = _T("上传成功,");
	}
	else
	{
		strTemp = _T("上传失败,");
	}
	strText += strTemp;

	//MES返回值
	strTemp.Format(_T("%d,"), MACHINE.m_nMesInterfaceCode);
	strText += strTemp;

	//入壳压力最大值(N)
	strText += MACHINE.m_strInprocess + _T(",");

	//入壳压力最小值(N)
	strTemp.Format(_T("%.3f,"), m_Pallet.m_dPressNormValue[0]);
	strText += strTemp;

	//入壳压力最大值(N)
	strTemp.Format(_T("%.3f,"), m_Pallet.m_dPressNormValue[1]);
	strText += strTemp;

	//入壳结果
	strTemp.Format(_T("%s,"), m_Pallet.m_bIsNg?_T("NG"):_T("OK"));
	strText += strTemp;

	//入壳工位
	strText += MACHINE.m_strInstation + _T(",");

	//操作者ID
	strText += _T("1000,");

	// 班次
	if(coletime.GetHour() >= 8 && coletime.GetHour() < 20)
	{
		strTemp =_T("白班,");
	}
	else
	{
		strTemp =_T("晚班,");
	}
	strText += strTemp;

	// 工序名
	strText += _T("入壳前段,");

	// 设备ID
	strText += _T("INSHELLID,");

	// 设备工位
	strText += _T("初步入壳,\n");

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
					ShowMessage(MSG_FU_AIR_ALARM);  //负气压报警，请检查!
					Sleep(1000);
				}
			}
			if(m_IAirPressCheck > -1)
			{
				if(INPUTS[m_IAirPressCheck]->IsOn())
				{
					ShowMessage(MSG_AIR_ALARM);   //气压报警，请检查!
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
				BLMessageBox(_T("MES未登录,设备不能生产"));
				MACHINE.SetBuzzerStatue(FALSE);
				return;
			}

			if (1 != MACHINE.m_objMes.X_IsMESOnLine())
			{
				MACHINE.m_pRunCtrl->Stop();
				MACHINE.SetBuzzerStatue(TRUE);
				BLMessageBox(_T("欣旺达MES后台不在线,设备不能生产"));
				MACHINE.SetBuzzerStatue(FALSE);
				return;
			}
		}
	}

	if (m_nCount == 60)  //十分钟上传一次设备生产数据
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