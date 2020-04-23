/****************************************************************************************
文件名：CRunProcessCoreDust.h
功能描述：电芯除尘
历史修改记录：
*****************************************************************************************/
#pragma once
#include "../../RunProcess/RunProcessOnLoadOrOffPlate.h"

class CRunProcessCoreDust : public CRunProcessOnLoadOrOffPlate
{

protected:
	enum AUTO_STEPSEX
	{
		AUTO_WORK_STAR=0,
		AUTO_WAIT_DUST_END,
		AUTO_WORK_END,
	};

public:
	CRunProcessCoreDust(int nID);
	virtual ~CRunProcessCoreDust(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);
	
protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	BOOL InitIO();
	BOOL GetNextNeedSingal();
	void AutoOperationFeed();
	
	
public:
	void Destory();
protected:
	/*
	    int m_IDamperPush;			//除尘阻挡气缸推出感应
		int m_IDamperReturn;		//除尘阻挡气缸回退感应
		int m_IJackingPush;        //除尘顶升气缸推出感应
		int m_IJackingReturn;      //除尘顶升气缸回退感应
		int m_IHasPlate01;		   //除尘顶升有料感应1
		int m_IHasPlate02;		   //除尘顶升有料感应2
	    int m_ODamperReturn;       //除尘阻挡气缸回退电磁阀
		int m_OJackingPush;       //除尘顶升气缸推出电磁阀
		int m_OJackingReturn;     //除尘顶升气缸回退电磁阀
		*/

		
		int m_dwStarDust;
	
};