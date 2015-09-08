#include "stdafx.h"
#include "Editor.h"
#include "GameView.h"
#include "CameraCtrl.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CGameView, CDialogEx)

CGameView::CGameView(CWnd* pParent)
	: CDialogEx(IDD_GAMEVIEW, pParent)
{
	m_pGame = NULL;
}

CGameView::~CGameView()
{
	SAFE_RELEASE(m_pGame);
	theApp.pGameView = NULL;
	theApp.RemoveProcesser(this);
	DestroyWindow();
}

void CGameView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGameView, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CGameView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	theApp.AddProcesser(this);

	theApp.pGameView = this;

	typedef IGame*(*CreateGame)();

	HMODULE hm = LoadLibrary(_T("Game.dll"));
	CHECK(hm);

	CreateGame func;
	func = (CreateGame)GetProcAddress(hm, "CreateGame");
	CHECK(func);

	m_pGame = func();
	CHECK(m_pGame);
	CHECK(m_pGame->Init(theApp.core, GetSafeHwnd()));
	m_pGame->EditorPlay(theApp.scene);

Exit0:
	return TRUE;
}

void CGameView::Update(float deltaTime)
{
	if (!IsWindow(GetSafeHwnd()) || !IsWindowVisible())
		return;

	if (m_pGame)
	{
		m_pGame->Update(deltaTime);
		m_pGame->Draw();
	}
}

void CGameView::OnClose()
{
	__super::OnClose();
	delete this;
}

LRESULT CGameView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pGame)
		m_pGame->WndProc(GetSafeHwnd(), message, wParam, lParam);
	return __super::WindowProc(message, wParam, lParam);
}
