#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Editor.h"
#include "SceneLoader.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CEditorApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CEditorApp::OnAppAbout)
END_MESSAGE_MAP()

CEditorApp::CEditorApp()
{
	m_bHiColorIcons = TRUE;

	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���: 
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Editor.AppID.NoVersion"));

	core = nullptr;
	scene = nullptr;
	camera = nullptr;
	selected = nullptr;
	mLastTime = 0;
}

CEditorApp theApp;

BOOL CEditorApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	if (!InitCore())
		return FALSE;

	// ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ���
	// ����Ȼ��������ΪӦ�ó���������ڶ���
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// ���������ؿ�ܼ�����Դ
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

BOOL CEditorApp::InitCore()
{
	BOOL ret = FALSE;

	core = SECore::CreateCore();
	CHECK(core);

	scene = core->CreateScene();
	CHECK(scene);

	CHECK(SceneLoader::Load(scene, _T("scene.json")));

	ret = TRUE;
Exit0:
	return ret;
}

int CEditorApp::ExitInstance()
{
	SAFE_RELEASE(scene);
	SAFE_RELEASE(core);
	SAFE_RELEASE(camera);

	AfxOleTerm(FALSE);
	return CWinApp::ExitInstance();
}

BOOL CEditorApp::OnIdle(LONG lCount)
{
	DWORD currentTime = timeGetTime();
	DWORD dwDelta = currentTime - mLastTime;

	if (dwDelta > 0)
	{
		mLastTime = currentTime;

		float deltaTime = dwDelta / 1000.0f;

		//deltaTime *= 0.1f;

		if (!m_processers.empty())
		{
			std::list<IIdleProcesser*>::iterator iter = m_processers.begin();
			std::list<IIdleProcesser*>::iterator iterEnd = m_processers.end();
			for (; iter != iterEnd; ++iter)
			{
				(*iter)->Update(deltaTime);
			}
		}
	}

	CWinApp::OnIdle(lCount);

	return TRUE;
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

void CEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CEditorApp �Զ������/���淽��
void CEditorApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CEditorApp::LoadCustomState()
{
}

void CEditorApp::SaveCustomState()
{
}

// CEditorApp ��Ϣ�������
