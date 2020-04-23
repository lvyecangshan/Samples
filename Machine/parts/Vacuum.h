
/****************************************************************************************
文件名：Vacuum.h
功能描述：真空基类
历史修改记录：
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"


typedef struct  _tagVacuumInfo
{
	int IVacuumCylOpen;
	int IVacuumCylClose;
	int OVacuumCylOpen;
	int OVacuumCylClose;
	
	CString strIVacuumCylOpen;
	CString strIVacuumCylClose;
	CString strOVacuumCylOpen;
	CString strOVacuumCylClose;
	

	_tagVacuumInfo()
	{
		IVacuumCylOpen		= -1;
		IVacuumCylClose		= -1;
		OVacuumCylOpen		= -1;
		OVacuumCylClose		= -1;		
		strIVacuumCylOpen	= _T("");
		strIVacuumCylClose	= _T("");
		strOVacuumCylOpen	= _T("");
		strOVacuumCylClose	= _T("");
		
	}

}tagVacuumInfo,*pTagVacuumInfo;

typedef CArray<pTagVacuumInfo,pTagVacuumInfo> pTagVacuumInfoArray;

//真空基类
class CVacuum
{
public:
	CVacuum(CRunProcess *pParent);
	CVacuum();
	~CVacuum(void);
	//设置父句柄
	void SetParent(CRunProcess *pParent);
	//增加夹抓气缸
	virtual BOOL AddVacuum(CString strIVacuumCylOpen,CString strIVacuumCylClose,
						   CString strOVacuumCylOpen,CString strOVacuumCylClose);
	//得到真空数量
	int GetVacuumCount();
	//真空关闭
	virtual BOOL Close(int nIndex=-1);
	//真空打开
	virtual BOOL Open(int nIndex=-1);


private:
	
protected:
	CRunProcess* m_pParent;
	pTagVacuumInfoArray m_pCylArray;
	

};

