#include "dl32Graphics.h"

using namespace std;

wstring STRING_WSTRING(const string& s);
LPCWSTR STRING_LPCWSTR(const string& s);

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

DL32VERTEXTEXTURED::DL32VERTEXTEXTURED()
{
	z=DL32CONSTS_D3DVERTEX_Z;rhw=DL32CONSTS_D3DVERTEX_RHW;
	diffuse=DL32COLOR_WHITE;specular=DL32COLOR_BLACK;
	tx=-1;ty=-1;
}

DL32VERTEXTEXTURED::DL32VERTEXTEXTURED(float x,float y,int z,D3DCOLOR diffuse,D3DCOLOR specular,float tx,float ty)
{
	this->x=x;this->y=y;this->z=(DL32MACROS_GRAPHICS_ZLEVELINDEX(z)+1)*DL32CONSTS_D3DVERTEX_Z;
	this->rhw=DL32CONSTS_D3DVERTEX_RHW;
	this->diffuse=diffuse;this->specular=specular;
	this->tx=tx;this->ty=ty;
}

DL32VERTEXTEXTURED::DL32VERTEXTEXTURED(const dl32Vertex &vertex)
{
	x=vertex.x;y=vertex.y;z=(DL32MACROS_GRAPHICS_ZLEVELINDEX(vertex.Z)+1)*DL32CONSTS_D3DVERTEX_Z;
	rhw=DL32CONSTS_D3DVERTEX_RHW;
	diffuse=vertex.color;specular=DL32COLOR_BLACK;
	tx=-1;ty=-1;
}

DL32VERTEXTEXTURED::DL32VERTEXTEXTURED(dl32Point2D &point,int Z,dl32Color diffuse,dl32Color specular,float tx,float ty)
{
	x=point.x;
	y=point.y;
	z=(DL32MACROS_GRAPHICS_ZLEVELINDEX(Z)+1)*DL32CONSTS_D3DVERTEX_Z;
	this->diffuse=diffuse;
	this->specular=specular;
	rhw=DL32CONSTS_D3DVERTEX_RHW;
	this->tx=tx;
	this->ty=ty;
}

bool DL32TEXTURE::LoadSurface()
{
	if(this->Surface==NULL)
		if(FAILED(this->Texture->GetSurfaceLevel(0,&Surface)))
			return false;
		else
			return true;
	else
		return false;
}

bool DL32TEXTURE::ReleaseSurface()
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

dl322DCamera::dl322DCamera()
{
	m11=1;m12=0;m13=0;
	m21=0;m22=1;m23=0;
	m31=0;m32=0;m33=1;
}

dl322DCamera::dl322DCamera(dl323x3Matrix &Transformation)
{
	m11=Transformation.m11;m12=Transformation.m12;m13=Transformation.m13;
	m21=Transformation.m21;m22=Transformation.m22;m23=Transformation.m23;
	m31=Transformation.m31;m32=Transformation.m32;m33=Transformation.m33;
}

void dl322DCamera::Rotate(float Rotation)
{
	*this=dl323x3Matrix::Mul(dl32Transformation2D::Rotation(Rotation),*this);
}

void dl322DCamera::Rotate(dl32Point2D Center,float Rotation)
{
	*this=dl323x3Matrix::Mul(dl32Transformation2D::Rotation(Center,Rotation),*this);
}

void dl322DCamera::Traslate(float x,float y)
{
	*this=dl323x3Matrix::Mul(dl32Transformation2D::Translation(-x,-y),*this);
}

