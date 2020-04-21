/*******************************************************************
�ļ�����Output.h
���ߣ�              �汾��A1        ���ڣ�
��������������ƽ̨���I/O������
��ʷ�޸ļ�¼��
*******************************************************************/
#pragma once

class CDeviceManager;

class  COutput
{
	friend CDeviceManager;

// constructors / destructors
public:
	COutput();
	~COutput();

public:
	/*********************************************************************************
	//�����������ر����I/O
	//�����������
	//��������: TRUE��ʾ�رճɹ���FALSE��ʾ�ر�ʧ��
	**********************************************************************************/
	_declspec(dllexport) BOOL Off(void);

	/*********************************************************************************
	//���������������I/O
	//�����������
	//��������: TRUE��ʾ�򿪳ɹ���FALSE��ʾ��ʧ��
	**********************************************************************************/
	_declspec(dllexport) BOOL On(void);

	/*********************************************************************************
	//�����������ж����I/O�Ƿ�ر�
	//�����������
	//��������: TRUE��ʾ�رգ�FALSE��ʾ��
	**********************************************************************************/
	_declspec(dllexport) BOOL IsOff(void);

	/*********************************************************************************
	//�����������ж����I/O�Ƿ��
	//�����������
	//��������: TRUE��ʾ�򿪣�FALSE��ʾ�ر�
	**********************************************************************************/
	_declspec(dllexport) BOOL IsOn(void);

	_declspec(dllexport) BOOL operator =(const BOOL bState);
	_declspec(dllexport) BOOL operator ==(const BOOL bState);
	_declspec(dllexport) BOOL operator !=(const BOOL bState);

public:
	int	    m_nIndex;		//������
	CString m_strName;		//I/O����
	BOOL	m_bMonitorChange;	//�豸���й����У��Ƿ������˹��޸����I/O״̬

private:
	int		m_nCard;		//����
	int		m_nPoint;		//��λ
	int		m_nFromLib;		//��
	BOOL	m_bOnState;		//��Ӧ����/����Ч

	// the old state during machine stop
	BOOL	m_bSavedState;

protected:

	static BOOL ConfigModule(int nTotalOutput);
	BOOL ReadConfig(int nIndex);

	// monitor the change of status
	void SaveState(void);
	BOOL IsStateChanged(void);
};