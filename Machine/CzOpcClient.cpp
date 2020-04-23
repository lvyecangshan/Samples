#include "StdAfx.h"
#include "CzOpcClient.h"


//构造函数
CzOpcClient::CzOpcClient()
{
	//CoInitialize(NULL);
	initPtr();
	loadDll();
}

//析构函数
CzOpcClient::~CzOpcClient()
{
	if (hDllInst)
	{
		FreeLibrary(hDllInst);
		initPtr();
	}
	//CoUninitialize();
	initPtr();
}

//初始化函数
void CzOpcClient::initPtr()
{
	ConnectServer = NULL;
	isConnected = NULL;
	CloseServer = NULL;
	ReadString = NULL;
	ReadInt = NULL;
	ReadFloat = NULL;
	ReadBoolean = NULL;
	ReadDateTime = NULL;
	WriteString = NULL;
	WriteInt = NULL;
	WriteFloat = NULL;
	WriteBoolean = NULL;
	WriteDateTime = NULL;
	ReadStringArray = NULL;
	WriteStringArray = NULL;
}

//加载dll
int CzOpcClient::loadDll()
{
	if (hDllInst)
	{
		FreeLibrary(hDllInst);
		hDllInst=NULL;
		initPtr();
	}
	int ret=0;
	hDllInst=LoadLibrary(L"OpcClientDll.dll");
	if(hDllInst)
	{
		//动态加载每一个函数(共15个)
		//------
		ConnectServer=(dll_ConnectServer)GetProcAddress(hDllInst,"ConnectServer");
		if(ConnectServer)
		{
			ret++;
		}
		//------
		isConnected=(dll_isConnected)GetProcAddress(hDllInst,"isConnected");
		if(isConnected)
		{
			ret++;
		}
		//------
		CloseServer=(dll_CloseServer)GetProcAddress(hDllInst,"CloseServer");
		if(CloseServer)
		{
			ret++;
		}
		//------
		ReadString=(dll_ReadString)GetProcAddress(hDllInst,"ReadString");
		if(ReadString)
		{
			ret++;
		}
		//------
		ReadInt=(dll_ReadInt)GetProcAddress(hDllInst,"ReadInt");
		if(ReadInt)
		{
			ret++;
		}
		//------
		ReadFloat=(dll_ReadFloat)GetProcAddress(hDllInst,"ReadFloat");
		if(ReadFloat)
		{
			ret++;
		}
		//------
		ReadBoolean=(dll_ReadBoolean)GetProcAddress(hDllInst,"ReadBoolean");
		if(ReadBoolean)
		{
			ret++;
		}
		//------
		ReadDateTime=(dll_ReadDateTime)GetProcAddress(hDllInst,"ReadDateTime");
		if(ReadDateTime)
		{
			ret++;
		}
		//------
		WriteString=(dll_WriteString)GetProcAddress(hDllInst,"WriteString");
		if(WriteString)
		{
			ret++;
		}
		//------
		WriteInt=(dll_WriteInt)GetProcAddress(hDllInst,"WriteInt");
		if(WriteInt)
		{
			ret++;
		}
		//------
		WriteFloat=(dll_WriteFloat)GetProcAddress(hDllInst,"WriteFloat");
		if(WriteFloat)
		{
			ret++;
		}
		//------
		WriteBoolean=(dll_WriteBoolean)GetProcAddress(hDllInst,"WriteBoolean");
		if(WriteBoolean)
		{
			ret++;
		}
		//------
		WriteDateTime=(dll_WriteDateTime)GetProcAddress(hDllInst,"WriteDateTime");
		if(WriteDateTime)
		{
			ret++;
		}
		//------
		ReadStringArray=(dll_ReadStringArray)GetProcAddress(hDllInst,"ReadStringArray");
		if(ReadStringArray)
		{
			ret++;
		}
		//------
		WriteStringArray=(dll_WriteStringArray)GetProcAddress(hDllInst,"WriteStringArray");
		if(WriteStringArray)
		{
			ret++;
		}
	}
	return ret;
}

