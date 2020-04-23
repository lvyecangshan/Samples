/****************************************************************************************
文件名：Stopper.h
功能描述：阻挡气缸（简单类型）
特征：1阻挡+1个有料感应.
历史修改记录：
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"


typedef struct  _tagStopperInfo
{
	int IStopperCylPush;
	int IStopperCylReturn;
	int OStopperCylReturn;

	int IHasMaterial;      //有料感应

	CString strIStopperCylPush;
	CString strIStopperCylReturn;
	CString strOStopperCylReturn; 

	CString strIHasMaterial;	 //有料感应

	_tagStopperInfo()
	{
		IStopperCylPush		= -1;
		IStopperCylReturn	= -1;
		OStopperCylReturn	= -1;

		IHasMaterial			= -1;

		strIStopperCylPush		= _T("");
		strIStopperCylReturn	= _T("");
		strOStopperCylReturn	= _T("");

		strIHasMaterial				= _T("");

	}

}tagStopperInfo,*pTagStopperInfo;

typedef CArray<pTagStopperInfo,pTagStopperInfo> pTagStopperInfoArray;

class CStopper
{
public:
	CStopper(CRunProcess *pParent);
	CStopper();
	~CStopper(void);
	//设置父句柄
	void SetParent(CRunProcess *pParent);

	//阻挡下降
	virtual BOOL CylinderDown(int nIndex=-1);
	//阻挡上升
	virtual BOOL CylinderUp(int nIndex=-1);


	//增加夹抓气缸
	virtual BOOL AddStopper(CString strIStopperCylPush,CString strIStopperCylReturn,
		CString strOStopperCylReturn,CString strIHasMaterial);

	//得到气缸数量
	int GetStopperCount();

	//判断是否有料
	BOOL HasMaterial(int nIndex);

	//判断有料感应（不报警）
	BOOL HasMaterial_IsOn(int nIndex);

	//判断有料感应（不报警）
	BOOL HasMaterial_IsOff(int nIndex);

private:
	//气缸关闭
	virtual BOOL Down(int nIndex=-1);
	virtual BOOL Up(int nIndex = -1);
protected:
	CRunProcess* m_pParent;
	pTagStopperInfoArray m_pCylArray;


};
