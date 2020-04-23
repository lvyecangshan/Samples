/****************************************************************************************
文件名：CRunProcessPlateDust.h
功能描述：电芯除尘
历史修改记录：
*****************************************************************************************/
#pragma once
#include "../../RunProcess/RunProcessOnLoadOrOffPlate.h"

class CRunProcessPlateDust : public CRunProcessOnLoadOrOffPlate
{
protected:
	

	enum AUTO_STEPSEX
	{
		AUTO_WORK_STAR=0,
		AUTO_WAIT_DUST_END,
		AUTO_WORK_END,
	};

public:
	CRunProcessPlateDust(int nID);
	virtual ~CRunProcessPlateDust(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);
	void AutoOperationFeed();
	BOOL GetNextNeedSingal();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	BOOL InitIO();	
public:
	void Destory();
protected:
	    int m_IDamperPush;			//X2607 除尘阻挡气缸推出感应
		int m_IDamperReturn;		//X2608	除尘阻挡气缸回退感应
		int m_IJackingPush;        //X2609	除尘顶升气缸推出感应
		int m_IJackingReturn;      //X2610	除尘顶升气缸回退感应
		int m_IHasPlate01;		   //X2611	除尘顶升有料感应1
		int m_IHasPlate02;		   //X2612	除尘顶升有料感应2
	    int m_ODamperReturn;       //Y2602	除尘阻挡气缸回退电磁阀
		int m_OJackingPush;       //Y2603	除尘顶升气缸推出电磁阀
		int m_OJackingReturn;     //Y2604	除尘顶升气缸回退电磁阀
		int m_ODustOpen;         //Y2410    除尘电磁阀

		DWORD m_dwStarCoreDustTime;  //开始除尘时间
		DWORD m_dwCoreDustTime;      //除尘持续时间

		int m_dwStarDust;
	
};