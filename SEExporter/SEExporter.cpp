#include "stdafx.h"
#include "Model.h"
#include "Skeleton.h"
#include "SEExporter.h"

// Dummy function for progress bar
DWORD WINAPI fnProgressBar(LPVOID arg)
{
	return(0);
}

#define SEExporter_CLASS_ID Class_ID(0x79d613a4, 0x4f21c3ae)

class SEExporter : public SceneExport
{
public:
	enum ExportType
	{
		eExportMesh,
		eExportAnimation,
	};
public:
	int ExtCount(); // Number of extensions supported
	const TCHAR *	Ext(int n); // Extension #n (i.e. "3DS")
	const TCHAR *	LongDesc(); // Long ASCII description (i.e. "Autodesk 3D Studio File")
	const TCHAR *	ShortDesc(); // Short ASCII description (i.e. "3D Studio")
	const TCHAR *	AuthorName(); // ASCII Author name
	const TCHAR *	CopyrightMessage(); // ASCII Copyright message
	const TCHAR *	OtherMessage1(); // Other message #1
	const TCHAR *	OtherMessage2(); // Other message #2
	unsigned int Version(); // Version number * 100 (i.e. v3.01 = 301)
	void ShowAbout(HWND hWnd); // Show DLL's "About..." box

	BOOL SupportsOptions(int ext, DWORD options);
	int DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts = FALSE, DWORD options = 0);

	SEExporter();
	~SEExporter();

public:
	IGameScene * pIgame;
	static HWND hParams;
	float igameVersion;
	float exporterVersion;
	bool showPrompts;
	bool exportSelected;
	int cS;
	int staticFrame;
	ExportType exportType;
};