void dl322DCamera::Traslate(dl32Vector2D Translation)
{
	*this=dl323x3Matrix::Mul(dl32Transformation2D::Translation(-Translation.x,-Translation.y),*this);
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

dl32Mesh::dl32Mesh(dl32VertexTrapezoid Verts,int width,int height,dl32MeshPatch *Sectors,int SectorsCount)
{
	this->~dl32Mesh();
}

dl32Mesh::dl32Mesh(dl32AABB2D Area,int width, int height,dl32MeshPatch *patches,int PatchesCount)
{
	int areawidth,areaheight,stepx,stepy;

	this->width=width;
	this->height=height;
	this->verts=vector<DL32VERTEXTEXTURED>(width*height);

	areawidth=Area.GetWidth();
	areaheight=Area.GetHeight();

	stepx=areawidth/(width-1);
	stepy=areaheight/(height-1);

	for(int i=0;i<width;++i)
		for(int j=0;j<height;++j)
			verts[i+j*width]=DL32VERTEXTEXTURED(Area.Position.x+i*stepx,Area.Position.y+j*stepy,0,DL32COLOR_BLACK,0,-1,-1);

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
	if(Index>=0 && Index<patches.size())
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
	for(int i=0;i<verts.size();++i)
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

bool dl32GraphicsClass::Frame()
{
	if(!_working) return false;

	DL32BUFFEROBJECT Object;
	DWORD Ticks;
	int RenderBufferLevelSize,ActiveLevelsSize=0;
	int Index;

	_d3dDevice->Clear(0,NULL,D3DCLEAR_TARGET,_backColor,0,0);
	_d3dDevice->BeginScene();

	if(FillD3DBuffers() || _textDraw)
	{
		ActiveLevelsSize=_renderBufferActiveLevels.size();

		for(int i=0;i<ActiveLevelsSize;++i)
		{
			Index=_renderBufferActiveLevels[i];
			RenderBufferLevelSize=_renderBuffer[Index]->size();

			for(int j=0;j<RenderBufferLevelSize;++j)
			{
				Object=_renderBuffer[Index]->at(j);

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

			delete _renderBuffer[Index];
			_renderBuffer[Index]=NULL;
		}
	}

	_d3dDevice->EndScene();
	_d3dDevice->Present(NULL,NULL,NULL,NULL);

	if(_d3dVertexBufferOK) _d3dVertexBuffer->Release();
	if(_d3dIndexBufferOK) _d3dIndexBuffer->Release();
	_vertexBuffer.clear();
	_indexBuffer.clear();
	_renderBufferActiveLevels.clear();

	_textDraw=false;

	Ticks=GetTickCount();
	if(Ticks-_lastTicksCount>=1000)
	{
		_lastTicksCount=Ticks;
		_frameRate=_frameCount;
		_frameCount=0;
	}
	_frameCount++;

	return true;
}

bool dl32GraphicsClass::InitializeDirect3D(HWND hwnd,int Width,int Height,bool Windowed,dl32ColorDepth colorDepth, bool tripleBuffer, bool vSync, int refreshRate)
{
	D3DDISPLAYMODE displayMode;

	ZeroMemory(&_d3dPresentParameters,sizeof(D3DPRESENT_PARAMETERS));
	_d3d=Direct3DCreate9(D3D_SDK_VERSION);
	_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&displayMode);

	if(vSync)
		_d3dPresentParameters.PresentationInterval=D3DPRESENT_INTERVAL_ONE;
	else
		_d3dPresentParameters.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;

	_d3dPresentParameters.SwapEffect=D3DSWAPEFFECT_DISCARD;
	_d3dPresentParameters.Windowed=BOOL(Windowed);
	_d3dPresentParameters.hDeviceWindow=hwnd;

	if(Windowed)
	{
		_d3dPresentParameters.BackBufferFormat=displayMode.Format;
		_d3dPresentParameters.BackBufferWidth=Width;
		_d3dPresentParameters.BackBufferHeight=Height;
	}
	else
	{
		if(colorDepth==DL32CD_16BIT)
			_d3dPresentParameters.BackBufferFormat=D3DFMT_R5G6B5;
		else
			_d3dPresentParameters.BackBufferFormat=D3DFMT_X8B8G8R8;

		_d3dPresentParameters.BackBufferCount=2+abs(tripleBuffer);
		_d3dPresentParameters.BackBufferWidth=displayMode.Width;
		_d3dPresentParameters.BackBufferHeight=displayMode.Height;
	}

	if(!FAILED(_d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,D3DCREATE_HARDWARE_VERTEXPROCESSING,&_d3dPresentParameters, &_d3dDevice)) || 
	   !FAILED(_d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&_d3dPresentParameters, &_d3dDevice)))
	{
		_d3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
		return true;
	}
	else
		return false;
}

dl32GraphicsClass::dl32GraphicsClass()
{
	_working=false;
	_usingVertexBuffer=false;
}

