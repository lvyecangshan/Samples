#pragma once
struct tagDJPVariant
{
	VARTYPE vt;
	union
	{
		LPCTSTR lpszValue;
		long	lValue;
		double  dlValue;
		BOOL    bValue;
	};
};
class __declspec(dllexport) CDJPVariant : public tagDJPVariant
{
public:
	CDJPVariant(void);
	~CDJPVariant(void);
	CDJPVariant(const CDJPVariant& varSrc);
	CDJPVariant(LPCTSTR lpszSrc);
	CDJPVariant(long lSrc);
	CDJPVariant(double dlSrc);
	CDJPVariant(BOOL bSrc);

	const CDJPVariant& operator=(const LPCTSTR lpszSrc);
	const CDJPVariant& operator=(long lSrc);
	const CDJPVariant& operator=(double dlSrc);
	const CDJPVariant& operator=(BOOL bSrc);
};
