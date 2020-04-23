/****************************************************************************************
文件名：LineMotor.h
功能描述：线休电机基类 chen_2019.7.16
历史修改记录：
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"
#include "LineMotorThread.h"

#define AllMOMTOR 3 //

typedef struct  _tagLineMotorInfo
{
	int m_nOLinemotor[AllMOMTOR];   

	CString strOLinemotor[AllMOMTOR];

	_tagLineMotorInfo()
	{
		for (int i=0;i<AllMOMTOR;i++)   
		{
			m_nOLinemotor[i] = -1;
			strOLinemotor[i] = _T("");
		}
	}

}tagLineMotorInfo,*ptagLineMotorInfo;

typedef CArray<ptagLineMotorInfo,ptagLineMotorInfo> pTagLineMotorInfoArray;

//线休电机类
class CLineMotor:public LineMotorThread
{
public:
	CLineMotor(CRunProcess *pParent);
	CLineMotor();
	~CLineMotor(void);
	void PowerUpRestart(void);

public:
	//设置父句柄
	void SetParent(CRunProcess *pParent);

	//电机启动
	virtual BOOL MotorStart(int nIndex=-1);

	//全部电机启动
	virtual BOOL AllMotorStart();

	//电机停止
	virtual BOOL MotorStop(int nIndex=-1);

	//全部电机停止
	virtual BOOL AllMotorStop();

	//单模组最多配置三个电机
	virtual BOOL AddMotor(CString strOLinemotor0,CString strOLinemotor1 = _T(""),CString strOLinemotor2 = _T(""));

	//得到电机数量
	int GetCylinderCount();

	virtual void RunWhile();

	//延时 
	BOOL CountTime();  

	//获得机器状态
	UINT GetMachinState();

private:

	BOOL m_nTime;
	BOOL bResult;

private:
	//电机启动
	BOOL Start(int nIndex=-1);

	//全部启动
	BOOL AllStart();

	//电机停止
	BOOL Stop(int nIndex=-1);

	//全部停止
	BOOL AllStop();

protected:
	CRunProcess* m_pParent;
	pTagLineMotorInfoArray m_pLeMotorArray;

};
