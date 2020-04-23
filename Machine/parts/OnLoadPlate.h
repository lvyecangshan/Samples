
/****************************************************************************************
�ļ�����OnLoadPlate.h
�������������ϣ���о���̣�����
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"


typedef struct  _tagOnLoadPlateInfo
{
	int IJackingPush;				//���������Ƴ�
	int IJackingReturn;             //�������׻���
	int IDamperPush;				//�赲�����Ƴ���Ӧ
	int IDamperReturn;				//�赲�����Ƴ�����	
	int IHasPlate;					//�����̸�Ӧ
	int IHasCore;					//�е�о��Ӧ

	int ODamperReturn;				//�赲���׻��˵�ŷ�
	int OJackingPush;				//���������Ƴ���ŷ�
	int OJackingReturn;              //�������׻��˵�ŷ�

	int IFullCheck;
	
	
	CString strIJackingPush;        //���������Ƴ�
	CString strIJackingReturn;	    //�������׻���
	CString strIDamperPush;         //�赲�����Ƴ���Ӧ
	CString strIDamperReturn;       //�赲�����Ƴ�����	
	CString strIHasPlate;           //�����̸�Ӧ
	CString strIHasCore;            //�е�о��Ӧ

	CString strODamperReturn;      //�赲���׻��˵�ŷ�
	CString strOJackingPush;       //���������Ƴ���ŷ�
	CString strOJackingReturn;    //�������׻��˵�ŷ�

	CString strIFullCheck;
	

	_tagOnLoadPlateInfo()
	{
		IDamperPush		= -1;
		IDamperReturn	= -1;
		IJackingPush	= -1;
		IJackingReturn	= -1;
		IHasPlate		= -1;
		IHasCore		= -1;
		IFullCheck		=  -1;
		ODamperReturn	= -1;
		OJackingPush	= -1;
		OJackingReturn	= -1;

		strIDamperPush		= _T("");
		strIDamperReturn	= _T("");
		strIJackingPush		= _T("");
		strIJackingReturn	= _T("");
		strIHasPlate		= _T("");
		strIHasCore			= _T("");
		strODamperReturn	= _T("");
		strOJackingPush		= _T("");
		strOJackingReturn	= _T("");	
		strIFullCheck		= _T("");
		
	}

}tagOnLoadPlateInfo,*pTagOnLoadPlateInfo;

typedef CArray<pTagOnLoadPlateInfo,pTagOnLoadPlateInfo> pTagOnLoadPlateInfoArray;

//���ϣ���о���̣�����
class COnLoadPlate
{
public:
	COnLoadPlate(CRunProcess *pParent);
	COnLoadPlate();
	~COnLoadPlate(void);
	//���ø����
	void SetParent(CRunProcess *pParent);
	//���׻���(close)

	virtual BOOL JackingReturn(int nIndex=-1);
	virtual BOOL JackingPush(int nIndex=-1);

	virtual BOOL DamperReturn(int nIndex=-1);
	virtual BOOL DamperPush(int nIndex=-1);

	
	//�������ϣ���о���̣�����
	virtual BOOL AddOnLoadPlate(CString strIDamperPush,CString strIDamperReturn,CString strIJackingPush,CString strIJackingReturn,
								CString strIHasPlate,CString strIHasCore,
								CString strODamperReturn,CString strOJackingPush,CString strOJackingReturn,
								CString strIFullCheck);
	//�õ���������
	int GetOnLoadPlateCount();

	//�ж��Ƿ�����
	BOOL HasCore(int nIndex);

	BOOL HasPlate(int nIndex);
	//�Ƿ�����
	BOOL IsMaterialFull(int nIndex=-1);

	//�ж����ϸ�Ӧ����������
	BOOL HasMaterial_IsOn(int nIndex);

	//�ж����ϸ�Ӧ����������
	BOOL HasMaterial_IsOff(int nIndex);

private:
	//���׹ر�
	virtual BOOL JackingClose(int nIndex=-1);
	virtual BOOL JackingOpen(int nIndex = -1);
	virtual BOOL DamperClose(int nIndex=-1);
	virtual BOOL DamperOpen(int nIndex=-1);
protected:
	CRunProcess* m_pParent;
	pTagOnLoadPlateInfoArray m_pCylArray;
	

};

