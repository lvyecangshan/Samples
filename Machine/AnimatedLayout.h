#pragma once

class CAnimatedLayout : public CStatic
{
	DECLARE_DYNAMIC(CAnimatedLayout)

public:
	CAnimatedLayout();
	virtual ~CAnimatedLayout();

protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

public:
	void DrawRobotOffload(CDC& memDC, CRect rcClient);
	void DrawBatteryPnp(CDC& memDC, CRect rcClient);
	void DrawOnloadCore(CDC& memDC, CRect rcClient);
	void DrawCoreInShell(CDC& memDC, CRect rcClient);
	void DrawStack(CDC& memDC, CRect rcArea, CString* strModuleName, CRunProcess** pRun);
};
