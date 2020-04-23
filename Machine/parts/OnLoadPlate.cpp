#include "StdAfx.h"
#include "OnLoadPlate.h"


COnLoadPlate::COnLoadPlate(CRunProcess *pParent)
{
	m_pParent = pParent;
}

COnLoadPlate::COnLoadPlate()
{
	m_pParent = NULL;
}


COnLoadPlate::~COnLoadPlate(void)
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
BOOL COnLoadPlate::JackingClose(int nIndex)
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
		m_pParent->APP_OUTPUTS_Off(m_pCylArray[i]->OJackingPush);
		m_pParent->APP_OUTPUTS_On(m_pCylArray[i]->OJackingReturn);

		if(WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->OJackingReturn,ON,MSG_SHOW_DEFAULT) &&
			WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->OJackingPush,OFF,MSG_SHOW_DEFAULT))
		{
			continue;
		}

		return FALSE;
	}
	return TRUE;
}

//气缸回退
BOOL COnLoadPlate::DamperClose(int nIndex)
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
		
		m_pParent->APP_OUTPUTS_On(m_pCylArray[i]->ODamperReturn);

		if(WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IDamperReturn,ON,MSG_SHOW_DEFAULT) &&
			WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IDamperPush,OFF,MSG_SHOW_DEFAULT))
		{
			continue;
		}

		return FALSE;
	}
	return TRUE;
}


//气缸回退
BOOL COnLoadPlate::DamperOpen(int nIndex)
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

		m_pParent->APP_OUTPUTS_Off(m_pCylArray[i]->ODamperReturn);

		if(WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IDamperReturn,OFF,MSG_SHOW_DEFAULT)
			 &&WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IDamperPush,ON,MSG_SHOW_DEFAULT))
		{
			continue;
		}

		return FALSE;
	}
	return TRUE;
}

BOOL COnLoadPlate::JackingOpen(int nIndex)
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

		m_pParent->APP_OUTPUTS_On(m_pCylArray[i]->OJackingPush);
		m_pParent->APP_OUTPUTS_Off(m_pCylArray[i]->OJackingReturn);

		if(WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->OJackingReturn,OFF,MSG_SHOW_DEFAULT)
			&&WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->OJackingPush,ON,MSG_SHOW_DEFAULT))
		{
			continue;
		}

		return FALSE;
	}
	return TRUE;
}


//增加上料（电芯料盘）部件
BOOL COnLoadPlate::AddOnLoadPlate(CString strIDamperPush,CString strIDamperReturn,CString strIJackingPush,CString strIJackingReturn,
								  CString strIHasPlate,CString strIHasCore,
	                              CString strODamperReturn,CString strOJackingPush,CString strOJackingReturn,
								  CString strIFullCheck)
{
	ASSERT(m_pParent != NULL);
	pTagOnLoadPlateInfo pair = new tagOnLoadPlateInfo();	
	
	pair->strIDamperPush	= strIDamperPush;
	pair->strIDamperReturn	= strIDamperReturn;
	pair->strIJackingPush		= strIJackingPush;
	pair->strIJackingReturn		= strIJackingReturn;	
	pair->strIHasPlate		= strIHasPlate;	
	pair->strIHasCore		= strIHasCore;	
	pair->strIFullCheck		= strIFullCheck;

	pair->strODamperReturn	= strODamperReturn;	
	pair->strOJackingPush		= strOJackingPush;	
	pair->strOJackingReturn		= strOJackingReturn;	

	m_pParent->APP_Add_Input(&(pair->IDamperPush),pair->strIDamperPush);
	m_pParent->APP_Add_Input(&(pair->IDamperReturn),pair->strIDamperReturn);
	m_pParent->APP_Add_Input(&(pair->IJackingPush),pair->strIJackingPush);
	m_pParent->APP_Add_Input(&(pair->IJackingReturn),pair->strIJackingReturn);
	m_pParent->APP_Add_Input(&(pair->IHasPlate),pair->strIHasPlate);
	m_pParent->APP_Add_Input(&(pair->IHasCore),pair->strIHasCore);
	m_pParent->APP_Add_Input(&(pair->IFullCheck),pair->strIFullCheck);

	m_pParent->APP_Add_Output(&(pair->ODamperReturn),pair->strODamperReturn);
	m_pParent->APP_Add_Output(&(pair->OJackingPush),pair->strOJackingPush);
	m_pParent->APP_Add_Output(&(pair->OJackingReturn),pair->strOJackingReturn);
	m_pCylArray.Add(pair);

	return TRUE;
}

//设置父句柄
void COnLoadPlate::SetParent(CRunProcess *pParent)
{
	m_pParent = pParent;

}
int COnLoadPlate::GetOnLoadPlateCount()
{
	return m_pCylArray.GetSize();
}
//气缸回退
BOOL COnLoadPlate::JackingReturn(int nIndex)
{
	return JackingClose(nIndex);
}

BOOL COnLoadPlate::JackingPush(int nIndex)
{
	return JackingOpen(nIndex);
}


BOOL COnLoadPlate::DamperReturn(int nIndex)
{
	return DamperClose(nIndex);
}

BOOL COnLoadPlate::DamperPush(int nIndex)
{
	return DamperOpen(nIndex);
}

//判断是否有电芯
BOOL COnLoadPlate::HasCore(int nIndex)
{
	ASSERT(m_pParent != NULL);
	if(IsNoHardware())
	{
		return TRUE;
	}
	return m_pParent->APP_CheckInput(m_pCylArray[nIndex]->IHasCore,ON);	
}

//判断是否有电芯
BOOL COnLoadPlate::HasPlate(int nIndex)
{
	ASSERT(m_pParent != NULL);
	if(IsNoHardware())
	{
		return TRUE;
	}
	return m_pParent->APP_CheckInput(m_pCylArray[nIndex]->IHasPlate,ON);	
}

BOOL COnLoadPlate::IsMaterialFull(int nIndex)
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
		if(m_pCylArray[i]->IFullCheck != -1)
		{
			if(m_pParent->APP_INPUTS_IsOn(m_pCylArray[i]->IFullCheck))
			{
				return TRUE;
			}
		}
		
	}
	return FALSE;
}

BOOL COnLoadPlate::HasMaterial_IsOn(int nIndex)
{
	ASSERT(m_pParent != NULL);

	if(IsNoHardware())
	{
		return TRUE;
	}

	return m_pParent->APP_INPUTS_IsOn(m_pCylArray[nIndex]->IHasPlate);	
}

BOOL COnLoadPlate::HasMaterial_IsOff(int nIndex)
{
	ASSERT(m_pParent != NULL);

	if(IsNoHardware())
	{
		return TRUE;
	}

	return m_pParent->APP_INPUTS_IsOff(m_pCylArray[nIndex]->IHasPlate);	
}


