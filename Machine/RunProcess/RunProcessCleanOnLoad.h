/****************************************************************************************
�ļ�����RunprocessRunprocessShortEdgeclean.h
���ߣ�chenshengjin					�汾��A1						���ڣ�2019/7/12
��������: �����������λ
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "RunProcess.h"
#include "..\parts\OnLoadPlate.h"

//���������������λ
class CRunProcessCleanOnLoad:public CRunProcess
{
protected:
    enum POWER_UP_STEPS
    {
		PU_CHECK_HAVE_CORE,
		PU_JACKING_CY_DOWN,
		PU_CHECK_HAVE_PlATE,
		PU_STOPPER_UP,
        PU_FINISH,
    };
    enum AUTO_STEPS
    {
        AUTO_SEND_NEED_PLATE_EVEND,
		AUTO_WAIT_GIVE_PLATI_EVEND,
		AUTO_BEGING_RECEIVE_PLATE,
		AUTO_JACKING_CY_UP,
		AUTO_JUDGE_WORK_STATION,
		AUTO_JUDGE_HAVE_CORE,
		AUTO_SEND_GIVE_PLATE_EVEND,
		AUTO_WAIT_CLEAN_WORK_FINISH,
		AUTO_JACKING_CY_DOWN,
		AUTO_JUDGE_NEXT_STATION_NEED_PLATE_EVENT,
		AUTO_STOPPER_DOWN,
		AUTO_SEND_PLATE,
		AUTO_JUDGE_NEXT_RECEIVE_PLATE_FINISH,
		AUTO_STOPPER_UP,
		AUTO_WORK_END,

    };
protected:
    void PowerUpOperation(void);
    void PowerUpRestart(void);
	void AutoOperation(void);
public:
    CRunProcessCleanOnLoad(int nID);
    virtual ~CRunProcessCleanOnLoad();
public:
   
	BOOL ReceivePlate();

	BOOL SendPlate();

public:
    BOOL InitializeConfig(CString strProcessModule);
    BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
    BOOL CheckOutputCanActive(COutput *pOutput);
    void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);

public:

	DWORD Count;

	CRunProcessCleanOnLoad* m_pOnLoad2;  //ָ���������λ2,ֻ������λ1��

	COnLoadPlate m_OnLoad;   //������

 };
 