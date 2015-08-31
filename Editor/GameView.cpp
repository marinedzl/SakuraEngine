#include "stdafx.h"
#include "Editor.h"
#include "GameView.h"
#include "CameraCtrl.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CGameView, CDialogEx)

CGameView::CGameView(CWnd* pParent)
	: CDialogEx(IDD_GAMEVIEW, pParent)
{
	mCameraCtrl = NULL;
	m_pRenderTarget = NULL;
}

CGameView::~CGameView()
{
	theApp.pGameView = NULL;
	theApp.RemoveProcesser(this);
	DestroyWindow();
	SAFE_DELETE(mCameraCtrl);
	SAFE_RELEASE(m_pRenderTarget);
}

void CGameView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGameView, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

BOOL CGameView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pRenderTarget = SECore::CreateRenderTarget(GetSafeHwnd());
	theApp.AddProcesser(this);

	mCameraCtrl = new CameraCtrl(theApp.scene->GetCamera());

	theApp.pGameView = this;

	return TRUE;
}

void CGameView::Update(float deltaTime)
{
	if (!IsWindow(GetSafeHwnd()) || !IsWindowVisible())
		return;

	if (mCameraCtrl)
		mCameraCtrl->Update(deltaTime);

	theApp.scene->GetConfig()->EnableGizmo(false);

	m_pRenderTarget->Begin();
	theApp.scene->Draw(m_pRenderTarget);
	m_pRenderTarget->End();
}


void CGameView::OnClose()
{
	__super::OnClose();
	delete this;
}


BOOL CGameView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (mCameraCtrl)
	{
		mCameraCtrl->Scroll(-(float)zDelta * 0.01f);
	}
	return __super::OnMouseWheel(nFlags, zDelta, pt);
}
