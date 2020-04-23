
/****************************************************************************************
�ļ�����Cylinder.h
�����������������׻���
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"


typedef struct  _tagSCtrolCylinderInfo
{
	int ICylinderCylOpen;
	int ICylinderCylClose;
	int OCylinderCylOpen;
	
	CString strICylinderCylOpen;
	CString strICylinderCylClose;
	CString strOCylinderCylOpen;
	CString strOCylinderCylClose;
	

	_tagSCtrolCylinderInfo()
	{
		ICylinderCylOpen		= -1;
		ICylinderCylClose		= -1;
		OCylinderCylOpen		= -1;
		
		strICylinderCylOpen	= _T("");
		strICylinderCylClose	= _T("");
		strOCylinderCylOpen	= _T("");
		strOCylinderCylClose	= _T("");
		
	}

}tagSCtrolCylinderInfo,*pTagSCtrolCylinderInfo;

typedef CArray<pTagSCtrolCylinderInfo,pTagSCtrolCylinderInfo> pTagSCtrolCylinderInfoArray;

//�������׻���
class SCtrolCylinder
{
public:
	SCtrolCylinder(CRunProcess *pParent);
	SCtrolCylinder();
	~SCtrolCylinder(void);
	//���ø����
	void SetParent(CRunProcess *pParent);
	//�����Ƴ�(open)
	virtual BOOL SCtrolCyPush(int nIndex=-1);
	//���׻���(close)
	virtual BOOL SCtrolCyReturn(int nIndex=-1);
	//���׶���(open)
	virtual BOOL SCtrolCyUp(int nIndex=-1);
	//�����½�(close)
	virtual BOOL SCtrolCyDown(int nIndex=-1);

	//��������(open)
	virtual BOOL SCtrolCyRight(int nIndex=-1);
	//��������(close)
	virtual BOOL SCtrolCyLeft(int nIndex=-1);
	

	
	//���Ӽ�ץ����
	virtual BOOL AddSCtrolCylinder(CString strICylinderCylOpen,CString strICylinderCylClose,
						   CString strOCylinderCylOpen);
	//�õ���������
	int GetCylinderCount();

private:
	//���׹ر�
	virtual BOOL Close(int nIndex=-1);
	//���״�
	virtual BOOL Open(int nIndex=-1);

protected:
	CRunProcess* m_pParent;
	pTagSCtrolCylinderInfoArray m_pSCtrolCylArray;
	
};

