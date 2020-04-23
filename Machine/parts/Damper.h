
/****************************************************************************************
文件名：Damper.h
功能描述：阻挡气缸基类
历史修改记录：
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"


typedef struct  _tagDamperInfo
{
	int IDamperCylPush;
	int IDamperCylReturn;
	int ODamperCylReturn;
	int IHasMaterial;
	int ICheckDirection;
	int IFullCheck0;
	int IFullCheck1;
	
	CString strIDamperCylPush;
	CString strIDamperCylReturn;
	CString strODamperCylReturn; 
	CString strIHasMaterial;	 //有料感应
	CString strICheckDirection;  //防呆
	CString strIFullCheck0;
	CString strIFullCheck1;
	

	_tagDamperInfo()
	{
		IDamperCylPush		= -1;
		IDamperCylReturn	= -1;
		ODamperCylReturn	= -1;
		IFullCheck0			=  -1;
		IFullCheck1			=  -1;


		IHasMaterial			= -1;
		ICheckDirection			= -1;
		strIDamperCylPush		= _T("");
		strIDamperCylReturn	    = _T("");
		strODamperCylReturn	    = _T("");
		strIHasMaterial			= _T("");
		strICheckDirection		= _T("");
		strIFullCheck0			= _T("");
		strIFullCheck1			= _T("");
		
	}

}tagDamperInfo,*pTagDamperInfo;

typedef CArray<pTagDamperInfo,pTagDamperInfo> pTagDamperInfoArray;

//阻挡气缸基类
class CDamper
{
public:
	CDamper(CRunProcess *pParent);
	CDamper();
	~CDamper(void);
	//设置父句柄
	void SetParent(CRunProcess *pParent);
	//气缸回退(close)
	virtual BOOL Return(int nIndex=-1);
	virtual BOOL Push(int nIndex=-1);

	
	//增加夹抓气缸
	virtual BOOL AddDamper(CString strIDamperCylPush,CString strIDamperCylReturn,
						   CString strODamperCylReturn,CString strIHasMaterial,CString strICheckDirection,
						   CString strIFullCheck0,CString strIFullCheck1);
	//得到气缸数量
	int GetDamperCount();
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
	pTagDamperInfoArray m_pCylArray;
	

};

