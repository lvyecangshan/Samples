#include "StdAfx.h"
#include "OnOrOffLoadPlate.h"


COnOrOffLoadPlate::COnOrOffLoadPlate(CRunProcess *pParent)
{
	m_pParent = pParent;
}

COnOrOffLoadPlate::COnOrOffLoadPlate()
{
	m_pParent = NULL;
}


COnOrOffLoadPlate::~COnOrOffLoadPlate(void)
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
BOOL COnOrOffLoadPlate::JackingClose(int nIndex)
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
BOOL COnOrOffLoadPlate::DamperClose(int nIndex)
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
BOOL COnOrOffLoadPlate::DamperOpen(int nIndex)
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

		if(WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IDamperReturn,OFF,MSG_SHOW_DEFAULT) &&
			WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IDamperPush,ON,MSG_SHOW_DEFAULT))
		{
			continue;
		}

		return FALSE;
	}
	return TRUE;
}





BOOL COnOrOffLoadPlate::JackingOpen(int nIndex)
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

		if(WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->OJackingReturn,OFF,MSG_SHOW_DEFAULT) &&
			WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->OJackingPush,ON,MSG_SHOW_DEFAULT))
		{
			continue;
		}

		return FALSE;
	}
	return TRUE;
}


//增加上料（电芯料盘）部件
BOOL COnOrOffLoadPlate::AddOnOrOffLoadPlate(CString strIDamperPush,CString strIDamperReturn,CString strIJackingPush,CString strIJackingReturn,
								  CString strIHasPlate00,CString strIHasCore00,CString strICheckDirection00,
	                              CString strODamperReturn,CString strOJackingPush,CString strOJackingReturn,
								  CString strIHasPlate01,CString strIHasCore01,CString strICheckDirection01,
								  CString strIFullCheck0,CString strIFullCheck1)
{
	ASSERT(m_pParent != NULL);
	pTagOnOrOffLoadPlateInfo pair = new tagOnOrOffLoadPlateInfo();	
	
	pair->strIDamperPush	= strIDamperPush;
	pair->strIDamperReturn	= strIDamperReturn;
	pair->strIJackingPush	= strIJackingPush;
	pair->strIJackingReturn	= strIJackingReturn;	
	pair->strIHasPlate00	= strIHasPlate00;	
	pair->strIHasCore00		= strIHasCore00;
	pair->strICheckDirection00    = strICheckDirection00;
	pair->strIHasPlate01	= strIHasPlate01 ;	
	pair->strIHasCore01		= strIHasCore01;
	pair->strICheckDirection01	= strICheckDirection01;
	pair->strIFullCheck0				= strIFullCheck0;
	pair->strIFullCheck1				= strIFullCheck1;
	

	pair->strODamperReturn	= strODamperReturn;	
	pair->strOJackingPush		= strOJackingPush;	
	pair->strOJackingReturn		= strOJackingReturn;	

	m_pParent->APP_Add_Input(&(pair->IDamperPush),pair->strIDamperPush);
	m_pParent->APP_Add_Input(&(pair->IDamperReturn),pair->strIDamperReturn);
	m_pParent->APP_Add_Input(&(pair->IJackingPush),pair->strIJackingPush);
	m_pParent->APP_Add_Input(&(pair->IJackingReturn),pair->strIJackingReturn);
	m_pParent->APP_Add_Input(&(pair->IHasPlate00),pair->strIHasPlate00);
	m_pParent->APP_Add_Input(&(pair->IHasCore00),pair->strIHasCore00);
	m_pParent->APP_Add_Input(&(pair->ICheckDirection00),pair->strICheckDirection00);

	
	m_pParent->APP_Add_Input(&(pair->IHasPlate01),pair->strIHasPlate01);
	m_pParent->APP_Add_Input(&(pair->IHasCore01),pair->strIHasCore01);
	m_pParent->APP_Add_Input(&(pair->ICheckDirection01),pair->strICheckDirection01);
	m_pParent->APP_Add_Input(&(pair->IFullCheck0),pair->strIFullCheck0);
	m_pParent->APP_Add_Input(&(pair->IFullCheck1),pair->strIFullCheck1);
	
	m_pParent->APP_Add_Output(&(pair->ODamperReturn),pair->strODamperReturn);
	m_pParent->APP_Add_Output(&(pair->OJackingPush),pair->strOJackingPush);
	m_pParent->APP_Add_Output(&(pair->OJackingReturn),pair->strOJackingReturn);
	m_pCylArray.Add(pair);

	return TRUE;
}

//设置父句柄
void COnOrOffLoadPlate::SetParent(CRunProcess *pParent)
{
	m_pParent = pParent;

}
int COnOrOffLoadPlate::GetOnOrOffLoadPlateCount()
{
	return m_pCylArray.GetSize();
}
//气缸回退
BOOL COnOrOffLoadPlate::JackingReturn(int nIndex)
{
	return JackingClose(nIndex);
}

