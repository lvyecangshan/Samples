#pragma once


// CMachineAbout �Ի���

class CMachineAbout : public CDialogEx
{
	DECLARE_DYNAMIC(CMachineAbout)

public:
	CMachineAbout(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMachineAbout();

// �Ի�������
	enum { IDD = IDD_DLG_MACHINE_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
