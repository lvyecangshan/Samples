
/****************************************************************************************
文件名：Elevator.h
功能描述：升降基类
历史修改记录：
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"


typedef struct  _tagElevatorInfo
{
	
		int ILineSafeUp;			//上层线体安全
		int ILineSafeDown;			//下层线体安全
		int IStopBackPush;			//止回气缸推出感应		
		int IStopBackReturn;		//止回气缸回退感应
		int IHasPlate1;				//料盘1到位感应
		int IHasPlate2;				//料盘1到位感应
		int IElevSafe;				//升降料盘安全感应

		int OStopBackReturn;		//止回气缸推出电磁阀
		int OForwardRun;			//电机正转
		int OReverseRun;            //电机反转

		CString strILineSafeUp;			
		CString strILineSafeDown;			
		CString strIStopBackPush;				
		CString strIStopBackReturn;		
		CString strIHasPlate1;				
		CString strIHasPlate2;				
		CString strIElevSafe;	

		CString strOStopBackReturn;		
		CString strOForwardRun;			
		CString strOReverseRun;           
	

	_tagElevatorInfo()
	{
		ILineSafeUp		 = -1;
		ILineSafeDown	 = -1;
		IStopBackPush	 = -1;
		IStopBackReturn	 = -1;
		IHasPlate1		 = -1;
		IHasPlate2		 = -1;
		IElevSafe		 = -1;
		OStopBackReturn	 = -1;
		OForwardRun		 = -1;
		OReverseRun		 = -1;
		
	}

}tagElevatorInfo,*pTagElevatorInfo;

//升降基类
class CElevator
{
public:
	CElevator(CRunProcess *pParent);
	CElevator();
	~CElevator(void);
	//设置父句柄
	void SetParent(CRunProcess *pParent);
	//增加夹抓气缸
	virtual BOOL AddElevator(CString strILineSafeUp,CString strILineSafeDown,CString strIElevSafe,
							 CString strIStopBackPush,CString strIStopBackReturn,
							 CString strIHasPlate1,CString strIHasPlate2,
							 CString strOStopBackReturn,CString strOForwardRun,CString strOReverseRun);

	//确认是否安全
	BOOL CheckWorkSafe();
	BOOL HasPlate();
	BOOL NoPlate();
	//电机正转
	BOOL ForwardRun(); 
	//电机正转停止
	BOOL ForwardStop(); 
	//电机反转
	BOOL ReverseRun();
	//电机反转停止
	BOOL ReverseStop();


protected:
	CRunProcess* m_pParent;
	pTagElevatorInfo m_pElevator;
	
	

};

