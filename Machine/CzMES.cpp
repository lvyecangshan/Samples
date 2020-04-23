#include "StdAfx.h"
#include "CzMES.h"

const int MESCOUNTTIME = 120;//延时次数
const int DELAYMS = 50;//延时时间     总等待时间就等于DELAYMS * DELAYMS  即120*50=6000 ms
const int DELAYCLEAR=20;//这个是设置等待结果后要清零MES信号的时间差，不延时的话可能会没清零成功，因此需要延时时间

CzMES::CzMES(void)
{
	strNG = L"";
}


CzMES::~CzMES(void)
{
}

BOOLEAN CzMES::IsInRange(CString strValue,CString strMin,CString strMax)//比较字符串是否在范围内
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

BOOLEAN CzMES::X_WaitYes(CString strname)//等到有信号
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
BOOLEAN CzMES::X_WaitNo(CString strname)//等到无信号
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


int CzMES::X_MesLogin(BOOLEAN flag)//设置MES在线，在连接以后，要先写入1，软件退出的时候要给调用FALSE
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

int CzMES::X_IsMESOnLine()//判断MES端是否在线
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

int CzMES::X_UpdateSetting(X_ValueSetting value,BOOLEAN ParamaterChangeFlag)//上传参数
{
	CStringArray arrName,arrValue;
	//电池类型
	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ParameterCheck_SetValue_BatteryType");
	arrValue.Add(value.strBatteryType);
	
	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ParameterCheck_SetValue_Speed");
	arrValue.Add(value.strSpeed);

	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ParameterCheck_SetValue_Pressure");
	arrValue.Add(value.strPressure);

	

	//如果是参数更改后的上传，还要多写一项参数更改的标志位
	if(ParamaterChangeFlag)
	{
		arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_ParameterCheck_SetValue_ValueUpdate");
		arrValue.Add(L"1");
	}

	return X_WriteStringArray(arrName,arrValue);

}

int CzMES::X_DownloadParameter(X_ValueRange &valueRange)//下载参数，包括最大值最小值
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
		valueRange.strBatteryType = arrValue.GetAt(0);//电池类型
		valueRange.strSpeed_Max = arrValue.GetAt(1);//长边热熔温度
		valueRange.strSpeed_Min = arrValue.GetAt(2);//长边热熔温度
		valueRange.strPressure_Max = arrValue.GetAt(3);//长边热熔时间
		valueRange.strPressure_Min = arrValue.GetAt(4);//长边热熔时间

		return 1;
	}
	return 0;
}

int CzMES::X_CompareParameter(X_ValueSetting value,X_ValueRange valueRange)//比较参数并且上传到服务器
{
	BOOLEAN NGFlag=FALSE;
	int NGCount=0;
    strNG=L"";//NG的字符串比较
	//电池类型匹配
	if(value.strBatteryType.CompareNoCase(valueRange.strBatteryType)!=0)
	{
		NGFlag=TRUE;
		if(NGCount==0)
		{
			strNG=strNG+L"电池类型不匹配";
		}
		else
		{
              strNG=strNG+L";电池类型不匹配";
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
			strNG=strNG+L"入壳速度越界";
		}
		else
		{
			strNG=strNG+L";入壳速度越界";
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
			strNG=strNG+L"入壳压力越界";
		}
		else
		{
			strNG=strNG+L";入壳压力越界";
		}
		NGCount++;
	}

	//比较后根据NGFlag是不是为TRUE
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
    
	//在上传前，必须保证
	CString cstr=L"";
	BOOLEAN Flag=X_WaitNo(L"ns=1;s=t|DEVICE_B4_QRK_ParameterCheck_SetValue_Result_Ready");

	if(Flag)
	{
		return X_WriteStringArray(arrName,arrValue);
	}
	return 0;

}

int CzMES::X_UpdateBarCode(CString strCode,CString &strResult)//上传条码并且获取结果
{
	CString cstr=L"";

	BOOLEAN Flag=X_WaitNo(L"ns=1;s=t|DEVICE_B4_QRK_StageCheck_Battery_Code_Ready");
	if(!Flag) {strResult=L""; return 0;}
	
	CStringArray arrName,arrValue;
	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_StageCheck_Battery_Code");
	arrValue.Add(strCode);

	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_StageCheck_Battery_Code_Ready");
	arrValue.Add(L"1");
	//上传条码
	int iret = X_WriteStringArray(arrName,arrValue);
	if(iret!=1) {strResult=L""; return 0;}
	//上传条码后获取检验结果
	Flag=X_WaitYes(L"ns=1;s=t|MES_B4_QRK_StageCheck_Result_Ready");
	//获取到1以后需要对信号置 0
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

int CzMES::X_UpdateStatus(X_Status status)//更新机器状态，良品数量，不良品数量
{
	CStringArray arrName,arrValue;
	//生产节拍

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

int CzMES::X_UpdateProductData(X_Status status)//更新机器状态，良品数量，不良品数量
{
	CStringArray arrName,arrValue;
	//生产节拍
	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_DeviceInfo_CycleTime");
	arrValue.Add(status.strCycleTime);

	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_DeviceInfo_QuantityOfGoodProduct");
	arrValue.Add(status.strQuantityGood);

	arrName.Add(L"ns=1;s=t|DEVICE_B4_QRK_DeviceInfo_QuantityOfDefectiveProduct");
	arrValue.Add(status.strQuantityDefective);


	return X_WriteStringArray(arrName,arrValue);



}

int CzMES::X_UpdateData(X_BatteryData &value,X_ValueRange valueRange)//更新数据和检测结果
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
			strNG=strNG+L"入壳速度越界";
		}
		else
		{
            strNG=strNG+L";入壳速度越界";
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
			strNG=strNG+L"入壳压力越界";
		}
		else
		{
			strNG=strNG+L";入壳压力越界";
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