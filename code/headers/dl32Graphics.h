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
#include "dl32Timing.h"

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

#define DL32CONSTS_d3dVertex_Z 0.5
#define DL32CONSTS_d3dVertex_RHW 1

const dl32Range DL32CONSTS_GRAPHICS_RENDERBUFFERRANGE = dl32Range(DL32CONSTS_GRAPHICS_MINZLEVEL,DL32CONSTS_GRAPHICS_MAXZLEVEL);

//DECLARACIÓN E IMPLEMENTACIÓN (INLINE) DE EXCEPCIONES:
///////////////////////////////////////////////////////
class dl32GraphicsException:public dl32Exception
{
public:
	dl32GraphicsException(char* message = DEFAULTEXCEPTIONMESSAGE(dl32GraphicsException)):dl32Exception(message){}
};

class dl32Direct3DInitFailedException:public dl32Exception
{
private:
	D3DPRESENT_PARAMETERS _presentParameters;
public:
	dl32Direct3DInitFailedException(D3DPRESENT_PARAMETERS presentParameters,char* message = DEFAULTEXCEPTIONMESSAGE(dl32Direct3DInitFailedException)):dl32Exception(message){}
	D3DPRESENT_PARAMETERS GetPresentParameters(){return _presentParameters;}
};

class dl32NotInitializedGraphicsException:public dl32GraphicsException
{
public:
	dl32NotInitializedGraphicsException(char* message = DEFAULTEXCEPTIONMESSAGE(dl32NotInitializedGraphicsException)):dl32GraphicsException(message){}
};

class dl32ZLevelOutOfRangeException:public dl32OutOfRangeException
{
public:
	dl32ZLevelOutOfRangeException(dl32Range range, int index, char* message = DEFAULTEXCEPTIONMESSAGE(dl32ZLevelOutOfRangeException)):dl32OutOfRangeException(range,index,message){}
	dl32ZLevelOutOfRangeException(int index, char* message = DEFAULTEXCEPTIONMESSAGE(dl32ZLevelOutOfRangeException)):dl32OutOfRangeException(DL32CONSTS_GRAPHICS_RENDERBUFFERRANGE,index,message){}
};

//CLASES DEL MÓDULO PROPIAMENTE DICHO:
//////////////////////////////////////

struct dl32Vertex:dl32Point2D
{
	dl32Color color;
	int Z;
	float tx,ty;

	dl32Vertex();
	dl32Vertex(float x, float y, dl32Color color,int Z=0,float tx = -1, float ty = -1);
	dl32Vertex(dl32Point2D point, dl32Color color, int Z=0,float tx = -1, float ty = -1){x=point.x;y=point.y;this->color=color;this->Z=Z;this->tx=tx;this->ty=ty;}

	static dl32Point2D Baricenter(dl32Vertex PointList[],int PointCount);
};

typedef dl32Vertex dl32Triangle[3];
typedef dl32Vertex dl32VertexTrapezoid[4];
typedef dl32Point2D dl322DPointTrapezoid[4];

typedef dl32Vertex dl32Pixel;

#define GRAPHICS_GetTrapezoidCenter(x) dl32Point2D((x[0].x+x[1].x+x[2].x+x[3].x)/4,(x[0].y+x[1].y+x[2].y+x[3].y)/4)

struct _d3dVertex:public dl32Point3D
{
	float rhw;
	dl32Color diffuse;
	dl32Color specular;
	float tx,ty;

	_d3dVertex();
	_d3dVertex(const dl32Vertex &vertex);
	_d3dVertex(const dl32Vertex &vertex,int Z,float tx = -1, float ty = -1,bool useVertexTextureCoordinates = false);
	_d3dVertex(const dl32Point2D &point,int Z,dl32Color diffuse,dl32Color specular=0,float tx=-1,float ty=-1);
	_d3dVertex(float x,float y,int z,dl32Color diffuse,dl32Color specular = DL32COLOR_BLACK,float tx = -1,float ty = -1);
};

const DWORD _d3dVertex_FVF = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1);
const int _d3dVertex_SIZE=sizeof(_d3dVertex);

