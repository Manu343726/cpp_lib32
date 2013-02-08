#include "dl32Graphics.h"

using namespace std;

wstring STRING_WSTRING(const string& s);
LPCWSTR STRING_LPCWSTR(const string& s);

struct PolylineJoint
{
	dl32Point2D Up;
	dl32Point2D Down;

	PolylineJoint(){};

	PolylineJoint(dl32Point2D up, dl32Point2D down)
	{
		Up=up;
		Down=down;
	};
};

void ComputePolyline(dl32Point2D points[],int pointCount,float width,PolylineJoint* &out);
void ParseJointsToVertexBuffer(PolylineJoint joints[],int JointCount,dl32Point2D* &out);
void ParseJointsToVertexBuffer(PolylineJoint joints[],dl32Vertex colors[],int JointCount,dl32Vertex* &out);
dl32Point2D* vertexArrayToPoint2D(dl32Vertex vertexArray[],int Count);

wstring STRING_WSTRING(const string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

LPCWSTR STRING_LPCWSTR(const string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r.c_str();
}

_d3dVertex::_d3dVertex()
{
	z=DL32CONSTS_d3dVertex_Z;rhw=DL32CONSTS_d3dVertex_RHW;
	diffuse=DL32COLOR_WHITE;specular=DL32COLOR_BLACK;
	tx=-1;ty=-1;
}

_d3dVertex::_d3dVertex(float x,float y,int z,dl32Color diffuse,dl32Color specular,float tx,float ty)
{
	this->x=x;this->y=y;this->z=(DL32MACROS_GRAPHICS_ZLEVELINDEX(z)+1)*DL32CONSTS_d3dVertex_Z;
	this->rhw=DL32CONSTS_d3dVertex_RHW;
	this->diffuse=diffuse;this->specular=specular;
	this->tx=tx;this->ty=ty;
}

_d3dVertex::_d3dVertex(const dl32Vertex &vertex)
{
	x=vertex.x;y=vertex.y;z=(DL32MACROS_GRAPHICS_ZLEVELINDEX(vertex.Z)+1)*DL32CONSTS_d3dVertex_Z;
	rhw=DL32CONSTS_d3dVertex_RHW;
	diffuse=vertex.color;specular=DL32COLOR_BLACK;
	tx=-1;ty=-1;
}

_d3dVertex::_d3dVertex(const dl32Vertex &vertex,int Z,float tx,float ty)
{
	x=vertex.x;y=vertex.y;z=(DL32MACROS_GRAPHICS_ZLEVELINDEX(Z)+1)*DL32CONSTS_d3dVertex_Z;
	rhw=DL32CONSTS_d3dVertex_RHW;
	diffuse=vertex.color;specular=DL32COLOR_BLACK;
	this->tx = tx;
	this->ty = ty;
}

_d3dVertex::_d3dVertex(const dl32Point2D &point,int Z,dl32Color diffuse,dl32Color specular,float tx,float ty)
{
	x=point.x;
	y=point.y;
	z=(DL32MACROS_GRAPHICS_ZLEVELINDEX(Z)+1)*DL32CONSTS_d3dVertex_Z;
	this->diffuse=diffuse;
	this->specular=specular;
	rhw=DL32CONSTS_d3dVertex_RHW;
	this->tx=tx;
	this->ty=ty;
}

bool _d3dTexture::LoadSurface()
{
	if(this->Surface==NULL)
		if(FAILED(this->Texture->GetSurfaceLevel(0,&Surface)))
			return false;
		else
			return true;
	else
		return false;
}

bool _d3dTexture::ReleaseSurface()
{
	if(Surface!=NULL)
	{		
		delete Surface;
		Surface=NULL;

		return true;
	}
	else
		return false;
}

dl32Vertex::dl32Vertex()
{
	x=0;
	y=0;
	Z=0;
	color=0;
}

dl32Vertex::dl32Vertex(float x, float y, dl32Color color,int Z)
{
	this->x=x;
	this->y=y;
	this->Z=Z;
	this->color=color;
}

dl32Point2D dl32Vertex::Baricenter(dl32Vertex PointList[],int PointCount)
{
	dl32Point2D Return;

	for(int i=0;i<PointCount;++i)
	{
		/*Return.x+=(PointList+i*TypeSize)->x;
		Return.y+=(PointList+i*TypeSize)->y;*/
		Return.x+=PointList[i].x;
		Return.y+=PointList[i].y;
	}

	return dl32Point2D(Return.x/PointCount,Return.y/PointCount);
}

void dl322DCamera::Rotate(float Rotation)
{
	*this=dl32Matrix3x3::Mul(dl32Transformation2D::Rotation(Rotation),*this);
}

void dl322DCamera::Rotate(dl32Point2D Center,float Rotation)
{
	*this=dl32Matrix3x3::Mul(dl32Transformation2D::Rotation(Center,Rotation),*this);
}

void dl322DCamera::Traslate(float x,float y)
{
	*this=dl32Matrix3x3::Mul(dl32Transformation2D::Translation(-x,-y),*this);
}

void dl322DCamera::Traslate(dl32Vector2D Translation)
{
	*this=dl32Matrix3x3::Mul(dl32Transformation2D::Translation(-Translation.x,-Translation.y),*this);
}

void dl322DCamera::SetPosition(float x, float y)
{
	m13=-x;
	m23=-y;
}

void dl322DCamera::SetPosition(dl32Point2D &Position)
{
	m13=-Position.x;
	m23=-Position.y;
}

dl32Point2D dl322DCamera::GetPosition()
{
	return dl32Point2D(-m13,-m23);
}

dl32MeshPatch::dl32MeshPatch(int texture,int x,int y,int width,int height,dl32Color color)
{
	this->texture=texture;
	this->x=x;
	this->y=y;
	this->width=width;
	this->height=height;
	this->color=color;
}

void dl32Mesh::ProcessPatches(dl32MeshPatch *PatchesArray,int PatchesCount)
{
	int offset=indexes.size();

	for(int i=0;i<PatchesCount;++i)
		for(int k=PatchesArray[i].y;k<PatchesArray[i].y+PatchesArray[i].height;++k)
			for(int j=PatchesArray[i].x;j<PatchesArray[i].x+PatchesArray[i].width;++j)
			{
				verts[k*width+j].diffuse=COLOR_Blend(verts[k*width+j].diffuse,PatchesArray[i].color);
				verts[k*width+j].tx=j/float(PatchesArray[i].width-1);
				verts[k*width+j].ty=k/float(PatchesArray[i].height-1);

				if(j<PatchesArray[i].x+PatchesArray[i].width-1 && k<PatchesArray[i].y+PatchesArray[i].height-1)
				{
					if(j==PatchesArray[i].x)
						indexes[offset-PatchesCount+i].push_back((k+1)*width+j);

					indexes[offset-PatchesCount+i].push_back(k*width+j);
					indexes[offset-PatchesCount+i].push_back((k+1)*width+j+1);

					if(j==PatchesArray[i].x+PatchesArray[i].width-2)
						indexes[offset-PatchesCount+i].push_back(k*width+j+1);
				}
			}
}

dl32Mesh::dl32Mesh()
{
	width=0;
	height=0;
}

dl32Mesh::dl32Mesh(dl32Mesh &Mesh)
{
	if(this!=&Mesh)
	{
		int VertexCount=Mesh.width*Mesh.height;
		width=Mesh.width;
		height=Mesh.height;
		verts=Mesh.verts;
		patches=Mesh.patches;
		indexes=Mesh.indexes;
	}
}

dl32Mesh::dl32Mesh(dl32AABB2D Area,int width, int height,dl32MeshPatch *patches,int PatchesCount)
{
	int areawidth,areaheight,stepx,stepy;

	this->width=width;
	this->height=height;
	this->verts=vector<_d3dVertex>(width*height);

	areawidth=Area.GetWidth();
	areaheight=Area.GetHeight();

	stepx=areawidth/(width-1);
	stepy=areaheight/(height-1);

	for(int i=0;i<width;++i)
		for(int j=0;j<height;++j)
			verts[i+j*width]=_d3dVertex(Area.Position.x+i*stepx,Area.Position.y+j*stepy,0,DL32COLOR_BLACK,0,-1,-1);

	if(patches!=NULL && PatchesCount>0)
	{
		this->patches.assign(patches,patches+PatchesCount);
		this->indexes=vector<vector<int>>(PatchesCount);
		ProcessPatches(patches,PatchesCount);
	}
}

dl32Mesh::~dl32Mesh()
{
	width=0;
	height=0;
}

dl32Mesh& dl32Mesh::operator=(dl32Mesh &Mesh)
{
	if(this!=&Mesh)
	{
		int VertexCount=Mesh.width*Mesh.height;
		width=Mesh.width;
		height=Mesh.height;
		verts=Mesh.verts;
		patches=Mesh.patches;
		indexes=Mesh.indexes;
	}

	return *this;
}

bool dl32Mesh::AddPatch(dl32MeshPatch &patch)
{
	if(IsValid(patch))
	{
		indexes.push_back(vector<int>());
		patches.push_back(patch);
		ProcessPatches(&patch,1);
		return true;
	}
	else
		return false;
}

#if DL32FLAGS_CHECKARRAYINDEXES
dl32Vertex dl32Mesh::GetVertex(int x, int y)
{
	if(x>=0 && x<width && y>=0 && y<height)
		return dl32Vertex(verts[x+y*width].x,verts[x+y*width].y,verts[x+y*width].diffuse);
	else
		return dl32Vertex();
}

dl32MeshPatch& dl32Mesh::GetPatch(int Index)
{
	if(Index>=0 && (unsigned int)Index<patches.size())
		return patches[Index];
	else
		return dl32MeshPatch();
}
#else
dl32Vertex dl32Mesh::GetVertex(int x, int y)
{
	return dl32Vertex(verts[x+y*width].x,verts[x+y*width].y,verts[x+y*width].diffuse);
}

dl32MeshPatch& dl32Mesh::GetPatch(int Index)
{
	return &patches[Index];
}
#endif

int dl32Mesh::GetWidth()
{
	return width;
}

int dl32Mesh::GetHeight()
{
	return height;
}

int dl32Mesh::GetVertexCount()
{
	return width*height;
}

int dl32Mesh::GetPatchesCount()
{
	return patches.size();
}

bool dl32Mesh::IsValid(dl32MeshPatch &patch)
{
	return patch.x>=0 && patch.x+patch.width<=width && patch.y>=0 && patch.y+patch.height<=height;
}

dl32Point2D dl32Mesh::GetPatchCenter(dl32MeshPatch &patch)
{
	if(IsValid(patch))
	{
		dl32Point2D center;
		int vertexcount=patch.width*patch.height;

		for(int i=patch.x;i<patch.x+patch.width;++i)
			for(int j=patch.y;j<patch.y+patch.height;++j)
			{
				center.x+=verts[i+j*width].x;
				center.y+=verts[i+j*width].y;
			}

			return dl32Point2D(center.x/vertexcount,center.y/vertexcount);
	}
	else
		return dl32Point2D();
}

dl32Point2D dl32Mesh::GetMeshCenter()
{
	dl32Point2D Center;

	for(int i=0;i<width*height;++i)
	{
		Center.x+=verts[i].x;
		Center.y+=verts[i].y;
	}

	return dl32Point2D(Center.x/(width*height),Center.y/(width*height));
}

void dl32Mesh::Transformation(dl32Transformation2D Transformation)
{
	for(unsigned int i=0;i<verts.size();++i)
		Transformation.Apply(&verts[i].x,&verts[i].y);
}

void dl32Mesh::Transformation(dl32Transformation2D Transformation,dl32MeshPatch &patch)
{
	if(IsValid(patch))
	{
		for(int i=patch.x;i<patch.x+patch.width;++i)
			for(int j=patch.y;j<patch.y+patch.height;++j)
				Transformation.Apply(&verts[i+width*j].x,&verts[i+width*j].y);
	}
}

void dl32Mesh::SetColor(dl32Color color,dl32MeshPatch &patch)
{
	if(IsValid(patch))
	{
		patch.color=color;

		for(int i=patch.y;i<patch.y+patch.height;++i)
			for(int j=patch.x;j<patch.x+patch.width;++j)
				verts[i*width+j].diffuse=color;
	}
}

DL32BUFFEROBJECT::DL32BUFFEROBJECT(int StartIndex,int VertexCount,int PrimitiveCount,D3DPRIMITIVETYPE)
{
	this->StartIndex=StartIndex;
	this->VertexCount=VertexCount;
	this->BaseIndex=-1;
	this->PrimitiveCount=PrimitiveCount;
	this->PrimitiveType=PrimitiveType;
}

void dl32GraphicsClass::ResetRenderStates()
{
	_d3dDevice->SetFVF(_d3dVertex_FVF);

	_d3dDevice->SetRenderState(D3DRS_LIGHTING, 0);
	_d3dDevice->SetRenderState(D3DRS_SPECULARENABLE, 1);
	_d3dDevice->SetRenderState(D3DRS_CULLMODE, 0);

	_d3dDevice->SetRenderState(D3DRS_LASTPIXEL, 1);

	_d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
	_d3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, 1);
	_d3dDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	_d3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	_d3dDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS,1);
}

