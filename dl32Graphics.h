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
#include "dl32Window.h"

using namespace std;

#define DL32DEFAULTS_GRAPHICSCOLORDEPTH DL32CD_16BIT
#define DL32DEFAULTS_GRAPHICS_WINDOWED true
#define DL32DEFAULTS_GRAPHCS_TRIPLEBUFFER false 
#define DL32DEFAULTS_GRAPHICS_VSYNC false 
#define DL32DEFAULTS_GRAPHICS_REFRESHRATE 60

#define DL32CONSTS_GRAPHICS_MINZLEVEL -7
#define DL32CONSTS_GRAPHICS_MAXZLEVEL 7
#define DL32DEFAULTS_GRAPHICS_ZLEVELSCOUNT DL32CONSTS_GRAPHICS_MAXZLEVEL - DL32CONSTS_GRAPHICS_MINZLEVEL + 1
#define DL32DEFAULTS_GRAPHICS_ZLEVEL (DL32DEFAULTS_GRAPHICS_ZLEVELSCOUNT - 1) / 2
#define DL32MACROS_GRAPHICS_ZLEVELINDEX(zLevel) zLevel - DL32CONSTS_GRAPHICS_MINZLEVEL

#define DL32CONSTS_D3DVERTEX_Z 0.5
#define DL32CONSTS_D3DVERTEX_RHW 1

class dl32GraphicsClass;
typedef dl32GraphicsClass *PTDL32GRAPHICSCLASS;
 
typedef LPDIRECT3DDEVICE9 PTD3DDEVICE;
typedef void (*PTRENDERLOOPPROC)(PTDL32GRAPHICSCLASS);

struct dl32Vertex:dl32Point2D
{
	dl32Color color;
	int Z;

	dl32Vertex();
	dl32Vertex(float x, float y, dl32Color color,int Z=0);

	static dl32Point2D Baricenter(dl32Vertex PointList[],int PointCount);
};

typedef dl32Vertex *PTDL32VERTEX;

typedef dl32Vertex dl32Triangle[3];
typedef dl32Vertex dl32VertexTrapezoid[4];
typedef dl32Point2D dl322DPointTrapezoid[4];

typedef dl32Vertex dl32Pixel;

#define GRAPHICS_GetTrapezoidCenter(x) dl32Point2D((x[0].x+x[1].x+x[2].x+x[3].x)/4,(x[0].y+x[1].y+x[2].y+x[3].y)/4)

struct DL32VERTEXTEXTURED:public dl32Point3D
{
	float rhw;
	D3DCOLOR diffuse;
	D3DCOLOR specular;
	float tx,ty;

	DL32VERTEXTEXTURED();
	DL32VERTEXTEXTURED(const dl32Vertex &vertex);
	DL32VERTEXTEXTURED(dl32Point2D &point,int Z,dl32Color diffuse,dl32Color specular=0,float tx=-1,float ty=-1);
	DL32VERTEXTEXTURED(float x,float y,int z,D3DCOLOR diffuse,D3DCOLOR specular,float tx,float ty);
};

const DWORD DL32VERTEXTEXTURED_FVF = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1);
typedef DL32VERTEXTEXTURED *PTDL32VERTEXTEXTURED_VERTEX;
const int DL32VERTEXTEXTURED_SIZE=sizeof(DL32VERTEXTEXTURED);

class dl322DCamera:public dl32Transformation2D
{
public:
	dl322DCamera();
	dl322DCamera(dl32Matrix3x3 &Transformation);

	void SetPosition(float x=0,float y=0);
	void SetPosition(dl32Point2D &Position);
	dl32Point2D GetPosition();

	void Rotate(float Angle);
	void Rotate(dl32Point2D Center,float Rotation);
	void SetRotation(float Rotation=0);
	float GetRotation();

	void Traslate(float x,float y);
	void Traslate(dl32Vector2D Translation);
};

struct DL32TEXTURE
{
	LPDIRECT3DTEXTURE9 Texture;
	LPDIRECT3DSURFACE9 Surface;
	int OriginalWidth;
	int OriginalHeight;
	D3DCOLOR ColorKey;
	D3DFORMAT Format;
	int Index;

	DL32TEXTURE(){Texture=NULL;Surface=NULL;Index=-1;};
	bool LoadSurface();
	bool ReleaseSurface();
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
	dl32Mesh(dl32AABB2D Area,int width, int height,dl32MeshPatch *Patches=NULL,int SectorsCount=0);
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

