#pragma once

enum LOT_DATA_ITEM
{
	LOT_NUM,
	LOT_QUANTITY,
	LOT_OPERATOR_ID,
	LOT_SHIFT,
	LOT_PACKAGE,
	LOT_START_DATE,
	LOT_START_TIME,
	LOT_RUN_TIME,
	LOT_STOP_TIME,
	LOT_JAM_TIME,
	LOT_JAM_COUNT,
	LOT_MTBA,
	LOT_TOTAL_OUTPUT,
	LOT_NET_UPH,
	LOT_SPRINT_UPH,
};

class __declspec(dllexport) CLot
{
public:
	CLot();
	virtual ~CLot();

	virtual BOOL OpenLot(CString strLotNum, int lotQuantity, CString strOperatorID, CString strShift, CString strPackage);
	BOOL CloseLot();
	void ChangeState(int newState);

	void IncreaseJamCount(int count) { m_nJamCount += count; }

	static void GetLotHistory(CStringArray &astrLotNum, CStringArray &astrLotStart);

	virtual BOOL SaveLot();

	virtual CString GetLotDataString(enum LOT_DATA_ITEM eItem);

	void CalcSprintUPH(BOOL bDoNotModifyIfLess = TRUE);

	double GetTotalYield() { return m_nTotalPass <= 0 ? 0.0 : m_nTotalPass / (double)(m_nTotalPass + m_nTotalReject); }

public:
	CString		m_strLotNum;
	int			m_nLotQuantity;
	CString		m_strOperatorID;
	CString		m_strShift;
	CString		m_strPackage;
	BOOL		m_bLotOpenned;

	int			m_nTotalPass;
	int			m_nTotalReject;
	int			m_nTotalPassForUPH;

public:
	COleDateTime m_tLotStart;
	COleDateTime m_tLotStop;

protected:
	COleDateTime m_tCurStateStart;
	COleDateTimeSpan m_tRunTime;
	COleDateTimeSpan m_tStopTime;
	COleDateTimeSpan m_tJamTime;

	int m_nState;
	int m_nJamCount;
	DWORD m_dwSprintUPH;
	clock_t m_tSprintUPHStartClock;
};
