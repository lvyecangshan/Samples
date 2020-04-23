#include "StdAfx.h"
#include "Damper.h"


CDamper::CDamper(CRunProcess *pParent)
{
	m_pParent = pParent;
}

CDamper::CDamper()
{
	m_pParent = NULL;
}


CDamper::~CDamper(void)
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
BOOL CDamper::Close(int nIndex)
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
		m_pParent->APP_OUTPUTS_On(m_pCylArray[i]->ODamperCylReturn);
		if(WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IDamperCylReturn,ON,MSG_SHOW_DEFAULT) &&
			WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IDamperCylPush,OFF,MSG_SHOW_DEFAULT))
		{
			continue;
		}
			
		return FALSE;
	}
	return TRUE;
}

BOOL CDamper::Open(int nIndex)
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
		m_pParent->APP_OUTPUTS_Off(m_pCylArray[i]->ODamperCylReturn);
		if(WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IDamperCylReturn,OFF,MSG_SHOW_DEFAULT) &&
			WAIT_OK == m_pParent->APP_WaitInput(m_pCylArray[i]->IDamperCylPush,ON,MSG_SHOW_DEFAULT))
		{
			continue;
		}

		return FALSE;
	}
	return TRUE;
}


//��������
BOOL CDamper::AddDamper(CString strIDamperCylPush,CString strIDamperCylReturn,
						CString strODamperCylReturn,CString strIHasMaterial,CString strICheckDirection,
						CString strIFullCheck0,CString strIFullCheck1)
{
	ASSERT(m_pParent != NULL);

	pTagDamperInfo pair = new tagDamperInfo();
	
	
	pair->strIDamperCylPush	= strIDamperCylPush;
	pair->strIDamperCylReturn	= strIDamperCylReturn;
	pair->strODamperCylReturn	= strODamperCylReturn;
	pair->strIHasMaterial			= strIHasMaterial;	
	pair->strICheckDirection		= strICheckDirection;	
	pair->strIFullCheck0				= strIFullCheck0;
	pair->strIFullCheck1				= strIFullCheck1;

	m_pParent->APP_Add_Input(&(pair->IDamperCylPush),pair->strIDamperCylPush);
	m_pParent->APP_Add_Input(&(pair->IDamperCylReturn),pair->strIDamperCylReturn);
	m_pParent->APP_Add_Input(&(pair->IHasMaterial),pair->strIHasMaterial);
	m_pParent->APP_Add_Input(&(pair->ICheckDirection),pair->strICheckDirection);
	m_pParent->APP_Add_Input(&(pair->ICheckDirection),pair->strICheckDirection);
	m_pParent->APP_Add_Input(&(pair->IFullCheck0),pair->strIFullCheck0);
	m_pParent->APP_Add_Input(&(pair->IFullCheck1),pair->strIFullCheck1);

	m_pParent->APP_Add_Output(&(pair->ODamperCylReturn),pair->strODamperCylReturn);
	
	m_pCylArray.Add(pair);

	return TRUE;
}

//���ø����
void CDamper::SetParent(CRunProcess *pParent)
{
	m_pParent = pParent;

}
int CDamper::GetDamperCount()
{
	return m_pCylArray.GetSize();
}
//���׻���
BOOL CDamper::Return(int nIndex)
{
	return Close(nIndex);
}

BOOL CDamper::Push(int nIndex)
{
	return Open(nIndex);
}

//�ж��Ƿ�����
BOOL CDamper::HasMaterial(int nIndex)
{
	ASSERT(m_pParent != NULL);
	if(IsNoHardware())
	{
		return TRUE;
	}
	return m_pParent->APP_CheckInput(m_pCylArray[nIndex]->IHasMaterial,ON);	
}
//�Ƿ�����
BOOL CDamper::IsMaterialFull(int nIndex)
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
		if(i==0 && m_pCylArray[i]->IFullCheck0 != -1)
		{
			if(m_pParent->APP_INPUTS_IsOn(m_pCylArray[i]->IFullCheck0))
			{
				return TRUE;
			}
		}

		if(i==1 && m_pCylArray[i]->IFullCheck1 != -1)
		{
			if(m_pParent->APP_INPUTS_IsOn(m_pCylArray[i]->IFullCheck1))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

