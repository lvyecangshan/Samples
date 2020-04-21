//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// RunCtrl.h
// RunCtrl - control all run class and act as a process control center
// Last Update: 2008/11/03
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

// includes
#include "Run.h"
#include "Runtask.h"
#include "Maintenance.h"
#include "Monitor.h"
#include "Lot.h"
#include "Package.h"

typedef BOOL (*FUN_BEFORE_START_ACTIVE)(void); // true is enable start, false is disable start.

typedef void (*FUN_BEFORE_STOP_ACTIVE)(void); 

// RunCtrl class
class  CRunCtrl : public CRunTask
{
public:
	// constructor/destructor
	 __declspec(dllexport) CRunCtrl();
	~CRunCtrl();

public:
	// enable/disable step mode
	__declspec(dllexport) void SetStepMode(BOOL bStepMode);
    
	//
	__declspec(dllexport) BOOL Initialize(int nRuns, CRun **run, CMaintenance *pMaintenance, CMonitor *pMonitor, HWND hProcessRunMsgWnd);
	// retrieve the status of step mode
	__declspec(dllexport) BOOL IsStepMode(void) { return m_bStepMode; }

	// SetSystemOutput: tell RunCtrl the common output pointers for later use, including door locking...
	//  please refer to enum COMMON_SYSTEM_OUTPUT
	__declspec(dllexport) void SetSystemOutput(int nOutput, COutput *pOutput);

	__declspec(dllexport) COutput* GetSystemOutput(int nOutput);
	// SetSystemInput: tell RunCtrl the common inputs pointers, including "START" "STOP" "RESET" button, ...
	//  please refer to enum enum COMMON_SWITCH
	__declspec(dllexport) void SetSystemInput(int nSwitchNo, CInput *pInput);

	__declspec(dllexport) CInput* GetSystemInput(int nInput);
	// BypassDoor: enable or disable SafetyDoor, if enabled, when door open, machine cannot run
	__declspec(dllexport) void BypassDoor(BOOL bState);

	// IsDoorBypass: return the safety door bypass setting
	__declspec(dllexport) BOOL IsDoorBypass();

	// SetSystemWarning: trigger the system warning flag
	__declspec(dllexport) void SetSystemWarning(UINT nWarningID, BOOL bState);

	// IsSystemWarning: simply return the system warning flag
	__declspec(dllexport) BOOL IsSystemWarning() { return m_nSystemWarning > 0; }

	// GetMcState: return the machines state defined by enum MACHINE_STATE
	__declspec(dllexport) int GetMcState();

	// Start: called when user press "START" button, this will trigger the start flag and return immediately
	//  the time and condition of start will be checked then start
	__declspec(dllexport) void  Start();

	// Stop: called when user press "STOP" button, this will trigger the stop flag and return immediately
	//  the time and condition of stop will be checked then stop machine
	__declspec(dllexport) void Stop();

	// Reset: called when user press "RESET" button, this will trigger the reset flag and return immediately
	//  the time and condition of reset will be checked then reset
	__declspec(dllexport) void Reset();

	// Restart: this will restart all runs and reset machine to just start software condition (idle)
	__declspec(dllexport) void Restart();

	// EmStop: emergency stop, this will reset all start/stop/reset/restart... process trigger flag and stop all motors
	__declspec(dllexport) void EmStop();

	// EmStopRelease: use to tell run control emergency stop has been removed, then RunCtrl will do corresponding recovering
	__declspec(dllexport) void EmStopRelease(void);

	// GetRun: return the pointer for a perticular CRun, normally do not use this, dangrous to operate directly
	__declspec(dllexport) CRun * GetRun(int nIndex) { return m_pRuns[nIndex]; }

	// GetRunCount: return the number of runs this RunCtrl is controlling
	__declspec(dllexport) int GetRunCount() { return m_numRun; }

	// SwitchDown: to tell RunCtrl which system switch has been pressed down or release up
	__declspec(dllexport) void SwitchDown(int nSwitch, BOOL bOn);
	
	__declspec(dllexport) void ResumeRun(int nID);

public:
	// lot pointer, in case some module need to have information
	 CLot	*m_pLot;

	// package pointer, in case some module need to use package data
	 CPackage		*m_pPackage;

	// maintenance thread pointer, must new an object when writting machine code
	//  CMaintenance can be overrided and assigned the derived object pointer to here
	 CMaintenance	*m_pMaintenance;

	// monitor thread, if need to montior some I/O or else all the time, override this, new an object and assign the pointer here
	 CMonitor		*m_pMonitor;
    // use for user before start run check something
	 FUN_BEFORE_START_ACTIVE m_pBeforeStart;
    // used for user before stop run customer process
	 FUN_BEFORE_STOP_ACTIVE m_pBeforeStop;
protected:
	// Override the runthread function from parents
	UINT RunThread();

	// check is all run modules has finished initialization
	BOOL IsAllRunsPowerUpDone(void);

	// to erase the power up complete is true flag (set to false)
	void ResetPowerUpComplete(void);

	// to lock or release safty door according to settings
	void DoorControl();

	// called when RESET is pressed after JAM, put custom reset action here
	void JamResetAllRun(void);

	// reset all run thread to the beginning state, no jam, power up restart...
	void RestartAllRun();

	// pause all run thread
	void SuspendAllRun();

	// continue / wake up all run threads from stop or suspend state
	BOOL ResumeAllRun();

	// set the machine state and do neccessary job
	void SetMcState(int nState);

	// to check if any run thread is in jam, if yes, on the reset indicator
	BOOL IsJam();

	// delete all run threads
	void EndAllRun(void);

	//	Emergency stop release control
	void EmStopReleaseCtrl(void);

	//	start machine control
	int StartCtrl();

	//	stop machine control
	void StopCtrl();

	//	jam reset machine control
	void ResetCtrl();

	//	restart machine control
	void RestartCtrl();
	
	//	Emstop machine control
	void EmStopCtrl();

	//	check current machine control
	void CheckMcCtrl();

protected:
	// machine state
	int		m_nMcState;
    
	// run modules
	int		m_numRun;
	CRun    **m_pRuns;

	// handle of message process window
	HWND	m_hwndMainApp;

	// system warning flag, user can define custom types of warning
	int		m_nSystemWarning;

	// step mode, means all runs run step by step, pause after 1 step
	BOOL	m_bStepMode;

	// private
	VOID	*m_pPrivate;
public:
	  BOOL m_bActionChangedMoitor;
	  BOOL m_bRunningEnableMaintance;
};