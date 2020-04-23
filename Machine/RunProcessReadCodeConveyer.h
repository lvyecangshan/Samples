#pragma once
#include "RunProcess.h"
#include "NetSenseClient.h"
//--读条码线体--
class CRunProcessReadCodeConveyer : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_CONNECT_BAR_CODE,
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_SEND_REQUEST_PALLET_EVENT,
		AUTO_WAIT_PALLET_DATA,
		AUTO_CHECK_LINE_HAVE_CORE,
		AUTO_JUDGE_NEED_CODE,
		AUTO_LOAD_MES,
		AUTO_CHECK_CODE_LENGTH,
		AUTO_CHECK_CODE_THREE,
		AUTO_WAIT_NEXT_STATION_REQUIRE_EVENT,
		AUTO_SEND_PALLET_DATA,
		AUTO_WORK_END,
	};

public:
	CRunProcessReadCodeConveyer(int nID);
	virtual ~CRunProcessReadCodeConveyer(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL ShowToolsDlg();

public:
	void UpdateLanguage();
	BOOL SaveParameter();
	BOOL ReadRunParameter();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

public :
	int m_ICheckProduct;
	CNetSenseClient m_pReadCode;
	CString m_strIp;
	int m_nPort;
	int m_nReadCodeTimes;
	BOOL m_bReadEnable;
	int m_IReadSingle;
	int m_OReadFinish;
	CString m_strScanCmd;
	CString m_strErr;
	int m_nRoned;
	int m_nWaitTime;
	int m_nMesFailCount;
	CString strCodeResult ;
	BOOL m_bCanStarScaleCode; //临时调用，通知扫码
};