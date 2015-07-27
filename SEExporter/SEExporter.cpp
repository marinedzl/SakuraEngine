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

void ConvertVector2(Vector2& dst, const Point3& src)
{
	dst.x = src.x;
	dst.y = src.y;
}

void ConvertVector3(Vector3& dst, const Point3& src)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}

IGameMaterial* GetMaterialById(IGameMaterial *mat, int id)
{
	for (int i = 0; i < mat->GetSubMaterialCount(); i++)
		if (mat->GetMaterialID(i) == id)
			return mat->GetSubMaterial(i);

	return NULL;
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
	void ExportMaterials();
	void DumpMesh(IGameNode* gn);
	void DumpMaterial(IGameMaterial* gMaterial);
	void ExtractVertices(FaceEx* gFace, IGameMesh* gMesh, ModelFile::Mesh* mesh);

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
	ModelFile::Model mModel;
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

	ExportMaterials();

	TSTR buf;
	int topLevelCount = pIgame->GetTopLevelNodeCount();
	for (int loop = 0; loop < topLevelCount; loop++)
	{
		IGameNode* pGameNode = pIgame->GetTopLevelNode(loop);
		if (pGameNode->IsTarget())
			continue;

		buf.printf(_T("Processing: %s"), pGameNode->GetName());
		GetCOREInterface()->ProgressUpdate((int)((float)loop / pIgame->GetTotalNodeCount() * 100.0f), FALSE, buf.data());

		ExportChildNode(pGameNode);
		pGameNode->ReleaseIGameObject();

		for (int i = 0; i < pGameNode->GetChildCount(); ++i)
		{
			IGameNode* child = pGameNode->GetNodeChild(i);
			if (child->IsTarget())
				continue;

			ExportChildNode(child);
			child->ReleaseIGameObject();
		}
	}

	pIgame->ReleaseIGame();

	TSTR path, file, ext;
	SplitFilename(TSTR(name), &path, &file, &ext);

	mModel.WriteFile(path, file);

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

void SEExporter::ExportMaterials()
{
	int matCount = pIgame->GetRootMaterialCount();
	for (int j = 0; j<matCount; j++)
		DumpMaterial(pIgame->GetRootMaterial(j));
}

void SEExporter::DumpMesh(IGameNode* gNode)
{
	CHECK(gNode);

	IGameMesh* gMesh = dynamic_cast<IGameMesh*>(gNode->GetIGameObject());
	CHECK(gMesh);

	gMesh->SetCreateOptimizedNormalList();
	CHECK(gMesh->InitializeData());

	DWORD vertexChannels = ModelFile::VertexChannel::Position | ModelFile::VertexChannel::Normal | ModelFile::VertexChannel::Tangent;

	if (gMesh->GetNumberOfMapVerts(1) > 0)
		vertexChannels |= ModelFile::VertexChannel::TexCoords0;

	if (gMesh->GetNumberOfMapVerts(2) > 0)
		vertexChannels |= ModelFile::VertexChannel::TexCoords1;

	if (gMesh->GetNumberOfMapVerts(3) > 0)
		vertexChannels |= ModelFile::VertexChannel::TexCoords2;

	IGameMaterial* mat = gNode->GetNodeMaterial();

	if (mat == NULL || !mat->IsMultiType())
	{
		ModelFile::Mesh* mesh = mModel.AddMesh(gNode->GetName());
		mesh->vertexChannels = vertexChannels;

		if (mat != NULL)
		{
			mesh->mtlName = mat->GetMaterialName();
		}
		else
			log(_T("no material found for %s\n"), gNode->GetName());

		int faceCount = gMesh->GetNumberOfFaces();
		mesh->vertices.reserve(faceCount * 3);
		for (int i = 0; i < faceCount; ++i)
			ExtractVertices(gMesh->GetFace(i), gMesh, mesh);
	}
	else
	{
		log(_T("node %s has multi type material %s\n"), gNode->GetName(), mat->GetMaterialName());

		Tab<int> matIds = gMesh->GetActiveMatIDs();

		for (int i = 0; i < matIds.Count(); ++i)
		{
			TSTR mtlName;
			TSTR meshName;

			IGameMaterial* subMat = GetMaterialById(mat, matIds[i]);
			if (subMat != NULL)
			{
				mtlName = subMat->GetMaterialName();
				meshName.printf(_T("%s_%s"), gNode->GetName(), subMat->GetMaterialName());
			}
			else
			{
				meshName.printf(_T("%s_%s"), gNode->GetName(), i);
			}

			ModelFile::Mesh* mesh = mModel.AddMesh(meshName);
			mesh->mtlName = mtlName;
			mesh->vertexChannels = vertexChannels;

			Tab<FaceEx*> gFaces = gMesh->GetFacesFromMatID(matIds[i]);

			int faceCount = gFaces.Count();
			mesh->vertices.reserve(faceCount * 3);
			for (int j = 0; j < faceCount; ++j)
			{
				ExtractVertices(gFaces[j], gMesh, mesh);
			}
		}
	}

Exit0:
	;
}

void SEExporter::ExtractVertices(FaceEx* gFace, IGameMesh* gMesh, ModelFile::Mesh* mesh)
{
	ModelFile::Vertex vert;
	GMatrix objectTM = gMesh->GetIGameObjectTM();
	int faceIndex = gFace->meshFaceIndex;

	for (int i = 0; i < 3; ++i)
	{
		ConvertVector3(vert.pos, gMesh->GetVertex(gFace->vert[i]));

		if (mesh->vertexChannels & ModelFile::VertexChannel::TexCoords0)
			ConvertVector2(vert.uv0, gMesh->GetMapVertex(1, gMesh->GetFaceTextureVertex(gFace->meshFaceIndex, i, 1)));

		if (mesh->vertexChannels & ModelFile::VertexChannel::TexCoords1)
			ConvertVector2(vert.uv1, gMesh->GetMapVertex(2, gMesh->GetFaceTextureVertex(gFace->meshFaceIndex, i, 2)));

		if (mesh->vertexChannels & ModelFile::VertexChannel::TexCoords2)
			ConvertVector2(vert.uv1, gMesh->GetMapVertex(3, gMesh->GetFaceTextureVertex(gFace->meshFaceIndex, i, 3)));

		Point3 normal = gMesh->GetNormal(faceIndex, i);

		Point3 a(objectTM.GetRow(0).x, objectTM.GetRow(0).y, objectTM.GetRow(0).z);
		Point3 b(objectTM.GetRow(1).x, objectTM.GetRow(1).y, objectTM.GetRow(1).z);
		Point3 c(objectTM.GetRow(2).x, objectTM.GetRow(2).y, objectTM.GetRow(2).z);

		Point3 p1 = CrossProd(a, b);
		if (DotProd(p1, c) < 0)
			normal = -normal;

		ConvertVector3(vert.norm, normal);

		int tangentIndex = gMesh->GetFaceVertexTangentBinormal(gFace->meshFaceIndex, i);
		ConvertVector3(vert.tan, gMesh->GetTangent(tangentIndex));

		mesh->vertices.push_back(vert);
	}
}

class TexturesSlotsHelper
{
private:
	std::map<int, const char*> slotNames;

	TexturesSlotsHelper()
	{
		slotNames[ID_AM] = "Ambient";
		slotNames[ID_DI] = "Diffuse";
		slotNames[ID_SP] = "Specular";
		slotNames[ID_SH] = "ShininesNs";
		slotNames[ID_SS] = "ShininessStrength";
		slotNames[ID_SI] = "SelfIllumination";
		slotNames[ID_OP] = "Opacity";
		slotNames[ID_FI] = "FilterColor";
		slotNames[ID_BU] = "Bump";
		slotNames[ID_RL] = "Reflection";
		slotNames[ID_RR] = "Refraction";
		slotNames[ID_DP] = "Displacement";
	}

public:
	static TexturesSlotsHelper& GetInstance()
	{
		static TexturesSlotsHelper instance;
		return instance;
	}

	const char* GetSlotName(int slotId)
	{
		std::map<int, const char *>::iterator it = slotNames.find(slotId);
		if (it != slotNames.end())
			return it->second;

		return NULL;
	}
};

void SEExporter::DumpMaterial(IGameMaterial* gMaterial)
{
	if (gMaterial->IsMultiType())
	{
		int subMatCount = gMaterial->GetSubMaterialCount();

		for (int i = 0; i < subMatCount; i++)
			DumpMaterial(gMaterial->GetSubMaterial(i));
		return;
	}

	ModelFile::Material* material = mModel.AddMaterial(gMaterial->GetMaterialName());

	Point3 ambient;
	Point3 diffuse;
	Point3 specular;
	Point3 emissive;

	gMaterial->GetAmbientData()->GetPropertyValue(ambient);
	gMaterial->GetDiffuseData()->GetPropertyValue(diffuse);
	gMaterial->GetSpecularData()->GetPropertyValue(specular);
	gMaterial->GetEmissiveData()->GetPropertyValue(emissive);

	gMaterial->GetOpacityData()->GetPropertyValue(material->opacity);
	gMaterial->GetGlossinessData()->GetPropertyValue(material->glossiness);
	gMaterial->GetSpecularLevelData()->GetPropertyValue(material->specularLevel);
	gMaterial->GetEmissiveAmtData()->GetPropertyValue(material->emissiveAmount);

	ConvertVector3(material->ambient, ambient);
	ConvertVector3(material->diffuse, diffuse);
	ConvertVector3(material->specular, specular);
	ConvertVector3(material->emissive, emissive);

	int texsCount = gMaterial->GetNumberOfTextureMaps();

	for (int i = 0; i < texsCount; i++)
	{
		IGameTextureMap* tex = gMaterial->GetIGameTextureMap(i);

		int texSlot = tex->GetStdMapSlot();

		if (texSlot != -1)
		{
			const char* slotName = TexturesSlotsHelper::GetInstance().GetSlotName(texSlot);
			const TCHAR* texFileName = tex->GetBitmapFileName();
			if (texFileName == NULL)
			{
				log(_T("error: no texture filename for slot %d, ensure to set it in 3d studio max"), texSlot);
			}
			else
			{
				TSTR path, file, ext;
				SplitFilename(TSTR(texFileName), &path, &file, &ext);
				file += ext;
				material->textures[slotName] = file;
			}
		}
	}
}
