
/****************************************************************************************
文件名：Finger.h
功能描述：夹爪基类
历史修改记录：
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"
#include "LineMotorThread.h"


typedef struct  _tagFingerInfo
{
	int IFingerCylOpen;
	int IFingerCylClose;
	int OFingerCylOpen;
	int OFingerCylClose;

	int IHasMaterial;
	int IPress;

	CString strIFingerCylOpen;
	CString strIFingerCylClose;
	CString strOFingerCylOpen;
	CString strOFingerCylClose;

	CString strIFingerPressCylPush;
	CString strIFingerPressCylReturn;
	CString strOFingerPressCylPush;

	CString strIHasMaterial;
	CString strIPress;

	_tagFingerInfo()
	{
		IFingerCylOpen		= -1;
		IFingerCylClose		= -1;
		OFingerCylOpen		= -1;
		OFingerCylClose		= -1;
		IHasMaterial		= -1;
		IPress				= -1;
		strIFingerCylOpen	= _T("");
		strIFingerCylClose	= _T("");
		strOFingerCylOpen	= _T("");
		strOFingerCylClose	= _T("");
		strIHasMaterial     = _T("");
		strIPress           = _T("");
	}

}tagFingerInfo,*pTagFingerInfo;

typedef CArray<pTagFingerInfo,pTagFingerInfo> pTagFingerInfoArray;  

//夹爪基类
class CFinger:public LineMotorThread
{
public:
	CFinger(CRunProcess *pParent);
	CFinger();
	~CFinger(void);
	//设置父句柄
	void SetParent(CRunProcess *pParent);
	//夹爪夹紧
	virtual BOOL Close(int nIndex=-1);
	//夹爪打开
	virtual BOOL Open(int nIndex=-1);
	//是否有料
	virtual BOOL HasMaterial(int nIndex=-1);
	//是否无料
	virtual BOOL HasNoMaterial(int nIndex=-1);

	BOOL CheckHasCore(int nIndex);

	//是否压料
	virtual BOOL HasPressMaterial(int nIndex=-1);
	//增加夹抓气缸
	virtual BOOL AddFinger(CString strIFingerCylOpen,CString strIFingerCylClose,
						   CString strOFingerCylOpen,CString strOFingerCylClose,
						   CString strIHasMaterial = _T(""),CString strIPress = _T(""));
	//得到夹抓数量
	int GetFingerCount();

	//夹爪防撞感应
	virtual void RunWhile();

protected:
	CRunProcess* m_pParent;
	pTagFingerInfoArray m_pCylArray;
	

};