//--------------------------------
//功能：连接OPC服务器
//输入参数
//strserver opc服务网址，比如opc.tcp://118.24.36.220:62547/DataAccessServer
//--------------------------------
//返回值 
//1-连接成功，0连接服务器失败
int CzOpcClient::X_ConnectServer(CString strserver)
{
	CStringA astr1=(CStringA)strserver;
	int iret=ConnectServer(astr1.GetBuffer(0));
	astr1.ReleaseBuffer();
	return iret;
}

//-----------------------------
//判断是否连接服务器
//返回值
//1-已经连接服务器，0-未连接服务器
int CzOpcClient::X_isConnected()
{
	return isConnected();
}

//-----------------
//关闭服务器连接
void CzOpcClient::X_CloseServer()
{
	return CloseServer();
}

//----------------
//读取字符串
//输入参数
//strnode  节点名字全称，比如  ns=2;s=Machines/Machine B/Name
//返回值
//返回节点的字符串的值
CString CzOpcClient::X_ReadString(CString strnode)
{
	char *parray=new char[6144];
   memset(parray,0,6144);
   CStringA astrin=CStringA(strnode);
   int iret=ReadString(astrin.GetBuffer(0),parray);
   astrin.ReleaseBuffer();
   CStringA astrout(parray);
   CString cstrout=(CString)astrout;
   delete[] parray;
   return cstrout;
   
   //return L"";
  
}

//--------------------------
//读取int型节点数据
//输入参数
//strnode  节点名字全称，比如  ns=2;s=Machines/Machine B/TestValueInt
//返回值
//返回节点int值
int CzOpcClient::X_ReadInt(CString strnode)
{
	CStringA astrin=CStringA(strnode);
	int iret=ReadInt(astrin.GetBuffer(0));
	astrin.ReleaseBuffer();
	return iret;
}

//--------------------------
//读取float型节点数据
//输入参数
//strnode  节点名字全称，比如  ns=2;s=Machines/Machine B/TestValueFloat
//返回值
//返回节点float值
float CzOpcClient::X_ReadFloat(CString strnode)
{
	CStringA astrin=CStringA(strnode);
	float iret=ReadFloat(astrin.GetBuffer(0));
	astrin.ReleaseBuffer();
	return iret;
}

//--------------------------
//读取Boolean型节点数据
//输入参数
//strnode  节点名字全称，比如  ns=2;s=Machines/Machine B/IsFault
//返回值
//1-代表节点值为true，0-代表节点值未false
int CzOpcClient::X_ReadBoolean(CString strnode)
{
	CStringA astrin=CStringA(strnode);
	int iret=ReadBoolean(astrin.GetBuffer(0));
	astrin.ReleaseBuffer();
	return iret;
}

//--------------------------
//读取DateTime型节点数据
//输入参数
//strnode  节点名字全称，比如  ns=2;s=Machines/Machine B/AlarmTime
//返回值
//DateTime转成的字符串
CString CzOpcClient::X_ReadDateTime(CString strnode)
{
	char *parray=new char[6144];
	memset(parray,0,6144);
	CStringA astrin=CStringA(strnode);
	int iret=ReadDateTime(astrin.GetBuffer(0),parray);
	astrin.ReleaseBuffer();
	CStringA astrout(parray);
	CString cstrout=(CString)astrout;
	delete[] parray;
	return cstrout;
}

//--------------------------
//设置String型节点数据
//输入参数
//strnode  节点名字全称，比如  ns=2;s=Machines/Machine B/Name
//strvalue  节点的String数据
//返回值
//1-写入成功，0-写入失败
int CzOpcClient::X_WriteString(CString strnode,CString strvalue)
{
	CStringA astrnode,astrvalue;
	astrnode=CStringA(strnode);
	astrvalue=CStringA(strvalue);
	int iret=WriteString(astrnode.GetBuffer(0),astrvalue.GetBuffer(0));
	astrnode.ReleaseBuffer();
	astrvalue.ReleaseBuffer();
	return iret;
}

//--------------------------
//设置int型节点数据
//输入参数
//strnode  节点名字全称，比如  ns=2;s=Machines/Machine B/TestValueInt
//ivalue  节点的int数据
//返回值
//1-写入成功，0-写入失败
int CzOpcClient::X_WriteInt(CString strnode,int ivalue)
{
	CStringA astrnode;
	astrnode=CStringA(strnode);
	int iret=WriteInt(astrnode.GetBuffer(0),ivalue);
	astrnode.ReleaseBuffer();
	return iret;
}

