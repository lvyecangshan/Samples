/****************************************************************************************
�ļ�����RunProcessElevatorMylar.h
����������������
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "../../RunProcess/RunProcessElevator.h"

class CRunProcessElevatorMylar : public CRunProcessElevator
{
protected:
	
	enum MOTOR_LOC_Z
	{
		SEND_MOTOR_WAIT_POSTIN=0,  //�ȴ�
		SEND_MOTOR_RECEIVE_MATERIAL_POSTION,//����λ
		SEND_MOTOR_FEEDING_MATERIAL_POSTION,//����λ
	};

public:
	CRunProcessElevatorMylar(int nID);
	virtual ~CRunProcessElevatorMylar(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

	//�ƶ�������λ
	BOOL MoveToReceiveMaterialPostion();
	//�ƶ�������λ
	BOOL MoveToFeedingMaterialPostion();



  
   BOOL GetNextNeedSingal();


public:
	void Destory();
public:
	//���ϸ�Ӧ
    int m_ICheckLineFull;
private:
	
};