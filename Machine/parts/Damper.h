
/****************************************************************************************
�ļ�����Damper.h
�����������赲���׻���
��ʷ�޸ļ�¼��
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
	CString strIHasMaterial;	 //���ϸ�Ӧ
	CString strICheckDirection;  //����
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

//�赲���׻���
class CDamper
{
public:
	CDamper(CRunProcess *pParent);
	CDamper();
	~CDamper(void);
	//���ø����
	void SetParent(CRunProcess *pParent);
	//���׻���(close)
	virtual BOOL Return(int nIndex=-1);
	virtual BOOL Push(int nIndex=-1);

	
	//���Ӽ�ץ����
	virtual BOOL AddDamper(CString strIDamperCylPush,CString strIDamperCylReturn,
						   CString strODamperCylReturn,CString strIHasMaterial,CString strICheckDirection,
						   CString strIFullCheck0,CString strIFullCheck1);
	//�õ���������
	int GetDamperCount();
	//�ж��Ƿ�����
	BOOL HasMaterial(int nIndex);
	//�Ƿ�����
	BOOL IsMaterialFull(int nIndex=-1);

private:
	//���׹ر�
	virtual BOOL Close(int nIndex=-1);
	virtual BOOL Open(int nIndex = -1);
protected:
	CRunProcess* m_pParent;
	pTagDamperInfoArray m_pCylArray;
	

};

