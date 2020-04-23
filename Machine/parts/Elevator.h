
/****************************************************************************************
�ļ�����Elevator.h
������������������
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"


typedef struct  _tagElevatorInfo
{
	
		int ILineSafeUp;			//�ϲ����尲ȫ
		int ILineSafeDown;			//�²����尲ȫ
		int IStopBackPush;			//ֹ�������Ƴ���Ӧ		
		int IStopBackReturn;		//ֹ�����׻��˸�Ӧ
		int IHasPlate1;				//����1��λ��Ӧ
		int IHasPlate2;				//����1��λ��Ӧ
		int IElevSafe;				//�������̰�ȫ��Ӧ

		int OStopBackReturn;		//ֹ�������Ƴ���ŷ�
		int OForwardRun;			//�����ת
		int OReverseRun;            //�����ת

		CString strILineSafeUp;			
		CString strILineSafeDown;			
		CString strIStopBackPush;				
		CString strIStopBackReturn;		
		CString strIHasPlate1;				
		CString strIHasPlate2;				
		CString strIElevSafe;	

		CString strOStopBackReturn;		
		CString strOForwardRun;			
		CString strOReverseRun;           
	

	_tagElevatorInfo()
	{
		ILineSafeUp		 = -1;
		ILineSafeDown	 = -1;
		IStopBackPush	 = -1;
		IStopBackReturn	 = -1;
		IHasPlate1		 = -1;
		IHasPlate2		 = -1;
		IElevSafe		 = -1;
		OStopBackReturn	 = -1;
		OForwardRun		 = -1;
		OReverseRun		 = -1;
		
	}

}tagElevatorInfo,*pTagElevatorInfo;

//��������
class CElevator
{
public:
	CElevator(CRunProcess *pParent);
	CElevator();
	~CElevator(void);
	//���ø����
	void SetParent(CRunProcess *pParent);
	//���Ӽ�ץ����
	virtual BOOL AddElevator(CString strILineSafeUp,CString strILineSafeDown,CString strIElevSafe,
							 CString strIStopBackPush,CString strIStopBackReturn,
							 CString strIHasPlate1,CString strIHasPlate2,
							 CString strOStopBackReturn,CString strOForwardRun,CString strOReverseRun);

	//ȷ���Ƿ�ȫ
	BOOL CheckWorkSafe();
	BOOL HasPlate();
	BOOL NoPlate();
	//�����ת
	BOOL ForwardRun(); 
	//�����תֹͣ
	BOOL ForwardStop(); 
	//�����ת
	BOOL ReverseRun();
	//�����תֹͣ
	BOOL ReverseStop();


protected:
	CRunProcess* m_pParent;
	pTagElevatorInfo m_pElevator;
	
	

};

