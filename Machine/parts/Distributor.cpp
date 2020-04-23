#include "StdAfx.h"
#include "Distributor.h"


CDistributor::CDistributor(CRunProcess *pParent)
{
	m_pParent = pParent;
}

CDistributor::CDistributor()
{
	m_pParent = NULL;
}


CDistributor::~CDistributor(void)
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

//气缸回退
BOOL CDistributor::Close(int nIndex)
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
		if(m_pParent->m_bDryRun)
		{
			m_pParent->APP_Wait_Signal_Sleep();
			continue;
		}
		m_pParent->APP_OUTPUTS_On(m_pCylArray[i]->ODistributorCylReturn);
		if(WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IDistributorCylReturn,ON,MSG_SHOW_DEFAULT) &&
			WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IDistributorCylPush,OFF,MSG_SHOW_DEFAULT))
		{
			continue;
		}
			
		return FALSE;
	}
	return TRUE;
}

BOOL CDistributor::Open(int nIndex)
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
		if(m_pParent->m_bDryRun)
		{
			m_pParent->APP_Wait_Signal_Sleep();
			continue;
		}
		m_pParent->APP_OUTPUTS_Off(m_pCylArray[i]->ODistributorCylReturn);
		if(WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IDistributorCylReturn,OFF,MSG_SHOW_DEFAULT) &&
			WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IDistributorCylPush,ON,MSG_SHOW_DEFAULT))
		{
			continue;
		}

		return FALSE;
	}
	return TRUE;
}


//增加气缸
BOOL CDistributor::AddDistributor(CString strIDistributorCylPush,CString strIDistributorCylReturn,
								  CString strODistributorCylReturn,CString strIHasMaterial,CString strICheckDirection,
								  CString strIFullCheck)
{
	ASSERT(m_pParent != NULL);

	pTagDistributorInfo pair = new tagDistributorInfo();
	
	
	pair->strIDistributorCylPush	= strIDistributorCylPush;
	pair->strIDistributorCylReturn	= strIDistributorCylReturn;
	pair->strODistributorCylReturn	= strODistributorCylReturn;
	pair->strIHasMaterial			= strIHasMaterial;	
	pair->strICheckDirection		= strICheckDirection;
	pair->strIFullCheck				= strIFullCheck;

	m_pParent->APP_Add_Input(&(pair->IDistributorCylPush),pair->strIDistributorCylPush);
	m_pParent->APP_Add_Input(&(pair->IDistributorCylReturn),pair->strIDistributorCylReturn);
	m_pParent->APP_Add_Input(&(pair->IHasMaterial),pair->strIHasMaterial);
	m_pParent->APP_Add_Input(&(pair->ICheckDirection),pair->strICheckDirection);
	m_pParent->APP_Add_Input(&(pair->IFullCheck),pair->strIFullCheck);

	m_pParent->APP_Add_Output(&(pair->ODistributorCylReturn),pair->strODistributorCylReturn);
	m_pCylArray.Add(pair);

	return TRUE;
}

//设置父句柄
void CDistributor::SetParent(CRunProcess *pParent)
{
	m_pParent = pParent;

}
int CDistributor::GetDistributorCount()
{
	return m_pCylArray.GetSize();
}
//气缸回退
BOOL CDistributor::Return(int nIndex)
{
	return Close(nIndex);
}

BOOL CDistributor::Push(int nIndex)
{
	return Open(nIndex);
}

//判断是否有料
BOOL CDistributor::HasMaterial(int nIndex)
{
	ASSERT(m_pParent != NULL);
	if(IsNoHardware())
	{
		return TRUE;
	}
	return m_pParent->APP_CheckInput(m_pCylArray[nIndex]->IHasMaterial,ON);	
}


//是否料满
BOOL CDistributor::IsMaterialFull(int nIndex)
{
	ASSERT(m_pParent != NULL);
	if(IsNoHardware())
	{
		return FALSE;
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
		if(m_pParent->APP_INPUTS_IsOn(m_pCylArray[i]->IFullCheck))
		{
			return TRUE;
		}
	}
	return FALSE;
}

