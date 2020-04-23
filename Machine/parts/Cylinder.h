
/****************************************************************************************
�ļ�����Cylinder.h
�������������׻���
��ʷ�޸ļ�¼��
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

//���׻���
class CCylinder : public SCtrolCylinder
{
public:
	CCylinder(CRunProcess *pParent);
	CCylinder();
	~CCylinder(void);
	//���ø����
	void SetParent(CRunProcess *pParent);
	//�����Ƴ�(open)
	virtual BOOL Push(int nIndex=-1);
	//���׻���(close)
	virtual BOOL Return(int nIndex=-1);
	//���׶���(open)
	virtual BOOL Up(int nIndex=-1);
	//�����½�(close)
	virtual BOOL Down(int nIndex=-1);

	//��������(open)
	virtual BOOL Right(int nIndex=-1);
	//��������(close)
	virtual BOOL Left(int nIndex=-1);
	

	
	//���Ӽ�ץ����
	virtual BOOL AddCylinder(CString strICylinderCylOpen,CString strICylinderCylClose,
						   CString strOCylinderCylOpen,CString strOCylinderCylClose);
	//�õ���������
	int GetCylinderCount();

private:
	//���׹ر�
	virtual BOOL Close(int nIndex=-1);
	//���״�
	virtual BOOL Open(int nIndex=-1);

protected:
	CRunProcess* m_pParent;
	pTagCylinderInfoArray m_pCylArray;

};

