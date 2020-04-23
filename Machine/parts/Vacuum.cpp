#include "StdAfx.h"
#include "Vacuum.h"


CVacuum::CVacuum(CRunProcess *pParent)
{
	m_pParent = pParent;
}

CVacuum::CVacuum()
{
	m_pParent = NULL;
}


CVacuum::~CVacuum(void)
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

//夹爪夹紧
BOOL CVacuum::Close(int nIndex)
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
		m_pParent->APP_OUTPUTS_On(m_pCylArray[i]->OVacuumCylClose);
		m_pParent->APP_OUTPUTS_Off(m_pCylArray[i]->OVacuumCylOpen);

		if(m_pParent->m_bDryRun)
		{
			m_pParent->APP_Wait_Signal_Sleep();
			continue;
		}
		if(WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IVacuumCylClose,ON,MSG_SHOW_DEFAULT) &&
		   WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IVacuumCylOpen,OFF,MSG_SHOW_DEFAULT))
		{
			continue;
		}
		return FALSE;
	}
	return TRUE;
}
//夹爪打开
BOOL CVacuum::Open(int nIndex)
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
		m_pParent->APP_OUTPUTS_Off(m_pCylArray[i]->OVacuumCylClose);
		m_pParent->APP_OUTPUTS_On(m_pCylArray[i]->OVacuumCylOpen);

		if(m_pParent->m_bDryRun)
		{
			m_pParent->APP_Wait_Signal_Sleep();
			continue;
		}
		if(WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IVacuumCylClose,OFF,MSG_SHOW_DEFAULT) &&
			WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IVacuumCylOpen,ON,MSG_SHOW_DEFAULT))
		{
			continue;
		}
		return FALSE;
	}
	return TRUE;
}

//增加夹抓气缸
BOOL CVacuum::AddVacuum(CString strIVacuumCylOpen,CString strIVacuumCylClose,
						CString strOVacuumCylOpen,CString strOVacuumCylClose)
{
	ASSERT(m_pParent != NULL);

	pTagVacuumInfo pair = new tagVacuumInfo();
	pair->IVacuumCylOpen	 = -1;
	pair->IVacuumCylClose	 = -1;
	pair->OVacuumCylOpen	 = -1;
	pair->OVacuumCylClose	 = -1;
	
	pair->strIVacuumCylOpen		= strIVacuumCylOpen;
	pair->strIVacuumCylClose	= strIVacuumCylClose;
	pair->strOVacuumCylOpen		= strOVacuumCylOpen;
	pair->strOVacuumCylClose	= strOVacuumCylClose;	
	

	m_pParent->APP_Add_Input(&(pair->IVacuumCylOpen),pair->strIVacuumCylOpen);
	m_pParent->APP_Add_Input(&(pair->IVacuumCylClose),pair->strIVacuumCylClose);


	m_pParent->APP_Add_Output(&(pair->OVacuumCylOpen),pair->strOVacuumCylOpen);
	m_pParent->APP_Add_Output(&(pair->OVacuumCylClose),pair->strOVacuumCylClose);

	m_pCylArray.Add(pair);

	return TRUE;
}

