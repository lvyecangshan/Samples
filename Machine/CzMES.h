#pragma once
#include "czopcclient.h"
//需要上传的参数设定值
typedef struct
{
  CString strBatteryType;//电池类型
  CString strSpeed;//入壳速度
  CString strPressure;//入壳压力

}X_ValueSetting;

//下载的MES给定的上下限值
typedef struct  
{
   CString strBatteryType;//电池类型
   CString strSpeed_Max;//入壳速度
   CString strSpeed_Min;//入壳速度
   CString strPressure_Max;//入壳压力
   CString strPressure_Min;//入壳压力
  
}X_ValueRange;

//机器的状态值
typedef struct  
{
	CString strCycleTime;//生产节拍
	CString strQuantityGood;//良品数量
	CString strQuantityDefective;//不良品数量
	CString strRun;//运行状态
	CString strWaiting;//运行中待料状态
	CString strStop;//正常停机状态
	CString strFault;//机器故障停机
	CString strErrCode;//故障停机故障码
}X_Status;
//上传的每个电池的数据
typedef struct  
{
	CString strBatteryCode;//电芯条码
	CString strSpeed;//入壳速度
	CString strPressure;//入壳压力
	
	CString strTime;//生产时间
	CString strBatteryResult;//产品质量判定
	CString strErrCode;//产品NG详情

}X_BatteryData;

class CzMES :
	public CzOpcClient
{
public:
	CzMES(void);
	~CzMES(void);
	CString strNG;  //数据比较结果
public:
	BOOLEAN IsInRange(CString strValue,CString strMin,CString strMax);//比较字符串是否在范围内
	BOOLEAN X_WaitYes(CString strname);//等到有信号
	BOOLEAN X_WaitNo(CString strname);//等到无信号
public:
    int X_MesLogin(BOOLEAN flag);//设置MES在线，在连接以后，要先写入1，软件退出的时候要给调用FALSE
	int X_IsMESOnLine();//判断MES端是否在线
	int X_UpdateSetting(X_ValueSetting value,BOOLEAN ParamaterChangeFlag=FALSE);//上传参数
	int X_DownloadParameter(X_ValueRange &valueRange);//下载参数，包括最大值最小值
    int X_CompareParameter(X_ValueSetting value,X_ValueRange valueRange);//比较参数并且上传到服务器
	int X_UpdateBarCode(CString strCode,CString &strResult);//上传条码并且获取结果
	int X_UpdateData(X_BatteryData &value,X_ValueRange valueRange);//更新数据和检测结果
	int X_UpdateStatus(X_Status status);//更新机器状态，良品数量，不良品数量
	int X_UpdateProductData(X_Status status);//更新机器状态，良品数量，不良品数量

};

