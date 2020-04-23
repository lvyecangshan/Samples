
/****************************************************************************************
文件名：OnLoadPlate.h
功能描述：上料（电芯料盘）部件
历史修改记录：
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"


typedef struct  _tagOnLoadPlateInfo
{
	int IJackingPush;				//顶升气缸推出
	int IJackingReturn;             //顶升气缸回退
	int IDamperPush;				//阻挡气缸推出感应
	int IDamperReturn;				//阻挡气缸推出回退	
	int IHasPlate;					//有料盘感应
	int IHasCore;					//有电芯感应

	int ODamperReturn;				//阻挡气缸回退电磁阀
	int OJackingPush;				//顶升气缸推出电磁阀
	int OJackingReturn;              //顶升气缸回退电磁阀

	int IFullCheck;
	
	
	CString strIJackingPush;        //顶升气缸推出
	CString strIJackingReturn;	    //顶升气缸回退
	CString strIDamperPush;         //阻挡气缸推出感应
	CString strIDamperReturn;       //阻挡气缸推出回退	
	CString strIHasPlate;           //有料盘感应
	CString strIHasCore;            //有电芯感应

	CString strODamperReturn;      //阻挡气缸回退电磁阀
	CString strOJackingPush;       //顶升气缸推出电磁阀
	CString strOJackingReturn;    //顶升气缸回退电磁阀

	CString strIFullCheck;
	

	_tagOnLoadPlateInfo()
	{
		IDamperPush		= -1;
		IDamperReturn	= -1;
		IJackingPush	= -1;
		IJackingReturn	= -1;
		IHasPlate		= -1;
		IHasCore		= -1;
		IFullCheck		=  -1;
		ODamperReturn	= -1;
		OJackingPush	= -1;
		OJackingReturn	= -1;

		strIDamperPush		= _T("");
		strIDamperReturn	= _T("");
		strIJackingPush		= _T("");
		strIJackingReturn	= _T("");
		strIHasPlate		= _T("");
		strIHasCore			= _T("");
		strODamperReturn	= _T("");
		strOJackingPush		= _T("");
		strOJackingReturn	= _T("");	
		strIFullCheck		= _T("");
		
	}

}tagOnLoadPlateInfo,*pTagOnLoadPlateInfo;

typedef CArray<pTagOnLoadPlateInfo,pTagOnLoadPlateInfo> pTagOnLoadPlateInfoArray;

//上料（电芯料盘）部件
class COnLoadPlate
{
public:
	COnLoadPlate(CRunProcess *pParent);
	COnLoadPlate();
	~COnLoadPlate(void);
	//设置父句柄
	void SetParent(CRunProcess *pParent);
	//气缸回退(close)

	virtual BOOL JackingReturn(int nIndex=-1);
	virtual BOOL JackingPush(int nIndex=-1);

	virtual BOOL DamperReturn(int nIndex=-1);
	virtual BOOL DamperPush(int nIndex=-1);

	
	//增加上料（电芯料盘）部件
	virtual BOOL AddOnLoadPlate(CString strIDamperPush,CString strIDamperReturn,CString strIJackingPush,CString strIJackingReturn,
								CString strIHasPlate,CString strIHasCore,
								CString strODamperReturn,CString strOJackingPush,CString strOJackingReturn,
								CString strIFullCheck);
	//得到气缸数量
	int GetOnLoadPlateCount();

	//判断是否有料
	BOOL HasCore(int nIndex);

	BOOL HasPlate(int nIndex);
	//是否料满
	BOOL IsMaterialFull(int nIndex=-1);

	//判断有料感应（不报警）
	BOOL HasMaterial_IsOn(int nIndex);

	//判断有料感应（不报警）
	BOOL HasMaterial_IsOff(int nIndex);

private:
	//气缸关闭
	virtual BOOL JackingClose(int nIndex=-1);
	virtual BOOL JackingOpen(int nIndex = -1);
	virtual BOOL DamperClose(int nIndex=-1);
	virtual BOOL DamperOpen(int nIndex=-1);
protected:
	CRunProcess* m_pParent;
	pTagOnLoadPlateInfoArray m_pCylArray;
	

};

