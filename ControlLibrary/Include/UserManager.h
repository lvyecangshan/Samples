//////////////////////////////////////////////////////////////////////
// Copyright (c) 2006 Zhang Songling, All Rights Reserved
//  zhangsongling@gmail.com
//////////////////////////////////////////////////////////////////////
#pragma once

#include <afxtempl.h>

enum USER_TYPE
{
    USER_INVALID,
	USER_OPERATOR,
	USER_MAINTENANCE,
	USER_TECHNICIAN = 2,
	USER_ENGINEER,
	USER_ADMIN,	// engineer that can manage all engineer/technician/operator's accounts
	USER_SERVICE,
	USER_SUPERUSER,		// service man that can manage all user's accounts
	NUM_OF_USER_TYPES,
};

class __declspec(dllexport) CUser : CObject
{
	DECLARE_SERIAL(CUser);
public:
	CUser();
    CUser(CString strUserName, CString strPassword, enum USER_TYPE eType);
	~CUser();
    CUser& operator = (const CUser &user);
	virtual void Serialize(CArchive& ar);

	int	m_eType;
	CString m_strName;
	CString m_strPassword; // encrypted
};

/////////////////////////////////////////////////////////////////////////////
// user manager class which take care of all the user manipulation related
class __declspec(dllexport) CUserManager
{
public:
	CUserManager();
	virtual ~CUserManager();

    CString GetName() { return m_strUserName; }
    enum USER_TYPE GetType() { return m_eType; }
	CString GetTypeName(enum USER_TYPE eType) { return m_strTypeName[eType]; }
	BOOL SetTypeName(enum USER_TYPE eType, CString strName); 
    CString GetErrorMessage() { return m_strError; }

	void ShowType(enum USER_TYPE eType, BOOL bShow) { m_bShowType[eType] = bShow; }
	BOOL IsTypeShown(enum USER_TYPE eType) { return m_bShowType[eType]; }

    BOOL AddUser(CString strUserName, CString strPassword, enum USER_TYPE eType);
    BOOL DeleteUsers(CStringList &strlstUserNames);
    enum USER_TYPE GetUserType(CString strUserName, CString strPassword);
    BOOL ChangePassword(CString strUserName, CString strOldPassword, CString strNewPassword);
    BOOL ChangeUserName(CString strOldUserName, CString strNewUserName);
    BOOL ChangeType(CString strUserName, enum USER_TYPE eNewType);
    
	BOOL DeleteUserPassword(CString strUserName);

    BOOL CheckInvalidCharacter(CString strItemName, CString strItemText, BOOL bAllowEmpty);
    BOOL ReadUserListFromFile(CList<CUser, CUser&> *m_pList);
    BOOL WriteUserListToFile(CList<CUser, CUser&> *m_pList);

    BOOL Login(CWnd* pParentWnd);
    void Logout();
    void ShowUserAccountsDlg(CWnd* pParentWnd);
    void ShowChangePasswordDlg(CWnd* pParentWnd, CString strUserName=_T(""));

	void EnableIndividualPassword(BOOL bEnable) { m_bEnableIndividual = bEnable; }
	BOOL IsIndividualPasswordEnabled() { return m_bEnableIndividual; }

protected:
    CString EncordPassword(CString strRawPassword);

protected:
    CString			m_strUserName;
    enum USER_TYPE	m_eType;
	CString			m_strTypeName[NUM_OF_USER_TYPES];
	BOOL			m_bShowType[NUM_OF_USER_TYPES];
    int				m_iTimeOut;
	BOOL			m_bEnableIndividual;
    CString			m_strError; // string that store the error message
};