//--------------------------
//设置float型节点数据
//输入参数
//strnode  节点名字全称，比如  ns=2;s=Machines/Machine B/TestValueFloat
//fvalue  节点的float数据
//返回值
//1-写入成功，0-写入失败
int CzOpcClient::X_WriteFloat(CString strnode,float fvalue)
{
	CStringA astrnode;
	astrnode=CStringA(strnode);
	int iret=WriteFloat(astrnode.GetBuffer(0),fvalue);
	astrnode.ReleaseBuffer();
	return iret;
}

//--------------------------
//设置Boolean型节点数据
//输入参数
//strnode  节点名字全称，比如  ns=2;s=Machines/Machine B/IsFault
//ivalue  1-写入true值，0-写入false值
//返回值
//1-写入成功，0-写入失败
int CzOpcClient::X_WriteBoolean(CString strnode,int bvalue)
{
	CStringA astrnode;
	astrnode=CStringA(strnode);
	int iret=WriteBoolean(astrnode.GetBuffer(0),bvalue);
	astrnode.ReleaseBuffer();
	return iret;
}

//--------------------------
//设置DateTime型节点数据
//输入参数
//strnode  节点名字全称，比如  ns=2;s=Machines/Machine B/AlarmValue
//dtvalue  日期时间的字符串格式
//返回值
//1-写入成功，0-写入失败
int CzOpcClient::X_WriteDateTime(CString strnode,CString strvalue)
{
	CStringA astrnode,astrvalue;
	astrnode=CStringA(strnode);
	astrvalue=CStringA(strvalue);
	int iret=WriteDateTime(astrnode.GetBuffer(0),astrvalue.GetBuffer(0));
	astrnode.ReleaseBuffer();
	astrvalue.ReleaseBuffer();
	return iret;
}


//--------------------------
//批量读取String型节点数据
//输入参数
//strnodes  多个节点名字全称，只能都是string类型的，中间用@@连接，比如  ns=2;s=Machines/Machine A/Name@@ns=2;s=Machines/Machine B/Name
//返回值
//多个节点字符串数据通过@@连接起来， 
//比如 12345@@hijklmn  代表12345是读取到的第一个节点（ns=2;s=Machines/Machine A/Name）的数据，hijklmn是读取到的第二个节点(ns=2;s=Machines/Machine B/Name)的数据
CString CzOpcClient::X_ReadStringArray(CString strnode)
{
	char *parray=new char[6144];
	memset(parray,0,6144);
	CStringA astrin=CStringA(strnode);
	int iret=ReadStringArray(astrin.GetBuffer(0),parray);
	astrin.ReleaseBuffer();
	CStringA astrout(parray);
	CString cstrout=(CString)astrout;
	delete[] parray;
	return cstrout;
}

//--------------------------
//批量设置String型节点数据
//输入参数
//strnodes  多个节点名字全称，只能都是string类型的，中间用@@连接，比如  ns=2;s=Machines/Machine A/Name@@ns=2;s=Machines/Machine B/Name
//aryValues 多个节点字符串数据通过@@连接起来，比如 12345@@hijklmn 
//代表12345是设置第一个节点（ns=2;s=Machines/Machine A/Name）的数据，hijklmn是设置第二个节点(ns=2;s=Machines/Machine B/Name)的数据
int CzOpcClient::X_WriteStringArray(CString strnode,CString strvalue)
{
	CStringA astrnode,astrvalue;
	astrnode=CStringA(strnode);
	astrvalue=CStringA(strvalue);
	int iret=WriteStringArray(astrnode.GetBuffer(0),astrvalue.GetBuffer(0));
	astrnode.ReleaseBuffer();
	astrvalue.ReleaseBuffer();
	return iret;
}

