/****************************************************************************************
�ļ�����Stopper.h
�����������赲���ף������ͣ�
������1�赲+1�����ϸ�Ӧ.
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"


typedef struct  _tagStopperInfo
{
	int IStopperCylPush;
	int IStopperCylReturn;
	int OStopperCylReturn;

	int IHasMaterial;      //���ϸ�Ӧ

	CString strIStopperCylPush;
	CString strIStopperCylReturn;
	CString strOStopperCylReturn; 

	CString strIHasMaterial;	 //���ϸ�Ӧ

	_tagStopperInfo()
	{
		IStopperCylPush		= -1;
		IStopperCylReturn	= -1;
		OStopperCylReturn	= -1;

		IHasMaterial			= -1;

		strIStopperCylPush		= _T("");
		strIStopperCylReturn	= _T("");
		strOStopperCylReturn	= _T("");

		strIHasMaterial				= _T("");

	}

}tagStopperInfo,*pTagStopperInfo;

typedef CArray<pTagStopperInfo,pTagStopperInfo> pTagStopperInfoArray;

class CStopper
{
public:
	CStopper(CRunProcess *pParent);
	CStopper();
	~CStopper(void);
	//���ø����
	void SetParent(CRunProcess *pParent);

	//�赲�½�
	virtual BOOL CylinderDown(int nIndex=-1);
	//�赲����
	virtual BOOL CylinderUp(int nIndex=-1);


	//���Ӽ�ץ����
	virtual BOOL AddStopper(CString strIStopperCylPush,CString strIStopperCylReturn,
		CString strOStopperCylReturn,CString strIHasMaterial);

	//�õ���������
	int GetStopperCount();

	//�ж��Ƿ�����
	BOOL HasMaterial(int nIndex);

	//�ж����ϸ�Ӧ����������
	BOOL HasMaterial_IsOn(int nIndex);

	//�ж����ϸ�Ӧ����������
	BOOL HasMaterial_IsOff(int nIndex);

private:
	//���׹ر�
	virtual BOOL Down(int nIndex=-1);
	virtual BOOL Up(int nIndex = -1);
protected:
	CRunProcess* m_pParent;
	pTagStopperInfoArray m_pCylArray;


};
