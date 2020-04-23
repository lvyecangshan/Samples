// MachineAbout.cpp : 实现文件
//

#include "stdafx.h"
#include "Machine.h"
#include "MachineAbout.h"
#include "afxdialogex.h"


// CMachineAbout 对话框

IMPLEMENT_DYNAMIC(CMachineAbout, CDialogEx)

CMachineAbout::CMachineAbout(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMachineAbout::IDD, pParent)
{

}

CMachineAbout::~CMachineAbout()
{
}

void CMachineAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMachineAbout, CDialogEx)
END_MESSAGE_MAP()


// CMachineAbout 消息处理程序
