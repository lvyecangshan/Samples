
/****************************************************************************************
文件名：Distributor.h
功能描述：分料器
历史修改记录：
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"


typedef struct  _tagDistributorInfo
{
	int IDistributorCylPush;
	int IDistributorCylReturn;
	int ODistributorCylReturn;
	int IHasMaterial;
	int ICheckDirection;
	int IFullCheck;
	
	CString strIDistributorCylPush;
	CString strIDistributorCylReturn;
	CString strODistributorCylReturn; 
	CString strIHasMaterial;	 //有料感应
	CString strICheckDirection;  //防呆
	CString strIFullCheck;
	

	_tagDistributorInfo()
	{
		IDistributorCylPush		= -1;
		IDistributorCylReturn	= -1;
		ODistributorCylReturn	= -1;
		IHasMaterial			= -1;
		IFullCheck              = -1;

		ICheckDirection				= -1;
		strIDistributorCylPush		= _T("");
		strIDistributorCylReturn	= _T("");
		strODistributorCylReturn	= _T("");
		strIHasMaterial				= _T("");
		strICheckDirection			= _T("");
		strIFullCheck				= _T("");
		
	}

}tagDistributorInfo,*pTagDistributorInfo;

typedef CArray<pTagDistributorInfo,pTagDistributorInfo> pTagDistributorInfoArray;

//分料器
class CDistributor
{
public:
	CDistributor(CRunProcess *pParent);
	CDistributor();
	~CDistributor(void);
	//设置父句柄
	void SetParent(CRunProcess *pParent);
	//气缸回退(close)
	virtual BOOL Return(int nIndex=-1);
	virtual BOOL Push(int nIndex=-1);

	
	//增加夹抓气缸
	virtual BOOL AddDistributor(CString strIDistributorCylPush,CString strIDistributorCylReturn,
						       CString strODistributorCylReturn,CString strIHasMaterial,CString strICheckDirection,
							   CString strIFullCheck);
	//得到气缸数量
	int GetDistributorCount();
	//判断是否有料
	BOOL HasMaterial(int nIndex);
	//是否料满
	BOOL IsMaterialFull(int nIndex=-1);

private:
	//气缸关闭
	virtual BOOL Close(int nIndex=-1);
	virtual BOOL Open(int nIndex = -1);
protected:
	CRunProcess* m_pParent;
	pTagDistributorInfoArray m_pCylArray;
	

};

