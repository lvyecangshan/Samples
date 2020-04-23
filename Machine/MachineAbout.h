#pragma once


// CMachineAbout 对话框

class CMachineAbout : public CDialogEx
{
	DECLARE_DYNAMIC(CMachineAbout)

public:
	CMachineAbout(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMachineAbout();

// 对话框数据
	enum { IDD = IDD_DLG_MACHINE_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
