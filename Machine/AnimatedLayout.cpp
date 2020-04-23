// AnimatedLayout.cpp : implementation file
//

#include "stdafx.h"
#include "Machine.h"
#include "AnimatedLayout.h"
// CAnimatedLayout


IMPLEMENT_DYNAMIC(CAnimatedLayout, CStatic)

	CAnimatedLayout::CAnimatedLayout()
{
}

CAnimatedLayout::~CAnimatedLayout()
{
}


BEGIN_MESSAGE_MAP(CAnimatedLayout, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CAnimatedLayout message handlers

void CAnimatedLayout::OnPaint()
{
	CPaintDC dc(this);

	CRect rcClient, rcArea;
	CBitmap bitmap;
	CDC memDC,*pDC = NULL;

	pDC = &dc;

	GetClientRect(rcClient);

	memDC.CreateCompatibleDC(pDC);

	bitmap.CreateCompatibleBitmap(pDC,rcClient.Width(),rcClient.Height());

	memDC.SelectObject(bitmap);

	memDC.FillSolidRect(rcClient,RGB(255,255,255));
	CFont safefont, *pSafeOldFont;
	VERIFY(safefont.CreateFont(30, 8, 0, 0,  FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("微软雅黑")));
	pSafeOldFont = memDC.SelectObject(&safefont);
	CBrush BSafeDoorBrush;
	CBrush BRedBrush(RGB(255,0,0));
	CBrush BGreenBrush(RGB(0,255,0));
	CString strMsg;
	strMsg = MACHINE.m_bUseSafeDoor? _T("安全门处于开启状态") :_T("安全门处于屏蔽状态");
	CRect  DoorRect;
	DoorRect.SetRect(rcClient.left + 20,rcClient.top +20, rcClient.left + 180, rcClient.top +80);
	if (MACHINE.m_bUseSafeDoor)
	{
		memDC.SelectObject(&BGreenBrush);
	}
	else
	{
		memDC.SelectObject(&BRedBrush);
	}
	memDC.Rectangle(DoorRect);
	memDC.DrawText(strMsg, DoorRect, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	BGreenBrush.DeleteObject();
	BRedBrush.DeleteObject();
	safefont.DeleteObject();
	pSafeOldFont = NULL;
	// TODO: add your code
	//堆栈
	CString strModuleName[3] = {_T("上料堆栈1"), _T("下料堆栈"), _T("上料堆栈2")};
	CRunProcess* pRun[3] = {MACHINE.m_pOnloadShellStack[0], MACHINE.m_pOffloadShellStack, MACHINE.m_pOnloadShellStack[1]};

	rcArea.left = rcClient.left + 30;
	rcArea.top = rcClient.top + rcClient.Height()/4;
	rcArea.right = rcClient.Width()/6;
	rcArea.bottom = rcClient.bottom - rcClient.Height()/4;

	DrawStack(memDC, rcArea, strModuleName, pRun);

	//清洗
	strModuleName[0] = _T("清洗前");
	strModuleName[1] = _T("清洗");
	strModuleName[2] = _T("清洗后");
	pRun[0] = MACHINE.m_pCleanBefore;
	pRun[1] = MACHINE.m_pCleanShell;
	pRun[2] = MACHINE.m_pCleanAfter;

	rcArea.left = rcClient.Width()/6 + 80;
	rcArea.top = rcClient.top + rcClient.Height()/3;
	rcArea.right = rcArea.left + rcClient.Width()/12;
	rcArea.bottom = rcArea.top + rcClient.Height()/4;

	DrawStack(memDC, rcArea, strModuleName, pRun);


	// 电芯入壳
	rcArea.left = rcClient.Width() * 2/6 + 60;
	rcArea.right = rcArea.left + rcClient.Width() /5;
	rcArea.top = rcClient.top + rcClient.Height()*4/10;
	rcArea.bottom = rcArea.top + rcClient.Height()/4;
	DrawCoreInShell(memDC, rcArea);


	// 抓取电芯
	rcArea.left = rcClient.Width() * 2/6 + 60;
	rcArea.right = rcArea.left + rcClient.Width() /5;
	rcArea.top = rcClient.top + rcClient.Height()/7;
	rcArea.bottom = rcArea.top + rcClient.Height()/5;
	DrawOnloadCore(memDC, rcArea);


	// 取入壳电池
	rcArea.left = rcClient.Width() * 2/6 + 60;
	rcArea.right = rcArea.left + rcClient.Width() /5;
	rcArea.top = rcClient.top + rcClient.Height()*7/10;
	rcArea.bottom = rcArea.top + rcClient.Height()/8;
	DrawBatteryPnp(memDC, rcArea);


	//机器人下料
	rcArea.left = rcClient.right - rcClient.Width()*3/10;
	rcArea.right = rcArea.left + rcClient.Width() /4;
	rcArea.top = rcClient.top + rcClient.Height()*3/10;
	rcArea.bottom = rcArea.top + rcClient.Height()/2;
	DrawRobotOffload(memDC, rcArea);


	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	bitmap.DeleteObject();

	memDC.DeleteDC();

	ReleaseDC(pDC);
}

BOOL CAnimatedLayout::OnEraseBkgnd( CDC* pDC )
{
	return TRUE;
}

void CAnimatedLayout::DrawRobotOffload(CDC& memDC, CRect rcClient)
{
	CString strTemp;
	CRect rcArea;
	CBrush backGroundBrush(RGB(145,160,168)), moduleBrush;

	memDC.FillRect(rcClient, &backGroundBrush);

	rcArea.left = rcClient.left + rcClient.Width()/3;
	rcArea.right = rcArea.left + 100;

	rcArea.top = rcClient.top + rcClient.Height()/2 - 50;
	rcArea.bottom = rcArea.top + 100;

	if(MACHINE.m_pRobotOffload != NULL)
	{
		BOOL bEnableRobot = MACHINE.m_pRobotOffload->m_bEnableRobot;

		if(bEnableRobot)
		{
			moduleBrush.CreateSolidBrush(RGB(0, 255, 185));
		}
		else
		{
			moduleBrush.CreateSolidBrush(RGB(185, 255, 185));
		}
	}

	memDC.SelectObject(&moduleBrush);
	memDC.Ellipse(rcArea);
	memDC.DrawText(_T("ROBOT"), rcArea, DT_SINGLELINE|DT_CENTER|DT_VCENTER);


	rcArea.left = rcClient.left + rcClient.Width()/10;
	rcArea.right = rcArea.left + rcClient.Width()/5;

	rcArea.top = rcClient.top + rcClient.Height()/4;
	rcArea.bottom = rcArea.top + rcClient.Height()/2;

	CRect rc;
	CString strModuleName[2];
	for (int i = 0; i < 2; i++)
	{
		rc.left = rcArea.left;
		rc.right = rcArea.right;

		rc.top = rcArea.top + i*rcArea.Height()/2;
		rc.bottom = rc.top + rcArea.Height()/2;

		if(MACHINE.m_pRobotOffload != NULL)
		{
			moduleBrush.DeleteObject();
			if(MACHINE.m_pRobotOffload->IsModuleEnable())
			{
				if(MACHINE.m_pRobotOffload->IsJamed())
				{
					moduleBrush.CreateSolidBrush(RGB(255, 0, 0));
				}
				else if(MACHINE.m_pRobotOffload->m_pTempPallet[i].m_bIsValid)
				{
					moduleBrush.CreateSolidBrush(RGB(0, 155, 39));
				}
				else
				{
					moduleBrush.CreateSolidBrush(RGB(185, 255, 185));
				}
			}
			else
			{
				moduleBrush.CreateSolidBrush(RGB(142, 142, 142));
			}
		}
		memDC.SelectObject(&moduleBrush);
		memDC.Rectangle(rc);
		strModuleName[i].Format(_T("夹爪#%d"), i+1);
		memDC.DrawText(strModuleName[i], rc, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	}


	rcArea.left = rcClient.left + rcClient.Width()*4/10;
	rcArea.right = rcArea.left + rcClient.Width()/5;

	rcArea.top = rcClient.top + rcClient.Height()/8;
	rcArea.bottom = rcArea.top + rcClient.Height()/6;

	moduleBrush.DeleteObject();
	if(MACHINE.m_pPressNgLine != NULL)
	{
		if(!MACHINE.m_pPressNgLine->IsModuleEnable())
		{
			moduleBrush.CreateSolidBrush(RGB(142, 142, 142));
		}
		else
		{
			if(MACHINE.m_pPressNgLine->m_ICheckLineFull > -1 && INPUTS[MACHINE.m_pPressNgLine->m_ICheckLineFull]->IsOn())
			{
				strTemp.Format(_T("%s\r\nNg线料满"), MACHINE.m_pPressNgLine->m_strName);
				moduleBrush.CreateSolidBrush(RGB(255, 0, 0));
			}
			else
			{
				strTemp = MACHINE.m_pPressNgLine->m_strName;
				moduleBrush.CreateSolidBrush(RGB(185, 255, 185));
			}
		}
		memDC.SelectObject(&moduleBrush);
		memDC.Rectangle(rcArea);
		memDC.DrawText(strTemp, rcArea, DT_CENTER|DT_VCENTER);
	}

	rcArea.left = rcClient.left + rcClient.Width()*4/10;
	rcArea.right = rcArea.left + rcClient.Width()/5;

	rcArea.top = rcClient.top + rcClient.Height()*7/10;
	rcArea.bottom = rcArea.top + rcClient.Height()/6;

	if(MACHINE.m_pRobotOffload != NULL)
	{
		moduleBrush.DeleteObject();
		if(MACHINE.m_pRobotOffload->IsModuleEnable())
		{
			if(MACHINE.m_pRobotOffload->m_pBufferPallet.m_bIsValid)
			{
				moduleBrush.CreateSolidBrush(RGB(0, 155, 39));
			}
			else
			{
				moduleBrush.CreateSolidBrush(RGB(185, 255, 185));
			}
		}
		else
		{
			moduleBrush.CreateSolidBrush(RGB(142, 142, 142));
		}
		memDC.SelectObject(&moduleBrush);
		memDC.Rectangle(rcArea);
		memDC.DrawText(_T("缓存"), rcArea, DT_CENTER|DT_VCENTER);
	}
}

void CAnimatedLayout::DrawBatteryPnp(CDC& memDC, CRect rcClient)
{
	CRect rcArea;
	CString strModuleName[2];
	CBrush backGroundBrush(RGB(145,160,168)), moduleBrush;

	memDC.FillRect(rcClient, &backGroundBrush);

	rcClient.DeflateRect(10, 10, 10, 10);

	for (int i = 0; i < 2; i++)
	{
		rcArea.left = rcClient.left + i*rcClient.Width()/2;
		rcArea.right = rcArea.left + rcClient.Width()/2;

		rcArea.top = rcClient.top;
		rcArea.bottom = rcClient.bottom;

		strModuleName[i].Format(_T("取电池夹爪%d"), i+1);
		if(MACHINE.m_pBatteryOffloadPnp[i] != NULL)
		{
			moduleBrush.DeleteObject();

			if(MACHINE.m_pBatteryOffloadPnp[i]->IsModuleEnable())
			{
				if(MACHINE.m_pBatteryOffloadPnp[i]->IsJamed())
				{
					moduleBrush.CreateSolidBrush(RGB(255, 0, 0));
				}
				else if(MACHINE.m_pBatteryOffloadPnp[i]->m_Pallet.m_bIsValid)
				{
					moduleBrush.CreateSolidBrush(RGB(0, 155, 39));
				}
				else
				{
					moduleBrush.CreateSolidBrush(RGB(185, 255, 185));
				}
			}
			else
			{
				moduleBrush.CreateSolidBrush(RGB(142, 142, 142));
			}
		}
		memDC.SelectObject(&moduleBrush);
		memDC.Rectangle(rcArea);
		memDC.DrawText(strModuleName[i], rcArea, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	}
	moduleBrush.DeleteObject();
	backGroundBrush.DeleteObject();
}


void CAnimatedLayout::DrawOnloadCore(CDC& memDC, CRect rcClient)
{
	CRect rcArea[4];
	CBrush backGroundBrush(RGB(145,160,168)), moduleBrush;

	memDC.FillRect(rcClient, &backGroundBrush);

	rcClient.DeflateRect(10, 10, 10, 10);

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0;  j < 2; j++)
		{
			rcArea[2*i+j].left = rcClient.left + j*rcClient.Width()/2 + 5;
			rcArea[2*i+j].right = rcArea[2*i+j].left + rcClient.Width()/2 - 10;

			rcArea[2*i+j].top = rcClient.top + i*rcClient.Height()/2 + 5;
			rcArea[2*i+j].bottom = rcArea[2*i+j].top + rcClient.Height()/2 - 10;
		}
	}


	if(MACHINE.m_pBarCodeNgLine != NULL)
	{
		CString strTemp;

		moduleBrush.DeleteObject();

		if(!MACHINE.m_pBarCodeNgLine->IsModuleEnable())
		{
			moduleBrush.CreateSolidBrush(RGB(142, 142, 142));
		}
		else
		{
			if(MACHINE.m_pBarCodeNgLine->m_ICheckLineFull > -1 && INPUTS[MACHINE.m_pBarCodeNgLine->m_ICheckLineFull]->IsOn())
			{
				strTemp.Format(_T("%s\r\nNg线料满"), MACHINE.m_pBarCodeNgLine->m_strName);
				moduleBrush.CreateSolidBrush(RGB(255, 0, 0));
			}
			else
			{
				strTemp = MACHINE.m_pBarCodeNgLine->m_strName;
				moduleBrush.CreateSolidBrush(RGB(185, 255, 185));
			}
		}
		memDC.SelectObject(&moduleBrush);
		memDC.Rectangle(rcArea);
		memDC.DrawText(strTemp, rcArea[0], DT_CENTER|DT_VCENTER);
	}


	if(MACHINE.m_pOnloadCore != NULL)
	{
		moduleBrush.DeleteObject();

		if(!MACHINE.m_pOnloadCore->IsModuleEnable())
		{
			moduleBrush.CreateSolidBrush(RGB(142, 142, 142));
		}
		else
		{
			if(MACHINE.m_pOnloadCore->m_pBufferPallet.m_bIsValid)
			{
				moduleBrush.CreateSolidBrush(RGB(0, 155, 39));
			}
			else
			{
				moduleBrush.CreateSolidBrush(RGB(185, 255, 185));
			}
		}
		memDC.SelectObject(&moduleBrush);
		memDC.Rectangle(rcArea[1]);

		memDC.DrawText(_T("电芯缓存"), rcArea[1], DT_CENTER|DT_SINGLELINE|DT_VCENTER);


		moduleBrush.DeleteObject();

		if(!MACHINE.m_pOnloadCore->IsModuleEnable())
		{
			moduleBrush.CreateSolidBrush(RGB(142, 142, 142));
		}
		else
		{
			if(MACHINE.m_pOnloadCore->IsJamed())
			{
				moduleBrush.CreateSolidBrush(RGB(255, 0, 0));
			}
			else if(MACHINE.m_pOnloadCore->m_pTempPallet[0].m_bIsValid)
			{
				moduleBrush.CreateSolidBrush(RGB(0, 155, 39));
			}
			else
			{
				moduleBrush.CreateSolidBrush(RGB(185, 255, 185));
			}
		}

		memDC.SelectObject(&moduleBrush);
		memDC.Rectangle(rcArea[2]);

		memDC.DrawText(_T("电芯夹爪#1"), rcArea[2], DT_CENTER|DT_SINGLELINE|DT_VCENTER);



		moduleBrush.DeleteObject();

		if(!MACHINE.m_pOnloadCore->IsModuleEnable())
		{
			moduleBrush.CreateSolidBrush(RGB(142, 142, 142));
		}
		else
		{
			if(MACHINE.m_pOnloadCore->IsJamed())
			{
				moduleBrush.CreateSolidBrush(RGB(255, 0, 0));
			}
			else if(MACHINE.m_pOnloadCore->m_pTempPallet[1].m_bIsValid)
			{
				moduleBrush.CreateSolidBrush(RGB(0, 155, 39));
			}
			else
			{
				moduleBrush.CreateSolidBrush(RGB(185, 255, 185));
			}
		}

		memDC.SelectObject(&moduleBrush);
		memDC.Rectangle(rcArea[3]);

		memDC.DrawText(_T("电芯夹爪#2"), rcArea[3], DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	}
	moduleBrush.DeleteObject();
	backGroundBrush.DeleteObject();
}

void CAnimatedLayout::DrawCoreInShell(CDC& memDC, CRect rcClient)
{
	CRect rcArea;
	CBrush backGroundBrush(RGB(145,160,168)), moduleBrush;

	memDC.FillRect(rcClient, &backGroundBrush);

	rcClient.DeflateRect(10, 10, 10, 10);

	CRunProcess* pRun[4] = {MACHINE.m_pBatteryCore[0], MACHINE.m_pBatteryCore[1], MACHINE.m_pBatteryInShell[0], MACHINE.m_pBatteryInShell[1]};

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0;  j < 2; j++)
		{
			rcArea.left = rcClient.left + j*rcClient.Width()/2 + 5;
			rcArea.right = rcArea.left + rcClient.Width()/2 - 10;

			rcArea.top = rcClient.top + i*rcClient.Height()/2 + 5;
			rcArea.bottom = rcArea.top + rcClient.Height()/2 - 10;

			if(pRun[i*2+j] != NULL)
			{
				moduleBrush.DeleteObject();

				if(pRun[i*2+j]->IsModuleEnable())
				{
					if(pRun[i*2+j]->IsJamed())
					{
						moduleBrush.CreateSolidBrush(RGB(255, 0, 0));
					}
					else if(pRun[i*2+j]->m_Pallet.m_bIsValid)
					{
						moduleBrush.CreateSolidBrush(RGB(0, 155, 39));
					}
					else
					{
						moduleBrush.CreateSolidBrush(RGB(185, 255, 185));
					}
				}
				else
				{
					moduleBrush.CreateSolidBrush(RGB(142, 142, 142));
				}
			}
			memDC.SelectObject(&moduleBrush);
			memDC.Rectangle(rcArea);
			memDC.DrawText(pRun[i*2+j]->m_strName, rcArea, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
		}
	}

	moduleBrush.DeleteObject();
	backGroundBrush.DeleteObject();
}

void CAnimatedLayout::DrawStack(CDC& memDC, CRect rcArea, CString* strModuleName, CRunProcess** pRun)
{
	CRect rcStack;
	CFont moduleFont,  *pOldFont;
	CBrush backGroundBrush(RGB(145,160,168));
	CBrush stackBrush, *pOldBrush;

	VERIFY(moduleFont.CreateFont(25, 8, 0, 0,  FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("微软雅黑")));

	memDC.FillRect(rcArea, &backGroundBrush);

	rcArea.DeflateRect(10, 10, 10, 10);

	for (int i = 0; i < 3; i++)
	{
		rcStack.top = rcArea.top + i * rcArea.Height()/3;
		rcStack.bottom = rcStack.top + rcArea.Height()/3 - 5;
		rcStack.left = rcArea.left;
		rcStack.right = rcArea.right;

		if(pRun[i] != NULL)
		{
			stackBrush.DeleteObject();
			if(pRun[i]->IsModuleEnable())
			{
				if(pRun[i]->IsJamed())
				{
					stackBrush.CreateSolidBrush(RGB(255, 0, 0));
				}
				else if(pRun[i]->m_bShellVaild)
				{
					stackBrush.CreateSolidBrush(RGB(0, 155, 39));
				}
				else
				{
					stackBrush.CreateSolidBrush(RGB(185, 255, 185));
				}
			}
			else
			{
				stackBrush.CreateSolidBrush(RGB(142, 142, 142));
			}

			pOldBrush = memDC.SelectObject(&stackBrush);
		}

		memDC.Rectangle(rcStack);
		memDC.SetBkMode(TRANSPARENT);
		pOldFont = memDC.SelectObject(&moduleFont);
		memDC.DrawText(pRun[i]->m_strName, rcStack, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	}

	backGroundBrush.DeleteObject();
	moduleFont.DeleteObject();
	pOldFont = NULL;
	stackBrush.DeleteObject();
}