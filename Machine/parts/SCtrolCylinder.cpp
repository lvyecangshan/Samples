#include "StdAfx.h"
#include "SCtrolCylinder.h"


SCtrolCylinder::SCtrolCylinder(CRunProcess *pParent)
{
	m_pParent = pParent;
}

SCtrolCylinder::SCtrolCylinder()
{
	m_pParent = NULL;
}


SCtrolCylinder::~SCtrolCylinder(void)
{
	int nSize = m_pSCtrolCylArray.GetSize();
	for(int i = 0; i < nSize; i ++)
	{
		if(m_pSCtrolCylArray[i] != NULL)
		{
			delete m_pSCtrolCylArray[i];
			m_pSCtrolCylArray[i] = NULL;
		}
	}
	m_pSCtrolCylArray.RemoveAll();
}

//气缸回退
BOOL SCtrolCylinder::Close(int nIndex)
{
	ASSERT(m_pParent != NULL);
	if(IsNoHardware())
	{
		return TRUE;
	}
	int nStart = 0;
	int nEnd = m_pSCtrolCylArray.GetSize();
	if(nIndex != -1)
	{
		nStart = nIndex;
		nEnd = nIndex;
	}
	for(int i = nStart; i < nEnd; i ++)
	{
		
		m_pParent->APP_OUTPUTS_Off(m_pSCtrolCylArray[i]->OCylinderCylOpen);
		if(m_pParent->m_bDryRun)
		{
			m_pParent->APP_Wait_Signal_Sleep();
			continue;
		}
		if(WAIT_OK == m_pParent->APP_WaitInput(m_pSCtrolCylArray[i]->ICylinderCylClose,ON,MSG_SHOW_DEFAULT)
		    &&WAIT_OK == m_pParent->APP_WaitInput(m_pSCtrolCylArray[i]->ICylinderCylOpen,OFF,MSG_SHOW_DEFAULT))
		{
			continue;
		}
		return FALSE;
	}
	return TRUE;
}
//气缸打开
BOOL SCtrolCylinder::Open(int nIndex)
{
	ASSERT(m_pParent != NULL);
	if(IsNoHardware())
	{
		return TRUE;
	}
	int nStart = 0;
	int nEnd = m_pSCtrolCylArray.GetSize();
	if(nIndex != -1)
	{
		nStart = nIndex;
		nEnd = nIndex;
	}
	for(int i = nStart; i < nEnd; i ++)
	{
		m_pParent->APP_OUTPUTS_On(m_pSCtrolCylArray[i]->OCylinderCylOpen);
		if(m_pParent->m_bDryRun)
		{
			m_pParent->APP_Wait_Signal_Sleep();
			continue;
		}
		if(WAIT_OK == m_pParent->APP_WaitInput(m_pSCtrolCylArray[i]->ICylinderCylClose,OFF,MSG_SHOW_DEFAULT) &&
			WAIT_OK == m_pParent->APP_WaitInput(m_pSCtrolCylArray[i]->ICylinderCylOpen,ON,MSG_SHOW_DEFAULT))
		{
			continue;
		}
		return FALSE;
	}
	return TRUE;
}

//增加气缸
BOOL SCtrolCylinder::AddSCtrolCylinder(CString strICylinderCylOpen,CString strICylinderCylClose,
						CString strOCylinderCylOpen)
{
	ASSERT(m_pParent != NULL);

	pTagSCtrolCylinderInfo pair = new tagSCtrolCylinderInfo();

	pair->ICylinderCylOpen	 = -1;
	pair->ICylinderCylClose	 = -1;
	pair->OCylinderCylOpen	 = -1;
	
	pair->strICylinderCylOpen		= strICylinderCylOpen;
	pair->strICylinderCylClose	= strICylinderCylClose;
	pair->strOCylinderCylOpen		= strOCylinderCylOpen;
	

	m_pParent->APP_Add_Input(&(pair->ICylinderCylOpen),pair->strICylinderCylOpen);
	m_pParent->APP_Add_Input(&(pair->ICylinderCylClose),pair->strICylinderCylClose);

	m_pParent->APP_Add_Output(&(pair->OCylinderCylOpen),pair->strOCylinderCylOpen);

	m_pSCtrolCylArray.Add(pair);

	return TRUE;
}

//设置父句柄
void SCtrolCylinder::SetParent(CRunProcess *pParent)
{
	m_pParent = pParent;
}
int SCtrolCylinder::GetCylinderCount()
{
	return m_pSCtrolCylArray.GetSize();
}

//气缸推出
BOOL SCtrolCylinder::SCtrolCyPush(int nIndex)
{
	return Open(nIndex);
}
//气缸回退
BOOL SCtrolCylinder::SCtrolCyReturn(int nIndex)
{
	return Close(nIndex);
}

//气缸顶升
BOOL SCtrolCylinder::SCtrolCyUp(int nIndex)
{
	return Open(nIndex);
}
//气缸下降
BOOL SCtrolCylinder::SCtrolCyDown(int nIndex)
{
	return Close(nIndex);
}

//气缸向右(open)
BOOL SCtrolCylinder::SCtrolCyRight(int nIndex)
{
	return Open(nIndex);
}
//气缸向左(close)
BOOL SCtrolCylinder::SCtrolCyLeft(int nIndex)
{
	return Close(nIndex);
}
