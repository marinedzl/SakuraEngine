#include "stdafx.h"
#include "Editor.h"
#include "GameWnd.h"

IMPLEMENT_DYNAMIC(CGameWnd, CWnd)

CGameWnd::CGameWnd()
{
	m_pGame = nullptr;
	mChangingSize = false;
	theApp.AddProcesser(this);
}

CGameWnd::~CGameWnd()
{
	theApp.RemoveProcesser(this);
	SAFE_RELEASE(m_pGame);
}

void CGameWnd::Resize()
{
	RECT rect;
	GetClientRect(&rect);
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;
	if (w == 0 || h == 0)
		return;

	if (m_pGame)
		m_pGame->Resize(w, h);
}

void CGameWnd::Update(float deltaTime)
{
	if (m_pGame)
		m_pGame->Update(deltaTime);

	if (IsWindow(GetSafeHwnd()) && IsWindowVisible())
	{
		if (m_pGame)
			m_pGame->Draw();
	}
}

BOOL CGameWnd::OnInitUpdate()
{
	BOOL ret = FALSE;

	typedef IGame*(*CreateGame)();

	HMODULE hm = LoadLibrary(_T("Game.dll"));
	CHECK(hm);

	CreateGame func;
	func = (CreateGame)GetProcAddress(hm, "CreateGame");
	CHECK(func);

	RECT rect;
	GetClientRect(&rect);
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	m_pGame = func();
	CHECK(m_pGame);
	CHECK(m_pGame->Init(theApp.core, GetSafeHwnd(), w, h));
	m_pGame->EditorPlay(theApp.scene);

	ret = TRUE;
Exit0:
	return ret;
}

BEGIN_MESSAGE_MAP(CGameWnd, CWnd)
	ON_WM_EXITSIZEMOVE()
	ON_WM_ENTERSIZEMOVE()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CGameWnd::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	if (!mChangingSize)
		Resize();
}

void CGameWnd::OnExitSizeMove()
{
	__super::OnExitSizeMove();
	Resize();
	mChangingSize = false;
}

void CGameWnd::OnEnterSizeMove()
{
	mChangingSize = true;
	__super::OnEnterSizeMove();
}


LRESULT CGameWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pGame)
		m_pGame->WndProc(GetSafeHwnd(), message, wParam, lParam);
	return __super::WindowProc(message, wParam, lParam);
}


void CGameWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 __super::OnPaint()
	if (!m_pGame)
	{
		OnInitUpdate();
	}
}