	dl32Point2D GetPatchCenter(dl32MeshPatch &patch);
	dl32Point2D GetMeshCenter();

	void Transformation(dl32Transformation2D Transformation);
	void Transformation(dl32Transformation2D Transformation,dl32MeshPatch &patch);
	void Transformation(dl32Transformation2D Transformation,int PatchIndex);

	void SetColor(dl32Color color,dl32MeshPatch &patch);
};

struct dl32FontData
{
	int size;
	bool italic;
	bool underline;
	bool strikeout;
	dl32String name;
};

struct DL32TEXTDATA
{
	ID3DXFont *font;
	char* text;
	RECT rect;
	DWORD format;
	dl32Color color;

	DL32TEXTDATA(){font=NULL;text=NULL;};
};

struct dl32Pen
{
	float width;
	dl32Color color;
};

enum dl32TextAlign
{
	DL32TA_LEFT,
	DL32TA_RIGHT,
	DL32TA_UP,
	DL32TA_DOWN,
	DL32TA_UPLEFT,
	DL32TA_UPRIGHT,
	DL32TA_DOWNRIGHT,
	DL32TA_DOWNLEFT,
	DL32TA_CENTER
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
	DL32TEXTDATA *textdata;
	D3DPRIMITIVETYPE PrimitiveType;
	DL32RENDERBUFFERCALLTYPE CallType;

	DL32BUFFEROBJECT() {BaseIndex=-1;textdata=NULL;};
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

enum dl32ColorDepth
{
	DL32CD_16BIT,
	DL32CD_32BIT
};

class dl32GraphicsClass
{
private:
	bool _working;
protected:
	LPDIRECT3D9 _d3d;	
	LPDIRECT3DDEVICE9 _d3dDevice;
	D3DPRESENT_PARAMETERS _d3dPresentParameters;
	bool InitializeDirect3D(HWND hwnd,int Width,int Height,bool Windowed,dl32ColorDepth colorDepth, bool tripleBuffer, bool vSync, int refreshRate);

	bool _clearScene;
	bool _textDraw;
	D3DCOLOR _backColor;
	DL32RENDERSTATEDATA _renderData;

	void ResetRenderStates();
	void ResetBlendingStages();
	void ResetTextureStages();

	vector<DL32BUFFEROBJECT>* _renderBuffer[DL32CONSTS_GRAPHICS_MAXZLEVEL-DL32CONSTS_GRAPHICS_MINZLEVEL+1];
	vector<int> _renderBufferActiveLevels;
	bool ActivateRenderBufferLevel(int Index);

	vector<DL32VERTEXTEXTURED> _vertexBuffer;
	vector<int> _indexBuffer;
	IDirect3DVertexBuffer9* _d3dVertexBuffer;
	IDirect3DIndexBuffer9* _d3dIndexBuffer;
	bool _d3dVertexBufferOK;
	bool _d3dIndexBufferOK;
	bool _usingVertexBuffer;
	bool FillD3DBuffers();

	vector<DL32TEXTURE> _textures;
	vector<ID3DXFont*> _fonts;

	int _renderTarget;

	DWORD _lastTicksCount;
	int _frameRate;
	int _frameCount;
public:
	//Instanciamiento:
	//-----------------------------------------
	dl32GraphicsClass();
	dl32GraphicsClass(HWND hwnd,int Width,int Height,dl32ColorDepth colorDepth=DL32DEFAULTS_GRAPHICSCOLORDEPTH,
					  bool Windowed=DL32DEFAULTS_GRAPHICS_WINDOWED, bool tripleBuffer = DL32DEFAULTS_GRAPHCS_TRIPLEBUFFER,
					  bool vSync = DL32DEFAULTS_GRAPHICS_VSYNC, int refreshRate = DL32DEFAULTS_GRAPHICS_REFRESHRATE)
					  throw(dl32Direct3DInitFailedException);

	dl32GraphicsClass(dl32Window* window ,dl32ColorDepth colorDepth=DL32DEFAULTS_GRAPHICSCOLORDEPTH,
		bool Windowed=DL32DEFAULTS_GRAPHICS_WINDOWED, bool tripleBuffer = DL32DEFAULTS_GRAPHCS_TRIPLEBUFFER,
		bool vSync = DL32DEFAULTS_GRAPHICS_VSYNC, int refreshRate = DL32DEFAULTS_GRAPHICS_REFRESHRATE)
		throw(dl32Direct3DInitFailedException);

