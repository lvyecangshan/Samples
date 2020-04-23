#pragma once
#include "czopcclient.h"
//��Ҫ�ϴ��Ĳ����趨ֵ
typedef struct
{
  CString strBatteryType;//�������
  CString strSpeed;//����ٶ�
  CString strPressure;//���ѹ��

}X_ValueSetting;

//���ص�MES������������ֵ
typedef struct  
{
   CString strBatteryType;//�������
   CString strSpeed_Max;//����ٶ�
   CString strSpeed_Min;//����ٶ�
   CString strPressure_Max;//���ѹ��
   CString strPressure_Min;//���ѹ��
  
}X_ValueRange;

//������״ֵ̬
typedef struct  
{
	CString strCycleTime;//��������
	CString strQuantityGood;//��Ʒ����
	CString strQuantityDefective;//����Ʒ����
	CString strRun;//����״̬
	CString strWaiting;//�����д���״̬
	CString strStop;//����ͣ��״̬
	CString strFault;//��������ͣ��
	CString strErrCode;//����ͣ��������
}X_Status;
//�ϴ���ÿ����ص�����
typedef struct  
{
	CString strBatteryCode;//��о����
	CString strSpeed;//����ٶ�
	CString strPressure;//���ѹ��
	
	CString strTime;//����ʱ��
	CString strBatteryResult;//��Ʒ�����ж�
	CString strErrCode;//��ƷNG����

}X_BatteryData;

class CzMES :
	public CzOpcClient
{
public:
	CzMES(void);
	~CzMES(void);
	CString strNG;  //���ݱȽϽ��
public:
	BOOLEAN IsInRange(CString strValue,CString strMin,CString strMax);//�Ƚ��ַ����Ƿ��ڷ�Χ��
	BOOLEAN X_WaitYes(CString strname);//�ȵ����ź�
	BOOLEAN X_WaitNo(CString strname);//�ȵ����ź�
public:
    int X_MesLogin(BOOLEAN flag);//����MES���ߣ��������Ժ�Ҫ��д��1������˳���ʱ��Ҫ������FALSE
	int X_IsMESOnLine();//�ж�MES���Ƿ�����
	int X_UpdateSetting(X_ValueSetting value,BOOLEAN ParamaterChangeFlag=FALSE);//�ϴ�����
	int X_DownloadParameter(X_ValueRange &valueRange);//���ز������������ֵ��Сֵ
    int X_CompareParameter(X_ValueSetting value,X_ValueRange valueRange);//�Ƚϲ��������ϴ���������
	int X_UpdateBarCode(CString strCode,CString &strResult);//�ϴ����벢�һ�ȡ���
	int X_UpdateData(X_BatteryData &value,X_ValueRange valueRange);//�������ݺͼ����
	int X_UpdateStatus(X_Status status);//���»���״̬����Ʒ����������Ʒ����
	int X_UpdateProductData(X_Status status);//���»���״̬����Ʒ����������Ʒ����

};

