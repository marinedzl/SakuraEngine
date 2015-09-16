#include "stdafx.h"
#include "GameView.h"

IMPLEMENT_DYNAMIC(CGameView, CDockablePane)

CGameView::CGameView()
{
}

CGameView::~CGameView()
{
}

BEGIN_MESSAGE_MAP(CGameView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


int CGameView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rect;
	GetClientRect(rect);

	// ������ͼ: 
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE;

	if (!mGameWnd.Create(NULL, _T("GameWnd"), dwViewStyle, rect, this, 4))
	{
		TRACE0("δ�ܴ���GameWnd!\n");
		return -1;      // δ�ܴ���
	}

	return 0;
}

void CGameView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	if (GetSafeHwnd() == NULL)
		return;
	CRect rectClient;
	GetClientRect(rectClient);
	mGameWnd.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

void CGameView::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	CRect rect;
	mGameWnd.GetWindowRect(rect);
	ScreenToClient(rect);

	rect.InflateRect(1, 1);
	dc.Draw3dRect(rect, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CGameView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	mGameWnd.SetFocus();
}
