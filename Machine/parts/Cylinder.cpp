#include "StdAfx.h"
#include "Cylinder.h"


CCylinder::CCylinder(CRunProcess *pParent):SCtrolCylinder(pParent)
{
	m_pParent = pParent;
}

CCylinder::CCylinder()
{
	m_pParent = NULL;
}


CCylinder::~CCylinder(void)
{
	int nSize = m_pCylArray.GetSize();
	for(int i = 0; i < nSize; i ++)
	{
		if(m_pCylArray[i] != NULL)
		{
			delete m_pCylArray[i];
			m_pCylArray[i] = NULL;
		}
	}
	m_pCylArray.RemoveAll();
}

//���׻���
BOOL CCylinder::Close(int nIndex)
{
	ASSERT(m_pParent != NULL);
	if(IsNoHardware())
	{
		return TRUE;
	}
	int nStart = 0;
	int nEnd = m_pCylArray.GetSize();
	if(nIndex != -1)
	{
		nStart = nIndex;
		nEnd = nIndex;
	}
	for(int i = nStart; i < nEnd; i ++)
	{
		
		m_pParent->APP_OUTPUTS_On(m_pCylArray[i]->OCylinderCylClose);
		m_pParent->APP_OUTPUTS_Off(m_pCylArray[i]->OCylinderCylOpen);

		if(m_pParent->m_bDryRun)
		{
			m_pParent->APP_Wait_Signal_Sleep();
			continue;
		}
		if(WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->ICylinderCylClose,ON,MSG_SHOW_DEFAULT) &&
		   WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->ICylinderCylOpen,OFF,MSG_SHOW_DEFAULT))
		{
			continue;
		}
		return FALSE;
	}
	return TRUE;
}
//���״�
BOOL CCylinder::Open(int nIndex)
{
	ASSERT(m_pParent != NULL);
	if(IsNoHardware())
	{
		return TRUE;
	}
	int nStart = 0;
	int nEnd = m_pCylArray.GetSize();
	if(nIndex != -1)
	{
		nStart = nIndex;
		nEnd = nIndex;
	}
	for(int i = nStart; i < nEnd; i ++)
	{
		m_pParent->APP_OUTPUTS_Off(m_pCylArray[i]->OCylinderCylClose);
		m_pParent->APP_OUTPUTS_On(m_pCylArray[i]->OCylinderCylOpen);

		if(m_pParent->m_bDryRun)
		{
			m_pParent->APP_Wait_Signal_Sleep();
			continue;
		}
		if(WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->ICylinderCylClose,OFF,MSG_SHOW_DEFAULT) &&
			WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->ICylinderCylOpen,ON,MSG_SHOW_DEFAULT))
		{
			continue;
		}
		return FALSE;
	}
	return TRUE;
}

//��������
BOOL CCylinder::AddCylinder(CString strICylinderCylOpen,CString strICylinderCylClose,
						CString strOCylinderCylOpen,CString strOCylinderCylClose)
{
	ASSERT(m_pParent != NULL);

	pTagCylinderInfo pair = new tagCylinderInfo();
	pair->ICylinderCylOpen	 = -1;
	pair->ICylinderCylClose	 = -1;
	pair->OCylinderCylOpen	 = -1;
	pair->OCylinderCylClose	 = -1;
	
	pair->strICylinderCylOpen		= strICylinderCylOpen;
	pair->strICylinderCylClose	= strICylinderCylClose;
	pair->strOCylinderCylOpen		= strOCylinderCylOpen;
	pair->strOCylinderCylClose	= strOCylinderCylClose;	
	

	m_pParent->APP_Add_Input(&(pair->ICylinderCylOpen),pair->strICylinderCylOpen);
	m_pParent->APP_Add_Input(&(pair->ICylinderCylClose),pair->strICylinderCylClose);


	m_pParent->APP_Add_Output(&(pair->OCylinderCylOpen),pair->strOCylinderCylOpen);
	m_pParent->APP_Add_Output(&(pair->OCylinderCylClose),pair->strOCylinderCylClose);

	m_pCylArray.Add(pair);

	return TRUE;
}

//���ø����
void CCylinder::SetParent(CRunProcess *pParent)
{
	m_pParent = pParent;
   SCtrolCylinder::SetParent(pParent);
}
int CCylinder::GetCylinderCount()
{
	return m_pCylArray.GetSize();
}

//�����Ƴ�
BOOL CCylinder::Push(int nIndex)
{
	return Open(nIndex);
}
//���׻���
BOOL CCylinder::Return(int nIndex)
{
	return Close(nIndex);
}

//���׶���
BOOL CCylinder::Up(int nIndex)
{
	return Open(nIndex);
}
//�����½�
BOOL CCylinder::Down(int nIndex)
{
	return Close(nIndex);
}

//��������(open)
BOOL CCylinder::Right(int nIndex)
{
	return Open(nIndex);
}
//��������(close)
BOOL CCylinder::Left(int nIndex)
{
	return Close(nIndex);
}