class SEExporterClassDesc :public ClassDesc2 {
public:
	int IsPublic() { return TRUE; }
	void * Create(BOOL loading = FALSE) { return new SEExporter(); }
	const TCHAR *	ClassName() { return GetString(IDS_CLASS_NAME); }
	SClass_ID SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	Class_ID ClassID() { return SEExporter_CLASS_ID; }
	const TCHAR* Category() { return GetString(IDS_CATEGORY); }
	const TCHAR* InternalName() { return _T("SEExporter"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE HInstance() { return hInstance; }				// returns owning module handle
};

static SEExporterClassDesc SEExporterDesc;
ClassDesc2* GetSEExporterClassDesc() { return &SEExporterDesc; }

SEExporter::SEExporter()
{
	staticFrame = 0;
	cS = IGameConversionManager::IGAME_D3D;
	exporterVersion = 1.0f;
	exportType = eExportMesh;
}

SEExporter::~SEExporter()
{
}

int SEExporter::ExtCount()
{
	//TODO: Returns the number of file name extensions supported by the plug-in.
	return 1;
}

const TCHAR *SEExporter::Ext(int n)
{
	//TODO: Return the 'i-th' file name extension (i.e. "3DS").
	return _T("mdl");
}

const TCHAR *SEExporter::LongDesc()
{
	//TODO: Return long ASCII description (i.e. "Targa 2.0 Image File")
	return _T("SakuraEngine");
}

const TCHAR *SEExporter::ShortDesc()
{
	//TODO: Return short ASCII description (i.e. "Targa")
	return _T("SakuraEngine");
}

const TCHAR *SEExporter::AuthorName()
{
	//TODO: Return ASCII Author name
	return _T("marine");
}

const TCHAR *SEExporter::CopyrightMessage()
{
	// Return ASCII Copyright message
	return _T("");
}

const TCHAR *SEExporter::OtherMessage1()
{
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *SEExporter::OtherMessage2()
{
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int SEExporter::Version()
{
	//TODO: Return Version number * 100 (i.e. v3.01 = 301)
	return (unsigned int)(exporterVersion * 100);
}

void SEExporter::ShowAbout(HWND hWnd)
{
	// Optional
}

BOOL SEExporter::SupportsOptions(int ext, DWORD options)
{
	// TODO Decide which options to support.  Simply return
	// true for each option supported by each Extension 
	// the exporter supports.

	return TRUE;
}

class MyErrorProc : public IGameErrorCallBack
{
public:
	void ErrorProc(IGameError error)
	{
		const TCHAR * buf = GetLastIGameErrorText();
		DebugPrint(_T("ErrorCode = %d ErrorText = %s\n"), error, buf);
	}
};

INT_PTR CALLBACK ISEExporterOptionsDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SEExporter *exp = DLGetWindowLongPtr<SEExporter*>(hWnd);

	switch (message) {
	case WM_INITDIALOG:
		exp = (SEExporter *)lParam;
		DLSetWindowLongPtr(hWnd, lParam);
		CenterWindow(hWnd, GetParent(hWnd));

		//Versioning
		TCHAR Title[256];
		_stprintf_s(Title, _T("SakuraEngine Exporter version %.3f; IGame version %.3f"),
			exp->exporterVersion, exp->igameVersion);
		SetWindowText(hWnd, Title);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BTN_BLD_SKE:
		{
			MaxPlugin::Skeleton::Instance().Build(exp->pIgame);
		}
		break;
		case IDC_BTN_EXP_SKE:
		{
			OPENFILENAME ofn;
			TCHAR szFileName[MAX_PATH] = _T("");
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = _T("Ske Files (*.ske)\0*.ske\0All Files (*.*)\0*.*\0");
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = _T("ske");
			ofn.lpstrInitialDir = _T(".\\");
			if (GetSaveFileName(&ofn))
			{
				MaxPlugin::Skeleton::Instance().WriteToFile(szFileName);
			}
		}
		break;
		case IDC_BTN_LOD_SKE:
		{
			OPENFILENAME ofn;
			TCHAR szFileName[MAX_PATH] = _T("");
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = _T("Ske Files (*.ske)\0*.ske\0All Files (*.*)\0*.*\0");
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = _T("ske");
			ofn.lpstrInitialDir = _T(".\\");
			if (GetOpenFileName(&ofn))
			{
				MaxPlugin::Skeleton::Instance().LoadFromFile(szFileName);
			}
		}
		break;
		case IDOK:
			if (IsDlgButtonChecked(hWnd, IDC_RADIO_ANIMATION))
			{
				exp->exportType = SEExporter::eExportAnimation;
			}
			else
			{
				exp->exportType = SEExporter::eExportMesh;
			}

			EndDialog(hWnd, 1);
			break;
		case IDCANCEL:
			EndDialog(hWnd, 0);
			break;
		}

	default:
		return FALSE;

	}
	return TRUE;

}

int SEExporter::DoExport(const TCHAR * name, ExpInterface* ei, Interface* i, BOOL suppressPrompts, DWORD options)
{
	Interface* ip = GetCOREInterface();

	MyErrorProc pErrorProc;

	UserCoord Whacky = {
		1,	//Right Handed
		1,	//X axis goes right
		3,	//Y Axis goes down
		4,	//Z Axis goes in.
		1,	//U Tex axis is right
		1,  //V Tex axis is Down
	};

	SetErrorCallBack(&pErrorProc);

	// Set a global prompt display switch
	showPrompts = suppressPrompts ? FALSE : TRUE;

	exportSelected = (options & SCENE_EXPORT_SELECTED) ? true : false;
	igameVersion = GetIGameVersion();

	DebugPrint(_T("3ds max compatible version %.2f\n"), GetSupported3DSVersion());

	pIgame = GetIGameInterface();

	IGameConversionManager * cm = GetConversionManager();
	cm->SetCoordSystem((IGameConversionManager::CoordSystem)cS);
	pIgame->InitialiseIGame(exportSelected);
	pIgame->SetStaticFrame(staticFrame);

	if (showPrompts)
	{
		// Prompt the user with our dialogbox, and get all the options.
		if (!DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_PANEL),
			i->GetMAXHWnd(), ISEExporterOptionsDlgProc, (LPARAM)this)) {
			return 1;
		}
	}

	ExportType exportType = eExportMesh;

	switch (exportType)
	{
	case eExportMesh:
	{
		MaxPlugin::Model model;
		if (model.Extract(pIgame))
			model.WriteFile(name);
		model.Clear();
	}
	break;
	case eExportAnimation:
	{

	}
	break;
	}

	pIgame->ReleaseIGame();

	ip->ProgressEnd();

	return TRUE;
}