class dl32Camera2D:public dl32Transformation2D
{
public:
	dl32Camera2D(){}
	dl32Camera2D(dl32Matrix3x3 &Transformation):dl32Transformation2D(Transformation){}

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

struct _d3dTexture
{
	LPDIRECT3DTEXTURE9 Texture;
	LPDIRECT3DSURFACE9 Surface;
	int OriginalWidth;
	int OriginalHeight;
	dl32Color ColorKey;
	D3DFORMAT Format;
	int Index;

	_d3dTexture(){Texture=NULL;Surface=NULL;Index=-1;}
	bool LoadSurface();
	bool ReleaseSurface();
};

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
	vector<_d3dVertex> verts;
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

	DL32TEXTDATA(){font=NULL;text=NULL;}
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
	RBCT_DRAWLINE,
	RBCT_DRAWPIXEL,
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

	DL32BUFFEROBJECT() {BaseIndex=-1;textdata=NULL;}
	DL32BUFFEROBJECT(int StartIndex,int VertexCount,int PrimitiveCount,D3DPRIMITIVETYPE);
	DL32BUFFEROBJECT(int StartIndex,int VertexCount,int BaseIndex,int IndexCount,int PrimitiveCount,D3DPRIMITIVETYPE);
};

struct DL32RENDERSTATEDATA
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

	dl32GraphicsClass(const dl32GraphicsClass&); //Así evitamos que se realicen copias
	dl32GraphicsClass& operator=(const dl32GraphicsClass&);//Así evitamos que se hagan asignaciones
protected:
	LPDIRECT3D9 _d3d;	
	LPDIRECT3DDEVICE9 _d3dDevice;
	D3DPRESENT_PARAMETERS _d3dPresentParameters;
	DWORD _bufferUsage;
	bool InitializeDirect3D(HWND hwnd,int Width,int Height,bool Windowed, bool tripleBuffer, bool vSync, int refreshRate);

	int _width,_height;

	bool _clearScene;
	bool _textDraw;
	dl32Color _backColor;
	DL32RENDERSTATEDATA _renderData;

	void ResetRenderStates();
	void ResetBlendingStages();
	void ResetTextureStages();

	vector<DL32BUFFEROBJECT> _renderBuffer;
	DL32BUFFEROBJECT &_lastObject;
	static DL32BUFFEROBJECT _mockObject;

	vector<_d3dVertex> _vertexBuffer;
	vector<int> _indexBuffer;
	IDirect3DVertexBuffer9* _d3dVertexBuffer;
	IDirect3DIndexBuffer9* _d3dIndexBuffer;
	bool _d3dVertexBufferOK;
	bool _d3dIndexBufferOK;
	bool _usingVertexBuffer;
	bool _FillD3DBuffers();
	void _applyCameraTransform(_d3dVertex* vertexBuffer);

	bool _cameraEnabled;

	vector<_d3dTexture> _textures;
	vector<ID3DXFont*> _fonts;

	int _renderTarget;

	DWORD _lastTicksCount;
	int _frameRate;
	int _frameCount;

	void _setup(HWND hwnd,int Width,int Height, bool Windowed, bool tripleBuffer, bool vSync, int refreshRate)throw(dl32Direct3DInitFailedException);