dl32GraphicsClass::dl32GraphicsClass(dl32Window* window,dl32ColorDepth colorDepth, bool Windowed, bool tripleBuffer, bool vSync, int refreshRate)throw(dl32Direct3DInitFailedException)
{
	_working=false;
	_usingVertexBuffer=false;

	if(!InitializeDirect3D(window->GetHwnd(),window->GetWidth(),window->GetHeight(),Windowed,colorDepth,tripleBuffer,vSync,refreshRate))
		throw dl32Direct3DInitFailedException("Direct3D initialization failed");
	else
	{
		_backColor=D3DCOLOR_ARGB(255,0,0,0);

		for(int i=0;i<DL32CONSTS_GRAPHICS_MAXZLEVEL-DL32CONSTS_GRAPHICS_MINZLEVEL;++i)
			_renderBuffer[i]=NULL;

		_lastTicksCount=GetTickCount();
		_frameCount=0;
		_frameRate=0;

		this->Start();

		_working=true;
	}
}

dl32GraphicsClass::dl32GraphicsClass(HWND hwnd,int Width,int Height,dl32ColorDepth colorDepth, bool Windowed, bool tripleBuffer, bool vSync, int refreshRate)throw(dl32Direct3DInitFailedException)
{
	_working=false;
	_usingVertexBuffer=false;

	if(!InitializeDirect3D(hwnd,Width,Height,Windowed,colorDepth,tripleBuffer,vSync,refreshRate))
		throw dl32Direct3DInitFailedException("Direct3D initialization failed");
	else
	{
		_backColor=D3DCOLOR_ARGB(255,0,0,0);

		for(int i=0;i<DL32CONSTS_GRAPHICS_MAXZLEVEL-DL32CONSTS_GRAPHICS_MINZLEVEL;++i)
			_renderBuffer[i]=NULL;

		_lastTicksCount=GetTickCount();
		_frameCount=0;
		_frameRate=0;

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

		for(int i=0;i<_renderBufferActiveLevels.size();++i)
			delete _renderBuffer[_renderBufferActiveLevels[i]];
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

		for(int i=0;i<_renderBufferActiveLevels.size();++i)
			delete _renderBuffer[_renderBufferActiveLevels[i]];
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

bool dl32GraphicsClass::FillD3DBuffers()
{
	void *VertexBufferArray,*IndexBufferArray;
	bool VertexBufferFailed;
	_d3dVertexBufferOK=false;
	_d3dIndexBufferOK=false;

	VertexBufferFailed=FAILED(_d3dDevice->CreateVertexBuffer(DL32VERTEXTEXTURED_SIZE*_vertexBuffer.size(),D3DUSAGE_DYNAMIC,DL32VERTEXTEXTURED_FVF,D3DPOOL_SYSTEMMEM,&_d3dVertexBuffer,NULL));

	if(VertexBufferFailed)
		return false;
	else
	{
		_usingVertexBuffer=true;

		_d3dDevice->SetStreamSource(0,_d3dVertexBuffer,0,DL32VERTEXTEXTURED_SIZE);
		_d3dDevice->SetFVF(DL32VERTEXTEXTURED_FVF);

		if(FAILED(_d3dVertexBuffer->Lock(0,0,&VertexBufferArray,D3DLOCK_DISCARD)))
			return false;
		else
		{
			memcpy(VertexBufferArray,_vertexBuffer.data(),_vertexBuffer.size()*DL32VERTEXTEXTURED_SIZE);
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

bool dl32GraphicsClass::ActivateRenderBufferLevel(int Index)
{
	if(_renderBuffer[Index]==NULL)
	{
		_renderBuffer[Index]=new vector<DL32BUFFEROBJECT>;
		_renderBufferActiveLevels.push_back(Index);
		return true;
	}
	else
		return false;
}

bool dl32GraphicsClass::DRAW_Triangle(float x0, float y0, float x1,float y1, float x2, float y2,dl32Color color,bool fill,int Z)
{
	if(!_working) return false;

	DL32BUFFEROBJECT Object;
	dl32Point2D NewVerts[3];

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL)
	{
		DL32VERTEXTEXTURED verts[3];

		NewVerts[0]=Camera.Apply(x0,y0);
		NewVerts[1]=Camera.Apply(x1,y1);
		NewVerts[2]=Camera.Apply(x2,y2);


		verts[0]=DL32VERTEXTEXTURED(NewVerts[0].x,NewVerts[0].y,Z,color,0,-1,-1);
		verts[1]=DL32VERTEXTEXTURED(NewVerts[1].x,NewVerts[1].y,Z,color,0,-1,-1);
		verts[2]=DL32VERTEXTEXTURED(NewVerts[2].x,NewVerts[2].y,Z,color,0,-1,-1);

		Object.StartIndex=_vertexBuffer.size();
		Object.VertexCount=3;
		Object.Texture=-1;

		Object.CallType=RBCT_DRAWTRIANGLE;

		_vertexBuffer.insert(_vertexBuffer.end(),verts,verts+3);

		if(fill)
		{
			Object.PrimitiveType=D3DPT_TRIANGLESTRIP;
			Object.PrimitiveCount=1;
		}
		else
		{
			Object.PrimitiveType=D3DPT_LINESTRIP;
			Object.PrimitiveCount=3;
			_vertexBuffer.push_back(verts[0]);
		}

		ActivateRenderBufferLevel(DL32MACROS_GRAPHICS_ZLEVELINDEX(Z));
		_renderBuffer[DL32MACROS_GRAPHICS_ZLEVELINDEX(Z)]->push_back(Object);

		return true;
	}
	else
		return false;
}

bool dl32GraphicsClass::DRAW_Triangle(dl32Vertex V0, dl32Vertex V1, dl32Vertex V2,bool fill,int Z)
{	
	if(!_working) return false;

	DL32BUFFEROBJECT Object;

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL)
	{
		DL32VERTEXTEXTURED verts[3];
		dl32Point2D NewVerts[3];

		NewVerts[0]=Camera.Apply(V0);
		NewVerts[1]=Camera.Apply(V1);
		NewVerts[2]=Camera.Apply(V2);

		verts[0]=DL32VERTEXTEXTURED(NewVerts[0].x,NewVerts[0].y,Z,V0.color,0,-1,-1);
		verts[1]=DL32VERTEXTEXTURED(NewVerts[1].x,NewVerts[1].y,Z,V1.color,0,-1,-1);
		verts[2]=DL32VERTEXTEXTURED(NewVerts[2].x,NewVerts[2].y,Z,V2.color,0,-1,-1);

		Object.StartIndex=_vertexBuffer.size();
		Object.VertexCount=3;
		Object.Texture=-1;

		Object.CallType=RBCT_DRAWTRIANGLE;

		_vertexBuffer.insert(_vertexBuffer.end(),verts,verts+3);

		if(fill)
		{
			Object.PrimitiveType=D3DPT_TRIANGLESTRIP;
			Object.PrimitiveCount=1;
		}
		else
		{
			Object.PrimitiveType=D3DPT_LINESTRIP;
			Object.PrimitiveCount=3;
			_vertexBuffer.push_back(verts[0]);
		}

		ActivateRenderBufferLevel(DL32MACROS_GRAPHICS_ZLEVELINDEX(Z));
		_renderBuffer[DL32MACROS_GRAPHICS_ZLEVELINDEX(Z)]->push_back(Object);

		return true;
	}
	else
		return false;
}

bool dl32GraphicsClass::DRAW_Polygon(const dl32Vertex Verts[],int Count,bool fill,int Z)
{
	if(!_working) return false;

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL && Count>=3)
	{
		DL32BUFFEROBJECT Object;
		dl32Point2D Baricenter,NewVert,InitVert;
		D3DCOLOR CenterColor=0;

		Object.StartIndex=_vertexBuffer.size();
		_vertexBuffer.reserve(_vertexBuffer.size()+Count);

		for(int i=0;i<Count;++i)
		{
			NewVert=Camera.Apply(Verts[i]);
			if(i==0) InitVert=NewVert;

			Baricenter.x+=NewVert.x;
			Baricenter.y+=NewVert.y;
			CenterColor+=Verts[i].color;

			_vertexBuffer.push_back(DL32VERTEXTEXTURED(NewVert.x,NewVert.y,Z,Verts[i].color,0,-1,-1));
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
			_vertexBuffer.insert(_vertexBuffer.begin()+_vertexBuffer.size()-Count,DL32VERTEXTEXTURED(Baricenter.x,Baricenter.y,Z,CenterColor,0,-1,-1));
			_vertexBuffer.push_back(DL32VERTEXTEXTURED(InitVert.x,InitVert.y,Z,Verts[0].color,0,-1,-1));
		}
		else
		{
			Object.PrimitiveType=D3DPT_LINESTRIP;
			Object.PrimitiveCount=Count;
			Object.VertexCount=Count+1;
			_vertexBuffer.push_back(DL32VERTEXTEXTURED(InitVert.x,InitVert.y,Z,Verts[0].color,0,-1,-1));
		}

		ActivateRenderBufferLevel(DL32MACROS_GRAPHICS_ZLEVELINDEX(Z));
		_renderBuffer[DL32MACROS_GRAPHICS_ZLEVELINDEX(Z)]->push_back(Object);

		return true;
	}
	else
		return false;
}

bool dl32GraphicsClass::DRAW_Polygon(const dl32Point2D Verts[],int Count,dl32Color color,bool fill,int Z)
{
	if(!_working) return false;

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL && Count>=3)
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
			NewVert=Camera.Apply(Verts[i]);
			if(i==0) InitVert=NewVert;

			Baricenter.x+=NewVert.x;
			Baricenter.y+=NewVert.y;

			_vertexBuffer.push_back(DL32VERTEXTEXTURED(NewVert.x,NewVert.y,Z,color,0,-1,-1));
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
			_vertexBuffer.insert(_vertexBuffer.begin()+Object.StartIndex,DL32VERTEXTEXTURED(Baricenter.x,Baricenter.y,Z,color,0,-1,-1));
			_vertexBuffer.push_back(DL32VERTEXTEXTURED(InitVert.x,InitVert.y,Z,color,0,-1,-1));
		}
		else
		{
			Object.PrimitiveType=D3DPT_LINESTRIP;
			Object.PrimitiveCount=Count;
			Object.VertexCount=Count+1;
			_vertexBuffer.push_back(DL32VERTEXTEXTURED(InitVert.x,InitVert.y,Z,color,0,-1,-1));
		}

		ActivateRenderBufferLevel(DL32MACROS_GRAPHICS_ZLEVELINDEX(Z));
		_renderBuffer[DL32MACROS_GRAPHICS_ZLEVELINDEX(Z)]->push_back(Object);

		return true;
	}
	else
		return false;
}

