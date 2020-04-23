#include "StdAfx.h"
#include "MachineProduct.h"

CMachineProduct::CMachineProduct(void)
{
}

CMachineProduct::~CMachineProduct(void)
{
}

BOOL CMachineProduct::SavePkgParam()
{
	if (!CPackage::SavePkgParam())
		return FALSE;

	CString strPathName = GetAbsolutePathName(PACKAGE_FOLDER) + m_strName + _T(".pkg");
	return TRUE;
}

BOOL CMachineProduct::LoadPkgParam(CString strPkgName)
{
	if (!CPackage::LoadPkgParam(strPkgName))
		return FALSE;

	CString strPathName = GetAbsolutePathName(PACKAGE_FOLDER) + m_strName + _T(".pkg");
	return TRUE;
}
