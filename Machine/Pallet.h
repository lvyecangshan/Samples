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
	BOOL m_bIsClean;  //清洁标记
	int m_nShellID;	// 入壳工位ID

	double m_dLimitPressValue;				// checkValue
	double m_dPressNormValue[2];		// 设定值 0:min  1:max

	CString m_strBarCode;
	CCriticalSection m_lockPallet;
};