#ifndef GRAPHICS_H
#define GRAPHICS_H
#pragma once

#include <d3d9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include <d3dx9.h>

#include "dl32Math.h"
#include "dl32Color.h"
#include "dl32String.h"

using namespace std;

#define DL32GCDEFAULTS_WINDOWED true

#define DL32GCDEFAULTS_ZLEVEL 0
#define DL32GC_MINZLEVEL -7
#define DL32GC_MAXZLEVEL 7
#define DL32GC_TOTALLEVELS DL32GC_MAXZLEVEL-DL32GC_MINZLEVEL

#define DL32D3DVERTEX_ZVALUE 0.5
#define DL32D3DVERTEX_RHWVALUE 1

class dl32GraphicsClass;
typedef dl32GraphicsClass *PTDL32GRAPHICSCLASS;
 
typedef LPDIRECT3DDEVICE9 PTD3DDEVICE;
typedef void (*PTRENDERLOOPPROC)(PTDL32GRAPHICSCLASS);

struct dl32Vertex:dl322DPoint
{
	dl32Color color;
	int Z;

	dl32Vertex();
	dl32Vertex(float x, float y, dl32Color color,int Z=0);

	static dl322DPoint Baricenter(dl32Vertex PointList[],int PointCount);
};

typedef dl32Vertex *PTDL32VERTEX;

typedef dl32Vertex dl32Triangle[3];
typedef dl32Vertex dl32VertexTrapezoid[4];
typedef dl322DPoint dl322DPointTrapezoid[4];

#define GRAPHICS_GetTrapezoidCenter(x) dl322DPoint((x[0].x+x[1].x+x[2].x+x[3].x)/4,(x[0].y+x[1].y+x[2].y+x[3].y)/4)

struct DL32VERTEXTEXTURED:public dl323DPoint
{
	float rhw;
	D3DCOLOR diffuse;
	D3DCOLOR specular;
	float tx,ty;

	DL32VERTEXTEXTURED();
	DL32VERTEXTEXTURED(const dl32Vertex &vertex);
	DL32VERTEXTEXTURED(float x,float y,int z,D3DCOLOR diffuse,D3DCOLOR specular,float tx,float ty);
};

const DWORD DL32VERTEXTEXTURED_FVF = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1);
typedef DL32VERTEXTEXTURED *PTDL32VERTEXTEXTURED_VERTEX;
const int DL32VERTEXTEXTURED_SIZE=sizeof(DL32VERTEXTEXTURED);

class dl322DCamera:public dl322DTransform
{
public:
	dl322DCamera();
	dl322DCamera(dl323x3Matrix &Transform);

	void SetPosition(float x=0,float y=0);
	void SetPosition(dl322DPoint &Position);
	dl322DPoint GetPosition();

	void Rotate(float Angle);
	void Rotate(dl322DPoint Center,float Rotation);
	void SetRotation(float Rotation=0);
	float GetRotation();

	void Traslate(float x,float y);
	void Traslate(dl322DVector Traslation);
};

struct DL32TEXTURE
{
	LPDIRECT3DTEXTURE9 Texture;
	int OriginalWidth;
	int OriginalHeight;
	D3DCOLOR ColorKey;
	D3DFORMAT Format;
	int Index;
};

typedef DL32TEXTURE *PTDL32TEXTURE;

class dl32MeshPatch
{
private:

public:
	int texture;
	int x,y,width,height;
	dl32Color color;

	dl32MeshPatch(int texture=-1,int x=0,int y=0,int width=0,int height=0,dl32Color color=DL32COLOR_WHITE);
};

class dl32Mesh
{
	friend class dl32Mesh;
	friend class dl32GraphicsClass;
private:
	vector<DL32VERTEXTEXTURED> verts;
	vector<dl32MeshPatch> patches;
	vector<vector<int>> indexes;
	int width;
	int height;

