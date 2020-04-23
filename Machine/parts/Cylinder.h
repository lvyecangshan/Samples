
/****************************************************************************************
文件名：Cylinder.h
功能描述：气缸基类
历史修改记录：
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"
#include "SCtrolCylinder.h"


typedef struct  _tagCylinderInfo
{
	int ICylinderCylOpen;
	int ICylinderCylClose;
	int OCylinderCylOpen;
	int OCylinderCylClose;
	
	CString strICylinderCylOpen;
	CString strICylinderCylClose;
	CString strOCylinderCylOpen;
	CString strOCylinderCylClose;
	

	_tagCylinderInfo()
	{
		ICylinderCylOpen		= -1;
		ICylinderCylClose		= -1;
		OCylinderCylOpen		= -1;
		OCylinderCylClose		= -1;		
		strICylinderCylOpen	= _T("");
		strICylinderCylClose	= _T("");
		strOCylinderCylOpen	= _T("");
		strOCylinderCylClose	= _T("");
		
	}

}tagCylinderInfo,*pTagCylinderInfo;

typedef CArray<pTagCylinderInfo,pTagCylinderInfo> pTagCylinderInfoArray;

//气缸基类
class CCylinder : public SCtrolCylinder
{
public:
	CCylinder(CRunProcess *pParent);
	CCylinder();
	~CCylinder(void);
	//设置父句柄
	void SetParent(CRunProcess *pParent);
	//气缸推出(open)
	virtual BOOL Push(int nIndex=-1);
	//气缸回退(close)
	virtual BOOL Return(int nIndex=-1);
	//气缸顶升(open)
	virtual BOOL Up(int nIndex=-1);
	//气缸下降(close)
	virtual BOOL Down(int nIndex=-1);

	//气缸向右(open)
	virtual BOOL Right(int nIndex=-1);
	//气缸向左(close)
	virtual BOOL Left(int nIndex=-1);
	

	
	//增加夹抓气缸
	virtual BOOL AddCylinder(CString strICylinderCylOpen,CString strICylinderCylClose,
						   CString strOCylinderCylOpen,CString strOCylinderCylClose);
	//得到气缸数量
	int GetCylinderCount();

private:
	//气缸关闭
	virtual BOOL Close(int nIndex=-1);
	//气缸打开
	virtual BOOL Open(int nIndex=-1);

protected:
	CRunProcess* m_pParent;
	pTagCylinderInfoArray m_pCylArray;

};