void dl32GraphicsClass::ResetBlendingStages()
{
	_d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	_d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void dl32GraphicsClass::ResetTextureStages()
{
	_d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	_d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	_d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	_d3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	_d3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	_d3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
}

void dl32GraphicsClass::Frame() throw(dl32NotInitializedGraphicsException)
{
	if(!_working) throw  dl32NotInitializedGraphicsException();

	DL32BUFFEROBJECT Object;
	DWORD Ticks;
	int RenderBufferLevelSize,ActiveLevelsSize=0;
	int Index;

	_d3dDevice->Clear(0,NULL,D3DCLEAR_TARGET,_backColor,0,0);
	_d3dDevice->BeginScene();

	if(_FillD3DBuffers() || _textDraw)
	{
		ResetTextureStages();
		ResetBlendingStages();

		for(int j=0;j<_renderBuffer.size();++j)
		{
			Object=_renderBuffer.at(j);

			if(Object.textdata==NULL)
			{
				if(Object.Texture!=_renderData.TextureIndex)
				{
					if(Object.Texture<0)
					{
						_renderData.Texture=NULL;
						_renderData.TextureIndex=-1;
						_d3dDevice->SetTexture(0,NULL);
					}
					else
					{
						_renderData.TextureIndex=Object.Texture;
						_renderData.Texture=_textures[Object.Texture].Texture;

						_d3dDevice->SetTexture(0,_textures[Object.Texture].Texture);
					}
				}

				if(Object.BaseIndex<0)
					_d3dDevice->DrawPrimitive(Object.PrimitiveType,Object.StartIndex,Object.PrimitiveCount);
				else
					_d3dDevice->DrawIndexedPrimitive(Object.PrimitiveType,Object.BaseIndex,0,Object.VertexCount,Object.StartIndex,Object.PrimitiveCount);
			}
			else
			{
				Object.textdata->font->DrawText(NULL,Object.textdata->text,-1,&Object.textdata->rect,Object.textdata->format,Object.textdata->color);
				delete Object.textdata;
			}
		}

		_renderBuffer.clear();
	}

	_d3dDevice->EndScene();
	_d3dDevice->Present(NULL,NULL,NULL,NULL);

	if(_d3dVertexBufferOK) _d3dVertexBuffer->Release();
	if(_d3dIndexBufferOK) _d3dIndexBuffer->Release();
	_vertexBuffer.clear();
	_indexBuffer.clear();

	_textDraw=false;

	Ticks=GetTickCount();
	if(Ticks-_lastTicksCount>=1000)
	{
		_lastTicksCount=Ticks;
		_frameRate=_frameCount;
		_frameCount=0;
	}
	else
		_frameCount++;
}

bool dl32GraphicsClass::InitializeDirect3D(HWND hwnd,int Width,int Height,bool Windowed, bool tripleBuffer, bool vSync, int refreshRate)
{
	D3DDISPLAYMODE displayMode;

	ZeroMemory(&_d3dPresentParameters,sizeof(D3DPRESENT_PARAMETERS));
	_d3d=Direct3DCreate9(D3D_SDK_VERSION);
	_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&displayMode);

	if(vSync)
		_d3dPresentParameters.PresentationInterval=D3DPRESENT_INTERVAL_ONE;
	else
		_d3dPresentParameters.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;

	_d3dPresentParameters.Windowed=BOOL(Windowed);
	_d3dPresentParameters.hDeviceWindow=hwnd;
	_d3dPresentParameters.BackBufferFormat=displayMode.Format;
	_d3dPresentParameters.BackBufferWidth=Width;
	_d3dPresentParameters.BackBufferHeight=Height;


	_d3dPresentParameters.SwapEffect=D3DSWAPEFFECT_DISCARD;
	_d3dPresentParameters.Windowed=BOOL(Windowed);
	_d3dPresentParameters.hDeviceWindow=hwnd;

	if(!Windowed)
		_d3dPresentParameters.BackBufferCount=2+abs(tripleBuffer);

	if(!FAILED(_d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,D3DCREATE_HARDWARE_VERTEXPROCESSING,&_d3dPresentParameters, &_d3dDevice)) || 
		!FAILED(_d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&_d3dPresentParameters, &_d3dDevice)))
	{
		ResetRenderStates();
		return true;
	}
	else
		return false;
}