	void ProcessPatches(dl32MeshPatch *PathesArray,int PatchesCount);
public:
	dl32Mesh();
	dl32Mesh(dl32Mesh &Mesh);
	dl32Mesh(dl32VertexTrapezoid Verts,int width,int height,dl32MeshPatch *Patches=NULL,int SectorsCount=0);
	dl32Mesh(dl322DAABB Area,int width, int height,dl32MeshPatch *Patches=NULL,int SectorsCount=0);
	~dl32Mesh();

	dl32Mesh& operator=(dl32Mesh &Mesh);

	dl32Vertex GetVertex(int x, int y);
	dl32MeshPatch& GetPatch(int Index);

	int GetWidth();
	int GetHeight();
	int GetPatchesCount();
	int GetVertexCount();

	bool IsValid(dl32MeshPatch &patch);
	bool AddPatch(dl32MeshPatch &patch);

	dl322DPoint GetPatchCenter(dl32MeshPatch &patch);
	dl322DPoint GetMeshCenter();

	void Transform(dl322DTransform transform);
	void Transform(dl322DTransform transform,dl32MeshPatch &patch);
	void Transform(dl322DTransform transform,int PatchIndex);

	void SetColor(dl32Color color,dl32MeshPatch &patch);
};

enum DL32RENDERBUFFERCALLTYPE
{
	RBCT_DRAWTRIANGLE,
	RBCT_DRAWTRAPEZOID,
	RBCT_DRAWPOLYGON,
	RBCT_DRAWBOX,
	RBCT_DRAWMAP,
	RBCT_DRAWVERTEXMAP,
	RBCT_MESH
};

struct DL32BUFFEROBJECT
{
	int StartIndex;
	int BaseIndex;
	int VertexCount;
	int PrimitiveCount;
	int Texture;
	D3DPRIMITIVETYPE PrimitiveType;
	DL32RENDERBUFFERCALLTYPE CallType;

	DL32BUFFEROBJECT() {BaseIndex=-1;};
	DL32BUFFEROBJECT(int StartIndex,int VertexCount,int PrimitiveCount,D3DPRIMITIVETYPE);
	DL32BUFFEROBJECT(int StartIndex,int VertexCount,int BaseIndex,int IndexCount,int PrimitiveCount,D3DPRIMITIVETYPE);
};

typedef struct DL32RENDERSTATEDATA
{
	int TextureIndex;
	LPDIRECT3DTEXTURE9 Texture;
};

typedef DL32RENDERSTATEDATA *PTDL32RENDERSTATEDATA;

enum DL32GRAPHICSCLASS_CTORERRORS
{
	DL32GCCE_NOERRORS,
	DL32GCCE_DEFAULTCTOR,
	DL32GCCE_D3DINITFAILED,
	DL32GCCE_CLASSRUNNING
};

class dl32GraphicsClass
{
private:
	bool Working;
	DL32GRAPHICSCLASS_CTORERRORS ConstructorError;
protected:
	LPDIRECT3D9 d3d;
	
	LPDIRECT3DDEVICE9 device;
	D3DPRESENT_PARAMETERS pp;

	vector<DL32BUFFEROBJECT>* RenderBuffer[DL32GC_MAXZLEVEL-DL32GC_MINZLEVEL+1];
	vector<int> RenderBufferActiveLevels;
	vector<DL32VERTEXTEXTURED> VertexBuffer;
	vector<int> IndexBuffer;
	IDirect3DVertexBuffer9 *d3dVertexBuffer;
	IDirect3DIndexBuffer9 *d3dIndexBuffer;
	bool d3dVertexBufferOK;
	bool d3dIndexBufferOK;
	bool UsingVertexBuffer;
	D3DCOLOR BackColor;
	vector<DL32TEXTURE> Textures;
	DL32RENDERSTATEDATA RenderData;

	PTRENDERLOOPPROC PreDrawProc;
	PTRENDERLOOPPROC PostDrawProc;

	bool InitializeDirect3D(HWND hwnd,int Width,int Height,bool Windowed=DL32GCDEFAULTS_WINDOWED);
	void WindowMessageLoop();
	bool FillD3DBuffers();
	bool ActivateRenderBufferLevel(int Index);
public:
	//Instanciamiento:
	//-----------------------------------------
	dl32GraphicsClass();
	dl32GraphicsClass(HWND hwnd,int Width,int Height,bool Windowed=DL32GCDEFAULTS_WINDOWED);
	~dl32GraphicsClass();

