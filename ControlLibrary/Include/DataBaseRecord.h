#pragma once
#include <vector>
using namespace std;
#include "EasyAutoControlExDef.h"
//////////////////////////////
#include "afxmt.h"
//#import "c:\program files\common files\system\ado\msado15.dll" 	no_namespace rename ("EOF", "adoEOF")
//#import "c:\program files\common files\system\ado\MSJRO.DLL" no_namespace   

#pragma warning(disable:4146)
#import "..\ControlLibrary\Other\Database\msado15.dll" \
	no_namespace \
	rename("EOF","adoEOF") rename("DataTypeEnum","adoDataTypeEnum") \
	rename("FieldAttributeEnum", "adoFielAttributeEnum") rename("EditModeEnum", "adoEditModeEnum") \
	rename("LockTypeEnum", "adoLockTypeEnum") rename("RecordStatusEnum", "adoRecordStatusEnum") \
	rename("ParameterDirectionEnum", "adoParameterDirectionEnum")
//#import "C:\Program Files\Common Files\System\ado\MSJRO.DLL"   no_namespace   rename("ReplicaTypeEnum", "_ReplicaTypeEnum")
#import "..\ControlLibrary\Other\Database\msjro.dll" rename_namespace("NewName")

#define PAGE_SIZE 100
class __declspec(dllexport) CDataBaseRecord
{
public:
    CDataBaseRecord(void);
    ~CDataBaseRecord(void);
public:
	BOOL CompactAndRepair(CString szSrcDbPath, CString szDstDbPath, CString szDbPassword); 
	BOOL CompactAndRepair(CString szDbPath = _T("")); 
	BOOL CompactAndRepair(CString szDbPath, CString szDbPassword); 
private: 
	BOOL FileExists(CString szPath); 
	CString GetFileName( CString szFullPath ); 
	CString m_szLastError; 
public:
    BOOL Initialize();
    BOOL Release();
    BOOL OpenDataBase();
    BOOL CloseDataBase();

	BOOL InsertModuleCTRecord(CModuleCT & ModuleCT);
	BOOL InsertStepCTRecord(CStepCT &StepCT);
	BOOL InsertAlarmRecord(CAlarmInfo &AlarmInfo);
	BOOL InsertYieldRecord(CYieldInfo &YieldInfo);
	BOOL ShowSingleTable( const CString & strSql,vector<CListHead> &vecHead,CListCtrl *ListView = NULL );
	BOOL DeleteSingleTable( const CString & strSql);
	BOOL WriteExcel(const CString &FileName,CListCtrl* ListView = NULL);
	BOOL ExportExcel(const CString &FileName,_RecordsetPtr pRecord = NULL);
	BOOL ShowSinglePage( const CString & strSql,vector<CListHead> &vecHead,CListCtrl *ListView = NULL,int nPage = 1);
	BOOL ShowPage(vector<CListHead> &vecHead,CListCtrl *ListView = NULL,int nPage = 1);
	int  GetRecordCount(int nType = 0);
public:
    _ConnectionPtr m_Connection;
    _RecordsetPtr m_Recordset;
	_RecordsetPtr m_RecordsetShow;
	_RecordsetPtr m_ArmRecordset;
	_RecordsetPtr m_YeildRecordset;
	_RecordsetPtr m_StepRecordset;
	_RecordsetPtr m_ModuleRecordset;
    BOOL m_bInitialize;
    BOOL m_bOpen;
    CCriticalSection m_LockRecord;
	//for user
private:
	CString m_strCurUserName;
	int m_nCurUserLevel;
public:
	BOOL LoadUserName(CComboBox *cbBox=NULL);
	BOOL Login(CString strName,CString strPass);
	BOOL Logout(){m_strCurUserName = _T("");m_nCurUserLevel = -1;return TRUE;}
	BOOL AddUserName(CString strName,CString strPass,int nLevel);
	BOOL ModifyUserName(CString strName,CString strOldPass,CString strNewPass,int nLevel);
	BOOL DeleteUserName(CString strName);
	BOOL ShowUserList(CListCtrl *listView = NULL);
	CString GetCurUserName(){return m_strCurUserName;}
	int GetCurUserLevel(){return m_nCurUserLevel;}

public:
	BOOL LoadProduct(CComboBox *cbBox = NULL);
	BOOL LoadProduct(vector<CProduct> &vecPorduct,map<int*,CString> &mapMotor);
	BOOL LoadProductEX(vector<CProductEX> &vecPorduct);
	BOOL LoadProduct(CListCtrl *listView = NULL);
	BOOL ProductAdd(CProduct &pProduct);
	BOOL ProductModify(CProduct &pProduct);
	BOOL ProductDelete(int nID);
	BOOL MotorPosAdd(int nPkg,int nMotor,CMotorPos &mPos);
	BOOL MotorPosModify(int nPkg,int nMotor,CMotorPos &mPos);
	BOOL MotorPosDelete(int nPkg,int nMotor,CMotorPos &mPos);
};
