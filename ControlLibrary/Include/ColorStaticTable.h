#pragma once

#include "ColorStatic.h"

#define MAX_CELL 1000
// CColorStaticTable

class __declspec(dllexport) CColorStaticTable : public CStatic
{
	DECLARE_DYNAMIC(CColorStaticTable)

public:
	typedef struct StaticTableItem
	{
		int m_nSize;
		int m_nMarginBefore;
		int m_nMarginAfter;
		int m_nAlign;
	};

public:
	CColorStaticTable();
	virtual ~CColorStaticTable();

	BOOL CreateTable(int nRow, int nCol, StaticTableItem *pRowItems, StaticTableItem *pColItems, BOOL bAlterColor);
	BOOL SetText(int nRow, int nCol, CString strText);
	BOOL SetInt(int nRow, int nCol, int nValue, int nDigit = 0);
	BOOL SetDouble(int nRow, int nCol, double dValue, int decimal = 2);
	void SetTextFont(int nRow, int nCol, int nPoint, LPCTSTR lpszFaceName, BOOL bBold = FALSE, BOOL bItalic = FALSE);

	BOOL SetTextColor(int nRow, int nCol, COLORREF clrText);
	BOOL SetBkgColor(int nRow, int nCol, COLORREF clrBkg);

	CString GetText(int nRow, int nCol);

	int GetNumRows() { return m_nRows; }
	int GetNumCols() { return m_nCols; }

	void EmptyTable(void);

protected:
	DECLARE_MESSAGE_MAP()

	CColorStatic *m_pwndStatics[MAX_CELL];

	int m_nRows;
	int m_nCols;
};


