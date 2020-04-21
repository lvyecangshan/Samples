/*******************************************************************
�ļ�����Input.h
���ߣ�              �汾��A1        ���ڣ�
��������������ƽ̨����I/O������
��ʷ�޸ļ�¼��
*******************************************************************/
#pragma once
// class CInput
class CDeviceManager;

class  CInput
{
	friend CDeviceManager;

public:
	CInput();
	~CInput();

public:

	/*********************************************************************************
	//�����������ж�����I/O�Ƿ�ر�
	//�����������
	//��������: TRUE��ʾ�رգ�FALSE��ʾ��
	**********************************************************************************/
	_declspec(dllexport) BOOL IsOff(void);

	/*********************************************************************************
	//�����������ж�����I/O�Ƿ��
	//�����������
	//��������: TRUE��ʾ�򿪣�FALSE��ʾ�ر�
	**********************************************************************************/
	_declspec(dllexport) BOOL IsOn(void);

	_declspec(dllexport) BOOL operator == (const BOOL bState);
	_declspec(dllexport) BOOL operator != (const BOOL bState);

public:
	int	    m_nIndex;		// ������
	CString m_strName;		// I/O����
	DWORD   m_dwTimeout;	// ��ʱʱ�䣬��λ������

protected:
	BOOL	m_bOnState;		// ��λ
	int		m_nCard;		// ����
	int		m_nPoint;		// ��λ
    int     m_nFromLib;		// ��

protected:
	static BOOL ConfigModule(int nTotalInput);
	BOOL ReadConfig(int nIndex);
};