dl32GraphicsClass::dl32GraphicsClass()
{
	_working=false;
	_usingVertexBuffer=false;
	_cameraEnabled = false;
}

void dl32GraphicsClass::_setup(HWND hwnd,int Width,int Height, bool Windowed, bool tripleBuffer, bool vSync, int refreshRate)throw(dl32Direct3DInitFailedException)
{
	_working=false;
	_usingVertexBuffer=false;

	dl32Vector2D windowSize = dl32Window::GetWindowSize(Width,Height,!Windowed);
	SetWindowPos(hwnd,NULL,0,0,(int)windowSize.x,(int)windowSize.y,SWP_NOMOVE | SWP_NOZORDER);

	if(!InitializeDirect3D(hwnd,Width,Height,Windowed,tripleBuffer,vSync,refreshRate))
		throw dl32Direct3DInitFailedException(_d3dPresentParameters);
	else
	{
		_backColor=DL32COLOR_BLACK;

		_lastTicksCount=GetTickCount();
		_frameCount=0;
		_frameRate=0;

		_cameraEnabled = false;

		this->Start();

		_working=true;
	}
}

bool dl32GraphicsClass::Exit()
{
	Dispose();
	return true;
}

void dl32GraphicsClass::Dispose()
{
	if(_working)
	{
		_working=false;

		if(_usingVertexBuffer && _d3dVertexBuffer!=NULL) _d3dVertexBuffer->Release();
		_d3dDevice->Release();
		_d3d->Release();
	}
}

dl32GraphicsClass::~dl32GraphicsClass()
{
	if(_working)
	{
		_working=false;

		if(_d3dVertexBufferOK && _d3dVertexBuffer!=NULL) _d3dVertexBuffer->Release();
		if(_d3dIndexBufferOK && _d3dIndexBuffer!=NULL) _d3dIndexBuffer->Release();

		_d3dDevice->Release();
		_d3d->Release();
	}
}

bool dl32GraphicsClass::Start()
{
	_renderData.TextureIndex=-1;
	_renderData.Texture=NULL;
	_working=true;
	_textDraw=false;

	return true;
}

void dl32GraphicsClass::DEVICE_SetBackgroundColor(dl32Color NewBackColor)
{
	_backColor=NewBackColor;
}

dl32Color dl32GraphicsClass::DEVICE_GetBackgroundColor()
{
	return _backColor;
}

void dl32GraphicsClass::_applyCameraTransform(_d3dVertex* vertexBuffer)
{
	for(unsigned int i=0;i<_vertexBuffer.size();++i)
		Camera.Apply(&vertexBuffer[i].x,&vertexBuffer[i].y);
}

