#pragma once

// COutputCtrl

class COutputCtrl : public CIconButton
{
	DECLARE_DYNAMIC(COutputCtrl)

public:
	COutputCtrl();
	virtual ~COutputCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:
	void SetState(BOOL bState);
private:
	BOOL m_bState;
	BOOL m_bLoad;
};