	dl322DCamera Camera;

	void Dispose();
	bool NoErrors();
	DL32GRAPHICSCLASS_CTORERRORS InitError();
	bool Start();
	bool Exit();
	//RenderProcs:
	//-----------------------------------------
	void DEVICE_SetPreDrawProc(PTRENDERLOOPPROC RenderProc);
	void DEVICE_SetPostDrawProc(PTRENDERLOOPPROC RenderProc);
	void DEVICE_RemovePreDrawProc();
	void DEVICE_RemovePostDrawProc();
	//Dibujo:
	// ----------------------------------------
	bool Frame();

	void DEVICE_SetBackgroundColor(dl32Color BackColor);
	dl32Color DEVICE_GetBackgroundColor();

	bool DRAW_Triangle(float x0, float y0, float x1,float y1, float x2, float y2,dl32Color color,bool fill=true,int Z=0);
	bool DRAW_Triangle(dl32Vertex V0, dl32Vertex V1, dl32Vertex V2,bool fill=true,int Z=0);
	bool DRAW_Triangle(const dl32Triangle Triangle,bool fill=true,int Z=0) {return DRAW_Triangle(Triangle[0],Triangle[1],Triangle[2],fill,Z);};

	bool DRAW_Trapezoid(const dl322DPointTrapezoid Trapezoid,dl32Color color,bool fill=true,int Z=0) {return DRAW_Polygon(Trapezoid,4,color,fill,Z);};
	bool DRAW_Trapezoid(const dl32VertexTrapezoid Trapezoid,bool fill=true,int Z=0) {return DRAW_Polygon(Trapezoid,4,fill,Z);};

	bool DRAW_Polygon(const dl32Vertex Verts[],int Count,bool fill=true,int Z=0);
	bool DRAW_Polygon(vector<dl32Vertex> &Verts,bool fill=true,int Z=0) {return DRAW_Polygon(Verts.data(),Verts.size(),fill,Z);};
	bool DRAW_Polygon(const dl322DPoint Verts[],int Count,dl32Color color,bool fill=true,int Z=0);
	bool DRAW_Polygon(vector<dl322DPoint> &Verts,dl32Color color,bool fill=true,int Z=0) {return DRAW_Polygon(Verts.data(),Verts.size(),color,fill,Z);};

	bool DRAW_Box(float x,float y,float width, float height,dl32Color color,bool fill=true,int Z=0);
	inline bool DRAW_Box(dl322DPoint position,float width,float height,dl32Color color,bool fill=true,int Z=0) {return DRAW_Box(position.x,position.y,width,height,color,fill,Z);};
	inline bool DRAW_Box(dl322DPoint position,dl322DVector dimensions,dl32Color color,bool fill=true,int Z=0) {return DRAW_Box(position.x,position.y,dimensions.x,dimensions.y,color,fill,Z);};
	inline bool DRAW_Box(dl322DAABB box,dl32Color color,bool fill=true,int Z=0) {return DRAW_Box(box.Position.x,box.Position.y,box.GetWidth(),box.GetHeight(),color,fill,Z);};

	bool DRAW_Map(int texture,float x,float y,float width, float height,int Z=0);
	bool DRAW_Map(int texture,dl322DPoint position,float width,float height,int Z=0);
	bool DRAW_Map(int texture,dl322DPoint position,dl322DVector dimensions,int Z=0);
	bool DRAW_Map(int texture,dl322DAABB Box,int Z=0);

	bool DRAW_VertexMap(int texture,const dl32VertexTrapezoid verts,int Z=0);

	bool DRAW_Mesh(dl32Mesh Mesh,int Z=0);
	//Texturas:
	//-----------------------------------------
	int MAP_Load(dl32String Filename,dl32Color ColorKey=0xFFFFFFFF,bool Smooth=false,bool GrayScale=false);
};
#endif

