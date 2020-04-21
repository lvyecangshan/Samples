/*******************************************************************
�ļ�����BLMessageBox.h
���ߣ�              �汾��A1        ���ڣ�
��������������ƽ̨������Ϣ���ܺ���
��ʷ�޸ļ�¼��
*******************************************************************/
#pragma once

/*********************************************************************************
//����������������ʾ��
//���������lpszMessage	��ʾ����
//			nType ��ʾ����ʽ 
//				MB_ABORTRETRYIGNORE ��ʾAbort��Retry��Ignore��ť
//				MB_OK				��ʾOK��ť
//				MB_OKCANCEL			��ʾOK��Cancel��ť
//				MB_RETRYCANCEL		��ʾRetry��Cancel��ť
//				MB_YESNO			��ʾYes��No��ť
//				MB_YESNOCANCEL		��ʾYes��No��Cancel��ť
//				MB_ICONINFORMATION	��ʾһ��iͼ�꣬��ʾ��ʾ
//				MB_ICONEXCLAMATION	��ʾһ����̾�ţ���ʾ����
//				MB_ICONSTOP			��ʾ����ͼ�꣬��ʾ��������ش���
//				MB_ICONQUESTION		��ʾ�ʺ�ͼ�꣬��ʾ����
//��������: IDABORT		��ֹ��ť
//			IDCANCEL	�˳���ť
//			IDIGNORE	���԰�ť
//			IDNO		NO��ť
//			IDOK		OK��ť
//			IDRETRY		���԰�ť
//			IDYES		YES��ť
**********************************************************************************/
int __declspec(dllexport) BLMessageBox(LPCTSTR lpszMessage, UINT nType = MB_OK | MB_ICONEXCLAMATION);

/*********************************************************************************
//����������������ʾ��
//���������lpszMessage	��ʾ����
//			nType	��ʾ����ʽ 
//				MB_ABORTRETRYIGNORE ��ʾAbort��Retry��Ignore��ť
//				MB_OK				��ʾOK��ť
//				MB_OKCANCEL			��ʾOK��Cancel��ť
//				MB_RETRYCANCEL		��ʾRetry��Cancel��ť
//				MB_YESNO			��ʾYes��No��ť
//				MB_YESNOCANCEL		��ʾYes��No��Cancel��ť
//				MB_ICONINFORMATION	��ʾһ��iͼ�꣬��ʾ��ʾ
//				MB_ICONEXCLAMATION	��ʾһ����̾�ţ���ʾ����
//				MB_ICONSTOP			��ʾ����ͼ�꣬��ʾ��������ش���
//				MB_ICONQUESTION		��ʾ�ʺ�ͼ�꣬��ʾ����
//			pastrBtnText	���ڸ�����ť��ʾ����
//��������: IDABORT		��ֹ��ť
//			IDCANCEL	�˳���ť
//			IDIGNORE	���԰�ť
//			IDNO		NO��ť
//			IDOK		OK��ť
//			IDRETRY		���԰�ť
//			IDYES		YES��ť
**********************************************************************************/
int __declspec(dllexport) BLMessageBox(LPCTSTR lpszMessage, UINT nType, CStringArray *pastrBtnText);


