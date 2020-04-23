
/****************************************************************************************
�ļ�����Vacuum.h
������������ջ���
��ʷ�޸ļ�¼��
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

//��ջ���
class CVacuum
{
public:
	CVacuum(CRunProcess *pParent);
	CVacuum();
	~CVacuum(void);
	//���ø����
	void SetParent(CRunProcess *pParent);
	//���Ӽ�ץ����
	virtual BOOL AddVacuum(CString strIVacuumCylOpen,CString strIVacuumCylClose,
						   CString strOVacuumCylOpen,CString strOVacuumCylClose);
	//�õ��������
	int GetVacuumCount();
	//��չر�
	virtual BOOL Close(int nIndex=-1);
	//��մ�
	virtual BOOL Open(int nIndex=-1);


private:
	
protected:
	CRunProcess* m_pParent;
	pTagVacuumInfoArray m_pCylArray;
	

};

