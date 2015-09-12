#include "stdafx.h"
#include "Editor.h"
#include "CameraCtrl.h"
#include "TransformCtrl.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CChildView::CChildView()
{
	mRT = nullptr;
	mCameraCtrl = nullptr;
	mTransformCtrl = nullptr;
	mIsInited = false;
}

CChildView::~CChildView()
{
	theApp.RemoveProcesser(this);
	SAFE_RELEASE(mRT);
	SAFE_DELETE(mTransformCtrl);
	SAFE_DELETE(mCameraCtrl);
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_COMMAND(ID_BUTTON_CAPTURE, &CChildView::OnButtonCapture)
END_MESSAGE_MAP()

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnInitUpdate()
{
	RECT rect;
	GetClientRect(&rect);
	mRT = theApp.core->CreateRenderTarget(GetSafeHwnd(), rect.right - rect.left, rect.bottom - rect.top);
	theApp.AddProcesser(this);

	theApp.camera = theApp.core->CreateCamera();
	theApp.camera->SetView(mRT->GetWidth(), mRT->GetHeight());

	mTransformCtrl = new TransformCtrl();
	mCameraCtrl = new CameraCtrl(theApp.camera);

	mIsInited = true;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this);

	if (!mIsInited)
		OnInitUpdate();
}

void CChildView::Update(float deltaTime)
{
	if (!IsWindowVisible())
		return;

	if (mCameraCtrl)
		mCameraCtrl->Update(deltaTime);

	theApp.scene->GetConfig()->EnableGizmo(true);
	theApp.scene->Draw(theApp.camera, mRT);
	theApp.scene->GetConfig()->EnableGizmo(false);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (mCameraCtrl)
	{
		mCameraCtrl->Move((float)point.x, (float)point.y);
	}
	if (mTransformCtrl)
	{
		mTransformCtrl->Move((float)point.x, (float)point.y);
	}
	__super::OnMouseMove(nFlags, point);
}

BOOL CChildView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (mCameraCtrl)
	{
		mCameraCtrl->Scroll(-(float)zDelta * 0.01f);
	}
	return __super::OnMouseWheel(nFlags, zDelta, pt);
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (mCameraCtrl)
	{
		mCameraCtrl->Begin((float)point.x, (float)point.y, CameraCtrl::eOpRotate);
		SetCapture();
	}
	__super::OnRButtonDown(nFlags, point);
}

void CChildView::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (mCameraCtrl)
	{
		mCameraCtrl->End();
		ReleaseCapture();
	}
	__super::OnRButtonUp(nFlags, point);
}

void CChildView::OnMButtonDown(UINT nFlags, CPoint point)
{
	if (mCameraCtrl)
	{
		mCameraCtrl->Begin((float)point.x, (float)point.y, CameraCtrl::eOpMove);
		SetCapture();
	}
	__super::OnMButtonDown(nFlags, point);
}

void CChildView::OnMButtonUp(UINT nFlags, CPoint point)
{
	if (mCameraCtrl)
	{
		mCameraCtrl->End();
		ReleaseCapture();
	}
	__super::OnMButtonUp(nFlags, point);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!theApp.scene)
		return;

	switch (mState)
	{
	case CChildView::eNormal:
	{
		SECore::Ray ray;
		SECore::RaycastHit hit;

		theApp.camera->ScreenPointToRay(ray, Vector3((float)point.x, (float)point.y, 0));

		theApp.scene->RaycastBound(ray, hit, 10000);

		if (hit.entity)
		{
			if (theApp.selected)
			{
				if (hit.entity == theApp.selected)
				{
					mState = eTransform;
					mTransformCtrl->Begin((float)point.x, (float)point.y, TransformCtrl::eOpMove);
					SetCapture();
				}
				else
				{
					//unselect
					theApp.selected->SetGizmoColor(Color(0.5f, 0.5f, 0.5f, 1));
					theApp.selected = nullptr;
					mTransformCtrl->Attach(nullptr);

					//select
					theApp.selected = hit.entity;
					theApp.selected->SetGizmoColor(Color(0, 0, 0.5f, 1));
					mTransformCtrl->Attach(theApp.selected);
				}
			}
			else
			{
				//select
				theApp.selected = hit.entity;
				theApp.selected->SetGizmoColor(Color(0, 0, 0.5f, 1));
				mTransformCtrl->Attach(theApp.selected);
			}
		}
		else
		{
			//unselect
			theApp.selected->SetGizmoColor(Color(0.5f, 0.5f, 0.5f, 1));
			theApp.selected = nullptr;
			mTransformCtrl->Attach(nullptr);
		}
	}
	break;
	case CChildView::eTransform:
	{
	}
	break;
	default:
		break;
	}

	__super::OnLButtonDown(nFlags, point);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	mState = eNormal;
	if (mTransformCtrl)
	{
		mTransformCtrl->End();
	}
	ReleaseCapture();
	__super::OnLButtonUp(nFlags, point);
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}

void CChildView::OnButtonCapture()
{
	if (mRT)
	{
		Update(0);
		mRT->CaptureToFile("Editor");
	}
}
