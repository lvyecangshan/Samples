// ListCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "Machine.h"
#include "ListCtrlEx.h"


// CListCtrlEx

IMPLEMENT_DYNAMIC(CListCtrlEx, CListCtrl)

CListCtrlEx::CListCtrlEx()
{

}

CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CListCtrlEx message handlers



BOOL CListCtrlEx::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return FALSE;
	//return CListCtrl::OnEraseBkgnd(pDC);
}

void CListCtrlEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CListCtrl::OnPaint() for painting messages

	CRect rcClient,rcClip,rcHeader;
	GetClientRect(&rcClient);

	GetHeaderCtrl()->GetWindowRect(&rcHeader);
	dc.GetClipBox(&rcClip);

	CDC MemDC;
	CBitmap MemBitmap, *pOldBitmap;

	MemDC.CreateCompatibleDC(&dc);
	int nMode = MemDC.SetMapMode(dc.GetMapMode());

	MemBitmap.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	pOldBitmap = MemDC.SelectObject(&MemBitmap);
	MemDC.FillSolidRect(&rcClient,GetBkColor());

	CRgn rgn;
	rgn.CreateRectRgn(rcClip.left,rcClip.top,rcClip.right,rcClip.bottom);
	MemDC.SelectClipRgn(&rgn);
	DefWindowProc(WM_PAINT,(WPARAM)MemDC.m_hDC,(LPARAM)0);
	MemDC.SelectClipRgn(NULL);

	if (rcClip.top < rcHeader.Height())
		rcClip.top = rcHeader.Height();

	dc.BitBlt(rcClip.left,rcClip.top,rcClip.Width(),rcClip.Height(),&MemDC,rcClip.left,rcClip.top,SRCCOPY);

	MemDC.SetMapMode(nMode);
	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
	MemBitmap.DeleteObject();
	rgn.DeleteObject();
}