bool dl32GraphicsClass::_FillD3DBuffers()
{
	void *VertexBufferArray,*IndexBufferArray;
	_d3dVertex* vertexBufferData = _vertexBuffer.data();
	bool VertexBufferFailed;
	_d3dVertexBufferOK=false;
	_d3dIndexBufferOK=false;

	VertexBufferFailed=FAILED(_d3dDevice->CreateVertexBuffer(_d3dVertex_SIZE*_vertexBuffer.size(),D3DUSAGE_DYNAMIC,_d3dVertex_FVF,D3DPOOL_SYSTEMMEM,&_d3dVertexBuffer,NULL));

	if(VertexBufferFailed)
		return false;
	else
	{
		_usingVertexBuffer=true;

		_d3dDevice->SetStreamSource(0,_d3dVertexBuffer,0,_d3dVertex_SIZE);
		_d3dDevice->SetFVF(_d3dVertex_FVF);

		if(FAILED(_d3dVertexBuffer->Lock(0,0,&VertexBufferArray,D3DLOCK_DISCARD)))
			return false;
		else
		{
			if(_cameraEnabled) _applyCameraTransform(vertexBufferData);

			memcpy(VertexBufferArray,vertexBufferData,_vertexBuffer.size()*_d3dVertex_SIZE);
			_d3dVertexBufferOK=true;
		}
	}

	if(_indexBuffer.size()>0)
	{
		if(FAILED(_d3dDevice->CreateIndexBuffer(sizeof(int)*_indexBuffer.size(),D3DUSAGE_DYNAMIC,D3DFMT_INDEX32,D3DPOOL_SYSTEMMEM,&_d3dIndexBuffer,NULL)))
			return false;
		else
		{
			_d3dDevice->SetIndices(_d3dIndexBuffer);
			if(FAILED(_d3dIndexBuffer->Lock(0,0,&IndexBufferArray,D3DLOCK_DISCARD)))
				return false;
			else
			{
				memcpy(IndexBufferArray,_indexBuffer.data(),_indexBuffer.size()*sizeof(int));
				_d3dIndexBufferOK=true;
			}
		}
	}

	return true;
}

void ComputePolyline(dl32Point2D points[],int pointCount,float width,PolylineJoint* &out)
{
	dl32Line2D supi,supi_1,infi,infi_1;//Recta del borde superior i, recta del borde superior i-1, recta del borde inferior i, recta del borde inferior i-1
	dl32Line2D jointLine;//Recta de la articulación
	dl32Vector2D direction;//Direccion del segmento
	dl32Point2D upcenter,downcenter;//Centro del borde superior, centro del borde inferior
	dl32Line2D centerLine;//Linea perpendicular al segmento por su centro

	PolylineJoint* joints = new PolylineJoint[pointCount]; //Array de joints (Lo que queremos calcular)

	for(int i=0; i<pointCount;++i)
	{
		if(i==0)//Caso 1: extremo inicial: El joint es un joint perpendicular normal
		{
			direction = dl32Vector2D(points[i],points[i+1]);
			jointLine = dl32Line2D(points[i],dl32Vector2D(points[i],points[i+1]).GetNormal()); //Recta perpendicular al segmento 
			joints[i] = PolylineJoint(jointLine.GetPointByParameter(width),
				jointLine.GetPointByParameter(-width));

			centerLine = dl32Line2D((points[i]+points[i+1])/2,direction.GetNormal()); //Recta perpendicular al segmento (centrada en él)

			supi = dl32Line2D(centerLine.GetPointByParameter(width),direction);//Borde superior
			infi = dl32Line2D(centerLine.GetPointByParameter(-width),direction);//Borde inferior
		}
		else if(i<pointCount-1)//Caso 2: Caso general
		{
			direction = dl32Vector2D(points[i],points[i+1]);
			centerLine = dl32Line2D((points[i]+points[i+1])/2,direction.GetNormal()); //Recta perpendicular al segmento (centrada en él)

			supi = dl32Line2D(centerLine.GetPointByParameter(width),direction);//Borde superior
			infi = dl32Line2D(centerLine.GetPointByParameter(-width),direction);//Borde inferior

			if(dl32Line2D::AreParallel(supi,supi_1))//Si los segmentos contiguos al joint son paralelos, el joint tambien es un joint perpendicular normal
			{
				jointLine = dl32Line2D(points[i],centerLine.GetDirection());
				joints[i] = PolylineJoint(jointLine.GetPointByParameter(width),
					jointLine.GetPointByParameter(-width));
			}
			else//Si no, los extremos del joint son las intersecciones entre los bordes de los segmentos contíguos:
			{
				joints[i] = PolylineJoint(dl32Line2D::Intersection(supi,supi_1,true),//Ya hemos chequeado antes el paralelismo
					dl32Line2D::Intersection(infi,infi_1,true));//Ya hemos chequeado antes el paralelismo
			}
		}
		else//Caso 3:  extremo final: El joint es un joint perpendicular normal
		{
			direction = dl32Vector2D(points[i-1],points[i]);
			jointLine = dl32Line2D(points[i],direction.GetNormal()); //Recta perpendicular al segmento

			joints[i] = PolylineJoint(jointLine.GetPointByParameter(width),
				jointLine.GetPointByParameter(-width));
		}	

		supi_1 = supi;
		infi_1 = infi;
	}

	out = joints;
}

void ParseJointsToVertexBuffer(PolylineJoint joints[],int JointCount,dl32Point2D* &out)
{
	out = new dl32Point2D[JointCount*2];

	for(int i=0;i<JointCount;++i)
	{
		out[i*2] = joints[i].Down;
		out[i*2+1] = joints[i].Up;
	}
}

void ParseJointsToVertexBuffer(PolylineJoint joints[],dl32Vertex colors[],int JointCount,dl32Vertex* &out)
{
	out = new dl32Vertex[JointCount*2];

	for(int i=0;i<JointCount;++i)
	{
		out[i*2] = dl32Vertex(joints[i].Down,colors[i].color);
		out[i*2+1] = dl32Vertex(joints[i].Up,colors[i].color);
	}
}

dl32Point2D* vertexArrayToPoint2D(dl32Vertex vertexArray[],int Count)
{
	dl32Point2D* points = new dl32Point2D[Count];

	for(int i=0;i<Count;++i)
		points[i] = dl32Point2D(vertexArray[i].x,vertexArray[i].y);

	return points;
}

void dl32GraphicsClass::DRAW_Line(dl32Point2D P1, dl32Point2D P2, dl32Color color, int Z)
{
	if(!_working) throw  dl32NotInitializedGraphicsException();

		DL32BUFFEROBJECT object;

		if(_renderBuffer.back().PrimitiveType == RBCT_DRAWLINE)
		{
			_renderBuffer.back().VertexCount += 2;
			_renderBuffer.back().PrimitiveCount += 1;
		}
		else
		{
			object.StartIndex = _vertexBuffer.size();

			object.VertexCount = 2;
			object.PrimitiveType = D3DPT_LINELIST;
			object.PrimitiveCount = 1;

			_renderBuffer.push_back(object);
		}

		_vertexBuffer.push_back(_d3dVertex(P1,Z,color));
		_vertexBuffer.push_back(_d3dVertex(P2,Z,color));
}

