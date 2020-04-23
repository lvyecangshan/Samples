
/****************************************************************************************
�ļ�����Finger.h
������������צ����
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"
#include "LineMotorThread.h"


typedef struct  _tagFingerInfo
{
	int IFingerCylOpen;
	int IFingerCylClose;
	int OFingerCylOpen;
	int OFingerCylClose;

	int IHasMaterial;
	int IPress;

	CString strIFingerCylOpen;
	CString strIFingerCylClose;
	CString strOFingerCylOpen;
	CString strOFingerCylClose;

	CString strIFingerPressCylPush;
	CString strIFingerPressCylReturn;
	CString strOFingerPressCylPush;

	CString strIHasMaterial;
	CString strIPress;

	_tagFingerInfo()
	{
		IFingerCylOpen		= -1;
		IFingerCylClose		= -1;
		OFingerCylOpen		= -1;
		OFingerCylClose		= -1;
		IHasMaterial		= -1;
		IPress				= -1;
		strIFingerCylOpen	= _T("");
		strIFingerCylClose	= _T("");
		strOFingerCylOpen	= _T("");
		strOFingerCylClose	= _T("");
		strIHasMaterial     = _T("");
		strIPress           = _T("");
	}

}tagFingerInfo,*pTagFingerInfo;

typedef CArray<pTagFingerInfo,pTagFingerInfo> pTagFingerInfoArray;  

//��צ����
class CFinger:public LineMotorThread
{
public:
	CFinger(CRunProcess *pParent);
	CFinger();
	~CFinger(void);
	//���ø����
	void SetParent(CRunProcess *pParent);
	//��צ�н�
	virtual BOOL Close(int nIndex=-1);
	//��צ��
	virtual BOOL Open(int nIndex=-1);
	//�Ƿ�����
	virtual BOOL HasMaterial(int nIndex=-1);
	//�Ƿ�����
	virtual BOOL HasNoMaterial(int nIndex=-1);

	BOOL CheckHasCore(int nIndex);

	//�Ƿ�ѹ��
	virtual BOOL HasPressMaterial(int nIndex=-1);
	//���Ӽ�ץ����
	virtual BOOL AddFinger(CString strIFingerCylOpen,CString strIFingerCylClose,
						   CString strOFingerCylOpen,CString strOFingerCylClose,
						   CString strIHasMaterial = _T(""),CString strIPress = _T(""));
	//�õ���ץ����
	int GetFingerCount();

	//��צ��ײ��Ӧ
	virtual void RunWhile();

protected:
	CRunProcess* m_pParent;
	pTagFingerInfoArray m_pCylArray;
	

};

