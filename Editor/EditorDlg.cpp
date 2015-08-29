#include "stdafx.h"
#include "Editor.h"
#include "EditorDlg.h"
#include "afxdialogex.h"
#include "GameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEditorDlg::CEditorDlg(CWnd* pParent)
	: CDialogEx(IDD_EDITOR_DIALOG, pParent)
{
	m_pGameView = NULL;
}

CEditorDlg::~CEditorDlg()
{
	DestroyWindow();
	SAFE_DELETE(m_pGameView);
}

void CEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEditorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_COMMAND(ID_OPEN_GAME_VIEW, &CEditorDlg::OnOpenGameView)
END_MESSAGE_MAP()

BOOL CEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	return TRUE;
}

void CEditorDlg::OnPaint()
{
	CDialogEx::OnPaint();
}

void CEditorDlg::OnClose()
{
	CDialogEx::OnClose();
	PostQuitMessage(0);
}

void CEditorDlg::OnOpenGameView()
{
	if (m_pGameView)
		return;
	m_pGameView = new CGameView();
	CHECK(m_pGameView);
	if (m_pGameView->Create(IDD_GAMEVIEW))
		m_pGameView->ShowWindow(SW_SHOW);
	else
		SAFE_DELETE(m_pGameView);
Exit0:
	;
}
