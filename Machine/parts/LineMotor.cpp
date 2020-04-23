
#include "StdAfx.h"
#include "LineMotor.h"


CLineMotor::CLineMotor(CRunProcess *pParent)
{
	m_nTime = 0;
	m_pParent = pParent;
	bResult = FALSE;
}

CLineMotor::CLineMotor()
{
	m_nTime = 0;
	m_pParent = NULL;
	bResult = FALSE;
}


CLineMotor::~CLineMotor(void)
{
	int nSize = m_pLeMotorArray.GetSize();
	for(int i = 0; i < nSize; i ++)
	{
		if(m_pLeMotorArray[i] != NULL)
		{
			delete m_pLeMotorArray[i];
			m_pLeMotorArray[i] = NULL;
		}
	}
	m_pLeMotorArray.RemoveAll();

}

//���ӵ��
BOOL CLineMotor::AddMotor(CString strOLinemotor0,CString strOLinemotor1/* = _T("")*/,CString strOLinemotor2/* = _T("")*/)
{
	ASSERT(m_pParent != NULL);

	ptagLineMotorInfo pair = new _tagLineMotorInfo();

	pair->strOLinemotor[0]	 = strOLinemotor0;
	pair->strOLinemotor[1]	 = strOLinemotor1;
	pair->strOLinemotor[2]	 = strOLinemotor2;

	for (int i=0;i<AllMOMTOR;i++) 
	{
		m_pParent->APP_Add_Output(&(pair->m_nOLinemotor[i]),pair->strOLinemotor[i]);
	}

	m_pLeMotorArray.Add(pair);

	return TRUE;
}

//���ø����
void CLineMotor::SetParent(CRunProcess *pParent)
{
	m_pParent = pParent;

}
int CLineMotor::GetCylinderCount()
{
	return m_pLeMotorArray.GetSize();
}

//�����������
BOOL CLineMotor::MotorStart(int nIndex)
{
	if (m_pLeMotorArray[nIndex]->m_nOLinemotor < 0)
	{
		return FALSE;
	}
	return Start(nIndex);
}

//ȫ���������
BOOL CLineMotor::AllMotorStart()
{
	return AllStart();
}

//�������ֹͣ
BOOL CLineMotor::MotorStop(int nIndex)
{
	if (m_pLeMotorArray[nIndex]->m_nOLinemotor < 0)
	{
		return FALSE;
	}

	return Stop(nIndex);
}

//ȫ�����ֹͣ
BOOL CLineMotor::AllMotorStop()
{
	return AllStop();
}

BOOL CLineMotor::Start(int nIndex)
{
	ASSERT(m_pParent != NULL);
	if(IsNoHardware())
	{
		return TRUE;
	}

	int Side = m_pLeMotorArray.GetSize();

	if(Side == 0)
	{
		CString strModule;
		strModule.Format(_T("%s��������:λ�á�LineMotor.cpp��"),m_pParent->m_strName);
		AfxMessageBox(strModule);
		return FALSE;
	}
	
	if (m_pLeMotorArray[0]->m_nOLinemotor[nIndex] > -1 && m_pParent->APP_OUTPUTS_IsOff(m_pLeMotorArray[0]->m_nOLinemotor[nIndex]))
	{
		m_pParent->APP_OUTPUTS_On(m_pLeMotorArray[0]->m_nOLinemotor[nIndex]);
	}

	return TRUE;
}

BOOL CLineMotor::AllStart()
{
	ASSERT(m_pParent != NULL);

	if(IsNoHardware())
	{
		return TRUE;
	}

	int Side = m_pLeMotorArray.GetSize();

	if(Side == 0)
	{
		CString strModule;
		strModule.Format(_T("%s��������:λ�á�LineMotor.cpp��"),m_pParent->m_strName);
		AfxMessageBox(strModule);
		return FALSE;
	}

	for(int i = 0; i < AllMOMTOR; i ++)
	{
		if (m_pLeMotorArray[0]->m_nOLinemotor[i] > -1 && m_pParent->APP_OUTPUTS_IsOff(m_pLeMotorArray[0]->m_nOLinemotor[i]))
		{
			m_pParent->APP_OUTPUTS_On(m_pLeMotorArray[0]->m_nOLinemotor[i]);
		}
	} 

	return TRUE;
}

BOOL CLineMotor::Stop(int nIndex)
{
	ASSERT(m_pParent != NULL);
	if(IsNoHardware())
	{
		return TRUE;
	}

	int Side = m_pLeMotorArray.GetSize();

	if(Side == 0)
	{
		CString strModule;
		strModule.Format(_T("%s��������:λ�á�LineMotor.cpp��"),m_pParent->m_strName);
		AfxMessageBox(strModule);
		return FALSE;
	}

	if (m_pLeMotorArray[0]->m_nOLinemotor[nIndex] > -1 && m_pParent->APP_OUTPUTS_IsOn(m_pLeMotorArray[0]->m_nOLinemotor[nIndex]))
	{
		m_pParent->APP_OUTPUTS_Off(m_pLeMotorArray[0]->m_nOLinemotor[nIndex]);
	}

	return TRUE;
}

BOOL CLineMotor::AllStop()
{
	ASSERT(m_pParent != NULL);

	if(IsNoHardware())
	{
		return TRUE;
	}

	int Side = m_pLeMotorArray.GetSize();

	if(Side == 0)
	{
		CString strModule;
		strModule.Format(_T("%s��������:λ�á�LineMotor.cpp��"),m_pParent->m_strName);
		AfxMessageBox(strModule);
		return FALSE;
	}

	for(int i = 0; i < AllMOMTOR; i ++)
	{
		if (m_pLeMotorArray[0]->m_nOLinemotor[i] > -1 && m_pParent->APP_OUTPUTS_IsOn(m_pLeMotorArray[0]->m_nOLinemotor[i]))
		{
			m_pParent->APP_OUTPUTS_Off(m_pLeMotorArray[0]->m_nOLinemotor[i]);
		}
	} 

	return TRUE;
}

BOOL CLineMotor::CountTime()
{
	if (!bResult)
	{
		m_nTime = GetTickCount();
		bResult = TRUE;
	}

	if (bResult)
	{
		if (GetTickCount() - m_nTime > 5000)
		{
			bResult = FALSE;
			return TRUE;
		}
	}

	return FALSE;
}

void CLineMotor::PowerUpRestart(void)
{
	m_nTime = 0;
	bResult = FALSE;
}

UINT CLineMotor::GetMachinState()
{
	int nMcStatus = -1;

	if (MACHINE.m_pRunCtrl != NULL)
	{
		nMcStatus = MACHINE.m_pRunCtrl->GetMcState();
	}

	if (nMcStatus == MC_STOP_RUN || nMcStatus == MC_INITIALIZED)
	{
		nMcStatus = MC_STOP_RUN;
	}
	else if (nMcStatus == MC_RUNNING || nMcStatus == MC_INITIALIZING)
	{
		nMcStatus = MC_RUNNING;
	}

	return nMcStatus;
}

void CLineMotor::RunWhile()
{
	//if (m_pParent->IsModuleEnable())  //������ʹ�òŶ�
	{
		m_pParent->MotorRun();
	}
}