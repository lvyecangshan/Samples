/*******************************************************************
�ļ�����IniHelper.h
���ߣ�              �汾��A1        ���ڣ�
��������������ƽ̨Ini�ļ��������ܺ���
��ʷ�޸ļ�¼��
*******************************************************************/
#pragma once

/*********************************************************************************
//������������ini�ļ��ж�ȡָ���ֶε��ַ���
//���������lpszSection			������
//			lpszKey				�ֶ�����
//			lpszDefault			Ĭ��ֵ
//			lpszIniFile			ini�ļ�·��������
//��������: CString�ַ���
**********************************************************************************/
CString __declspec(dllexport) IniReadString(LPCTSTR lpszSection, LPCTSTR lpszKey, const LPCTSTR lpszDefault, LPCTSTR lpszIniFile);

/*********************************************************************************
//������������ini�ļ��ж�ȡָ���ֶε�������ֵ
//���������lpszSection			������
//			lpszKey				�ֶ�����
//			nDefault			Ĭ��ֵ
//			lpszIniFile			ini�ļ�·��������
//��������: ini������ֵ
**********************************************************************************/
int		__declspec(dllexport) IniReadInt(LPCTSTR lpszSection, LPCTSTR lpszKey, const int nDefault, LPCTSTR lpszIniFile);

/*********************************************************************************
//������������ini�ļ��ж�ȡָ���ֶε�Booleanֵ
//���������lpszSection			������
//			lpszKey				�ֶ�����
//			bDefault			Ĭ��ֵ
//			lpszIniFile			ini�ļ�·��������
//��������: BOOLֵ
**********************************************************************************/
BOOL	__declspec(dllexport) IniReadBoolean(LPCTSTR lpszSection, LPCTSTR lpszKey, const BOOL bDefault, LPCTSTR lpszIniFile);

/*********************************************************************************
//������������ini�ļ��ж�ȡָ���ֶεĸ�������ֵ
//���������lpszSection			������
//			lpszKey				�ֶ�����
//			dDefault			Ĭ��ֵ
//			lpszIniFile			ini�ļ�·��������
//��������: double��������ֵ
**********************************************************************************/
double	__declspec(dllexport) IniReadDouble(LPCTSTR lpszSection, LPCTSTR lpszKey, const double dDefault, LPCTSTR lpszIniFile);

/*********************************************************************************
//������������ini�ļ��ж�ȡָ���ֶε�RGBֵ
//���������lpszSection			������
//			lpszKey				�ֶ�����
//			clrDefault			Ĭ��ֵ
//			lpszIniFile			ini�ļ�·��������
//��������: RGBֵ
**********************************************************************************/
COLORREF __declspec(dllexport) IniReadColor(LPCTSTR lpszSection, LPCTSTR lpszKey, const COLORREF clrDefault, LPCTSTR lpszIniFile);

/*********************************************************************************
//������������ini�ļ��ж�ȡָ���ֶεĸ�������ֵ
//���������lpszSection			������
//			lpszKey				�ֶ�����
//			nStn				��������ռ����
//			pArray				��������ָ��
//			lpszIniFile			ini�ļ�·��������
//��������: TRUE����ȡ�ɹ�	FALSE����ȡʧ��
**********************************************************************************/
BOOL __declspec(dllexport) IniReadIntArray(LPCTSTR lpszSection, LPCTSTR lpszKey, int nStn, int *pArray, LPCTSTR lpszIniFile);

/*********************************************************************************
//������������ָ��ini�ļ�д���ַ���
//���������lpszSection			������
//			lpszKey				�ֶ�����
//			lpszStrToWrite		д���ַ���
//			lpszIniFile			ini�ļ�·��������
//��������: TRUE��д��ɹ�	FALSE��д��ʧ��
**********************************************************************************/
BOOL __declspec(dllexport) IniWriteString(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszStrToWrite, LPCTSTR lpszIniFile);

/*********************************************************************************
//������������ָ��ini�ļ�д��������ֵ
//���������lpszSection			������
//			lpszKey				�ֶ�����
//			nValueToWrite		д��������ֵ
//			lpszIniFile			ini�ļ�·��������
//��������: TRUE��д��ɹ�	FALSE��д��ʧ��
**********************************************************************************/
BOOL __declspec(dllexport) IniWriteInt(LPCTSTR lpszSection, LPCTSTR lpszKey, const int nValueToWrite, LPCTSTR lpszIniFile);

/*********************************************************************************
//������������ָ��ini�ļ�д��BOOLֵ
//���������lpszSection			������
//			lpszKey				�ֶ�����
//			bValueToWrite		д��BOOLֵ
//			lpszIniFile			ini�ļ�·��������
//��������: TRUE��д��ɹ�	FALSE��д��ʧ��
**********************************************************************************/
BOOL __declspec(dllexport) IniWriteBoolean(LPCTSTR lpszSection, LPCTSTR lpszKey, const BOOL bValueToWrite, LPCTSTR lpszIniFile);

/*********************************************************************************
//������������ָ��ini�ļ�д��double��������ֵ
//���������lpszSection			������
//			lpszKey				�ֶ�����
//			dValueToWrite		д��double��������ֵ
//			lpszIniFile			ini�ļ�·��������
//��������: TRUE��д��ɹ�	FALSE��д��ʧ��
**********************************************************************************/
BOOL __declspec(dllexport) IniWriteDouble(LPCTSTR lpszSection, LPCTSTR lpszKey, const double dValueToWrite, LPCTSTR lpszIniFile);
