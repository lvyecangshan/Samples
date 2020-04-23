/****************************************************************************************
�ļ�����RunProcessElevatorCore.h
����������������
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "../../RunProcess/RunProcessElevator.h"

class CRunProcessElevatorCore : public CRunProcessElevator
{
protected:
	
	enum MOTOR_LOC_Z
	{
		SEND_MOTOR_WAIT_POSTIN=0,  //�ȴ�
		SEND_MOTOR_RECEIVE_MATERIAL_POSTION,//����λ
		SEND_MOTOR_FEEDING_MATERIAL_POSTION,//����λ
	};

public:
	CRunProcessElevatorCore(int nID);
	virtual ~CRunProcessElevatorCore(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

	BOOL GetNextNeedSingal();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

	//�ƶ�������λ
	BOOL MoveToReceiveMaterialPostion();
	//�ƶ�������λ
	BOOL MoveToFeedingMaterialPostion();


 
  
public:
	void Destory();
public:
	//���ϸ�Ӧ
    int m_ICheckLineFull;
private:
	



	
};