bool dl32GraphicsClass::DRAW_Box(float x,float y,float width, float height,dl32Color color,bool fill,int Z)
{
	dl322DPointTrapezoid Verts={dl32Point2D(x,y),dl32Point2D(x+width,y),dl32Point2D(x+width,y+height),dl32Point2D(x,y+height)};

	return DRAW_Trapezoid(Verts,color,fill,Z);
}

bool dl32GraphicsClass::DRAW_VertexMap(int texture,const dl32VertexTrapezoid verts,int Z)
{
	if(!_working) return false;

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL && texture>=0 && texture < _textures.size())
	{
		DL32BUFFEROBJECT Object;
		DL32VERTEXTEXTURED Verts[4];
		dl32Point2D NewVerts[4];

		NewVerts[0]=Camera.Apply(verts[0]);
		NewVerts[1]=Camera.Apply(verts[1]);
		NewVerts[2]=Camera.Apply(verts[2]);
		NewVerts[3]=Camera.Apply(verts[3]);

		Verts[0]=DL32VERTEXTEXTURED(NewVerts[0].x,NewVerts[0].y,Z,verts[0].color,0,0,0);
		Verts[1]=DL32VERTEXTEXTURED(NewVerts[1].x,NewVerts[1].y,Z,verts[1].color,0,1,0);
		Verts[3]=DL32VERTEXTEXTURED(NewVerts[2].x,NewVerts[2].y,Z,verts[2].color,0,1,1);
		Verts[2]=DL32VERTEXTEXTURED(NewVerts[3].x,NewVerts[3].y,Z,verts[3].color,0,0,1);

		Object.PrimitiveType=D3DPT_TRIANGLESTRIP;
		Object.PrimitiveCount=2;
		Object.StartIndex=_vertexBuffer.size();
		Object.VertexCount=4;
		Object.Texture=texture;

		Object.CallType=RBCT_DRAWVERTEXMAP;

		_vertexBuffer.insert(_vertexBuffer.end(),Verts,Verts+4);

		ActivateRenderBufferLevel(DL32MACROS_GRAPHICS_ZLEVELINDEX(Z));
		_renderBuffer[DL32MACROS_GRAPHICS_ZLEVELINDEX(Z)]->push_back(Object);

		return true;
	}
	else
		return false;
}