void dl32GraphicsClass::DRAW_Line(dl32Vertex V1, dl32Vertex V2,int Z)
{
	if(!_working) throw  dl32NotInitializedGraphicsException();

	DL32BUFFEROBJECT object;

	if(_renderBuffer.back().PrimitiveType == RBCT_DRAWLINE)
	{
		_renderBuffer.back().VertexCount += 2;
		_renderBuffer.back().PrimitiveCount += 1;
	}
	else
	{
		object.StartIndex = _vertexBuffer.size();

		object.VertexCount = 2;
		object.PrimitiveType = D3DPT_LINELIST;
		object.PrimitiveCount = 1;

		_renderBuffer.push_back(object);
	}

	_vertexBuffer.push_back(_d3dVertex(V1,Z));
	_vertexBuffer.push_back(_d3dVertex(V2,Z));
}

void dl32GraphicsClass::DRAW_Lines(dl32Vertex points[],int Count,int Z)
{
	if(!_working) throw  dl32NotInitializedGraphicsException();

	if(Count >= 2)
	{
		DL32BUFFEROBJECT object;

		object.StartIndex = _vertexBuffer.size();
		_vertexBuffer.reserve(_vertexBuffer.size() + Count);

		for(int i=0;i<Count;++i)
			_vertexBuffer.push_back(_d3dVertex(points[i],Z));

		object.VertexCount = Count;
		object.PrimitiveType = D3DPT_LINESTRIP;
		object.PrimitiveCount = Count-1;

		_renderBuffer.push_back(object);
	}
}

void dl32GraphicsClass::DRAW_Lines(dl32Point2D points[],int Count,dl32Color color,int Z)
{
	if(!_working) throw  dl32NotInitializedGraphicsException();

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL)
	{
		if(Count >= 2)
		{
			DL32BUFFEROBJECT object;

			object.StartIndex = _vertexBuffer.size();
			_vertexBuffer.reserve(_vertexBuffer.size() + Count);

			for(int i=0;i<Count;++i)
				_vertexBuffer.push_back(_d3dVertex(points[i],Z,color));

			object.VertexCount = Count;
			object.PrimitiveType = D3DPT_LINESTRIP;
			object.PrimitiveCount = Count-1;

			_renderBuffer.push_back(object);
		}
	}
	else
		throw  dl32ZLevelOutOfRangeException(Z);
}

void dl32GraphicsClass::DRAW_Polyline(dl32Point2D points[],int Count,dl32Color color,float width, bool fill,int Z)
{
	if(!_working) throw  dl32NotInitializedGraphicsException();

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL)
	{
		if(Count >= 2)
		{
			PolylineJoint* joints;
			dl32Point2D* vertices;
			bool retVal;

			ComputePolyline(points,Count,width,joints);
			ParseJointsToVertexBuffer(joints,Count,vertices);

			DRAW_TriangleStrip(vertices,Count*2,color,fill);

			DL32MEMORY_SAFEDELETE_ARRAY(joints);
			DL32MEMORY_SAFEDELETE_ARRAY(vertices);
		}
	}
	else
		throw  dl32ZLevelOutOfRangeException(Z);
}

void dl32GraphicsClass::DRAW_Polyline(dl32Vertex points[],int Count,float width, bool fill,int Z)
{
	if(!_working) throw  dl32NotInitializedGraphicsException();

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL)
	{
		if(Count >= 2)
		{
			PolylineJoint* joints;
			dl32Vertex* vertices;
			dl32Point2D* points2d = vertexArrayToPoint2D(points,Count);
			bool retVal;

			ComputePolyline(points2d,Count,width,joints);
			ParseJointsToVertexBuffer(joints,points,Count,vertices);

			DRAW_TriangleStrip(vertices,Count*2,fill);

			DL32MEMORY_SAFEDELETE_ARRAY(joints);
			DL32MEMORY_SAFEDELETE_ARRAY(vertices);
			DL32MEMORY_SAFEDELETE_ARRAY(points2d);
		}
	}
	else
		throw  dl32ZLevelOutOfRangeException(Z);
}

void dl32GraphicsClass::DRAW_Triangle(float x0, float y0, float x1,float y1, float x2, float y2,dl32Color color,bool fill,int Z)
{
	if(!_working) throw  dl32NotInitializedGraphicsException();

	DL32BUFFEROBJECT Object;

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL)
	{
		Object.VertexCount=3;
		Object.Texture=-1;
		Object.CallType=RBCT_DRAWTRIANGLE;
		Object.StartIndex=_vertexBuffer.size();

		_vertexBuffer.reserve(Object.StartIndex + 3);

		_vertexBuffer.push_back(_d3dVertex(x0,y0,Z,color,0,-1,-1));
		_vertexBuffer.push_back(_d3dVertex(x1,y1,Z,color,0,-1,-1));
		_vertexBuffer.push_back(_d3dVertex(x2,y2,Z,color,0,-1,-1));

		if(fill)
		{
			Object.PrimitiveType=D3DPT_TRIANGLESTRIP;
			Object.PrimitiveCount=1;
		}
		else
		{
			Object.PrimitiveType=D3DPT_LINESTRIP;
			Object.PrimitiveCount=3;
			_vertexBuffer.push_back(_vertexBuffer[Object.StartIndex]);
		}

		_renderBuffer.push_back(Object);
	}
	else
		throw  dl32ZLevelOutOfRangeException(Z);
}

void dl32GraphicsClass::DRAW_Triangle(dl32Vertex V0, dl32Vertex V1, dl32Vertex V2,bool fill,int Z)
{	
	if(!_working) throw  dl32NotInitializedGraphicsException();

	DL32BUFFEROBJECT Object;

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL)
	{
		Object.VertexCount=3;
		Object.Texture=-1;
		Object.CallType=RBCT_DRAWTRIANGLE;
		Object.StartIndex=_vertexBuffer.size();

		_vertexBuffer.reserve(Object.StartIndex + 3);

		_vertexBuffer.push_back(_d3dVertex(V0,Z));
		_vertexBuffer.push_back(_d3dVertex(V1,Z));
		_vertexBuffer.push_back(_d3dVertex(V2,Z));

		if(fill)
		{
			Object.PrimitiveType=D3DPT_TRIANGLESTRIP;
			Object.PrimitiveCount=1;
		}
		else
		{
			Object.PrimitiveType=D3DPT_LINESTRIP;
			Object.PrimitiveCount=3;
			_vertexBuffer.push_back(_vertexBuffer[Object.StartIndex]);
		}

		_renderBuffer.push_back(Object);
	}
	else
		throw  dl32ZLevelOutOfRangeException(Z);
}

void dl32GraphicsClass::DRAW_Triangle(dl32Point2D V0, dl32Point2D V1, dl32Point2D V2,dl32Color color,bool fill,int Z)
{
	return DRAW_Triangle(dl32Vertex(V0,color,Z),dl32Vertex(V1,color,Z),dl32Vertex(V2,color,Z),fill,Z);
}

