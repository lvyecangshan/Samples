
/****************************************************************************************
�ļ�����Distributor.h
����������������
��ʷ�޸ļ�¼��
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
	CString strIHasMaterial;	 //���ϸ�Ӧ
	CString strICheckDirection;  //����
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

//������
class CDistributor
{
public:
	CDistributor(CRunProcess *pParent);
	CDistributor();
	~CDistributor(void);
	//���ø����
	void SetParent(CRunProcess *pParent);
	//���׻���(close)
	virtual BOOL Return(int nIndex=-1);
	virtual BOOL Push(int nIndex=-1);

	
	//���Ӽ�ץ����
	virtual BOOL AddDistributor(CString strIDistributorCylPush,CString strIDistributorCylReturn,
						       CString strODistributorCylReturn,CString strIHasMaterial,CString strICheckDirection,
							   CString strIFullCheck);
	//�õ���������
	int GetDistributorCount();
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
	pTagDistributorInfoArray m_pCylArray;
	

};

