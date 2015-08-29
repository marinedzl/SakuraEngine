#include "stdafx.h"
#include "Editor.h"
#include "GameView.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CGameView, CDialogEx)

CGameView::CGameView(CWnd* pParent)
	: CDialogEx(IDD_GAMEVIEW, pParent)
{
}

CGameView::~CGameView()
{
	DestroyWindow();
}

void CGameView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGameView, CDialogEx)
END_MESSAGE_MAP()

BOOL CGameView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}
