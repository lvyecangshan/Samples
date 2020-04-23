
/****************************************************************************************
文件名：OnOrOffLoadPlate.h
功能描述：上料（电芯料盘）部件
历史修改记录：
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"


typedef struct  _tagOnOrOffLoadPlateInfo
{
	int IJackingPush;				//顶升气缸推出
	int IJackingReturn;             //顶升气缸回退
	int IDamperPush;				//阻挡气缸推出感应
	int IDamperReturn;				//阻挡气缸推出回退	

	int IHasPlate00;				//有料盘感应

	int IHasCore00;					//有电芯感应
	int ICheckDirection00;          //防呆
	int IHasPlate01;				//有料盘感应
	int IHasCore01;					//有电芯感应
	int ICheckDirection01;          //防呆
	int ODamperReturn;				//阻挡气缸回退电磁阀
	int OJackingPush;				//顶升气缸推出电磁阀
	int OJackingReturn;              //顶升气缸回退电磁阀
	int IFullCheck0;
	int IFullCheck1;

	CString strIDamperPush;
	CString strIDamperReturn;
	CString strIJackingPush; 
	CString strIJackingReturn;	 
	CString strIHasPlate00;
	CString strIHasCore00; 
	CString strICheckDirection00;
	CString strIHasPlate01;
	CString strIHasCore01; 
	CString strICheckDirection01;
	CString strODamperReturn;  
	CString strOJackingPush;  
	CString strOJackingReturn;
	CString strIFullCheck0;
	CString strIFullCheck1;
	

	_tagOnOrOffLoadPlateInfo()
	{
		IDamperPush		= -1;
		IDamperReturn	= -1;
		IJackingPush	= -1;
		IJackingReturn	= -1;
		IHasPlate00		= -1;
		IHasCore00		= -1;
		IHasPlate01		= -1;
		IHasCore01		= -1;
		ODamperReturn	= -1;
		OJackingPush		= -1;
		OJackingReturn		= -1;
		IFullCheck0			=  -1;
		IFullCheck1         = -1;

		strIDamperPush		= _T("");
		strIDamperReturn	= _T("");
		strIJackingPush		= _T("");
		strIJackingReturn	= _T("");
		strIHasPlate00		= _T("");
		strIHasCore00		= _T("");
		strIHasPlate01		= _T("");
		strIHasCore01		= _T("");
		strODamperReturn	= _T("");
		strOJackingPush		= _T("");
		strOJackingReturn	= _T("");
		strIFullCheck0		= _T("");
		strIFullCheck1		= _T("");

		
		
	}

}tagOnOrOffLoadPlateInfo,*pTagOnOrOffLoadPlateInfo;

typedef CArray<pTagOnOrOffLoadPlateInfo,pTagOnOrOffLoadPlateInfo> pTagOnOrOffLoadPlateInfoArray;

class COnOrOffLoadPlate
{
public:
	COnOrOffLoadPlate(CRunProcess *pParent);
	COnOrOffLoadPlate();
	~COnOrOffLoadPlate(void);
	//设置父句柄
	void SetParent(CRunProcess *pParent);
	//气缸回退(close)
	virtual BOOL JackingReturn(int nIndex=-1);
	virtual BOOL JackingPush(int nIndex=-1);
	virtual BOOL DamperReturn(int nIndex=-1);
	virtual BOOL DamperPush(int nIndex=-1);

	
	//增加上料（电芯料盘）部件
	virtual BOOL AddOnOrOffLoadPlate(CString strIDamperPush,CString strIDamperReturn,CString strIJackingPush,CString strIJackingReturn,
								CString strIHasPlate00,CString strIHasCore00,CString strICheckDirection00,
								CString strODamperReturn,CString strOJackingPush,CString strOJackingReturn,
								CString strIHasPlate01,CString strIHasCore01,CString strICheckDirection01,
								CString strIFullCheck0,CString strIFullCheck1);
	//得到气缸数量
	int GetOnOrOffLoadPlateCount();
	//判断是否有料
	BOOL HasCore(int nIndex=-1);
	BOOL NoCore(int nIndex);
	BOOL HasPlate(int nIndex);
	BOOL NoPlate(int nIndex=-1);
	//得到料盘个数
	int GetPlateCount(int nIndex);
	//得到当前料盘个数
	int GetCurPlateCount(int nIndex);
	//是否料满
	BOOL IsMaterialFull(int nIndex=-1);

private:
	//气缸关闭
	virtual BOOL JackingClose(int nIndex=-1);
	virtual BOOL JackingOpen(int nIndex = -1);
	virtual BOOL DamperClose(int nIndex=-1);
	virtual BOOL DamperOpen(int nIndex=-1);
protected:
	CRunProcess* m_pParent;
	pTagOnOrOffLoadPlateInfoArray m_pCylArray;
	

};

