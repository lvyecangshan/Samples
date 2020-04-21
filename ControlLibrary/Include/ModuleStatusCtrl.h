#pragma once
#include "colorstatictable.h"
#include "RunTask.h"
#include <vector>
using namespace std;
#define MAX_NUM_MODULE 100
#define MODULE_STATUS_TIMER 707

class __declspec(dllexport) CModuleStatusCtrl : public CColorStaticTable
{
public:
	CModuleStatusCtrl();
	virtual ~CModuleStatusCtrl();

public:
	BOOL Create(int nModules, CRunTask **pRunTasks, double dFirstColWidthRatio);

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

protected:
	void UpdateText();

protected:
	int m_nModules;
	vector<CRunTask *> m_ppRunTasks;
};
