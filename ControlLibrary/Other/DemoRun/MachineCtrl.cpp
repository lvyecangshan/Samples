#include "StdAfx.h"
#include "MachineCtrl.h"
#include "RunOnload.h"
#include "RunPnPArm.h"
#include "RunUnload.h"

CMachineCtrl *CMachineCtrl::m_pInstance = NULL;

CMachineCtrl::CMachineCtrl(void)
{   
}

CMachineCtrl::~CMachineCtrl(void)
{
}

CMachineCtrl * CMachineCtrl::GetInstance()
{
    if( !m_pInstance )
        m_pInstance = new CMachineCtrl();

    return m_pInstance;
}

void CMachineCtrl::Destroy()
{
	
    delete m_pInstance;
	m_pInstance = NULL;
}

BOOL CMachineCtrl::Initialize(HWND hMsgProcessWnd, int numMotor, int numInput, int numOutput, int numEvent, int numMachineTime)
{
	// call parent to init first
	if (!CCentralControl::Initialize(hMsgProcessWnd, numMotor, numInput, numOutput, numEvent, numMachineTime))
	{
		exit(0);
		return FALSE;
	}
	return TRUE;
}

BOOL CMachineCtrl::InitializeRunThreads(HWND hMsgProcessWnd)
{
	ASSERT(m_pRunCtrl == NULL);

	// initialize the run theads
	CRun *pRuns[NUM_OF_RUNS] = { NULL };
    pRuns[RUN_ONLOAD]		= new CRunOnload;
    pRuns[RUN_PNP_ARM]		= new CRunPnPArm;
    pRuns[RUN_UNLOAD]		= new CRunUnload;
	//// the main center control
    m_pRunCtrl = new CRunCtrl();
	if( !m_pRunCtrl )
	{
		AfxMessageBox(_T("Create RunCtrl Thread Failed"), MB_ICONSTOP | MB_SYSTEMMODAL);
        exit(0);
    }
	if(!m_pRunCtrl->Initialize(NUM_OF_RUNS, pRuns, NULL, NULL, AfxGetMainWnd()->m_hWnd))
	{
		exit(0);
		return FALSE;
	}

	m_pRunCtrl->m_pBeforeStart = &CMachineCtrl::TestStart;

	return TRUE;
}
BOOL CMachineCtrl::TestStart()
{
	return TRUE;
}