bool dl32GraphicsClass::DRAW_Mesh(dl32Mesh Mesh,int Z)
{
	if(!_working) return false;

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL)
	{
		DL32BUFFEROBJECT Object;
		int BaseIndex=_vertexBuffer.size();
		int StripVertexCount;
		int StripPrimitiveCount;

		Mesh.Transformation(Camera);

		for(int i=0;i<Mesh.verts.size();++i)
			Mesh.verts[i].z=(DL32MACROS_GRAPHICS_ZLEVELINDEX(Z)+1)*DL32CONSTS_D3DVERTEX_Z;

		_vertexBuffer.insert(_vertexBuffer.end(),Mesh.verts.begin(),Mesh.verts.end());

		for(int i=0;i<Mesh.patches.size();++i)
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

				ActivateRenderBufferLevel(DL32MACROS_GRAPHICS_ZLEVELINDEX(Z));
				_renderBuffer[DL32MACROS_GRAPHICS_ZLEVELINDEX(Z)]->push_back(Object);
			}

			_indexBuffer.insert(_indexBuffer.end(),Mesh.indexes[i].begin(),Mesh.indexes[i].end());
		}

		return true;
	}
	else
		return false;
}

bool dl32GraphicsClass::DRAW_Text(int font,float x,float y,dl32String text,dl32Color color,dl32TextAlign align,int Z)
{
	if(!_working) return false;

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL && font>=0 && font < _fonts.size())
	{
		DL32BUFFEROBJECT Object;
		RECT rect;

		Camera.Apply(&x,&y);

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

		ActivateRenderBufferLevel(DL32MACROS_GRAPHICS_ZLEVELINDEX(Z));
		_renderBuffer[DL32MACROS_GRAPHICS_ZLEVELINDEX(Z)]->push_back(Object);

		_textDraw=true;

		return true;
	}
	else
		return false;
}