void dl32GraphicsClass::DRAW_TriangleStrip(dl32Point2D points[], int pointsCount, dl32Color color, bool fill, int Z)
{
	if(!_working) throw  dl32NotInitializedGraphicsException();

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL)
	{
		if(pointsCount >= 3)
		{
			if(fill)
			{
				DL32BUFFEROBJECT object;

				object.StartIndex = _vertexBuffer.size();
				_vertexBuffer.reserve(_vertexBuffer.size()+pointsCount);

				for(int i=0;i<pointsCount;++i)
					_vertexBuffer.push_back(_d3dVertex(points[i],Z,color));

				object.PrimitiveCount = pointsCount-2;
				object.VertexCount = pointsCount;
				object.PrimitiveType = D3DPT_TRIANGLESTRIP;

				_renderBuffer.push_back(object);
			}
			else
			{
				bool retVal = true;
				int i=0;

				while(retVal && i<pointsCount-2)
				{
					DRAW_Triangle(points[i],points[i+1],points[i+2],color,fill,Z);
					++i;
				}
			}
		}
	}
	else
		throw  dl32ZLevelOutOfRangeException(Z);
}

void dl32GraphicsClass::DRAW_TriangleStrip(dl32Vertex points[], int pointsCount, bool fill, int Z)
{
	if(!_working) throw  dl32NotInitializedGraphicsException();

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL)
	{
		if(pointsCount >= 3)
		{
			if(fill)
			{
				DL32BUFFEROBJECT object;

				object.StartIndex = _vertexBuffer.size();
				_vertexBuffer.reserve(_vertexBuffer.size()+pointsCount);

				for(int i=0;i<pointsCount;++i)
					_vertexBuffer.push_back(_d3dVertex(points[i],Z));

				object.PrimitiveCount = pointsCount-2;
				object.VertexCount = pointsCount;
				object.PrimitiveType = D3DPT_TRIANGLESTRIP;

				_renderBuffer.push_back(object);
			}
			else
			{
				bool retVal = true;
				int i=0;

				while(retVal && i<pointsCount-2)
				{
					DRAW_Triangle(points[i],points[i+1],points[i+2],fill,Z);
					++i;
				}
			}
		}
	}
	else
		throw  dl32ZLevelOutOfRangeException(Z);
}

void dl32GraphicsClass::DRAW_Polygon(const dl32Vertex Verts[],int Count,bool fill,int Z)
{
	if(!_working) throw  dl32NotInitializedGraphicsException();

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL)
	{
		if(Count >= 3)
		{
			DL32BUFFEROBJECT Object;
			dl32Point2D Baricenter,InitVert;
			dl32Color CenterColor=0;

			Object.StartIndex=_vertexBuffer.size();
			_vertexBuffer.reserve(_vertexBuffer.size()+Count+2);//En el peor de los casos (fill = false) sobra un hueco reservado. Se usará posteriormente en otras llamadas

			for(int i=0;i<Count;++i)
			{
				Baricenter.x+=Verts[i].x;
				Baricenter.y+=Verts[i].y;
				CenterColor+=Verts[i].color;

				_vertexBuffer.push_back(_d3dVertex(Verts[i],Z));
			}

			Baricenter.x/=Count;
			Baricenter.y/=Count;
			CenterColor/=Count;

			Object.Texture=-1;

			Object.CallType=RBCT_DRAWPOLYGON;

			if(fill)
			{
				Object.PrimitiveType=D3DPT_TRIANGLEFAN;
				Object.PrimitiveCount=Count;
				Object.VertexCount=Count+2;
				_vertexBuffer.insert(_vertexBuffer.begin()+_vertexBuffer.size()-Count,_d3dVertex(Baricenter.x,Baricenter.y,Z,CenterColor,0,-1,-1));
				_vertexBuffer.push_back(_d3dVertex(Verts[0],Z));
			}
			else
			{
				Object.PrimitiveType=D3DPT_LINESTRIP;
				Object.PrimitiveCount=Count;
				Object.VertexCount=Count+1;
				_vertexBuffer.push_back(_d3dVertex(Verts[0],Z));
			}

			_renderBuffer.push_back(Object);
		}
	}
	else
		throw  dl32ZLevelOutOfRangeException(Z);
}

void dl32GraphicsClass::DRAW_Polygon(const dl32Point2D Verts[],int Count,dl32Color color,bool fill,int Z)
{
	if(!_working) throw  dl32NotInitializedGraphicsException();

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL)
	{
		if(Count >= 3)
		{
			DL32BUFFEROBJECT Object;
			dl32Point2D Baricenter,NewVert,InitVert;

			Object.StartIndex=_vertexBuffer.size();

			if(fill)
				_vertexBuffer.reserve(_vertexBuffer.size()+Count+2);
			else
				_vertexBuffer.reserve(_vertexBuffer.size()+Count+1);

			for(int i=0;i<Count;++i)
			{
				Baricenter.x+=Verts[i].x;
				Baricenter.y+=Verts[i].y;

				_vertexBuffer.push_back(_d3dVertex(Verts[i],Z,color,0,-1,-1));
			}

			Baricenter.x/=Count;
			Baricenter.y/=Count;

			Object.Texture=-1;

			Object.CallType=RBCT_DRAWPOLYGON;

			if(fill)
			{
				Object.PrimitiveType=D3DPT_TRIANGLEFAN;
				Object.PrimitiveCount=Count;
				Object.VertexCount=Count+2;
				_vertexBuffer.insert(_vertexBuffer.begin()+Object.StartIndex,_d3dVertex(Baricenter.x,Baricenter.y,Z,color,0,-1,-1));
				_vertexBuffer.push_back(_d3dVertex(Verts[0],Z,color,0,-1,-1));
			}
			else
			{
				Object.PrimitiveType=D3DPT_LINESTRIP;
				Object.PrimitiveCount=Count;
				Object.VertexCount=Count+1;
				_vertexBuffer.push_back(_d3dVertex(Verts[0],Z,color,0,-1,-1));
			}

			_renderBuffer.push_back(Object);
		}
	}
	else
		throw  dl32ZLevelOutOfRangeException(Z);
}

void dl32GraphicsClass::DRAW_Box(float x,float y,float width, float height,dl32Color color,bool fill,int Z)
{
	dl322DPointTrapezoid Verts={dl32Point2D(x,y),dl32Point2D(x+width,y),dl32Point2D(x+width,y+height),dl32Point2D(x,y+height)};

	return DRAW_Trapezoid(Verts,color,fill,Z);
}

