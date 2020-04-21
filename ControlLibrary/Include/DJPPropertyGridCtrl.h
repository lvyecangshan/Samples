#pragma once
#include <map>

// CDJPPropertyGridCtrl

class __declspec(dllexport) CDJPPropertyGridCtrl : public CMFCPropertyGridCtrl
{
	DECLARE_DYNAMIC(CDJPPropertyGridCtrl)

public:
	CDJPPropertyGridCtrl();
	virtual ~CDJPPropertyGridCtrl();
private:
	CString m_strValue;
	CString m_strFirst;
	CString m_strSecond;
protected:
	DECLARE_MESSAGE_MAP()
	CFont m_fntPropList;
	CMFCPropertyGridProperty* m_pPropety[3];
public:
	void SetPropListFont();
	void Init(CString strFirstTitle = _T(""),CString strSecondTitle = _T(""),CString strThirdTitle = _T(""));
	void AddGroup(map<void*,CString> &voidMap,int nIndex = 0);
};


