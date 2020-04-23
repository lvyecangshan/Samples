
/****************************************************************************************
文件名：Cylinder.h
功能描述：单控气缸基类
历史修改记录：
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"


typedef struct  _tagSCtrolCylinderInfo
{
	int ICylinderCylOpen;
	int ICylinderCylClose;
	int OCylinderCylOpen;
	
	CString strICylinderCylOpen;
	CString strICylinderCylClose;
	CString strOCylinderCylOpen;
	CString strOCylinderCylClose;
	

	_tagSCtrolCylinderInfo()
	{
		ICylinderCylOpen		= -1;
		ICylinderCylClose		= -1;
		OCylinderCylOpen		= -1;
		
		strICylinderCylOpen	= _T("");
		strICylinderCylClose	= _T("");
		strOCylinderCylOpen	= _T("");
		strOCylinderCylClose	= _T("");
		
	}

}tagSCtrolCylinderInfo,*pTagSCtrolCylinderInfo;

typedef CArray<pTagSCtrolCylinderInfo,pTagSCtrolCylinderInfo> pTagSCtrolCylinderInfoArray;

//单控气缸基类
class SCtrolCylinder
{
public:
	SCtrolCylinder(CRunProcess *pParent);
	SCtrolCylinder();
	~SCtrolCylinder(void);
	//设置父句柄
	void SetParent(CRunProcess *pParent);
	//气缸推出(open)
	virtual BOOL SCtrolCyPush(int nIndex=-1);
	//气缸回退(close)
	virtual BOOL SCtrolCyReturn(int nIndex=-1);
	//气缸顶升(open)
	virtual BOOL SCtrolCyUp(int nIndex=-1);
	//气缸下降(close)
	virtual BOOL SCtrolCyDown(int nIndex=-1);

	//气缸向右(open)
	virtual BOOL SCtrolCyRight(int nIndex=-1);
	//气缸向左(close)
	virtual BOOL SCtrolCyLeft(int nIndex=-1);
	

	
	//增加夹抓气缸
	virtual BOOL AddSCtrolCylinder(CString strICylinderCylOpen,CString strICylinderCylClose,
						   CString strOCylinderCylOpen);
	//得到气缸数量
	int GetCylinderCount();

private:
	//气缸关闭
	virtual BOOL Close(int nIndex=-1);
	//气缸打开
	virtual BOOL Open(int nIndex=-1);

protected:
	CRunProcess* m_pParent;
	pTagSCtrolCylinderInfoArray m_pSCtrolCylArray;
	
};