	~dl32GraphicsClass();

	dl322DCamera Camera;

	void Dispose();
	bool Start();
	bool Exit();
	//RenderProcs:
	//-----------------------------------------
	void DEVICE_SetPreDrawProc(PTRENDERLOOPPROC RenderProc);
	void DEVICE_SetPostDrawProc(PTRENDERLOOPPROC RenderProc);
	void DEVICE_RemovePreDrawProc();
	void DEVICE_RemovePostDrawProc();
	//RenderTargets:
	//----------------------------------------
	bool DEVICE_SetCanvas(int Texture=-1);
	bool DEVICE_SetDefaultCanvas(){return DEVICE_SetCanvas();};
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
	bool DRAW_Polygon(const dl32Point2D Verts[],int Count,dl32Color color,bool fill=true,int Z=0);
	bool DRAW_Polygon(vector<dl32Point2D> &Verts,dl32Color color,bool fill=true,int Z=0) {return DRAW_Polygon(Verts.data(),Verts.size(),color,fill,Z);};

	bool DRAW_Box(float x,float y,float width, float height,dl32Color color,bool fill=true,int Z=0);
	bool DRAW_Box(dl32Point2D position,float width,float height,dl32Color color,bool fill=true,int Z=0) {return DRAW_Box(position.x,position.y,width,height,color,fill,Z);};
	bool DRAW_Box(dl32Point2D position,dl32Vector2D dimensions,dl32Color color,bool fill=true,int Z=0) {return DRAW_Box(position.x,position.y,dimensions.x,dimensions.y,color,fill,Z);};
	bool DRAW_Box(dl32AABB2D box,dl32Color color,bool fill=true,int Z=0) {return DRAW_Box(box.Position.x,box.Position.y,box.GetWidth(),box.GetHeight(),color,fill,Z);};

	bool DRAW_Map(int texture,float x,float y,float width, float height,int Z=0);
	bool DRAW_Map(int texture,dl32Point2D position,float width,float height,int Z=0);
	bool DRAW_Map(int texture,dl32Point2D position,dl32Vector2D dimensions,int Z=0);
	bool DRAW_Map(int texture,dl32AABB2D Box,int Z=0);

	bool DRAW_VertexMap(int texture,const dl32VertexTrapezoid verts,int Z=0);

	bool DRAW_Mesh(dl32Mesh Mesh,int Z=0);

	bool DRAW_Text(int font,float x,float y,dl32String text,dl32Color color,dl32TextAlign align=DL32TA_UPLEFT,int Z=0);
	bool DRAW_Text(int font,dl32Point2D position,dl32String text,dl32Color color,dl32TextAlign align=DL32TA_UPLEFT,int Z=0){return DRAW_Text(font,position.x,position.y,text,color,align,Z);};

	bool DRAW_Pixel(float x, float y,dl32Color color,int Z=0);
	bool DRAW_Pixel(dl32Pixel pixel,int Z=0){return DRAW_Pixel(pixel.x,pixel.y,pixel.color,Z);};
	bool DRAW_Pixels(dl32Pixel pixels[],int count,int Z=0);
	bool DRAW_Pixels(dl32Point2D pixels[],dl32Color color,int count,int Z=0);
	bool DRAW_Pixels(dl32Color **pixels,float x,float y,int width,int height,int Z=0);

	bool DRAW_Lines(dl32Vertex points[],int Count,int Z=0);
	bool DRAW_Lines(dl32Point2D points[],int Count,dl32Color color,int Z=0);
	bool DRAW_Strip(dl32Pen pen,dl32Vertex points[],int Count,int texture=-1,int Z=0);
	bool DRAW_Strip(dl32Pen pen,dl32Point2D points[],int Count,int texture=-1,int Z=0);

	bool DRAW_Spline(dl32Spline* spline,dl32Color color,int PointsPerInterval=0,int Z=0);

	//Texturas:
	//-----------------------------------------
	int MAP_Load(dl32String Filename,dl32Color ColorKey=0xFFFFFFFF,bool Smooth=false,bool GrayScale=false);
	//Fuentes:
	//-----------------------------------------
	int FONT_LoadSystemFont(dl32String FontName,int size,bool bold,bool italic);
	//Información:
	//-----------------------------------------
	int FPS(){return _frameRate;};
};
#endif

