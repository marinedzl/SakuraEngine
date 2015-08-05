#include "stdafx.h"
#include "utilapi.h"
#include "Model.h"
#include "Skeleton.h"
#include "Animation.h"
#include "SEUtil.h"

#define SEUtil_CLASS_ID Class_ID(0x79d613a4, 0x4f21c3ad)

class MyErrorProc : public IGameErrorCallBack
{
public:
	void ErrorProc(IGameError error)
	{
		const TCHAR * buf = GetLastIGameErrorText();
		DebugPrint(_T("ErrorCode = %d ErrorText = %s\n"), error, buf);
	}
};

class SEUtil : public UtilityObj
{
public:
	virtual void BeginEditParams(Interface *ip, IUtil *iu);
	virtual void EndEditParams(Interface *ip, IUtil *iu);
	virtual void DeleteThis() {}

public:
	SEUtil() {}
	~SEUtil() {}
	void Init(HWND hWnd);
	void Destroy(HWND hWnd);
	static INT_PTR CALLBACK UtilDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	HWND hPanel;
	IUtil *iu;
	Interface *ip;
	IGameScene * pIgame;
};

static SEUtil seUtil;

class SEUtilClassDesc :public ClassDesc2 {
public:
	int IsPublic() { return TRUE; }
	void * Create(BOOL loading = FALSE) { return &seUtil; }
	const TCHAR *	ClassName() { return GetString(IDS_CLASS_NAME); }
	SClass_ID SuperClassID() { return UTILITY_CLASS_ID; }
	Class_ID ClassID() { return SEUtil_CLASS_ID; }
	const TCHAR* Category() { return GetString(IDS_CATEGORY); }
	const TCHAR* InternalName() { return _T("SEUtil"); } // returns fixed parsable name (scripter-visible name)
	HINSTANCE HInstance() { return hInstance; } // returns owning module handle
};

static SEUtilClassDesc SEUtilDesc;
ClassDesc2* GetSEUtilClassDesc() { return &SEUtilDesc; }

void SEUtil::BeginEditParams(Interface * ip, IUtil * iu)
{
	this->iu = iu;
	this->ip = ip;
	hPanel = ip->AddRollupPage(
		hInstance,
		MAKEINTRESOURCE(IDD_PANEL),
		UtilDlgProc,
		GetString(IDS_CLASS_NAME),
		0);
}

void SEUtil::EndEditParams(Interface * ip, IUtil * iu)
{
	this->iu = NULL;
	this->ip = NULL;
	ip->DeleteRollupPage(hPanel);
	hPanel = NULL;
}

void SEUtil::Init(HWND hWnd)
{
	MyErrorProc pErrorProc;
	SetErrorCallBack(&pErrorProc);

	pIgame = GetIGameInterface();

	IGameConversionManager * cm = GetConversionManager();
	cm->SetCoordSystem(IGameConversionManager::IGAME_D3D);
	pIgame->InitialiseIGame(false);
	pIgame->SetStaticFrame(0);
}

void SEUtil::Destroy(HWND hWnd)
{
	pIgame->ReleaseIGame();
}

bool SEGetSaveFileName(HWND hWnd, TCHAR* dst, const TCHAR* szFilter, const TCHAR* szExt)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = szFilter;
	ofn.lpstrFile = dst;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = szExt;
	ofn.lpstrInitialDir = _T(".\\");
	return GetSaveFileName(&ofn) == TRUE;
}

bool SEGetOpenFileName(HWND hWnd, TCHAR* dst, const TCHAR* szFilter, const TCHAR* szExt)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = szFilter;
	ofn.lpstrFile = dst;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = szExt;
	ofn.lpstrInitialDir = _T(".\\");
	return GetOpenFileName(&ofn) == TRUE;
}

INT_PTR CALLBACK SEUtil::UtilDlgProc(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_INITDIALOG:
		seUtil.Init(hWnd);
		break;

	case WM_DESTROY:
		seUtil.Destroy(hWnd);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BTN_BLD_SKE:
		{
			MaxPlugin::Skeleton::Instance().Build(seUtil.pIgame);
		}
		break;
		case IDC_BTN_EXP_SKE:
		{
			TCHAR szFileName[MAX_PATH] = _T("");
			if (SEGetSaveFileName(hWnd, szFileName, _T("SE Skeleton Files (*.ske)\0*.ske\0All Files (*.*)\0*.*\0"), _T("ske")))
			{
				MaxPlugin::Skeleton::Instance().WriteToFile(szFileName);
			}
		}
		break;
		case IDC_BTN_LOD_SKE:
		{
			TCHAR szFileName[MAX_PATH] = _T("");
			if (SEGetOpenFileName(hWnd, szFileName, _T("SE Skeleton Files (*.ske)\0*.ske\0All Files (*.*)\0*.*\0"), _T("ske")))
			{
				MaxPlugin::Skeleton::Instance().LoadFromFile(szFileName);
			}
		}
		break;
		case IDC_BTN_EXPORT_MESH:
		{
			TCHAR szFileName[MAX_PATH] = _T("");
			if (SEGetSaveFileName(hWnd, szFileName, _T("SE Model Files (*.mdl)\0*.mdl\0All Files (*.*)\0*.*\0"), _T("mdl")))
			{
				MaxPlugin::Model model;
				CHECK(MaxPlugin::Skeleton::Instance().IsInit());
				if (model.Extract(seUtil.pIgame))
					model.WriteFile(szFileName);
				model.Clear();
			}
		}
		break;
		case IDC_BTN_EXPORT_ANIMATION:
		{
			TCHAR szFileName[MAX_PATH] = _T("");
			if (SEGetSaveFileName(hWnd, szFileName, _T("SE Animation Files (*.anim)\0*.anim\0All Files (*.*)\0*.*\0"), _T("anim")))
			{
				MaxPlugin::Animation anim;
				CHECK(MaxPlugin::Skeleton::Instance().IsInit());
				if (anim.Extract(seUtil.pIgame))
					anim.WriteFile(szFileName);
				anim.Clear();
			}
		}
		break;
		}
		break;

		break;


	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		seUtil.ip->RollupMouseMessage(hWnd, msg, wParam, lParam);
		break;

	default:
		return FALSE;
	}

Exit0:
	return TRUE;
}