bool dl32GraphicsClass::DRAW_Pixel(float x, float y, dl32Color color, int Z)
{
	if(!_working) return false;

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL)
	{
		DL32BUFFEROBJECT Object;

		Object.PrimitiveType=D3DPT_POINTLIST;
		Object.PrimitiveCount=1;
		Object.StartIndex=_vertexBuffer.size();
		Object.VertexCount=1;

		_vertexBuffer.push_back(DL32VERTEXTEXTURED(x,y,Z,color,0,-1,-1));

		ActivateRenderBufferLevel(DL32MACROS_GRAPHICS_ZLEVELINDEX(Z));
		_renderBuffer[DL32MACROS_GRAPHICS_ZLEVELINDEX(Z)]->push_back(Object);

		return true;
	}
	else
		return false;
}

bool dl32GraphicsClass::DRAW_Pixels(dl32Pixel pixels[],int count,int Z)
{
	if(!_working) return false;

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL && count>0)
	{
		DL32BUFFEROBJECT Object;

		_vertexBuffer.reserve(_vertexBuffer.size()+count);

		Object.StartIndex=_vertexBuffer.size();
		Object.PrimitiveCount=count;
		Object.VertexCount=count;
		Object.PrimitiveType=D3DPT_POINTLIST;

		_vertexBuffer.insert(_vertexBuffer.end(),pixels,pixels+count);

		ActivateRenderBufferLevel(DL32MACROS_GRAPHICS_ZLEVELINDEX(Z));
		_renderBuffer[DL32MACROS_GRAPHICS_ZLEVELINDEX(Z)]->push_back(Object);

		return true;
	}
	else
		return false;
}

