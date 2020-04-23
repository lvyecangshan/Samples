#include "StdAfx.h"
#include "CzMES.h"

const int MESCOUNTTIME = 120;//��ʱ����
const int DELAYMS = 50;//��ʱʱ��     �ܵȴ�ʱ��͵���DELAYMS * DELAYMS  ��120*50=6000 ms
const int DELAYCLEAR=20;//��������õȴ������Ҫ����MES�źŵ�ʱ������ʱ�Ļ����ܻ�û����ɹ��������Ҫ��ʱʱ��

CzMES::CzMES(void)
{
	strNG = L"";
}


CzMES::~CzMES(void)
{
}

BOOLEAN CzMES::IsInRange(CString strValue,CString strMin,CString strMax)//�Ƚ��ַ����Ƿ��ڷ�Χ��
{
	double dValue = Str2Float(strValue);
	double dMin = Str2Float(strMin);
	double dMax = Str2Float(strMax);
	
	if(dValue>=dMin && dValue<=dMax)
	{
		return TRUE;
	}

	return FALSE;
}

BOOLEAN CzMES::X_WaitYes(CString strname)//�ȵ����ź�
{
	BOOLEAN Flag=FALSE;
	CString cstr;
	for(int i=0;i<MESCOUNTTIME;i++)
	{
		cstr = X_ReadString(strname);
		if(cstr.CompareNoCase(L"1")==0)
		{
			Flag=TRUE;
			break;
		}
		else
		{
			//UpLoadFlag=TRUE;
			//break;
		}

		::Sleep(DELAYMS);
	}
	return Flag;

}
BOOLEAN CzMES::X_WaitNo(CString strname)//�ȵ����ź�
{
	BOOLEAN Flag=FALSE;
	CString cstr;
	for(int i=0;i<MESCOUNTTIME;i++)
	{
		cstr = X_ReadString(strname);
		if(cstr.CompareNoCase(L"1")==0)
		{
			//Flag=TRUE;
			//break;
		}
		else
		{
			Flag=TRUE;
			break;
		}

		::Sleep(DELAYMS);
	}
	return Flag;
}


int CzMES::X_MesLogin(BOOLEAN flag)//����MES���ߣ��������Ժ�Ҫ��д��1������˳���ʱ��Ҫ������FALSE
{
	int iret;
	CString strname=L"ns=1;s=t|DEVICE_B4_QRK_MES_LOGIN";
	if(X_isConnected()==1)
	{
		if(flag)
		{
			iret = X_WriteString(strname,L"1");
		}
		else
		{
			iret = X_WriteString(strname,L"0");
		}
		
		return iret;
		
	}
	return 0;
}

