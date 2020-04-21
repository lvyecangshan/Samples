//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// general system helper for most common use
// last update: 2008/02/15
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once
#include "IniHelper.h"
#include "EasyLibdef.h"
// since application's current directory is changed during some file operation, and most of the case for machine
//  software, we need to read/write file from a fixed predefined location, so we need to have that location no matter
//  what is the current directory is (if you use fopen("a.txt"), then it will open the "a.txt" from the current directory,
//  and it may not be the exe's file directory!!
CString __declspec(dllexport) GetAbsolutePathName(CString strFileName);

// to extract the file name from the long complete path
//  bHasExtension means return with extension (somefile.txt) or return with pure file name only (somefile)
CString __declspec(dllexport) GetFileName(CString strPathName, BOOL bHasExtension = FALSE);

// for user to decode a string into point or rectangle (to 2 int value or 4 int value)
void __declspec(dllexport) DecodeStringToPoint(CPoint &pt, CString strText, CString strToken);
void __declspec(dllexport) DecodeStringToRect(CRect &rc, CString strText, CString strToken);

// get and set the current language used for interface and system objects (run, motor, io, events...)
CString __declspec(dllexport) GetLanguage();
void __declspec(dllexport) SetLanguage(CString strLang);

// why other system objects (run, motor, io, events...) no need to have a seperate load function but here
//  for messages, we need to have one? because for other system objects, the names will be changed during machine update language
//  at each system object manager module, but for messages, it is runtimely loaded during shown, so we need to determine
//  which string to load during runtime.
CString __declspec(dllexport) LoadWarnMessage(UINT ID);

// interface we use resource ID, system objects we have their own sections, here this "user defined" is for user
//  to define its own string ID for user's own purpose
//  bFromLibrary is to indicate which language file to use
CString __declspec(dllexport) LoadUserDefinedString(UINT ID, BOOL bFromLibrary = FALSE);

// no hardware flag
BOOL __declspec(dllexport) IsNoHardware();

// read all message from history
BOOL __declspec(dllexport) ReadAllHistory(LPCTSTR lpszPathFile,CStringArray &strArray);

// only used in history file
CString __declspec(dllexport) GetContentInLine(LPCTSTR lpszLine,LPCTSTR lpszName);

// add single message to history file
BOOL __declspec(dllexport) AddHistoryLine(LPCTSTR lpszPathFile,LPCTSTR lpszSectiong,int nTeams,LPCTSTR lpszAttrib,LPCTSTR lpszContent,...);