#pragma once
#include "package.h"

class CMachineProduct : public CPackage
{
public:
	CMachineProduct(void);
	~CMachineProduct(void);

public:
	BOOL SavePkgParam();
	BOOL LoadPkgParam(CString strPkgName);
};
