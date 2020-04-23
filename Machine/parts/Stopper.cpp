#include "StdAfx.h"
#include "Stopper.h"


CStopper::CStopper(CRunProcess *pParent)
{
	m_pParent = pParent;
}

CStopper::CStopper()
{
	m_pParent = NULL;
}


CStopper::~CStopper(void)
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
BOOL CStopper::Down(int nIndex)
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
		m_pParent->APP_OUTPUTS_On(m_pCylArray[i]->OStopperCylReturn);

		if(WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IStopperCylReturn,ON,MSG_SHOW_DEFAULT) &&
			WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IStopperCylPush,OFF,MSG_SHOW_DEFAULT))
		{
			continue;
		}

		return FALSE;
	}
	return TRUE;
}

BOOL CStopper::Up(int nIndex)
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
		m_pParent->APP_OUTPUTS_Off(m_pCylArray[i]->OStopperCylReturn);
		if(WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IStopperCylReturn,OFF,MSG_SHOW_DEFAULT) &&
			WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IStopperCylPush,ON,MSG_SHOW_DEFAULT))
		{
			continue;
		}

		return FALSE;
	}
	return TRUE;
}


//增加气缸
BOOL CStopper::AddStopper(CString strIStopperCylPush,CString strIStopperCylReturn,
	CString strOStopperCylReturn,CString strIHasMaterial)
{
	ASSERT(m_pParent != NULL);

	pTagStopperInfo pair = new tagStopperInfo();


	pair->strIStopperCylPush	= strIStopperCylPush;
	pair->strIStopperCylReturn	= strIStopperCylReturn;
	pair->strOStopperCylReturn	= strOStopperCylReturn;

	pair->strIHasMaterial			= strIHasMaterial;	

	m_pParent->APP_Add_Input(&(pair->IStopperCylPush),pair->strIStopperCylPush);
	m_pParent->APP_Add_Input(&(pair->IStopperCylReturn),pair->strIStopperCylReturn);
	m_pParent->APP_Add_Output(&(pair->OStopperCylReturn),pair->strOStopperCylReturn);

	m_pParent->APP_Add_Input(&(pair->IHasMaterial),pair->strIHasMaterial);
	m_pCylArray.Add(pair);

	return TRUE;
}

//设置父句柄
void CStopper::SetParent(CRunProcess *pParent)
{
	m_pParent = pParent;

}
int CStopper::GetStopperCount()
{
	return m_pCylArray.GetSize();
}

BOOL CStopper::CylinderDown(int nIndex)
{
	return Down(nIndex);
}

BOOL CStopper::CylinderUp(int nIndex)
{
	return Up(nIndex);
}

//判断是否有料
BOOL CStopper::HasMaterial(int nIndex)
{
	ASSERT(m_pParent != NULL);
	if(IsNoHardware())
	{
		return TRUE;
	}
	return m_pParent->APP_CheckInput(m_pCylArray[nIndex]->IHasMaterial,ON);	
}

BOOL CStopper::HasMaterial_IsOn(int nIndex)
{
	ASSERT(m_pParent != NULL);

	if(IsNoHardware())
	{
		return TRUE;
	}
	return m_pParent->APP_INPUTS_IsOn(m_pCylArray[nIndex]->IHasMaterial);	
}

BOOL CStopper::HasMaterial_IsOff(int nIndex)
{
	ASSERT(m_pParent != NULL);

	if(IsNoHardware())
	{
		return TRUE;
	}
	return m_pParent->APP_INPUTS_IsOff(m_pCylArray[nIndex]->IHasMaterial);	
}