BOOL COnOrOffLoadPlate::JackingPush(int nIndex)
{
	return JackingOpen(nIndex);
}


BOOL COnOrOffLoadPlate::DamperReturn(int nIndex)
{
	return DamperClose(nIndex);
}

BOOL COnOrOffLoadPlate::DamperPush(int nIndex)
{
	return DamperOpen(nIndex);
}

//判断是否有电芯
BOOL COnOrOffLoadPlate::HasCore(int nIndex)
{
	ASSERT(m_pParent != NULL);
	if(IsNoHardware())
	{
		return TRUE;
	}
	int nSatr = 0;
	int nEnd = m_pCylArray.GetSize();

	if(nIndex != -1)
	{
		nSatr = nIndex;
		nEnd = nIndex;
	}
	BOOL bHas = TRUE;
	for(int i = nSatr; i < nEnd; i ++)
	{
		if(m_pCylArray[nIndex]->IHasCore01 == -1)
		{
			if(m_pParent->APP_CheckInput(m_pCylArray[nIndex]->IHasCore00,OFF))
			{
				bHas = FALSE;
				break;
			}
		}
		else
		{
			if(m_pParent->APP_CheckInput(m_pCylArray[nIndex]->IHasCore00,OFF) || m_pParent->APP_CheckInput(m_pCylArray[nIndex]->IHasCore01,OFF))
			{
				bHas = FALSE;
				break;
			}
		}
	}
	return bHas;
	

}

BOOL COnOrOffLoadPlate::NoCore(int nIndex)
{
	ASSERT(m_pParent != NULL);
	if(IsNoHardware())
	{
		return TRUE;
	}
	if(m_pCylArray[nIndex]->IHasCore01 == -1)
	{
		return m_pParent->APP_CheckInput(m_pCylArray[nIndex]->IHasCore00,OFF);
	}
	else
	{
		return m_pParent->APP_CheckInput(m_pCylArray[nIndex]->IHasCore00,OFF) && m_pParent->APP_CheckInput(m_pCylArray[nIndex]->IHasCore01,OFF);
	}		
}


//得到料盘个数
int COnOrOffLoadPlate::GetPlateCount(int nIndex)
{
	if(m_pCylArray[nIndex]->IHasCore01 == -1)
	{
		return 1;
	}
	return 2;
}

//判断是否有电芯
BOOL COnOrOffLoadPlate::HasPlate(int nIndex)
{
	ASSERT(m_pParent != NULL);
	if(IsNoHardware())
	{
		return TRUE;
	}
	if(m_pCylArray[nIndex]->IHasPlate01 == -1)
	{
		return m_pParent->APP_CheckInput(m_pCylArray[nIndex]->IHasPlate00,ON);
	}
	else
	{
		return m_pParent->APP_CheckInput(m_pCylArray[nIndex]->IHasPlate00,ON) && m_pParent->APP_CheckInput(m_pCylArray[nIndex]->IHasPlate01,ON);
	}
		
}

//得到当前料盘个数
int COnOrOffLoadPlate::GetCurPlateCount(int nIndex)
{
	int nPlate = 0;
	if(m_pCylArray[nIndex]->IHasPlate01 == -1)
	{
		if(m_pParent->APP_CheckInput(m_pCylArray[nIndex]->IHasPlate00,ON))
		{
			nPlate ++;
		}
	}
	else
	{
		if(m_pParent->APP_CheckInput(m_pCylArray[nIndex]->IHasPlate00,ON))
		{
			nPlate ++;
		}
		if(m_pParent->APP_CheckInput(m_pCylArray[nIndex]->IHasPlate01,ON))
		{
			nPlate ++;
		}

	}
	return nPlate;

}

BOOL COnOrOffLoadPlate::NoPlate(int nIndex)
{
	ASSERT(m_pParent != NULL);
	if(IsNoHardware())
	{
		return TRUE;
	}
	if(m_pCylArray[nIndex]->IHasPlate01 == -1)
	{
		return m_pParent->APP_CheckInput(m_pCylArray[nIndex]->IHasPlate00,OFF);
	}
	else
	{
		return m_pParent->APP_CheckInput(m_pCylArray[nIndex]->IHasPlate00,OFF) && m_pParent->APP_CheckInput(m_pCylArray[nIndex]->IHasPlate01,OFF);
	}

}

BOOL COnOrOffLoadPlate::IsMaterialFull(int nIndex)
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
		if(i == 0 && m_pCylArray[i]->IFullCheck0 != -1)
		{
			if(m_pParent->APP_INPUTS_IsOn(m_pCylArray[i]->IFullCheck0))
			{
				return TRUE;
			}
		}

		if(i == 1 && m_pCylArray[i]->IFullCheck1 != -1)
		{
			if(m_pParent->APP_INPUTS_IsOn(m_pCylArray[i]->IFullCheck1))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}