bool dl32GraphicsClass::DRAW_Pixels(dl32Point2D pixels[],dl32Color color,int count,int Z)
{
	if(!_working) return false;

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL && count>0)
	{
		DL32BUFFEROBJECT Object;

		_vertexBuffer.reserve(_vertexBuffer.size()+count);

		Object.StartIndex=_vertexBuffer.size();
		Object.PrimitiveType=D3DPT_POINTLIST;
		Object.PrimitiveCount=count;
		Object.VertexCount=count;

		for(int i=0;i<count;++i)
			_vertexBuffer.push_back(DL32VERTEXTEXTURED(Camera.Apply(pixels[i]),Z,color));

		ActivateRenderBufferLevel(DL32MACROS_GRAPHICS_ZLEVELINDEX(Z));
		_renderBuffer[DL32MACROS_GRAPHICS_ZLEVELINDEX(Z)]->push_back(Object);

		return true;
	}
	else
		return false;
}

bool dl32GraphicsClass::DRAW_Pixels(dl32Color **pixels,float x,float y,int width,int height,int Z)
{
	if(!_working) return false;

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL && width>0 && height>0)
	{
		DL32BUFFEROBJECT Object;

		Object.StartIndex=_vertexBuffer.size();

		_vertexBuffer.reserve(_vertexBuffer.size()+width*height);

		for(int i=0;i<width;++i)
			for(int j=0;j<height;++j)
				_vertexBuffer.push_back(DL32VERTEXTEXTURED(Camera.Apply(x+i,y+j),Z,pixels[j][i]));

		Object.PrimitiveType=D3DPT_POINTLIST;
		Object.VertexCount=width*height;
		Object.PrimitiveCount=width*height;

		ActivateRenderBufferLevel(DL32MACROS_GRAPHICS_ZLEVELINDEX(Z));
		_renderBuffer[DL32MACROS_GRAPHICS_ZLEVELINDEX(Z)]->push_back(Object);

		return true;
	}
	else
		return false;
}