//--------------------------
//批量读取String型节点数据
//输入参数
//aryNode  多个节点名字全称数组，只能都是string类型的
//aryValue 这个是返回多个节点的数据数组
//返回值
//1-读取成功，0-读取失败 
/*
CStringArray arynode;
CStringArray aryvalue;
arynode.Add(L"ns=2;s=Machines/Machine A/Name");
arynode.Add(L"ns=2;s=Machines/Machine B/Name");
opc1.X_ReadStringArray(arynode,aryvalue);
if(aryvalue.GetCount()>0)
{
for(int i=0;i<aryvalue.GetCount();i++)
{
AfxMessageBox(aryvalue.GetAt(i));
}
}
*/
int CzOpcClient::X_ReadStringArray(CStringArray &aryNode,CStringArray &aryValue)
{
    CString strin=CStringArray2CString(aryNode);
	CString strout=X_ReadStringArray(strin);
	CString2CStringArray(strout,aryValue);
	if(aryValue.GetCount()>0)
	{
		return 1;
	}
	return 0;
}

//--------------------------
//批量写入String型节点数据
//输入参数
//aryNode  多个节点名字全称数组，只能都是string类型的
//aryValue 多个节点的数据数组
//返回值
//1-写入成功，0-写入失败 
/*
CStringArray arynode;
CStringArray aryvalue;
arynode.Add(L"ns=2;s=Machines/Machine A/Name");
arynode.Add(L"ns=2;s=Machines/Machine B/Name");
aryvalue.Add(L" hello ");
aryvalue.Add(L" world ");
opc1.X_WriteStringArray(arynode,aryvalue);
*/
int CzOpcClient::X_WriteStringArray(CStringArray &aryNode,CStringArray &aryValue)
{
	CString strin1,strin2;
	strin1=CStringArray2CString(aryNode);
	strin2=CStringArray2CString(aryValue);
	return X_WriteStringArray(strin1,strin2);
}


void CzOpcClient::SplitString(CString strsource, CStringArray &array1, CString division)
{
	array1.RemoveAll();
	int len=division.GetLength();
	int result;
	CString source=strsource;
	while(source.GetLength()>0)
	{
		result=source.Find(division);
		if(result>=0)
		{
			array1.Add(source.Mid(0,result));
			if(source.GetLength()>=result)
			{
				source=source.Mid(result+len,source.GetLength()-result);
			}else
			{
				source=L"";
			}
		}else 
		{
			if(source.GetLength()>0)
			{
				array1.Add(source);
				source=L"";
			}else;
		}
	}
}
//根据字符串"@@"分割CString到CStringList
void CzOpcClient::CString2CStringArray(CString strIn,CStringArray &dest)
{
	SplitString(strIn,dest,L"@@");
}
//把CStringList的多个字符串通过"@@"拼接成CString
CString CzOpcClient::CStringArray2CString(CStringArray &aryValue)
{
	CString strValues=L"";
	int nodeSize=aryValue.GetCount();
	if(nodeSize==1)
	{
		strValues=aryValue.GetAt(0);
	}
	else
	{
		for(int i=0;i<nodeSize-1;i++)
		{
			strValues=strValues+aryValue.GetAt(i)+L"@@";
		}
		strValues=strValues+aryValue.GetAt(nodeSize-1);
	}
	return strValues;
}

int CzOpcClient::Str2Int(CString cstr,int def)
{
	try
	{
		int m=_ttoi(cstr.GetBuffer(0));
		cstr.ReleaseBuffer();
		return m;
	}
	catch (...)
	{
		return def;
	}
}

double CzOpcClient::Str2Float(CString cstr,double def)
{
	try
	{
		//return atof(CString2Char(cstr));
		double m=_ttof(cstr.GetBuffer(0));
		cstr.ReleaseBuffer();
		return m;
	}
	catch (...)
	{
		return def;
	}
}

CString CzOpcClient::Int2Str(int ivalue)
{
	CString cstr;
	cstr.Format(L"%d",ivalue);
	return cstr;
}

CString CzOpcClient::Float2Str(double dvalue,int dotbits)
{
	CString cstr;
	switch(dotbits)
	{
	case 0:cstr.Format(L"%lf",dvalue);break;
	case 1:cstr.Format(L"%.1lf",dvalue);break;
	case 2:cstr.Format(L"%.2lf",dvalue);break;
	case 3:cstr.Format(L"%.3lf",dvalue);break;
	case 4:cstr.Format(L"%.4lf",dvalue);break;
	case 5:cstr.Format(L"%.5lf",dvalue);break;
	case 6:cstr.Format(L"%.6lf",dvalue);break;
	default:cstr.Format(L"%lf",dvalue);break;
	}
	//cstr.Format("%lf",dvalue);
	return cstr;
}