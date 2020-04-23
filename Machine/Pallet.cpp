#include "StdAfx.h"
#include "Pallet.h"

CPallet::CPallet(void)
{
	Release();
}

CPallet::CPallet(const CPallet &pPallet )
{
	m_lockPallet.Lock();
	m_bIsValid = pPallet.m_bIsValid;
	m_bIsNg = pPallet.m_bIsNg;
	m_bIsClean = m_bIsClean;
	m_nShellID = pPallet.m_nShellID;
	m_dLimitPressValue = pPallet.m_dLimitPressValue;
	m_dPressNormValue[0] = pPallet.m_dPressNormValue[0];
	m_dPressNormValue[1] = pPallet.m_dPressNormValue[1];
	m_strBarCode = pPallet.m_strBarCode;
	m_lockPallet.Unlock();
}

CPallet::~CPallet(void)
{
} 

void CPallet::CopyTo(CPallet* pReceive)
{
	ASSERT(pReceive != NULL);
	m_lockPallet.Lock();
	pReceive->m_lockPallet.Lock();
	pReceive->m_bIsClean=m_bIsClean;
	pReceive->m_bIsNg = m_bIsNg;
	pReceive->m_bIsValid = m_bIsValid;
	pReceive->m_nShellID = m_nShellID;
	pReceive->m_dLimitPressValue = m_dLimitPressValue;
	pReceive->m_dPressNormValue[0] = m_dPressNormValue[0];
	pReceive->m_dPressNormValue[1] = m_dPressNormValue[1];
	pReceive->m_strBarCode = m_strBarCode;
	m_bIsValid = FALSE;
	pReceive->m_lockPallet.Unlock();
	m_lockPallet.Unlock();
}


CPallet& CPallet::operator=( const CPallet &pPallet )
{
	if (this == &pPallet)
	{
		return *this;
	}

	m_lockPallet.Lock();
	
	m_bIsValid = pPallet.m_bIsValid;
	m_bIsNg = pPallet.m_bIsNg;
	m_bIsClean = pPallet.m_bIsClean;
	m_nShellID = pPallet.m_nShellID;
	m_dLimitPressValue = pPallet.m_dLimitPressValue;
	m_dPressNormValue[0] = pPallet.m_dPressNormValue[0];
	m_dPressNormValue[1] = pPallet.m_dPressNormValue[1];
	m_strBarCode = pPallet.m_strBarCode;

	m_lockPallet.Unlock();
	return *this;
}

void CPallet::Release()
{
	m_lockPallet.Lock();
	m_bIsValid = FALSE;
	m_bIsNg = FALSE;
	m_bIsClean = FALSE;
	m_nShellID = -1;
	m_dLimitPressValue = 0;
	m_dPressNormValue[0] = 0;
	m_dPressNormValue[1] = 0;
	m_strBarCode.Empty();
	m_lockPallet.Unlock();
}

void CPallet::ReadCfg( CString strSection, CString strPath )
{
	m_bIsValid = IniReadBoolean(strSection, _T("m_bIsValid"), FALSE, strPath);
	m_bIsNg = IniReadBoolean(strSection, _T("m_bIsNg"), FALSE, strPath);

	m_nShellID = IniReadInt(strSection, _T("m_nShellID"), -1, strPath);

	m_dLimitPressValue = IniReadDouble(strSection, _T("m_dLimitPressValue"), 0, strPath);
	m_dPressNormValue[0] =IniReadDouble(strSection, _T("m_dPressNormValue[0]"), 0, strPath);
	m_dPressNormValue[1] =IniReadDouble(strSection, _T("m_dPressNormValue[1]"), 0, strPath);
	   
	m_strBarCode = IniReadString(strSection, _T("m_strBarCode"), _T(""), strPath);
	return;
}

void CPallet::WriteCfg( CString strSection, CString strPath )
{
	IniWriteBoolean(strSection, _T("m_bIsValid"), m_bIsValid, strPath);
	IniWriteBoolean(strSection, _T("m_bIsNg"), m_bIsNg, strPath);
	
	IniWriteInt(strSection, _T("m_nShellID"), m_nShellID, strPath);

	IniWriteDouble(strSection, _T("m_dLimitPressValue"), m_dLimitPressValue, strPath);
	IniWriteDouble(strSection, _T("m_dPressNormValue[0]"), m_dPressNormValue[0], strPath);
	IniWriteDouble(strSection, _T("m_dPressNormValue[1]"), m_dPressNormValue[1], strPath);
	
	IniWriteString(strSection, _T("m_strBarCode"), m_strBarCode, strPath);
	
	return;
}
