#pragma once

// CPropertyPageWithLang dialog
class __declspec(dllexport) CPropertyPageWithLang : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropertyPageWithLang)

public:
	CPropertyPageWithLang(UINT nIDTemplate);
	virtual ~CPropertyPageWithLang();

	virtual void UpdateLanguage();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnSetActive();

	DECLARE_MESSAGE_MAP()

protected:
	CString m_strTag;
	BOOL	m_bWithInLibrary;
};
