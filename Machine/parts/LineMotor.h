/****************************************************************************************
�ļ�����LineMotor.h
�������������ݵ������ chen_2019.7.16
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"
#include "LineMotorThread.h"

#define AllMOMTOR 3 //

typedef struct  _tagLineMotorInfo
{
	int m_nOLinemotor[AllMOMTOR];   

	CString strOLinemotor[AllMOMTOR];

	_tagLineMotorInfo()
	{
		for (int i=0;i<AllMOMTOR;i++)   
		{
			m_nOLinemotor[i] = -1;
			strOLinemotor[i] = _T("");
		}
	}

}tagLineMotorInfo,*ptagLineMotorInfo;

typedef CArray<ptagLineMotorInfo,ptagLineMotorInfo> pTagLineMotorInfoArray;

//���ݵ����
class CLineMotor:public LineMotorThread
{
public:
	CLineMotor(CRunProcess *pParent);
	CLineMotor();
	~CLineMotor(void);
	void PowerUpRestart(void);

public:
	//���ø����
	void SetParent(CRunProcess *pParent);

	//�������
	virtual BOOL MotorStart(int nIndex=-1);

	//ȫ���������
	virtual BOOL AllMotorStart();

	//���ֹͣ
	virtual BOOL MotorStop(int nIndex=-1);

	//ȫ�����ֹͣ
	virtual BOOL AllMotorStop();

	//��ģ����������������
	virtual BOOL AddMotor(CString strOLinemotor0,CString strOLinemotor1 = _T(""),CString strOLinemotor2 = _T(""));

	//�õ��������
	int GetCylinderCount();

	virtual void RunWhile();

	//��ʱ 
	BOOL CountTime();  

	//��û���״̬
	UINT GetMachinState();

private:

	BOOL m_nTime;
	BOOL bResult;

private:
	//�������
	BOOL Start(int nIndex=-1);

	//ȫ������
	BOOL AllStart();

	//���ֹͣ
	BOOL Stop(int nIndex=-1);

	//ȫ��ֹͣ
	BOOL AllStop();

protected:
	CRunProcess* m_pParent;
	pTagLineMotorInfoArray m_pLeMotorArray;

};
