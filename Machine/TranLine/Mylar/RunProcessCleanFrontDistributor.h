/****************************************************************************************
功能描述：清洁前分料
历史修改记录：
*****************************************************************************************/
#pragma once
#include "RunProcess.h"
#include "..\..\Parts\Stopper.h"

//清洁前分料
class CRunProcessCleanFrontDistributor:public CRunProcess
{
protected:
    enum POWER_UP_STEPS
    {
        PU_CHECK_HAVE_PlATE,
		PU_STOPPER1_CYDER_DOWN,
		PU_STOPPER2_CYDER_UP,
        PU_FINISH,
    };
    enum AUTO_STEPS
    {
		AUTO_JUDGE__BEGING_RECEIVE_PLATE_EVENT,
		AUTO_BEGING_RECEIVE_PLATE,
		AUTO_JUDGE_NEXT_STATION_NEED_PLATE_EVENT,
		AUTO_STOPPER_CY_WORK,
		AUTO_WAIT_NEXT_STATION_RECEIVE_SUCCESS_EVENT,
		AUTO_STOPPER_CY_REGRESS,
        AUTO_WORK_FINISH,
    };
protected:
    void PowerUpOperation(void);
    void PowerUpRestart(void);
	void AutoOperation(void);
   
public:
    CRunProcessCleanFrontDistributor(int nID);
    virtual ~CRunProcessCleanFrontDistributor();
public:

	BOOL ReceivePlate();

	BOOL SendPlate();

	BOOL HavePlateReceive();

	DWORD Count;

public:
    BOOL InitializeConfig(CString strProcessModule);
    BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
    BOOL CheckOutputCanActive(COutput *pOutput);
    void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);

public:

	CRunProcess* m_pClaenOnLoad1;    //清洁上料位1
	CStopper Stopper;  //阻挡气缸

 };
 