
/****************************************************************************************
�ļ�����PressCoreCylinder.h
����������ѹ��о����
���������ص�ŷ�
��ʷ�޸ļ�¼��
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

//ѹ��о����
class PressCoreCylinder : public CCylinder
{
public:
	PressCoreCylinder(CRunProcess *pParent);
	PressCoreCylinder();
	~PressCoreCylinder(void);
	//���ø����
	void SetParent(CRunProcess *pParent);
	//�����Ƴ�(open)
	virtual BOOL PressCoreCyPush(int nIndex=-1);
	//���׻���(close)
	virtual BOOL PressCoreCyReturn(int nIndex=-1);

	BOOL CylinderPush(int nIndex=-1);

	BOOL CylinderReturn(int nIndex=-1);
	
	//���Ӽ�ץ����
	virtual BOOL AddPressCoreCylinder(CString strIPressCoreCylPush1,CString strIPressCoreCylReturn1,
						   CString strIPressCoreCylPush2,CString strIPressCoreCylReturn2,
						   CString strOPressCoreCylPush,CString strOPressCoreCylReturn = _T(""));
	//�õ���������
	int GetCylinderCount();

private:
	//���׹ر�
	virtual BOOL Close(int nIndex=-1);
	//���״�
	virtual BOOL Open(int nIndex=-1);

protected:
	CRunProcess* m_pParent;
	pTagPressCoreCylinderInfoArray m_pPressCoreCylArray;
	

};

