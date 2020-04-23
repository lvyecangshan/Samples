//�������ʱʱ���趨Ϊ5��
//1. ��ѯ״̬�����0x10��
//���ͣ�
// ��ʼλ   ��ַ   ����   ֹͣλ
// 0xAA     0x01   0x10   0xCC
// ��ѧ���ģ��ظ���
// ��ʼλ   ��ַ   ����         ֹͣλ
// 0xAA     0x01   return_sign   0xCC
//����˵�� Return_sign:
//0xB1-----׼����
//0xB2------׼����
//0xEE------û׼����

//2��������������0x20)
//���ͣ�
// ��ʼλ   ��ַ   ����   ֹͣλ
// 0xAA     0x01   0x20   0xCC
// ��ѧ���ģ��ظ���
// ��ʼλ   ��ַ   ����         ֹͣλ
// 0xAA     0x01   return_sign   0xCC
//����˵�� Return_sign:
//0xDD-----�ϸ�
//0xBB------���ڼ��
//0x11--------�������ź���Ӧ��
//0x22--------Ƶ�쳬��
//0x33--------���Գ���
//0x44--------ʧ�泬��
//0x55--------�迹����

#pragma once

#define READ_LIMIT 30

class CComPort
{
public:
	CComPort(void);
	~CComPort(void);
public:
	BOOL OpenPort(int iPort,DWORD dwBaudRate);
	BOOL ClsoePort();

	char Write(char *szCmd);
	BOOL Read(CString &strResult,int iSleepTime = 1000);
	
public:
	int m_iPort;
	BOOL m_bOpen;

private:
	HANDLE m_hCom;
	DWORD m_dwBaudRate;
	byte m_btStopBits;
	byte m_btByteSize;
	byte m_btParity;
	CCriticalSection m_PortLock;
};
