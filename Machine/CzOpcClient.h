#pragma once

//增加typedef的函数声明
typedef int (*dll_ConnectServer)(char*);
typedef int (*dll_isConnected)();
typedef void (*dll_CloseServer)();
typedef int (*dll_ReadString)(char* ,char*);
typedef int (*dll_ReadInt)(char* );
typedef float (*dll_ReadFloat)(char* );
typedef int (*dll_ReadBoolean)(char* );
typedef int (*dll_ReadDateTime)(char* ,char*);
typedef int (*dll_WriteString)(char* ,char*);
typedef int (*dll_WriteInt)(char* ,int );
typedef int (*dll_WriteFloat)(char*,float);
typedef int (*dll_WriteBoolean)(char*,int);
typedef int (*dll_WriteDateTime)(char*,char*);
typedef int (*dll_ReadStringArray)(char*,char*);
typedef int (*dll_WriteStringArray)(char*,char*);

class CzOpcClient
{
private:
	HINSTANCE hDllInst;
	void initPtr();
public:
	CzOpcClient();
	virtual  ~CzOpcClient();
	int loadDll();//加载dll
private:
	//函数变量声明(导出函数15个)
	dll_ConnectServer  ConnectServer;
	dll_isConnected  isConnected;
	dll_CloseServer  CloseServer;
	dll_ReadString  ReadString;
	dll_ReadInt  ReadInt;
	dll_ReadFloat  ReadFloat;
	dll_ReadBoolean  ReadBoolean;
	dll_ReadDateTime  ReadDateTime;
	dll_WriteString  WriteString;
	dll_WriteInt  WriteInt;
	dll_WriteFloat  WriteFloat;
	dll_WriteBoolean  WriteBoolean;
	dll_WriteDateTime  WriteDateTime;
	dll_ReadStringArray  ReadStringArray;
	dll_WriteStringArray  WriteStringArray;
	//这些是重新封装的函数
private:
	
	//根据字符串"@@"分割CString到CStringList
	void CString2CStringArray(CString strIn,CStringArray &dest);
	//把CStringList的多个字符串通过"@@"拼接成CString
	CString CStringArray2CString(CStringArray &aryValue);
public:
	//分割字符串
	void SplitString(CString strsource, CStringArray &array1, CString division);
	int Str2Int(CString cstr,int def=0);
	double Str2Float(CString cstr,double def=0);
	CString Int2Str(int ivalue);
	CString Float2Str(double dvalue,int dotbits=0);

public:
	//连接服务器
	int X_ConnectServer(CString strserver);
	//判断是否连接
	int X_isConnected();
	//关闭服务器连接
	void X_CloseServer();
	//读取String
	CString X_ReadString(CString strnode);
	//读取int
	int X_ReadInt(CString strnode);
	//读取float
	float X_ReadFloat(CString strnode);
	//读取boolean
	int X_ReadBoolean(CString strnode);
	//读取时间
	CString X_ReadDateTime(CString strnode);
	//写入string
	int X_WriteString(CString strnode,CString strvalue);
	//写入int
	int X_WriteInt(CString strnode,int ivalue);
	//写入float
	int X_WriteFloat(CString strnode,float fvalue);
	//写入boolean
	int X_WriteBoolean(CString strnode,int bvalue);
	//写入DateTime
	int X_WriteDateTime(CString strnode,CString strvalue);
	//批量读取string
	CString X_ReadStringArray(CString strnode);
    //批量写入string
	int X_WriteStringArray(CString strnode,CString strvalue);
	//封装后的批量读取String，用CStringArray
	int X_ReadStringArray(CStringArray &aryNode,CStringArray &aryValue);
	//封装后的批量写入String，用CStringArray
	int X_WriteStringArray(CStringArray &aryNode,CStringArray &aryValue);


	
};

