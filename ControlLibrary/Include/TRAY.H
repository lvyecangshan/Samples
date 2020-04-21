#pragma once

// tray class should not use MAX_COL and MAX_ROW as define array, use dynamic memory instead
// this is to prevent error calculation during indexing
#define JEDEC_TRAY_LENGTH	315.0
#define JEDEC_TRAY_WIDTH	135.9

#define TRAY_UNIT int

enum TRAY_CELL_FLAGS
{
	TRAY_NO_UNIT		= 0x00000000,
	TRAY_HAS_UNIT		= 0x00000001,
	TRAY_UNKOWN_UNIT	= 0x00000002,
	TRAY_FAIL			= 0x00000010,
	TRAY_FAIL_TPVI		= 0x00000020,
	TRAY_FAIL_BPVI		= 0x00000040,
	TRAY_FAIL_BALL		= 0x00000080,
	TRAY_FAIL_PREMARK	= 0x00000100,
	TRAY_FAIL_OCR		= 0x00000200,
	TRAY_FAIL_OCV		= 0x00000400,
	TRAY_FAIL_OS_TEST	= 0x00000800,
	TRAY_FAIL_ADV_TEST	= 0x00001000,
	TRAY_FAIL_LASER		= 0x00002000,
	TRAY_FAIL_LEAD		= 0x00004000,
	TRAY_REWORK_LEAD	= 0x00008000,
	TRAY_FAIL_ORN		= 0x00010000,
};

class __declspec(dllexport) CTrayRow
{
public:
	CTrayRow();
	CTrayRow(const int numUnits);
	virtual ~CTrayRow();

	BOOL Create(const int numUnits, TRAY_UNIT eDefaultUnit = TRAY_NO_UNIT);
	void ResetRow(TRAY_UNIT eDefaultUnit = TRAY_NO_UNIT);
	
	BOOL IsRowEmpty();
	BOOL IsRowGood();
	BOOL IsRowFull();
	int  GetUnitCount();
	CTrayRow&  operator = (const CTrayRow &row);
	TRAY_UNIT& operator[] (const int index);

protected:
	int *m_pUnits;
	int m_numUnits;
};

class __declspec(dllexport) CTray
{
public:
	CTray();
	~CTray();

public:
	CTray& operator = (const CTray &tray);
	CTrayRow&  operator[] (const int index);

	BOOL Create(int numRow, int numCol, TRAY_UNIT eDefaultUnit = TRAY_NO_UNIT);
	void ResetTray(TRAY_UNIT eDefaultUnit = TRAY_NO_UNIT);

	double GetDistanceToFrontEdge(int row, double dRowPitch);
	double GetDistanceToBackEdge(int row, double dRowPitch);

	int GetUnitCount();
	int GetPassedUnitCount();
	int GetFailedUnitCount();

	BOOL GetFirstGoodUnit(int &row, int &col);
	BOOL GetFirstEmptyPocket(int &row, int &col);

	int  GetFirstEmptyInWorkingRow(void);

	TRAY_UNIT GetWorkingRowUnit(int col);
	void SetWorkingRowUnit(int col, TRAY_UNIT unit);
	int GetNumRow(){ return m_numRow;}
	int GetNumCol(){ return m_numCol;}
	
public:
	CString		m_strName;
	int			m_iWorkingRow;
	BOOL		m_bValid;
protected:
	int         m_numRow;
	int         m_numCol;
protected:
	CTrayRow	*m_pRows;
};