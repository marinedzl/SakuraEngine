#include "stdafx.h"
#include "Editor.h"
#include "GameView.h"

IMPLEMENT_DYNAMIC(CGameView, CDockablePane)

CGameView::CGameView()
{
	m_pGame = nullptr;
	mChangingSize = false;
}

CGameView::~CGameView()
{
	SAFE_RELEASE(m_pGame);
}

void CGameView::Resize()
{
	if (!IsWindowVisible())
		return;

	RECT rect;
	GetClientRect(&rect);
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;
	if (w == 0 || h == 0)
		return;

	if (m_pGame)
		m_pGame->Resize(w, h);
}

void CGameView::Update(float deltaTime)
{
	if (m_pGame)
		m_pGame->Update(deltaTime);

	if (IsWindow(GetSafeHwnd()) && IsWindowVisible())
	{
		if (!m_pGame)
		{
			OnInitUpdate();
		}
		m_pGame->Draw();
	}
}

BOOL CGameView::OnInitUpdate()
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

BEGIN_MESSAGE_MAP(CGameView, CDockablePane)
	ON_WM_EXITSIZEMOVE()
	ON_WM_ENTERSIZEMOVE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CGameView::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	if (!mChangingSize)
		Resize();
}

void CGameView::OnExitSizeMove()
{
	__super::OnExitSizeMove();
	Resize();
	mChangingSize = false;
}

void CGameView::OnEnterSizeMove()
{
	mChangingSize = true;
	__super::OnEnterSizeMove();
}


LRESULT CGameView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pGame)
		m_pGame->WndProc(GetSafeHwnd(), message, wParam, lParam);
	return __super::WindowProc(message, wParam, lParam);
}