void dl32GraphicsClass::DRAW_VertexMap(int texture,const dl32VertexTrapezoid verts,int Z)
{
	if(!_working) throw  dl32NotInitializedGraphicsException();

	if(texture>=0 && (unsigned int)texture < _textures.size())
	{
		DL32BUFFEROBJECT Object;

		if(DL32DEBUG_GRAPHICS_DRAWCALLSMERGE && _renderBuffer.size() > 0 && _renderBuffer.back().CallType == RBCT_DRAWVERTEXMAP && _renderBuffer.back().Texture == texture)
		{
			_renderBuffer.back().PrimitiveCount += 2;
			_renderBuffer.back().VertexCount += 6;
		}
		else
		{
			Object.PrimitiveType=D3DPT_TRIANGLELIST;
			Object.PrimitiveCount=2;
			Object.StartIndex=_indexBuffer.size();
			Object.VertexCount=6;
			Object.BaseIndex = 0;
			Object.Texture=texture;
			Object.CallType=RBCT_DRAWVERTEXMAP;

			_renderBuffer.push_back(Object);
		}

		_vertexBuffer.reserve(_vertexBuffer.size() + 4);
		_vertexBuffer.push_back(_d3dVertex(verts[0],Z,0,0));//Arriba izquierda (Primer triangulo)
		_vertexBuffer.push_back(_d3dVertex(verts[1],Z,1,0));//Arriba derecha (Primer triangulo)
		_vertexBuffer.push_back(_d3dVertex(verts[3],Z,0,1));//Abajo izquierda (Primer triangulo)
		_vertexBuffer.push_back(_d3dVertex(verts[2],Z,1,1));//Abajo derecha (Segundo triangulo)

		_indexBuffer.reserve(_indexBuffer.size() + 6);
		_indexBuffer.push_back(_vertexBuffer.size() - 4);//Arriba izquierda (Primer triangulo)
		_indexBuffer.push_back(_vertexBuffer.size() - 3);//Arriba derecha (Primer triangulo)
		_indexBuffer.push_back(_vertexBuffer.size() - 2);//Abajo izquierda (Primer triangulo)
		_indexBuffer.push_back(_vertexBuffer.size() - 3);//Arriba derecha (Segundo triangulo)
		_indexBuffer.push_back(_vertexBuffer.size() - 2);//Abajo izquierda (Segundo triangulo)
		_indexBuffer.push_back(_vertexBuffer.size() - 1);//Abajo derecha (Segundo triangulo)
	}
}

void dl32GraphicsClass::DRAW_Mesh(dl32Mesh Mesh,int Z)
{
	if(!_working) throw  dl32NotInitializedGraphicsException();

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL)
	{DL32TIMING_BEGIN
		DL32BUFFEROBJECT Object;
		int BaseIndex=_vertexBuffer.size();
		int StripVertexCount;
		int StripPrimitiveCount;

		for(unsigned int i=0;i<Mesh.verts.size();++i)
			Mesh.verts[i].z=(DL32MACROS_GRAPHICS_ZLEVELINDEX(Z)+1)*DL32CONSTS_d3dVertex_Z;

		_vertexBuffer.insert(_vertexBuffer.end(),Mesh.verts.begin(),Mesh.verts.end());

		for(unsigned int i=0;i<Mesh.patches.size();++i)
		{
			//StripVertexCount=Mesh.verts.size();
			StripVertexCount=Mesh.verts.size();
			StripPrimitiveCount=(Mesh.patches[i].width-1)*2;

			for(int j=0;j<Mesh.patches[i].height-1;++j)
			{
				Object.BaseIndex=BaseIndex;
				Object.StartIndex=_indexBuffer.size()+(j*Mesh.patches[i].width*2);
				Object.PrimitiveType=D3DPT_TRIANGLESTRIP;
				Object.VertexCount=StripVertexCount;
				Object.PrimitiveCount=StripPrimitiveCount;
				Object.Texture=Mesh.patches[i].texture;

				Object.CallType=RBCT_MESH;

				_renderBuffer.push_back(Object);
			}

			_indexBuffer.insert(_indexBuffer.end(),Mesh.indexes[i].begin(),Mesh.indexes[i].end());
		}
	DL32TIMING_END}
	else
		throw  dl32ZLevelOutOfRangeException(Z);
}

void dl32GraphicsClass::DRAW_Text(int font,float x,float y,dl32String text,dl32Color color,dl32TextAlign align,int Z)
{
	if(!_working) throw  dl32NotInitializedGraphicsException();

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL && font>=0 && (unsigned int)font < _fonts.size())
	{
		DL32BUFFEROBJECT Object;
		RECT rect;

		Object.textdata=new DL32TEXTDATA;
		Object.textdata->color=color;
		Object.textdata->font=_fonts[font];
		Object.textdata->format=DT_TOP | DT_LEFT;
		Object.textdata->text=text.Copy();
		rect.left=x;
		rect.top=y;

		Object.textdata->font->DrawText(NULL,Object.textdata->text,-1,&rect,DT_CALCRECT,color);

		switch(align)
		{
		case DL32TA_UPLEFT:
			Object.textdata->rect=rect;break;
		case DL32TA_UPRIGHT:
			Object.textdata->rect.left=rect.left+rect.left-rect.right;
			Object.textdata->rect.right=rect.left;
			Object.textdata->rect.top=rect.top;
			Object.textdata->rect.bottom=rect.bottom;
			break;
		case DL32TA_DOWNRIGHT:
			Object.textdata->rect.left=rect.left+rect.left-rect.right;
			Object.textdata->rect.right=rect.left;
			Object.textdata->rect.top=rect.top+rect.top-rect.bottom;
			Object.textdata->rect.bottom=rect.top;
			break;
		case DL32TA_DOWNLEFT:
			Object.textdata->rect.left=rect.left;
			Object.textdata->rect.right=rect.right;
			Object.textdata->rect.top=rect.top+rect.top-rect.bottom;
			Object.textdata->rect.bottom=rect.top;
			break;
		case DL32TA_CENTER:
			Object.textdata->rect.left=rect.left+(rect.left-rect.right)/2;
			Object.textdata->rect.right=Object.textdata->rect.left+rect.right-rect.left;
			Object.textdata->rect.top=rect.top+(rect.top-rect.bottom)/2;
			Object.textdata->rect.bottom=Object.textdata->rect.top+rect.bottom-rect.top;
			break;
		case DL32TA_UP:
			Object.textdata->rect.left=rect.left+(rect.left-rect.right)/2;
			Object.textdata->rect.right=Object.textdata->rect.left+rect.right-rect.left;
			Object.textdata->rect.top=rect.top;
			Object.textdata->rect.bottom=rect.bottom;
			break;
		case DL32TA_RIGHT:
			Object.textdata->rect.left=rect.left+rect.left-rect.right;
			Object.textdata->rect.right=rect.left;
			Object.textdata->rect.top=rect.top+(rect.top-rect.bottom)/2;
			Object.textdata->rect.bottom=Object.textdata->rect.top+rect.bottom-rect.top;
			break;
		case DL32TA_DOWN:
			Object.textdata->rect.left=rect.left+(rect.left-rect.right)/2;
			Object.textdata->rect.right=Object.textdata->rect.left+rect.right-rect.left;
			Object.textdata->rect.top=rect.top+rect.top-rect.bottom;
			Object.textdata->rect.bottom=rect.top;
			break;
		case DL32TA_LEFT:
			Object.textdata->rect.left=rect.left;
			Object.textdata->rect.right=rect.right;
			Object.textdata->rect.top=rect.top+(rect.top-rect.bottom)/2;
			Object.textdata->rect.bottom=Object.textdata->rect.top+rect.bottom-rect.top;
			break;
		}

		_renderBuffer.push_back(Object);

		_textDraw=true;
	}
	else
		throw  dl32ZLevelOutOfRangeException(Z);
}