int CzMES::X_IsMESOnLine()//�ж�MES���Ƿ�����
{
	return 1;
	CString strin1 = X_ReadString(L"ns=1;s=t|MES_B4_QRK_MES_LOGIN");
	if(strin1.CompareNoCase(L"1")==0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int CzMES::X_UpdateSetting(X_ValueSetting value,BOOLEAN ParamaterChangeFlag)//�ϴ�����
{
	CStringArray arrName,arrValue;
	//�������
	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ParameterCheck_SetValue_BatteryType");
	arrValue.Add(value.strBatteryType);
	
	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ParameterCheck_SetValue_Speed");
	arrValue.Add(value.strSpeed);

	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ParameterCheck_SetValue_Pressure");
	arrValue.Add(value.strPressure);

	

	//����ǲ������ĺ���ϴ�����Ҫ��дһ��������ĵı�־λ
	if(ParamaterChangeFlag)
	{
		arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ParameterCheck_SetValue_ValueUpdate");
		arrValue.Add(L"1");
	}

	return X_WriteStringArray(arrName,arrValue);

}

int CzMES::X_DownloadParameter(X_ValueRange &valueRange)//���ز������������ֵ��Сֵ
{
	CStringArray arrName;
	CStringArray arrValue;
	arrName.Add(L"ns=1;s=t|MES_B4_QRK_ParameterCheck_SetValue_BatteryType");
	arrName.Add(L"ns=1;s=t|MES_B4_QRK_ParameterCheck_SetValueMAX_Speed");
	arrName.Add(L"ns=1;s=t|MES_B4_QRK_ParameterCheck_SetValueMIN_Speed");
	arrName.Add(L"ns=1;s=t|MES_B4_QRK_ParameterCheck_SetValueMAX_Pressure");
	arrName.Add(L"ns=1;s=t|MES_B4_QRK_ParameterCheck_SetValueMIN_Pressure");
	
	int countName=arrName.GetCount();
	int iret = X_ReadStringArray(arrName,arrValue);
	if(iret==1 && arrValue.GetCount()==countName)
	{
		valueRange.strBatteryType = arrValue.GetAt(0);//�������
		valueRange.strSpeed_Max = arrValue.GetAt(1);//���������¶�
		valueRange.strSpeed_Min = arrValue.GetAt(2);//���������¶�
		valueRange.strPressure_Max = arrValue.GetAt(3);//��������ʱ��
		valueRange.strPressure_Min = arrValue.GetAt(4);//��������ʱ��

		return 1;
	}
	return 0;
}

int CzMES::X_CompareParameter(X_ValueSetting value,X_ValueRange valueRange)//�Ƚϲ��������ϴ���������
{
	BOOLEAN NGFlag=FALSE;
	int NGCount=0;
    strNG=L"";//NG���ַ����Ƚ�
	//�������ƥ��
	if(value.strBatteryType.CompareNoCase(valueRange.strBatteryType)!=0)
	{
		NGFlag=TRUE;
		if(NGCount==0)
		{
			strNG=strNG+L"������Ͳ�ƥ��";
		}
		else
		{
              strNG=strNG+L";������Ͳ�ƥ��";
		}
		NGCount++;
	}
	
	if(IsInRange(value.strSpeed,valueRange.strSpeed_Min,valueRange.strSpeed_Max))
	{
		;
	}
	else
	{
		NGFlag=TRUE;
		if(NGCount==0)
		{
			strNG=strNG+L"����ٶ�Խ��";
		}
		else
		{
			strNG=strNG+L";����ٶ�Խ��";
		}
		NGCount++;
	}

	
	if(IsInRange(value.strPressure,valueRange.strPressure_Min,valueRange.strPressure_Max))
	{
		;
	}
	else
	{
		NGFlag=TRUE;
		if(NGCount==0)
		{
			strNG=strNG+L"���ѹ��Խ��";
		}
		else
		{
			strNG=strNG+L";���ѹ��Խ��";
		}
		NGCount++;
	}

	//�ȽϺ����NGFlag�ǲ���ΪTRUE
	CStringArray arrName,arrValue;
	if(NGFlag)
	{
		arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ParameterCheck_SetValue_Result");
		arrValue.Add(L"2");
		arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ParameterCheck_SetValue_Result_Detail");
		arrValue.Add(strNG);

	}
	else
	{
		arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ParameterCheck_SetValue_Result");
		arrValue.Add(L"1");
		arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ParameterCheck_SetValue_Result_Detail");
		arrValue.Add(L"");
	}
	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ParameterCheck_SetValue_Result_Ready");
	arrValue.Add(L"1");
    
	//���ϴ�ǰ�����뱣֤
	CString cstr=L"";
	BOOLEAN Flag=X_WaitNo(L"ns=1;s=t|DEVICE_B4_QRK_ParameterCheck_SetValue_Result_Ready");

	if(Flag)
	{
		return X_WriteStringArray(arrName,arrValue);
	}
	return 0;

}

int CzMES::X_UpdateBarCode(CString strCode,CString &strResult)//�ϴ����벢�һ�ȡ���
{
	CString cstr=L"";

	BOOLEAN Flag=X_WaitNo(L"ns=1;s=t|DEVICE_B4_QRK_StageCheck_Battery_Code_Ready");
	if(!Flag) {strResult=L""; return 0;}
	
	CStringArray arrName,arrValue;
	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_StageCheck_Battery_Code");
	arrValue.Add(strCode);

	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_StageCheck_Battery_Code_Ready");
	arrValue.Add(L"1");
	//�ϴ�����
	int iret = X_WriteStringArray(arrName,arrValue);
	if(iret!=1) {strResult=L""; return 0;}
	//�ϴ�������ȡ������
	Flag=X_WaitYes(L"ns=1;s=t|MES_B4_QRK_StageCheck_Result_Ready");
	//��ȡ��1�Ժ���Ҫ���ź��� 0
	if(Flag)
	{
		cstr= X_ReadString(L"ns=1;s=t|MES_B4_QRK_StageCheck_Result");
		::Sleep(DELAYCLEAR);
		X_WriteString(L"ns=1;s=t|MES_B4_QRK_StageCheck_Result_Ready",L"0");
		if(cstr.GetLength()>0)
		{
			strResult=cstr;
			return 1;
		}
	}
	
	strResult=L"";
	return 0;


}

int CzMES::X_UpdateStatus(X_Status status)//���»���״̬����Ʒ����������Ʒ����
{
	CStringArray arrName,arrValue;
	//��������

	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_DeviceInfo_RUN");
	arrValue.Add(status.strRun);

	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_DeviceInfo_Waiting");
	arrValue.Add(status.strWaiting);


	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_DeviceInfo_STOP");
	arrValue.Add(status.strStop);

	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_DeviceInfo_FAULT");
	arrValue.Add(status.strFault);

	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_DeviceInfo_Err_code");
	arrValue.Add(status.strErrCode);

	return X_WriteStringArray(arrName,arrValue);

	

}

int CzMES::X_UpdateProductData(X_Status status)//���»���״̬����Ʒ����������Ʒ����
{
	CStringArray arrName,arrValue;
	//��������
	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_DeviceInfo_CycleTime");
	arrValue.Add(status.strCycleTime);

	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_DeviceInfo_QuantityOfGoodProduct");
	arrValue.Add(status.strQuantityGood);

	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_DeviceInfo_QuantityOfDefectiveProduct");
	arrValue.Add(status.strQuantityDefective);


	return X_WriteStringArray(arrName,arrValue);



}

int CzMES::X_UpdateData(X_BatteryData &value,X_ValueRange valueRange)//�������ݺͼ����
{
	BOOLEAN NGFlag=FALSE;
	int NGCount=0;
	strNG=L"";

	if(IsInRange(value.strSpeed,valueRange.strSpeed_Min,valueRange.strSpeed_Max))
	{
		;
	}else
	{
		NGFlag=TRUE;
		if(NGCount==0)
		{
			strNG=strNG+L"����ٶ�Խ��";
		}
		else
		{
            strNG=strNG+L";����ٶ�Խ��";
		}
		NGCount++;
	}

	
	if(IsInRange(value.strPressure,valueRange.strPressure_Min,valueRange.strPressure_Max))
	{
		;
	}else
	{
		NGFlag=TRUE;
		if(NGCount==0)
		{
			strNG=strNG+L"���ѹ��Խ��";
		}
		else
		{
			strNG=strNG+L";���ѹ��Խ��";
		}
		NGCount++;
	}



	CStringArray arrName;
	CStringArray arrValue;
	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ResultUpload_Battery_Code");
	arrValue.Add(value.strBatteryCode);

	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ResultUpload_Speed");
	arrValue.Add(value.strSpeed);

	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ResultUpload_Pressure");
	arrValue.Add(value.strPressure);

	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ResultUpload_LaserPower");
	arrValue.Add(value.strTime);

	if(NGFlag)
	{
		arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ResultUpload_Result");
		arrValue.Add(L"2");
		arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ResultUpload_Result_Detail");
		arrValue.Add(strNG);
	}
	else
	{
		arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ResultUpload_Result");
		arrValue.Add(L"1");
		arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ResultUpload_Result_Detail");
		arrValue.Add(L"");
	}

	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ResultUpload_Ready");
	arrValue.Add(L"1");

	BOOLEAN Flag=X_WaitNo(L"ns=1;s=t|DEVICE_B4_QRK_ResultUpload_Ready");

	if(Flag)
	{
		return X_WriteStringArray(arrName,arrValue);
	}
	
	return 0;
	
	
	
}