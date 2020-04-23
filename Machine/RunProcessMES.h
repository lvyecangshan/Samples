#pragma once
#include "runprocess.h"

class CRunProcessMES : public CRunProcess
{
public:
	enum POWER_UP_STEPS
	{
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_SEND_REQUIRE_EVENT,
		AUTO_WAIT_START_EVENT,
		AUTO_SEND_DATA_TO_MES,
		AUTO_WRITE_LOCAL_MES_FILE,
		AUTO_WRITE_BACK_UP_FILE,
		AUTO_WORK_END,
	};

public:
	CRunProcessMES(int nID);
	~CRunProcessMES(void);

public:
	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	void CheckSafeDoor(void);
public:
	void PowerUpRestart(void);
	void PowerUpOperation(void);
	void AutoOperation(void);

public:
	BOOL CreateRecordFile(CString strMesLogPath);
private:
	int m_nCode;
	BOOL m_bUploadMesStatue;		// 上传是否成功
	static BOOL m_bSaveFileLock;
	int nRecode;
	CString strErr;
	int m_INegativePressure;  //负压
	int m_IAirPressCheck;   //气压
	int m_ISafeDoor[21];
	int m_nMesCodeCount;
	BOOL m_bThreadStart;
	BOOL m_bThreadEnd;

public:
	BOOL m_bRunning;
	HANDLE m_hThread;
	DWORD  m_dwThreadId;
	int m_nCount;  //计数做定时

	BOOL InitClientThread();
	void ReleaseClientThread();
	void RunWhile();
};