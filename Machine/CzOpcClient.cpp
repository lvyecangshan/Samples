#include "StdAfx.h"
#include "CzOpcClient.h"


//���캯��
CzOpcClient::CzOpcClient()
{
	//CoInitialize(NULL);
	initPtr();
	loadDll();
}

//��������
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

//��ʼ������
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

//����dll
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
		//��̬����ÿһ������(��15��)
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
//���ܣ�����OPC������
//�������
//strserver opc������ַ������opc.tcp://118.24.36.220:62547/DataAccessServer
//--------------------------------
//����ֵ 
//1-���ӳɹ���0���ӷ�����ʧ��
int CzOpcClient::X_ConnectServer(CString strserver)
{
	CStringA astr1=(CStringA)strserver;
	int iret=ConnectServer(astr1.GetBuffer(0));
	astr1.ReleaseBuffer();
	return iret;
}

//-----------------------------
//�ж��Ƿ����ӷ�����
//����ֵ
//1-�Ѿ����ӷ�������0-δ���ӷ�����
int CzOpcClient::X_isConnected()
{
	return isConnected();
}

//-----------------
//�رշ���������
void CzOpcClient::X_CloseServer()
{
	return CloseServer();
}

//----------------
//��ȡ�ַ���
//�������
//strnode  �ڵ�����ȫ�ƣ�����  ns=2;s=Machines/Machine B/Name
//����ֵ
//���ؽڵ���ַ�����ֵ
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
//��ȡint�ͽڵ�����
//�������
//strnode  �ڵ�����ȫ�ƣ�����  ns=2;s=Machines/Machine B/TestValueInt
//����ֵ
//���ؽڵ�intֵ
int CzOpcClient::X_ReadInt(CString strnode)
{
	CStringA astrin=CStringA(strnode);
	int iret=ReadInt(astrin.GetBuffer(0));
	astrin.ReleaseBuffer();
	return iret;
}

//--------------------------
//��ȡfloat�ͽڵ�����
//�������
//strnode  �ڵ�����ȫ�ƣ�����  ns=2;s=Machines/Machine B/TestValueFloat
//����ֵ
//���ؽڵ�floatֵ
float CzOpcClient::X_ReadFloat(CString strnode)
{
	CStringA astrin=CStringA(strnode);
	float iret=ReadFloat(astrin.GetBuffer(0));
	astrin.ReleaseBuffer();
	return iret;
}

//--------------------------
//��ȡBoolean�ͽڵ�����
//�������
//strnode  �ڵ�����ȫ�ƣ�����  ns=2;s=Machines/Machine B/IsFault
//����ֵ
//1-����ڵ�ֵΪtrue��0-����ڵ�ֵδfalse
int CzOpcClient::X_ReadBoolean(CString strnode)
{
	CStringA astrin=CStringA(strnode);
	int iret=ReadBoolean(astrin.GetBuffer(0));
	astrin.ReleaseBuffer();
	return iret;
}

//--------------------------
//��ȡDateTime�ͽڵ�����
//�������
//strnode  �ڵ�����ȫ�ƣ�����  ns=2;s=Machines/Machine B/AlarmTime
//����ֵ
//DateTimeת�ɵ��ַ���
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
//����String�ͽڵ�����
//�������
//strnode  �ڵ�����ȫ�ƣ�����  ns=2;s=Machines/Machine B/Name
//strvalue  �ڵ��String����
//����ֵ
//1-д��ɹ���0-д��ʧ��
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
//����int�ͽڵ�����
//�������
//strnode  �ڵ�����ȫ�ƣ�����  ns=2;s=Machines/Machine B/TestValueInt
//ivalue  �ڵ��int����
//����ֵ
//1-д��ɹ���0-д��ʧ��
int CzOpcClient::X_WriteInt(CString strnode,int ivalue)
{
	CStringA astrnode;
	astrnode=CStringA(strnode);
	int iret=WriteInt(astrnode.GetBuffer(0),ivalue);
	astrnode.ReleaseBuffer();
	return iret;
}

//--------------------------
//����float�ͽڵ�����
//�������
//strnode  �ڵ�����ȫ�ƣ�����  ns=2;s=Machines/Machine B/TestValueFloat
//fvalue  �ڵ��float����
//����ֵ
//1-д��ɹ���0-д��ʧ��
int CzOpcClient::X_WriteFloat(CString strnode,float fvalue)
{
	CStringA astrnode;
	astrnode=CStringA(strnode);
	int iret=WriteFloat(astrnode.GetBuffer(0),fvalue);
	astrnode.ReleaseBuffer();
	return iret;
}

//--------------------------
//����Boolean�ͽڵ�����
//�������
//strnode  �ڵ�����ȫ�ƣ�����  ns=2;s=Machines/Machine B/IsFault
//ivalue  1-д��trueֵ��0-д��falseֵ
//����ֵ
//1-д��ɹ���0-д��ʧ��
int CzOpcClient::X_WriteBoolean(CString strnode,int bvalue)
{
	CStringA astrnode;
	astrnode=CStringA(strnode);
	int iret=WriteBoolean(astrnode.GetBuffer(0),bvalue);
	astrnode.ReleaseBuffer();
	return iret;
}

//--------------------------
//����DateTime�ͽڵ�����
//�������
//strnode  �ڵ�����ȫ�ƣ�����  ns=2;s=Machines/Machine B/AlarmValue
//dtvalue  ����ʱ����ַ�����ʽ
//����ֵ
//1-д��ɹ���0-д��ʧ��
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
//������ȡString�ͽڵ�����
//�������
//strnodes  ����ڵ�����ȫ�ƣ�ֻ�ܶ���string���͵ģ��м���@@���ӣ�����  ns=2;s=Machines/Machine A/Name@@ns=2;s=Machines/Machine B/Name
//����ֵ
//����ڵ��ַ�������ͨ��@@���������� 
//���� 12345@@hijklmn  ����12345�Ƕ�ȡ���ĵ�һ���ڵ㣨ns=2;s=Machines/Machine A/Name�������ݣ�hijklmn�Ƕ�ȡ���ĵڶ����ڵ�(ns=2;s=Machines/Machine B/Name)������
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
//��������String�ͽڵ�����
//�������
//strnodes  ����ڵ�����ȫ�ƣ�ֻ�ܶ���string���͵ģ��м���@@���ӣ�����  ns=2;s=Machines/Machine A/Name@@ns=2;s=Machines/Machine B/Name
//aryValues ����ڵ��ַ�������ͨ��@@�������������� 12345@@hijklmn 
//����12345�����õ�һ���ڵ㣨ns=2;s=Machines/Machine A/Name�������ݣ�hijklmn�����õڶ����ڵ�(ns=2;s=Machines/Machine B/Name)������
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
//������ȡString�ͽڵ�����
//�������
//aryNode  ����ڵ�����ȫ�����飬ֻ�ܶ���string���͵�
//aryValue ����Ƿ��ض���ڵ����������
//����ֵ
//1-��ȡ�ɹ���0-��ȡʧ�� 
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
//����д��String�ͽڵ�����
//�������
//aryNode  ����ڵ�����ȫ�����飬ֻ�ܶ���string���͵�
//aryValue ����ڵ����������
//����ֵ
//1-д��ɹ���0-д��ʧ�� 
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
//�����ַ���"@@"�ָ�CString��CStringList
void CzOpcClient::CString2CStringArray(CString strIn,CStringArray &dest)
{
	SplitString(strIn,dest,L"@@");
}
//��CStringList�Ķ���ַ���ͨ��"@@"ƴ�ӳ�CString
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