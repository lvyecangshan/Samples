
/****************************************************************************************
�ļ�����OnOrOffLoadPlate.h
�������������ϣ���о���̣�����
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"


typedef struct  _tagOnOrOffLoadPlateInfo
{
	int IJackingPush;				//���������Ƴ�
	int IJackingReturn;             //�������׻���
	int IDamperPush;				//�赲�����Ƴ���Ӧ
	int IDamperReturn;				//�赲�����Ƴ�����	

	int IHasPlate00;				//�����̸�Ӧ

	int IHasCore00;					//�е�о��Ӧ
	int ICheckDirection00;          //����
	int IHasPlate01;				//�����̸�Ӧ
	int IHasCore01;					//�е�о��Ӧ
	int ICheckDirection01;          //����
	int ODamperReturn;				//�赲���׻��˵�ŷ�
	int OJackingPush;				//���������Ƴ���ŷ�
	int OJackingReturn;              //�������׻��˵�ŷ�
	int IFullCheck0;
	int IFullCheck1;

	CString strIDamperPush;
	CString strIDamperReturn;
	CString strIJackingPush; 
	CString strIJackingReturn;	 
	CString strIHasPlate00;
	CString strIHasCore00; 
	CString strICheckDirection00;
	CString strIHasPlate01;
	CString strIHasCore01; 
	CString strICheckDirection01;
	CString strODamperReturn;  
	CString strOJackingPush;  
	CString strOJackingReturn;
	CString strIFullCheck0;
	CString strIFullCheck1;
	

	_tagOnOrOffLoadPlateInfo()
	{
		IDamperPush		= -1;
		IDamperReturn	= -1;
		IJackingPush	= -1;
		IJackingReturn	= -1;
		IHasPlate00		= -1;
		IHasCore00		= -1;
		IHasPlate01		= -1;
		IHasCore01		= -1;
		ODamperReturn	= -1;
		OJackingPush		= -1;
		OJackingReturn		= -1;
		IFullCheck0			=  -1;
		IFullCheck1         = -1;

		strIDamperPush		= _T("");
		strIDamperReturn	= _T("");
		strIJackingPush		= _T("");
		strIJackingReturn	= _T("");
		strIHasPlate00		= _T("");
		strIHasCore00		= _T("");
		strIHasPlate01		= _T("");
		strIHasCore01		= _T("");
		strODamperReturn	= _T("");
		strOJackingPush		= _T("");
		strOJackingReturn	= _T("");
		strIFullCheck0		= _T("");
		strIFullCheck1		= _T("");

		
		
	}

}tagOnOrOffLoadPlateInfo,*pTagOnOrOffLoadPlateInfo;

typedef CArray<pTagOnOrOffLoadPlateInfo,pTagOnOrOffLoadPlateInfo> pTagOnOrOffLoadPlateInfoArray;

class COnOrOffLoadPlate
{
public:
	COnOrOffLoadPlate(CRunProcess *pParent);
	COnOrOffLoadPlate();
	~COnOrOffLoadPlate(void);
	//���ø����
	void SetParent(CRunProcess *pParent);
	//���׻���(close)
	virtual BOOL JackingReturn(int nIndex=-1);
	virtual BOOL JackingPush(int nIndex=-1);
	virtual BOOL DamperReturn(int nIndex=-1);
	virtual BOOL DamperPush(int nIndex=-1);

	
	//�������ϣ���о���̣�����
	virtual BOOL AddOnOrOffLoadPlate(CString strIDamperPush,CString strIDamperReturn,CString strIJackingPush,CString strIJackingReturn,
								CString strIHasPlate00,CString strIHasCore00,CString strICheckDirection00,
								CString strODamperReturn,CString strOJackingPush,CString strOJackingReturn,
								CString strIHasPlate01,CString strIHasCore01,CString strICheckDirection01,
								CString strIFullCheck0,CString strIFullCheck1);
	//�õ���������
	int GetOnOrOffLoadPlateCount();
	//�ж��Ƿ�����
	BOOL HasCore(int nIndex=-1);
	BOOL NoCore(int nIndex);
	BOOL HasPlate(int nIndex);
	BOOL NoPlate(int nIndex=-1);
	//�õ����̸���
	int GetPlateCount(int nIndex);
	//�õ���ǰ���̸���
	int GetCurPlateCount(int nIndex);
	//�Ƿ�����
	BOOL IsMaterialFull(int nIndex=-1);

private:
	//���׹ر�
	virtual BOOL JackingClose(int nIndex=-1);
	virtual BOOL JackingOpen(int nIndex = -1);
	virtual BOOL DamperClose(int nIndex=-1);
	virtual BOOL DamperOpen(int nIndex=-1);
protected:
	CRunProcess* m_pParent;
	pTagOnOrOffLoadPlateInfoArray m_pCylArray;
	

};