void dl32GraphicsClass::DRAW_Pixel(float x, float y, dl32Color color, int Z)
{
	if(!_working) throw  dl32NotInitializedGraphicsException();

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL)
	{
		DL32BUFFEROBJECT Object;

		Object.PrimitiveType=D3DPT_POINTLIST;
		Object.PrimitiveCount=1;
		Object.StartIndex=_vertexBuffer.size();
		Object.VertexCount=1;

		_vertexBuffer.push_back(_d3dVertex(x,y,Z,color,0,-1,-1));

		_renderBuffer.push_back(Object);
	}
	else
		throw  dl32ZLevelOutOfRangeException(Z);
}

void dl32GraphicsClass::DRAW_Pixels(dl32Pixel pixels[],int count,int Z)
{
	if(!_working) throw  dl32NotInitializedGraphicsException();

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL && count>0)
	{
		DL32BUFFEROBJECT Object;

		_vertexBuffer.reserve(_vertexBuffer.size()+count);

		Object.StartIndex=_vertexBuffer.size();
		Object.PrimitiveCount=count;
		Object.VertexCount=count;
		Object.PrimitiveType=D3DPT_POINTLIST;

		_vertexBuffer.insert(_vertexBuffer.end(),pixels,pixels+count);

		_renderBuffer.push_back(Object);
	}
	else
		throw  dl32ZLevelOutOfRangeException(Z);
}

void dl32GraphicsClass::DRAW_Pixels(dl32Point2D pixels[],dl32Color color,int count,int Z)
{
	if(!_working) throw  dl32NotInitializedGraphicsException();

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL && count>0)
	{
		DL32BUFFEROBJECT Object;

		_vertexBuffer.reserve(_vertexBuffer.size()+count);

		for(int i=0;i<count;++i)
			_vertexBuffer.push_back(_d3dVertex(pixels[i],Z,color));

		if(_renderBuffer.back().CallType == RBCT_DRAWPIXEL)
		{
			_renderBuffer.back().VertexCount += count;
			_renderBuffer.back().PrimitiveCount += count;
		}
		else
		{
			Object.StartIndex=_vertexBuffer.size();
			Object.PrimitiveType=D3DPT_POINTLIST;
			Object.PrimitiveCount=count;
			Object.VertexCount=count;
			Object.CallType = RBCT_DRAWPIXEL;

			_renderBuffer.push_back(Object);
		}
	}
	else
		throw  dl32ZLevelOutOfRangeException(Z);
}

void dl32GraphicsClass::DRAW_Pixels(dl32Color **pixels,float x,float y,int width,int height,int Z)
{
	if(!_working) throw  dl32NotInitializedGraphicsException();

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL && width>0 && height>0)
	{
		DL32BUFFEROBJECT Object;

		Object.StartIndex=_vertexBuffer.size();

		_vertexBuffer.reserve(_vertexBuffer.size()+width*height);

		for(int i=0;i<width;++i)
			for(int j=0;j<height;++j)
				_vertexBuffer.push_back(_d3dVertex(x+i,y+j,Z,pixels[j][i]));

		if(_renderBuffer.back().CallType == RBCT_DRAWPIXEL)
		{
			_renderBuffer.back().VertexCount += width*height;
			_renderBuffer.back().PrimitiveCount += width*height;
		}
		else
		{
			Object.StartIndex=_vertexBuffer.size();
			Object.PrimitiveType=D3DPT_POINTLIST;
			Object.PrimitiveCount=width*height;
			Object.VertexCount=width*height;
			Object.CallType = RBCT_DRAWPIXEL;

			_renderBuffer.push_back(Object);
		}
	}
	else
		throw  dl32ZLevelOutOfRangeException(Z);
}

void dl32GraphicsClass::DRAW_Spline(dl32Spline* spline,dl32Color color,int PointsPerInterval,int Z)
{
	if(!_working) throw  dl32NotInitializedGraphicsException();

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL)
	{
		DL32BUFFEROBJECT Object;
		int vertexcount=0;
		vector<dl32Point2D> points(spline->Interpolate(PointsPerInterval));

		_vertexBuffer.reserve(_vertexBuffer.size()+points.size());

		Object.StartIndex=_vertexBuffer.size();
		Object.PrimitiveType=D3DPT_LINESTRIP;
		Object.PrimitiveCount=points.size()-1;
		Object.VertexCount=points.size();

		for(unsigned int i=0;i<points.size();++i)
			_vertexBuffer.push_back(_d3dVertex(points[i],Z,color));

		_renderBuffer.push_back(Object);
	}
	else
		throw  dl32ZLevelOutOfRangeException(Z);
}


int dl32GraphicsClass::MAP_Load(dl32String Filename,dl32Color ColorKey,bool Smooth,bool GrayScale)
{
	_d3dTexture Texture;
	D3DFORMAT Format;
	DWORD Filter;
	D3DXIMAGE_INFO ImageInfo;

	if(Smooth)
		Filter=D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR;
	else
		Filter=D3DX_FILTER_TRIANGLE | D3DX_FILTER_NONE;

	if(GrayScale)
		Format=D3DFMT_A4L4;
	else
		Format=D3DFMT_A8R8G8B8;

	if(FAILED(D3DXCreateTextureFromFileEx(_d3dDevice,Filename.c_str(),D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,Format,D3DPOOL_MANAGED,Filter,Filter,ColorKey,&ImageInfo,NULL,&Texture.Texture)))
		return (int)NULL;
	else
	{
		Texture.OriginalWidth=ImageInfo.Width;
		Texture.OriginalHeight=ImageInfo.Height;
		Texture.ColorKey=ColorKey;
		Texture.Index=_textures.size();
		Texture.Format=Format;

		_textures.push_back(Texture);

		return Texture.Index;
	}
}

int dl32GraphicsClass::FONT_LoadSystemFont(dl32String FontName,int size,bool bold,bool italic)
{
	D3DXFONT_DESC fdesc;
	ID3DXFont *font=NULL;

	ZeroMemory(&fdesc,sizeof(D3DXFONT_DESC));

	fdesc.CharSet=DEFAULT_CHARSET;
	fdesc.Width=size;
	fdesc.Height=size*3;
	fdesc.Weight=bold ? 1000 : 500;
	fdesc.Italic=italic;
	strcpy(fdesc.FaceName,FontName.c_str());

	if(!FAILED(D3DXCreateFontIndirect(_d3dDevice,&fdesc,&font)))
	{
		_fonts.push_back(font);
		return _fonts.size()-1;
	}
	else
		return (int)NULL;
}