bool dl32GraphicsClass::DRAW_Strip(dl32Pen pen,dl32Point2D points[],int Count,int texture,int Z)
{
	if(!_working) return false;

	if(Z>=DL32CONSTS_GRAPHICS_MINZLEVEL && Z<=DL32CONSTS_GRAPHICS_MAXZLEVEL)
	{
		if(pen.width<=1)
			return true;//DRAW_Lines(points,Count,Z);
		else
		{
			DL32BUFFEROBJECT Object;
			dl32Line2D Line;
			dl32Vector2D preDirection,Direction;
			dl32Point2D preTransformationed,Transformationed,preUp,preDown,up,down,aux;
			int preSize,size,indexCount=0;
			float t0,t1;

			_vertexBuffer.reserve(_vertexBuffer.size()+Count);

			if(texture==-1)
			{
				t0=-1;
				t1=-1;
			}
			else
			{
				t0=0;
				t1=1;
			}

			preSize=_vertexBuffer.size();

			for(int i=0;i<Count;++i)
			{
				Transformationed=Camera.Apply(points[i]);
				size=_vertexBuffer.size()-preSize;

				if(i==0)
				{
					preDirection(Transformationed,Camera.Apply(points[1]));
					Line=dl32Line2D(Transformationed,dl32Vector2D(-preDirection.y,preDirection.x));
					preUp=Line.GetPointByParameter(pen.width/2);
					preDown=Line.GetPointByParameter(-pen.width/2);

					_vertexBuffer.push_back(DL32VERTEXTEXTURED(preDown,Z,pen.color,COLOR_RainbowGradient(i*1280/Count),t0,t1));
					_vertexBuffer.push_back(DL32VERTEXTEXTURED(preUp,Z,pen.color,COLOR_RainbowGradient(i*1280/Count),t0,t0));
					_indexBuffer.push_back(0);
					_indexBuffer.push_back(1);

					indexCount+=2;	
				}
				else if(i<Count-1)
				{
					//FASE 1: Calculamos datos de la unión i-ésima:
					Direction(preTransformationed,Transformationed);

					if(Direction!=preDirection)
						Line=dl32Line2D(Transformationed,(Direction-preDirection));
					else
						Line=dl32Line2D(Transformationed,dl32Vector2D(-Direction.y,Direction.x));



					up=Line.GetPointByParameter(pen.width/2);
					down=Line.GetPointByParameter(-pen.width/2);

					if(dl32Line2D(Transformationed,Direction).GetRelativePosition(up)<0)
					{
						aux=up;
						up=down;
						down=aux;
					}

					//FASE 2: Generamos el quad anterior:
					if(texture==-1)//Si no se usan texturas, generamos un strip contínuo. En caso contrario, generamos quad a quad:
					{
						_vertexBuffer.push_back(DL32VERTEXTEXTURED(down,Z,pen.color,COLOR_RainbowGradient(i*1280/Count),t1,t1));
						_vertexBuffer.push_back(DL32VERTEXTEXTURED(up,Z,pen.color,COLOR_RainbowGradient(i*1280/Count),t1,t0));
						_indexBuffer.push_back(size);
						_indexBuffer.push_back(size+1);

						indexCount+=2;
					}
					else
					{
						//Como se ve, la unión está duplicada, para generar quads independientes:
						_vertexBuffer.push_back(DL32VERTEXTEXTURED(down,Z,pen.color,COLOR_RainbowGradient(i*1280/Count),t1,t1));
						_vertexBuffer.push_back(DL32VERTEXTEXTURED(up,Z,pen.color,COLOR_RainbowGradient(i*1280/Count),t1,t0));
						_vertexBuffer.push_back(DL32VERTEXTEXTURED(down,Z,pen.color,COLOR_RainbowGradient(i*1280/Count),t0,t1));
						_vertexBuffer.push_back(DL32VERTEXTEXTURED(up,Z,pen.color,COLOR_RainbowGradient(i*1280/Count),t0,t0));
						_indexBuffer.push_back(size);
						_indexBuffer.push_back(size+1);
						_indexBuffer.push_back(size+2);
						_indexBuffer.push_back(size+3);

						indexCount+=4;
					}
				}
				else
				{
					Direction(preTransformationed,Transformationed);
					Line=dl32Line2D(Transformationed,dl32Vector2D(-Direction.y,Direction.x));
					up=Line.GetPointByParameter(pen.width/2);
					down=Line.GetPointByParameter(-pen.width/2);

					_vertexBuffer.push_back(DL32VERTEXTEXTURED(down,Z,pen.color,COLOR_RainbowGradient(i*1280/Count),t1,t1));
					_vertexBuffer.push_back(DL32VERTEXTEXTURED(up,Z,pen.color,COLOR_RainbowGradient(i*1280/Count),t1,t0));
					_indexBuffer.push_back(size);
					_indexBuffer.push_back(size+1);

					indexCount+=2;
				}

				if(i!=0)
				{
					preDirection=Direction;
					preUp=up;
					preDown=down;
				}

				preTransformationed=Transformationed;

				//DRAW_Pixel(up.x,up.y,DL32COLOR_YELLOW);
				//DRAW_Pixel(down.x,down.y,DL32COLOR_YELLOW);
			}

			Object.StartIndex=preSize;
			Object.VertexCount=indexCount;
			Object.PrimitiveCount=(texture==-1 ? 2*(Count-1) : 4*Count-6);
			Object.PrimitiveType=D3DPT_TRIANGLESTRIP;
			Object.Texture=texture;

			ActivateRenderBufferLevel(DL32MACROS_GRAPHICS_ZLEVELINDEX(Z));
			_renderBuffer[DL32MACROS_GRAPHICS_ZLEVELINDEX(Z)]->push_back(Object);

			return true;
		}
	}
	else
		return false;
}

bool dl32GraphicsClass::DRAW_Spline(dl32Spline* spline,dl32Color color,int PointsPerInterval,int Z)
{
	if(!_working) return false;

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

		for(int i=0;i<points.size();++i)
			_vertexBuffer.push_back(DL32VERTEXTEXTURED(Camera.Apply(points[i]),Z,COLOR_RainbowGradient(i*1280/points.size())));

		ActivateRenderBufferLevel(DL32MACROS_GRAPHICS_ZLEVELINDEX(Z));
		_renderBuffer[DL32MACROS_GRAPHICS_ZLEVELINDEX(Z)]->push_back(Object);

		return true;
	}
	else
		return false;
}


int dl32GraphicsClass::MAP_Load(dl32String Filename,dl32Color ColorKey,bool Smooth,bool GrayScale)
{
	DL32TEXTURE Texture;
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
		return DL32RETURN_ERRORCODE;
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
		return DL32RETURN_ERRORCODE;
}

