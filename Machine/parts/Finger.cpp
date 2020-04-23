#include "StdAfx.h"
#include "Finger.h"


CFinger::CFinger(CRunProcess *pParent)
{
	m_pParent = pParent;
}

CFinger::CFinger()
{
	m_pParent = NULL;
}


CFinger::~CFinger(void)
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
BOOL CFinger::Close(int nIndex)
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
		m_pParent->APP_OUTPUTS_On(m_pCylArray[i]->OFingerCylClose);
		m_pParent->APP_OUTPUTS_Off(m_pCylArray[i]->OFingerCylOpen);

		if(m_pParent->m_bDryRun)
		{
			m_pParent->APP_Wait_Signal_Sleep();
			continue;
		}
		if(WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IFingerCylClose,ON,MSG_SHOW_DEFAULT) &&
		   WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IFingerCylOpen,OFF,MSG_SHOW_DEFAULT))
		{
			continue;
		}
		return FALSE;
	}
	return TRUE;
}
//夹爪打开
BOOL CFinger::Open(int nIndex)
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
		m_pParent->APP_OUTPUTS_Off(m_pCylArray[i]->OFingerCylClose);
		m_pParent->APP_OUTPUTS_On(m_pCylArray[i]->OFingerCylOpen);

		if(m_pParent->m_bDryRun)
		{
			m_pParent->APP_Wait_Signal_Sleep();
			continue;
		}
		if(WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IFingerCylClose,OFF,MSG_SHOW_DEFAULT) &&
			WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IFingerCylOpen,ON,MSG_SHOW_DEFAULT))
		{
			continue;
		}
		return FALSE;
	}
	return TRUE;
}

BOOL CFinger::HasMaterial(int nIndex)
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
		if(m_pCylArray[i]->IHasMaterial == -1)
		{
			continue;
		}
		if(m_pParent->APP_INPUTS_IsOff(m_pCylArray[i]->IHasMaterial))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CFinger::HasNoMaterial(int nIndex)
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
		if(m_pCylArray[i]->IHasMaterial == -1)
		{
			continue;
		}
		if(m_pParent->APP_INPUTS_IsOn(m_pCylArray[i]->IHasMaterial))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CFinger::CheckHasCore(int nIndex)
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
		if(m_pCylArray[i]->IHasMaterial == -1)
		{
			continue;
		}
		if(m_pParent->APP_CheckInput(m_pCylArray[i]->IHasMaterial,OFF))
		{
			return FALSE;
		}
	}
	return TRUE;
}

//是否压料
BOOL CFinger::HasPressMaterial(int nIndex)
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
		if(m_pCylArray[i]->IPress == -1)
		{
			continue;
		}
		if(m_pParent->APP_INPUTS_IsOn(m_pCylArray[i]->IPress))
		{
			return TRUE;
		}
	}
	return FALSE;
}
//增加夹抓气缸
BOOL CFinger::AddFinger(CString strIFingerCylOpen,CString strIFingerCylClose,
						CString strOFingerCylOpen,CString strOFingerCylClose,
						CString strIHasMaterial,CString strIPress)
{
	ASSERT(m_pParent != NULL);

	pTagFingerInfo pair = new tagFingerInfo();
	pair->IFingerCylOpen	 = -1;
	pair->IFingerCylClose	 = -1;
	pair->OFingerCylOpen	 = -1;
	pair->OFingerCylClose	 = -1;
	pair->IPress			 = -1;
	pair->IHasMaterial		 = -1;
	pair->strIFingerCylOpen		= strIFingerCylOpen;
	pair->strIFingerCylClose	= strIFingerCylClose;
	pair->strOFingerCylOpen		= strOFingerCylOpen;
	pair->strOFingerCylClose	= strOFingerCylClose;	
	pair->strIHasMaterial		= strIHasMaterial;
	pair->strIPress				= strIPress;

	m_pParent->APP_Add_Input(&(pair->IFingerCylOpen),pair->strIFingerCylOpen);
	m_pParent->APP_Add_Input(&(pair->IFingerCylClose),pair->strIFingerCylClose);

	m_pParent->APP_Add_Input(&(pair->IHasMaterial),pair->strIHasMaterial);
	m_pParent->APP_Add_Input(&(pair->IPress),pair->strIPress);

	m_pParent->APP_Add_Output(&(pair->OFingerCylOpen),pair->strOFingerCylOpen);
	m_pParent->APP_Add_Output(&(pair->OFingerCylClose),pair->strOFingerCylClose);

	m_pCylArray.Add(pair);

	return TRUE;
}

//设置父句柄
void CFinger::SetParent(CRunProcess *pParent)
{
	m_pParent = pParent;

}
int CFinger::GetFingerCount()
{
	return m_pCylArray.GetSize();
}

void CFinger::RunWhile()
{
	m_pParent->Anticollision();
}