public:
	//Instanciamiento:
	//-----------------------------------------
	dl32GraphicsClass();
	dl32GraphicsClass(HWND hwnd,int Width,int Height,
					  bool Windowed=DL32DEFAULTS_GRAPHICS_WINDOWED, bool tripleBuffer = DL32DEFAULTS_GRAPHCS_TRIPLEBUFFER,
					  bool vSync = DL32DEFAULTS_GRAPHICS_VSYNC, int refreshRate = DL32DEFAULTS_GRAPHICS_REFRESHRATE) 
					  throw(dl32Direct3DInitFailedException) : _lastObject(_mockObject) {_setup(hwnd,Width,Height,Windowed,tripleBuffer,vSync,refreshRate);} //ERROR: NO DEBERÍA SER INLINE 

	dl32GraphicsClass(dl32Window* window ,
					  bool Windowed=DL32DEFAULTS_GRAPHICS_WINDOWED, bool tripleBuffer = DL32DEFAULTS_GRAPHCS_TRIPLEBUFFER,
					  bool vSync = DL32DEFAULTS_GRAPHICS_VSYNC, int refreshRate = DL32DEFAULTS_GRAPHICS_REFRESHRATE)
					  throw(dl32Direct3DInitFailedException) : _lastObject(_mockObject) {_lastObject = DL32BUFFEROBJECT();_setup(window->GetHwnd(),window->GetWidth(),window->GetHeight(),Windowed,tripleBuffer,vSync,refreshRate);} //ERROR: NO DEBERÍA SER INLINE 
	
	~dl32GraphicsClass();

	//Cámara:
	//----------------------------------------
	dl32Camera2D Camera;
	bool CAMERA_IsEnabled(){return _cameraEnabled;}
	void CAMERA_Enable(){_cameraEnabled = true;}
	void CAMERA_Disable(){_cameraEnabled = false;}

	void Dispose();
	bool Start();
	bool Exit();
	//RenderTargets:
	//----------------------------------------
	bool DEVICE_SetCanvas(int Texture=-1);
	bool DEVICE_SetDefaultCanvas(){return DEVICE_SetCanvas();}
	//Dibujo:
	// ----------------------------------------
	void Frame()throw(dl32NotInitializedGraphicsException);

	void DEVICE_SetBackgroundColor(dl32Color BackColor);
	dl32Color DEVICE_GetBackgroundColor();

	void DRAW_Line(dl32Point2D P1, dl32Point2D P2, dl32Color color, int Z=0)throw(dl32NotInitializedGraphicsException);
	void DRAW_Line(dl32Vertex V1, dl32Vertex V2,int Z=0)throw(dl32NotInitializedGraphicsException);
	void DRAW_Lines(dl32Vertex points[],int Count,int Z=0)throw(dl32NotInitializedGraphicsException);
	void DRAW_Lines(dl32Point2D points[],int Count,dl32Color color,int Z=0)throw(dl32NotInitializedGraphicsException);

	void DRAW_Polyline(dl32Point2D points[],int Count,dl32Color color,float width, bool fill = true,int Z=0)throw(dl32NotInitializedGraphicsException);
	void DRAW_Polyline(dl32Vertex vertices[],int Count,float width, bool fill = true,int Z=0)throw(dl32NotInitializedGraphicsException);

	void DRAW_Triangle(float x0, float y0, float x1,float y1, float x2, float y2,dl32Color color,bool fill=true,int Z=0)throw(dl32NotInitializedGraphicsException);
	void DRAW_Triangle(dl32Vertex V0, dl32Vertex V1, dl32Vertex V2,bool fill=true,int Z=0)throw(dl32NotInitializedGraphicsException);
	void DRAW_Triangle(dl32Point2D V0, dl32Point2D V1, dl32Point2D V2,dl32Color color,bool fill=true,int Z=0)throw(dl32NotInitializedGraphicsException);
	void DRAW_Triangle(const dl32Triangle Triangle,bool fill=true,int Z=0)throw(dl32NotInitializedGraphicsException) {DRAW_Triangle(Triangle[0],Triangle[1],Triangle[2],fill,Z);}

	void DRAW_TriangleStrip(dl32Point2D points[], int pointsCount, dl32Color color, bool fill = true, int Z=0)throw(dl32NotInitializedGraphicsException);
	void DRAW_TriangleStrip(dl32Vertex points[], int pointsCount, bool fill = true, int Z=0)throw(dl32NotInitializedGraphicsException);

	void DRAW_Trapezoid(const dl322DPointTrapezoid Trapezoid,dl32Color color,bool fill=true,int Z=0)throw(dl32NotInitializedGraphicsException) {DRAW_Polygon(Trapezoid,4,color,fill,Z);}
	void DRAW_Trapezoid(const dl32VertexTrapezoid Trapezoid,bool fill=true,int Z=0)throw(dl32NotInitializedGraphicsException) {DRAW_Polygon(Trapezoid,4,fill,Z);}

	void DRAW_Polygon(const dl32Vertex Verts[],int Count,bool fill=true,int Z=0)throw(dl32NotInitializedGraphicsException);
	void DRAW_Polygon(vector<dl32Vertex> &Verts,bool fill=true,int Z=0)throw(dl32NotInitializedGraphicsException) {DRAW_Polygon(Verts.data(),Verts.size(),fill,Z);}
	void DRAW_Polygon(const dl32Point2D Verts[],int Count,dl32Color color,bool fill=true,int Z=0)throw(dl32NotInitializedGraphicsException);
	void DRAW_Polygon(vector<dl32Point2D> &Verts,dl32Color color,bool fill=true,int Z=0)throw(dl32NotInitializedGraphicsException) {DRAW_Polygon(Verts.data(),Verts.size(),color,fill,Z);}

	void DRAW_Box(float x,float y,float width, float height,dl32Color color,bool fill=true,int Z=0)throw(dl32NotInitializedGraphicsException);
	void DRAW_Box(dl32Point2D position,float width,float height,dl32Color color,bool fill=true,int Z=0)throw(dl32NotInitializedGraphicsException) {DRAW_Box(position.x,position.y,width,height,color,fill,Z);}
	void DRAW_Box(dl32Point2D position,dl32Vector2D dimensions,dl32Color color,bool fill=true,int Z=0)throw(dl32NotInitializedGraphicsException) {DRAW_Box(position.x,position.y,dimensions.x,dimensions.y,color,fill,Z);}
	void DRAW_Box(dl32AABB2D box,dl32Color color,bool fill=true,int Z=0)throw(dl32NotInitializedGraphicsException) {DRAW_Box(box.Position.x,box.Position.y,box.GetWidth(),box.GetHeight(),color,fill,Z);}

	void DRAW_Map(int texture,float x,float y,float width, float height,int Z=0)throw(dl32NotInitializedGraphicsException);
	void DRAW_Map(int texture,dl32Point2D position,float width,float height,int Z=0)throw(dl32NotInitializedGraphicsException);
	void DRAW_Map(int texture,dl32Point2D position,dl32Vector2D dimensions,int Z=0)throw(dl32NotInitializedGraphicsException);
	void DRAW_Map(int texture,dl32AABB2D Box,int Z=0)throw(dl32NotInitializedGraphicsException);

	void DRAW_VertexMap(int texture,const dl32VertexTrapezoid verts,int Z=0,bool useCustomTextureCoordinates = false)throw(dl32NotInitializedGraphicsException);

	void DRAW_Mesh(dl32Mesh Mesh,int Z=0);

	void DRAW_Text(int font,float x,float y,dl32String text,dl32Color color,dl32TextAlign align=DL32TA_UPLEFT,int Z=0)throw(dl32NotInitializedGraphicsException);
	void DRAW_Text(int font,dl32Point2D position,dl32String text,dl32Color color,dl32TextAlign align=DL32TA_UPLEFT,int Z=0)throw(dl32NotInitializedGraphicsException) {DRAW_Text(font,position.x,position.y,text,color,align,Z);}

	void DRAW_Pixel(float x, float y,dl32Color color,int Z=0)throw(dl32NotInitializedGraphicsException);
	void DRAW_Pixel(dl32Pixel pixel,int Z=0)throw(dl32NotInitializedGraphicsException){DRAW_Pixel(pixel.x,pixel.y,pixel.color,Z);}
	void DRAW_Pixels(dl32Pixel pixels[],int count,int Z=0)throw(dl32NotInitializedGraphicsException);
	void DRAW_Pixels(dl32Point2D pixels[],dl32Color color,int count,int Z=0)throw(dl32NotInitializedGraphicsException);
	void DRAW_Pixels(dl32Color **pixels,float x,float y,int width,int height,int Z=0)throw(dl32NotInitializedGraphicsException);

	void DRAW_Strip(dl32Pen pen,dl32Vertex points[],int Count,int texture=-1,int Z=0)throw(dl32NotInitializedGraphicsException);
	void DRAW_Strip(dl32Pen pen,dl32Point2D points[],int Count,int texture=-1,int Z=0)throw(dl32NotInitializedGraphicsException);

	void DRAW_Spline(dl32Spline* spline,dl32Color color,int PointsPerInterval=0,int Z=0);

	//Texturas:
	//-----------------------------------------
	int MAP_Load(dl32String Filename,dl32Color ColorKey=DL32COLOR_WHITE,bool Smooth=false,bool GrayScale=false);
	//Fuentes:
	//-----------------------------------------
	int FONT_LoadSystemFont(dl32String FontName,int size,bool bold,bool italic);
	//Información:
	//-----------------------------------------
	int FPS(){return _frameRate;}
};
#endif

