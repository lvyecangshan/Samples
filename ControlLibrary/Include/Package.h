#pragma once

// this class is to be overwriten, should change to abstract class
class __declspec(dllexport) CPackage
{
public:
	CPackage();
	CPackage(const CPackage &package);
	virtual ~CPackage();

    CPackage& operator = (const CPackage &package);
	virtual BOOL SavePkgParam();
	BOOL SaveMotorParam();
	virtual BOOL LoadPkgParam(CString strPkgName);
	BOOL LoadMotorParam();

	static void GetPackages(CStringArray &strArray);
	static void SetSingleSetMotorParam(BOOL bSingleSet);

public:
	// some common packages
	CString m_strName;
	CString m_strDescription;
	UINT	m_nPackageID;
	BOOL	m_bEnabled;
};
