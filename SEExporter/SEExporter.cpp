#include "stdafx.h"
#include "SEExporter.h"

#include "decomp.h"
#include "IGame.h"
#include "IGameObject.h"
#include "IGameProperty.h"
#include "IGameControl.h"
#include "IGameModifier.h"
#include "IConversionManager.h"
#include "IGameError.h"
#include "3dsmaxport.h"

#include "Model.h"

// Dummy function for progress bar
DWORD WINAPI fnProgressBar(LPVOID arg)
{
	return(0);
}

// Replace some characters we don't care for.
TCHAR* FixupName(TCHAR *buf)
{
	static TCHAR buffer[256];
	TCHAR* cPtr;

	_tcscpy_s(buffer, buf);
	cPtr = buffer;

	while (*cPtr) {
		if (*cPtr == _T('"')) *cPtr = 39;	// Replace double-quote with single quote.
		else if (*cPtr <= 31) *cPtr = _T('_');	// Replace control characters with underscore
		cPtr++;
	}

	return buffer;
}

#define SEExporter_CLASS_ID Class_ID(0x79d613a4, 0x4f21c3ae)

class SEExporter : public SceneExport
{
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

	void ExportChildNode(IGameNode * child);

	void DumpMesh(IGameNode* gn);

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
	Model mModel;
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
	ISpinnerControl * spin;
	int ID;

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
		case IDOK:
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

int SEExporter::DoExport(const TCHAR * name, ExpInterface * ei, Interface * i, BOOL suppressPrompts, DWORD options)
{
	HRESULT hr;

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

	if (showPrompts)
	{
		// Prompt the user with our dialogbox, and get all the options.
		if (!DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_PANEL),
			i->GetMAXHWnd(), ISEExporterOptionsDlgProc, (LPARAM)this)) {
			return 1;
		}
	}

	ip->ProgressStart(_T("Exporting Using IGame.."), TRUE, fnProgressBar, NULL);

	pIgame = GetIGameInterface();

	IGameConversionManager * cm = GetConversionManager();
	cm->SetCoordSystem((IGameConversionManager::CoordSystem)cS);
	pIgame->InitialiseIGame(exportSelected);
	pIgame->SetStaticFrame(staticFrame);

	TSTR buf;
	int topLevelCount = pIgame->GetTopLevelNodeCount();
	for (int loop = 0; loop < topLevelCount; loop++)
	{
		IGameNode* pGameNode = pIgame->GetTopLevelNode(loop);

		//check for selected state - we deal with targets in the light/camera section
		if (pGameNode->IsTarget())
			continue;

		buf.printf(_T("Processing: %s"), pGameNode->GetName());
		GetCOREInterface()->ProgressUpdate((int)((float)loop / pIgame->GetTotalNodeCount() * 100.0f), FALSE, buf.data());

		ExportChildNode(pGameNode);
	}

	pIgame->ReleaseIGame();

	ip->ProgressEnd();
	return TRUE;
}

void SEExporter::ExportChildNode(IGameNode* child)
{
	IGameObject* obj = child->GetIGameObject();
	IGameObject::MaxType T = obj->GetMaxType();
	IGameObject::ObjectTypes igameType = obj->GetIGameType();

	switch (igameType)
	{
	case IGameObject::IGAME_MESH:
	{
		DumpMesh(child);
	}
	break;
	case IGameObject::IGAME_BONE:
	{
	}
	break;
	}
}

void ConvertVertex(Vector3& dst, const Point3& src)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}

void ConvertVector3(Vector3& dst, const Point3& src)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}

void SEExporter::DumpMesh(IGameNode* gn)
{
	CHECK(gn);

	IGameMesh* gm = dynamic_cast<IGameMesh*>(gn->GetIGameObject());
	CHECK(gm);

	gm->SetCreateOptimizedNormalList();
	CHECK(gm->InitializeData());

	Model::Mesh* mesh = mModel.AddMesh(gn->GetName());

	// dump Vertices
	if (gm->GetNumberOfVerts() > 0)
	{
		int count = gm->GetNumberOfVerts();
		mesh->CreateVertices(count);
		for (int i = 0; i < count; ++i)
		{
			Point3 v;
			gm->GetVertex(i, v);
			ConvertVertex(mesh->GetVertex(i), v);
		}
	}

	// dump Normals
	if (gm->GetNumberOfNormals() > 0)
	{
		int count = gm->GetNumberOfVerts();
		mesh->CreateNormals(count);
		for (int i = 0; i < count; ++i)
		{
			Point3 v;
			gm->GetNormal(i, v);
			ConvertVector3(mesh->GetNormal(i), v);
		}
	}

	{
		Tab<int> matidTab = gm->GetActiveMatIDs();
		if (matidTab.Count() > 0)
		{

		}
	}

	{
		Tab <DWORD> smgrps = gm->GetActiveSmgrps();
		if (smgrps.Count() > 0)
		{

		}
	}

	// dump Face data
	if (gm->GetNumberOfFaces() > 0)
	{
		int count = gm->GetNumberOfFaces();
		mesh->CreateFaces(count);
		for (int i = 0; i < count; ++i)
		{
			Model::Face& face = mesh->GetFace(i);
			FaceEx* f = gm->GetFace(i);
			face.matID = f->matID;
			face.smGrp = f->smGrp;
			for (size_t i = 0; i < 3; i++)
			{
				face.vert[i] = f->vert[i];
				face.norm[i] = f->norm[i];
				face.edgeVis[i] = f->edgeVis[i];
			}
		}
	}

	{
		Tab<int> mapNums = gm->GetActiveMapChannelNum();
		if (mapNums.Count() > 0)
		{
			int count = mapNums.Count();
			for (int i = 0; i < count; ++i)
			{
				int mapID = mapNums[i];

				int vCount = gm->GetNumberOfMapVerts(mapID);
				for (int j = 0; j < vCount; ++j)
				{
					Point3 v;
					if (gm->GetMapVertex(mapID, j, v))
					{
					}
				}

				int fCount = gm->GetNumberOfFaces();
				for (int k = 0; k < fCount; ++k)
				{
					DWORD v[3];
					if (gm->GetMapFaceIndex(mapID, k, v))
					{

					}
				}
			}
		}
	}
Exit0:
	;
}
