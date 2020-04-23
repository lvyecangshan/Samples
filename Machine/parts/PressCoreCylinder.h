
/****************************************************************************************
文件名：PressCoreCylinder.h
功能描述：压电芯气缸
特征：单控电磁阀
历史修改记录：
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"
#include "Cylinder.h"


typedef struct  _tagPressCoreCylinderInfo
{
	int IPressCoreCylPush1;
	int IPressCoreCylReturn1;
	int IPressCoreCylPush2;
	int IPressCoreCylReturn2;
	int OPressCoreCylPush;
	int OPressCoreCylReturn;
	
	CString strIPressCoreCylPush1;
	CString strIPressCoreCylReturn1;
	CString strIPressCoreCylPush2;
	CString strIPressCoreCylReturn2;
	CString strOPressCoreCylPush;
	CString strOPressCoreCylReturn;
	

	_tagPressCoreCylinderInfo()
	{
		IPressCoreCylPush1		    = -1;
		IPressCoreCylReturn1		= -1;
		IPressCoreCylPush2		    = -1;
		IPressCoreCylReturn2		= -1;
		OPressCoreCylPush	        = -1;
		OPressCoreCylReturn         = -1;

		strIPressCoreCylPush1	    = _T("");
		strIPressCoreCylReturn1	    = _T("");
		strIPressCoreCylPush2	    = _T("");
		strIPressCoreCylReturn2     = _T("");
		strOPressCoreCylPush        = _T("");
		strOPressCoreCylReturn      = _T("");
	}

}tagPressCoreCylinderInfo,*pTagPressCoreCylinderInfo;

typedef CArray<pTagPressCoreCylinderInfo,pTagPressCoreCylinderInfo> pTagPressCoreCylinderInfoArray;

//压电芯气缸
class PressCoreCylinder : public CCylinder
{
public:
	PressCoreCylinder(CRunProcess *pParent);
	PressCoreCylinder();
	~PressCoreCylinder(void);
	//设置父句柄
	void SetParent(CRunProcess *pParent);
	//气缸推出(open)
	virtual BOOL PressCoreCyPush(int nIndex=-1);
	//气缸回退(close)
	virtual BOOL PressCoreCyReturn(int nIndex=-1);

	BOOL CylinderPush(int nIndex=-1);

	BOOL CylinderReturn(int nIndex=-1);
	
	//增加夹抓气缸
	virtual BOOL AddPressCoreCylinder(CString strIPressCoreCylPush1,CString strIPressCoreCylReturn1,
						   CString strIPressCoreCylPush2,CString strIPressCoreCylReturn2,
						   CString strOPressCoreCylPush,CString strOPressCoreCylReturn = _T(""));
	//得到气缸数量
	int GetCylinderCount();

private:
	//气缸关闭
	virtual BOOL Close(int nIndex=-1);
	//气缸打开
	virtual BOOL Open(int nIndex=-1);

protected:
	CRunProcess* m_pParent;
	pTagPressCoreCylinderInfoArray m_pPressCoreCylArray;
	

};

