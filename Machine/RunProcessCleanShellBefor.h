/****************************************************************************************
文件名：RunProcessCleanShellBefor.h
功能描述：外壳清洗前翻转
历史修改记录：
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

class CRunProcessCleanShellBefor : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_WATI_FRONT_MODULE_HOME,
		PU_CHECK_HAVE_SHELL,
		PU_TRANSFER_CYL_RETURN,
		PU_ROTATE_RETURN,
		PU_CLOSE_VACU_AND_AIR,
		PU_BUTTON_RESET,
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_TRANSFER_CYL_RETURN,
		AUTO_ROTATE_RETURN,
		AUTO_SEND_NEED_REQUIRE_EVENT,
		AUTO_WAIT_START_EVENT,
		AUTO_VACUUM_OPEN,
		AUTO_CHECK_SHELL_DIRECTION,
		AUTO_MANUAL_PICK_SHELL_CLOSE_VACUUM,//人工取壳关真空
		AUTO_WAIT_MANUAL_PICK_SHELL_FINISH,//等待人工取壳完成
		AUTO_TRANSFER_CYL_PUSH,
		AUTO_WAIT_NEXT_MODULE_PICK,
		AUTO_ROTATE_PUSH,//控制除尘模组
		AUTO_WAIT_NEXT_MODULE_FINGER_CLOSE,
		AUTO_VACUUM_CLOSE,
		AUTO_SEND_SAFE_SIGNAL,
		AUTO_TRANSFER_CYL_RETURN_OFFLOAD,
		AUTO_WORK_END,
		NUM_OF_AUTO_STEPS,
	}; 

public:
	CRunProcessCleanShellBefor(int nID);
	virtual ~CRunProcessCleanShellBefor(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckOutputCanActive(COutput *pOutput);

protected:
public:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

public:
	BOOL RotateClyPush(BOOL bPush);//翻转气缸
	BOOL TransferCylPush(BOOL bPush);//无杆气缸动作函数
	BOOL OpenVacuum(BOOL bOpen);//打开真空
	BOOL CheckVacuum(BOOL bHave);//真空检测
	BOOL CheckDirection();//壳口检测

	BOOL ButtonCheck();//按钮按下检测

public:
	//壳口朝向检测
	int m_IShellDirection[2];
	//吸外壳真空
	int m_IVacuum[2];  
	int m_OVacuum[2];  

	//无杆气缸
	int m_ITransferCylPush;  
	int m_ITransferCylReturn;
	int m_OTransferCylPush;  
	int m_OTransferCylReturn;

	//翻转气缸
	int m_IRotateCylPush;  
	int m_IRotateCylReturn;
	int m_ORotateCylPush;  
	int m_ORotateCylReturn;

	//按钮灯
	int m_IManualBtn;
	int m_OManualBtnLight;
};