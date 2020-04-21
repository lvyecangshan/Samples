#pragma once


// CModuleListBox

class __declspec(dllexport) CModuleListBox : public CListBox
{
	DECLARE_DYNAMIC(CModuleListBox)

public:
	CModuleListBox();
	virtual ~CModuleListBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void LoadModule();
};


