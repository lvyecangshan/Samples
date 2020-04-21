#pragma once

// CSensorCtrl

class __declspec(dllexport) CSensorCtrl : public CStatic
{
	DECLARE_DYNAMIC(CSensorCtrl)

public:
	CSensorCtrl();
	virtual ~CSensorCtrl();

public:
	void SetTextColor(COLORREF crText);
    void SetState(BOOL bState);
	BOOL SetFont(TCHAR *szFaceName = _T("Arial"), int nPointSize = 12, int nWeight = FW_NORMAL, BOOL bItalic = FALSE);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

protected:
	virtual void PreSubclassWindow();
    virtual void DrawItem(LPDRAWITEMSTRUCT);

private:
	void Initialize();

private:
	CFont m_hFont;

	COLORREF m_crText;

	HICON m_hIconLEDON;
    HICON m_hIconLEDOFF;

    BOOL  m_bState;
};