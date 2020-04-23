#pragma once


class CPallet
{
public:
	CPallet();
	~CPallet();
	CPallet& operator=(const CPallet &pPallet);
	CPallet(const CPallet &pPallet);

public:
	void ReadCfg(CString strSection, CString strPath);
	void WriteCfg(CString strSection, CString strPath);
	void CopyTo(CPallet* pReceive);
	void Release();

public:
	BOOL m_bIsValid;
	BOOL m_bIsNg;
	BOOL m_bIsClean;  //�����
	int m_nShellID;	// ��ǹ�λID

	double m_dLimitPressValue;				// checkValue
	double m_dPressNormValue[2];		// �趨ֵ 0:min  1:max

	CString m_strBarCode;
	CCriticalSection m_lockPallet;
};