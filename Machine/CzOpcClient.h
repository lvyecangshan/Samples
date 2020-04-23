#pragma once

//����typedef�ĺ�������
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
	int loadDll();//����dll
private:
	//������������(��������15��)
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
	//��Щ�����·�װ�ĺ���
private:
	
	//�����ַ���"@@"�ָ�CString��CStringList
	void CString2CStringArray(CString strIn,CStringArray &dest);
	//��CStringList�Ķ���ַ���ͨ��"@@"ƴ�ӳ�CString
	CString CStringArray2CString(CStringArray &aryValue);
public:
	//�ָ��ַ���
	void SplitString(CString strsource, CStringArray &array1, CString division);
	int Str2Int(CString cstr,int def=0);
	double Str2Float(CString cstr,double def=0);
	CString Int2Str(int ivalue);
	CString Float2Str(double dvalue,int dotbits=0);

public:
	//���ӷ�����
	int X_ConnectServer(CString strserver);
	//�ж��Ƿ�����
	int X_isConnected();
	//�رշ���������
	void X_CloseServer();
	//��ȡString
	CString X_ReadString(CString strnode);
	//��ȡint
	int X_ReadInt(CString strnode);
	//��ȡfloat
	float X_ReadFloat(CString strnode);
	//��ȡboolean
	int X_ReadBoolean(CString strnode);
	//��ȡʱ��
	CString X_ReadDateTime(CString strnode);
	//д��string
	int X_WriteString(CString strnode,CString strvalue);
	//д��int
	int X_WriteInt(CString strnode,int ivalue);
	//д��float
	int X_WriteFloat(CString strnode,float fvalue);
	//д��boolean
	int X_WriteBoolean(CString strnode,int bvalue);
	//д��DateTime
	int X_WriteDateTime(CString strnode,CString strvalue);
	//������ȡstring
	CString X_ReadStringArray(CString strnode);
    //����д��string
	int X_WriteStringArray(CString strnode,CString strvalue);
	//��װ���������ȡString����CStringArray
	int X_ReadStringArray(CStringArray &aryNode,CStringArray &aryValue);
	//��װ�������д��String����CStringArray
	int X_WriteStringArray(CStringArray &aryNode,CStringArray &aryValue);


	
};

