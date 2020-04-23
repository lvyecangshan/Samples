#include "StdAfx.h"
#include "PressCorecyLinder.h"


PressCoreCylinder::PressCoreCylinder(CRunProcess *pParent) : CCylinder(pParent)
{
	m_pParent = pParent;
}

PressCoreCylinder::PressCoreCylinder()
{
	m_pParent = NULL;
}


PressCoreCylinder::~PressCoreCylinder(void)
{
	int nSize = m_pPressCoreCylArray.GetSize();
	for(int i = 0; i < nSize; i ++)
	{
		if(m_pPressCoreCylArray[i] != NULL)
		{
			delete m_pPressCoreCylArray[i];
			m_pPressCoreCylArray[i] = NULL;
		}
	}
	m_pPressCoreCylArray.RemoveAll();
}


BOOL PressCoreCylinder::Close(int nIndex)
{
	ASSERT(m_pParent != NULL);
	if(IsNoHardware())
	{
		return TRUE;
	}
	int nStart = 0;
	int nEnd = m_pPressCoreCylArray.GetSize();
	if(nIndex != -1)
	{
		nStart = nIndex;
		nEnd = nIndex;
	}
	for(int i = nStart; i < nEnd; i ++)
	{
		m_pParent->APP_OUTPUTS_On(m_pPressCoreCylArray[i]->OPressCoreCylPush);

		if (m_pPressCoreCylArray[i]->OPressCoreCylReturn > -1)
		{
			m_pParent->APP_OUTPUTS_Off(m_pPressCoreCylArray[i]->OPressCoreCylReturn);
		}

		if(m_pParent->m_bDryRun)
		{
			m_pParent->APP_Wait_Signal_Sleep();
			continue;
		}

		if(WAIT_OK == m_pParent->APP_WaitInput(m_pPressCoreCylArray[i]->IPressCoreCylPush1,ON,MSG_SHOW_DEFAULT)
		    &&WAIT_OK == m_pParent->APP_WaitInput(m_pPressCoreCylArray[i]->IPressCoreCylReturn1,OFF,MSG_SHOW_DEFAULT)
		    &&WAIT_OK == m_pParent->APP_WaitInput(m_pPressCoreCylArray[i]->IPressCoreCylPush2,ON,MSG_SHOW_DEFAULT)
		    &&WAIT_OK == m_pParent->APP_WaitInput(m_pPressCoreCylArray[i]->IPressCoreCylReturn2,OFF,MSG_SHOW_DEFAULT))
		{
			continue;
		}
		return FALSE;
	}
	return TRUE;
}

BOOL PressCoreCylinder::Open(int nIndex)
{
	ASSERT(m_pParent != NULL);
	if(IsNoHardware())
	{
		return TRUE;
	}
	int nStart = 0;
	int nEnd = m_pPressCoreCylArray.GetSize();
	if(nIndex != -1)
	{
		nStart = nIndex;
		nEnd = nIndex;
	}
	for(int i = nStart; i < nEnd; i ++)
	{
		m_pParent->APP_OUTPUTS_Off(m_pPressCoreCylArray[i]->OPressCoreCylPush);

		if (m_pPressCoreCylArray[i]->OPressCoreCylReturn > -1)
		{
			m_pParent->APP_OUTPUTS_On(m_pPressCoreCylArray[i]->OPressCoreCylReturn);
		}

		if(m_pParent->m_bDryRun)
		{
			m_pParent->APP_Wait_Signal_Sleep();
			continue;
		}
		if(WAIT_OK == m_pParent->APP_WaitInput(m_pPressCoreCylArray[i]->IPressCoreCylPush1,OFF,MSG_SHOW_DEFAULT)
			&&WAIT_OK == m_pParent->APP_WaitInput(m_pPressCoreCylArray[i]->IPressCoreCylReturn1,ON,MSG_SHOW_DEFAULT)
			&&WAIT_OK == m_pParent->APP_WaitInput(m_pPressCoreCylArray[i]->IPressCoreCylPush2,OFF,MSG_SHOW_DEFAULT)
			&&WAIT_OK == m_pParent->APP_WaitInput(m_pPressCoreCylArray[i]->IPressCoreCylReturn2,ON,MSG_SHOW_DEFAULT))
		{
			continue;
		}
		return FALSE;
	}
	return TRUE;
}

//增加气缸
BOOL PressCoreCylinder::AddPressCoreCylinder(CString strIPressCoreCylPush1,CString strIPressCoreCylReturn1,
											CString strIPressCoreCylPush2,CString strIPressCoreCylReturn2,
											CString strOPressCoreCylPush,CString strOPressCoreCylReturn /*= _T("")*/)
{
	ASSERT(m_pParent != NULL);

	pTagPressCoreCylinderInfo pair = new tagPressCoreCylinderInfo();
	
	pair->strIPressCoreCylPush1		= strIPressCoreCylPush1;
	pair->strIPressCoreCylReturn1	= strIPressCoreCylReturn1;
	pair->strIPressCoreCylPush2		= strIPressCoreCylPush2;
	pair->strIPressCoreCylReturn2	= strIPressCoreCylReturn2;	
	pair->strOPressCoreCylPush      = strOPressCoreCylPush;
	pair->strOPressCoreCylReturn    = strOPressCoreCylReturn;
	

	m_pParent->APP_Add_Input(&(pair->IPressCoreCylPush1),pair->strIPressCoreCylPush1);
	m_pParent->APP_Add_Input(&(pair->IPressCoreCylReturn1),pair->strIPressCoreCylReturn1);
	m_pParent->APP_Add_Input(&(pair->IPressCoreCylPush2),pair->strIPressCoreCylPush2);
	m_pParent->APP_Add_Input(&(pair->IPressCoreCylReturn2),pair->strIPressCoreCylReturn2);


	m_pParent->APP_Add_Output(&(pair->OPressCoreCylPush),pair->strOPressCoreCylPush);
	m_pParent->APP_Add_Output(&(pair->OPressCoreCylReturn),pair->strOPressCoreCylReturn);

	m_pPressCoreCylArray.Add(pair);

	return TRUE;
}

//设置父句柄
void PressCoreCylinder::SetParent(CRunProcess *pParent)
{
	m_pParent = pParent;
	CCylinder::SetParent(pParent);

}
int PressCoreCylinder::GetCylinderCount()
{
	return m_pPressCoreCylArray.GetSize();
}

//气缸推出
BOOL PressCoreCylinder::PressCoreCyPush(int nIndex)
{
	return Close(nIndex);
}

//气缸推出
BOOL PressCoreCylinder::CylinderPush(int nIndex)
{
	return Close(nIndex);
}

//气缸回退
BOOL PressCoreCylinder::PressCoreCyReturn(int nIndex)
{
	return Open(nIndex);
}

//气缸回退
BOOL PressCoreCylinder::CylinderReturn(int nIndex)
{
	return Open(nIndex);
}