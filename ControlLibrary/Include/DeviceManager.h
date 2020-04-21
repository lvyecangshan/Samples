#pragma once
#include "Input.h"
#include "Output.h"
#include "Motor.h"

#include <vector>
using namespace std;

class  CDeviceManager
{
public:
	 __declspec(dllexport) CDeviceManager();
	 __declspec(dllexport) ~CDeviceManager(void);
public:
	class CInputManager
	{
	public:
		__declspec(dllexport) CInput * operator[] (int nID);
		__declspec(dllexport) int GetNumInputs(){ return m_nInputs;}

	private:
		friend CDeviceManager;
		int		m_nInputs;
		vector<CInput*> m_ppInputs;
	};
	class COutputManager
	{
	public:
		__declspec(dllexport) COutput * operator[] (int nID);
		__declspec(dllexport) int GetNumOutputs(){ return m_nOutputs;}

	public:
		 void SaveAllOutputStates(void);
		 BOOL IsAnyOutputStateChanged(void);
	public:
		__declspec(dllexport) void SaveOutputStates(int nID);
		__declspec(dllexport) BOOL IsOutputStateChanged(int nID);
	private:
		friend CDeviceManager;
		int		m_nOutputs;
		vector<COutput*> m_ppOutputs;
	};
	class CMotorManager
	{
	public:
		__declspec(dllexport) CMotor * operator[] (int nID);
		__declspec(dllexport) int GetNumMotors() { return m_nMotors; }

	public:
		 void SetAllSvonOff();
		 void SaveAllMotorLocs(void);
		 void StopAllMotors();
		 void BreakAllMotion(BOOL bActive = TRUE);
		 BOOL IsAnyMotorLocChanged(void);
	public:
		__declspec(dllexport) void SaveMotorLocs(int nID);
		__declspec(dllexport) BOOL IsMotorLocChanged(int nID);
	private:
		 friend CDeviceManager;
		 int		m_nMotors;
		 vector<CMotor*> m_ppMotors;
	};
public:
	__declspec(dllexport) static CDeviceManager * GetInstance();
	__declspec(dllexport) CInputManager & GetInputManger(){return m_Input;}
	__declspec(dllexport) COutputManager & GetOutputManager(){return m_Output;}
	__declspec(dllexport) CMotorManager & GetMotorManager(){return m_Motor;}

public:
     CString m_strError;
	 CInputManager m_Input;
	 COutputManager m_Output;
	 CMotorManager m_Motor;

public:
	static void Destroy();
	BOOL Initialize(int nInputs, int nOutputs,int nMotors);

private:
	static	CDeviceManager *m_pInstance;
};

#define INPUTS  ((*CDeviceManager::GetInstance()).GetInputManger())
#define OUTPUTS  ((*CDeviceManager::GetInstance()).GetOutputManager())
#define MOTORS  ((*CDeviceManager::GetInstance()).GetMotorManager())
//#define DEVICECTR (*CDeviceManager::GetInstance())

