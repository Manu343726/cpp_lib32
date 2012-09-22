#include "dl32Graphics.h"

using namespace std;

wstring STRING_WSTRING(const string& s);
LPCWSTR STRING_LPCWSTR(const string& s);

PTDL32GRAPHICSCLASS GLOBALdl32GFXCLASS=NULL;

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
	z=DL32D3DVERTEX_ZVALUE;rhw=DL32D3DVERTEX_RHWVALUE;
	diffuse=DL32COLOR_WHITE;specular=DL32COLOR_BLACK;
	tx=-1;ty=-1;
}

DL32VERTEXTEXTURED::DL32VERTEXTEXTURED(float x,float y,int z,D3DCOLOR diffuse,D3DCOLOR specular,float tx,float ty)
{
	this->x=x;this->y=y;this->z=(z-DL32GC_MINZLEVEL+1)*DL32D3DVERTEX_ZVALUE;
	this->rhw=DL32D3DVERTEX_RHWVALUE;
	this->diffuse=diffuse;this->specular=specular;
	this->tx=tx;this->ty=ty;
}

DL32VERTEXTEXTURED::DL32VERTEXTEXTURED(const dl32Vertex &vertex)
{
	x=vertex.x;y=vertex.y;z=(vertex.Z-DL32GC_MINZLEVEL+1)*DL32D3DVERTEX_ZVALUE;
	rhw=DL32D3DVERTEX_RHWVALUE;
	diffuse=vertex.color;specular=DL32COLOR_BLACK;
	tx=-1;ty=-1;
}

//DL32VERTEXTEXTURED::DL32VERTEXTEXTURED(const DL32VERTEXTEXTURED &vertex)
//{
//	if(this!=&vertex)
//	{
//		this->x=vertex.x;
//		this->y=vertex.y;
//		this->z=vertex.z;
//		this->rhw=vertex.rhw;
//		this->diffuse=vertex.diffuse;
//		this->specular=vertex.specular;
//		this->tx=vertex.tx;
//		this->ty=vertex.ty;
//	}
//}

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

dl322DPoint dl32Vertex::Baricenter(dl32Vertex PointList[],int PointCount)
{
	dl322DPoint Return;

	for(int i=0;i<PointCount;++i)
	{
		/*Return.x+=(PointList+i*TypeSize)->x;
		Return.y+=(PointList+i*TypeSize)->y;*/
		Return.x+=PointList[i].x;
		Return.y+=PointList[i].y;
	}

	return dl322DPoint(Return.x/PointCount,Return.y/PointCount);
}

dl322DCamera::dl322DCamera()
{
	m11=1;m12=0;m13=0;
	m21=0;m22=1;m23=0;
	m31=0;m32=0;m33=1;
}

dl322DCamera::dl322DCamera(dl323x3Matrix &Transform)
{
	m11=Transform.m11;m12=Transform.m12;m13=Transform.m13;
	m21=Transform.m21;m22=Transform.m22;m23=Transform.m23;
	m31=Transform.m31;m32=Transform.m32;m33=Transform.m33;
}

void dl322DCamera::Rotate(float Rotation)
{
	*this=dl323x3Matrix::Mul(dl322DTransform::Rotation(Rotation),*this);
}

void dl322DCamera::Rotate(dl322DPoint Center,float Rotation)
{
	*this=dl323x3Matrix::Mul(dl322DTransform::Rotation(Center,Rotation),*this);
}

void dl322DCamera::Traslate(float x,float y)
{
	*this=dl323x3Matrix::Mul(dl322DTransform::Traslation(-x,-y),*this);
}

void dl322DCamera::Traslate(dl322DVector Traslation)
{
	*this=dl323x3Matrix::Mul(dl322DTransform::Traslation(-Traslation.x,-Traslation.y),*this);
}

void dl322DCamera::SetPosition(float x, float y)
{
	m13=-x;
	m23=-y;
}

void dl322DCamera::SetPosition(dl322DPoint &Position)
{
	m13=-Position.x;
	m23=-Position.y;
}

dl322DPoint dl322DCamera::GetPosition()
{
	return dl322DPoint(-m13,-m23);
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

dl32Mesh::dl32Mesh(dl322DAABB Area,int width, int height,dl32MeshPatch *patches,int PatchesCount)
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

dl322DPoint dl32Mesh::GetPatchCenter(dl32MeshPatch &patch)
{
	if(IsValid(patch))
	{
		dl322DPoint center;
		int vertexcount=patch.width*patch.height;

		for(int i=patch.x;i<patch.x+patch.width;++i)
			for(int j=patch.y;j<patch.y+patch.height;++j)
			{
				center.x+=verts[i+j*width].x;
				center.y+=verts[i+j*width].y;
			}

		return dl322DPoint(center.x/vertexcount,center.y/vertexcount);
	}
	else
		return dl322DPoint();
}

dl322DPoint dl32Mesh::GetMeshCenter()
{
	dl322DPoint Center;

	for(int i=0;i<width*height;++i)
	{
		Center.x+=verts[i].x;
		Center.y+=verts[i].y;
	}

	return dl322DPoint(Center.x/(width*height),Center.y/(width*height));
}

void dl32Mesh::Transform(dl322DTransform transform)
{
	for(int i=0;i<verts.size();++i)
		transform.Apply(&verts[i].x,&verts[i].y);
}

void dl32Mesh::Transform(dl322DTransform transform,dl32MeshPatch &patch)
{
	if(IsValid(patch))
	{
		for(int i=patch.x;i<patch.x+patch.width;++i)
			for(int j=patch.y;j<patch.y+patch.height;++j)
				transform.Apply(&verts[i+width*j].x,&verts[i+width*j].y);
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
	if(!Working) return false;

	DL32BUFFEROBJECT Object;
	int RenderBufferLevelSize,ActiveLevelsSize=0;
	int Index;

	if (PreDrawProc!=NULL) PreDrawProc(this);

	device->Clear(0,NULL,D3DCLEAR_TARGET,BackColor,0,0);
	device->BeginScene();
	
	if(FillD3DBuffers())
	{
		ActiveLevelsSize=RenderBufferActiveLevels.size();

		for(int i=0;i<ActiveLevelsSize;++i)
		{
			Index=RenderBufferActiveLevels[i];
			RenderBufferLevelSize=RenderBuffer[Index]->size();

			for(int j=0;j<RenderBufferLevelSize;++j)
			{
				Object=RenderBuffer[Index]->at(j);

				if(Object.Texture!=RenderData.TextureIndex)
				{
					if(Object.Texture<0)
					{
						RenderData.Texture=NULL;
						RenderData.TextureIndex=-1;
						device->SetTexture(0,NULL);
					}
					else
					{
						RenderData.TextureIndex=Object.Texture;
						RenderData.Texture=Textures[Object.Texture].Texture;

						device->SetTexture(0,Textures[Object.Texture].Texture);
					}
				}

				if(Object.BaseIndex<0)
					device->DrawPrimitive(Object.PrimitiveType,Object.StartIndex,Object.PrimitiveCount);
				else
					device->DrawIndexedPrimitive(Object.PrimitiveType,Object.BaseIndex,0,Object.VertexCount,Object.StartIndex,Object.PrimitiveCount);
			}

			delete RenderBuffer[Index];
			RenderBuffer[Index]=NULL;
		}
	}

	device->EndScene();
	device->Present(NULL,NULL,NULL,NULL);

	if(d3dVertexBufferOK) d3dVertexBuffer->Release();
	if(d3dIndexBufferOK) d3dIndexBuffer->Release();
	VertexBuffer.clear();
	IndexBuffer.clear();
	RenderBufferActiveLevels.clear();


	if (PostDrawProc!=NULL) PostDrawProc(this);

	return true;
}

bool dl32GraphicsClass::InitializeDirect3D(HWND hwnd,int Width,int Height,bool Windowed)
{
	d3d=Direct3DCreate9(D3D_SDK_VERSION);

	ZeroMemory(&pp,sizeof(D3DPRESENT_PARAMETERS));
	pp.Windowed=int(Windowed);
	pp.SwapEffect=D3DSWAPEFFECT_DISCARD;
	pp.BackBufferFormat=D3DFMT_UNKNOWN;

	if(FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,D3DCREATE_HARDWARE_VERTEXPROCESSING,&pp, &device)))
		return false;
	else
	{
		device->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
		return true;
	}
}

dl32GraphicsClass::dl32GraphicsClass()
{
	ConstructorError=DL32GCCE_DEFAULTCTOR;
	Working=false;
	UsingVertexBuffer=false;
}

dl32GraphicsClass::dl32GraphicsClass(HWND hwnd,int Width,int Height,bool Windowed)
{
	PreDrawProc=NULL;
	PostDrawProc=NULL;

	Working=false;
	UsingVertexBuffer=false;

	if(!InitializeDirect3D(hwnd,Width,Height,Windowed))
		ConstructorError=DL32GCCE_D3DINITFAILED;
	else
	{
		BackColor=D3DCOLOR_ARGB(255,0,0,0);
		GLOBALdl32GFXCLASS=this;
		ConstructorError=DL32GCCE_NOERRORS;
		Working=true;

		for(int i=0;i<DL32GC_MAXZLEVEL-DL32GC_MINZLEVEL;++i)
			RenderBuffer[i]=NULL;
	}
}

bool dl32GraphicsClass::Exit()
{
	Dispose();
	return true;
}

void dl32GraphicsClass::Dispose()
{
	if(Working)
	{
		Working=false;

		if(UsingVertexBuffer) d3dVertexBuffer->Release();
		device->Release();
		d3d->Release();

		for(int i=0;i<RenderBufferActiveLevels.size();++i)
			delete RenderBuffer[RenderBufferActiveLevels[i]];
	}
}

dl32GraphicsClass::~dl32GraphicsClass()
{
	Dispose();
}

bool dl32GraphicsClass::NoErrors()
{
	return ConstructorError==DL32GCCE_NOERRORS;
}

DL32GRAPHICSCLASS_CTORERRORS dl32GraphicsClass::InitError()
{
	return ConstructorError;
}

bool dl32GraphicsClass::Start()
{
	if(ConstructorError==DL32GCCE_NOERRORS)
	{
		RenderData.TextureIndex=-1;
		RenderData.Texture=NULL;
		Working=true;

		return true;
	}
	else
		return false;
}

void dl32GraphicsClass::DEVICE_SetPreDrawProc(PTRENDERLOOPPROC DrawProc)
{
	PreDrawProc=DrawProc;
}

void dl32GraphicsClass::DEVICE_SetPostDrawProc(PTRENDERLOOPPROC DrawProc)
{
	PreDrawProc=DrawProc;
}

void dl32GraphicsClass::DEVICE_RemovePreDrawProc()
{
	PreDrawProc=NULL;
}

void dl32GraphicsClass::DEVICE_RemovePostDrawProc()
{
	PostDrawProc=NULL;
}

void dl32GraphicsClass::DEVICE_SetBackgroundColor(dl32Color NewBackColor)
{
	BackColor=NewBackColor;
}

dl32Color dl32GraphicsClass::DEVICE_GetBackgroundColor()
{
	return BackColor;
}

bool dl32GraphicsClass::FillD3DBuffers()
{
	void *VertexBufferArray,*IndexBufferArray;
	bool VertexBufferFailed;
	d3dVertexBufferOK=false;
	d3dIndexBufferOK=false;

	VertexBufferFailed=FAILED(device->CreateVertexBuffer(DL32VERTEXTEXTURED_SIZE*VertexBuffer.size(),D3DUSAGE_DYNAMIC,DL32VERTEXTEXTURED_FVF,D3DPOOL_SYSTEMMEM,&d3dVertexBuffer,NULL));

	if(VertexBufferFailed)
		return false;
	else
	{
		UsingVertexBuffer=true;

		device->SetStreamSource(0,d3dVertexBuffer,0,DL32VERTEXTEXTURED_SIZE);
		device->SetFVF(DL32VERTEXTEXTURED_FVF);

		if(FAILED(d3dVertexBuffer->Lock(0,0,&VertexBufferArray,D3DLOCK_DISCARD)))
			return false;
		else
		{
			memcpy(VertexBufferArray,VertexBuffer.data(),VertexBuffer.size()*DL32VERTEXTEXTURED_SIZE);
			d3dVertexBufferOK=true;
		}
	}

	if(IndexBuffer.size()>0)
	{
		if(FAILED(device->CreateIndexBuffer(sizeof(int)*IndexBuffer.size(),D3DUSAGE_DYNAMIC,D3DFMT_INDEX32,D3DPOOL_SYSTEMMEM,&d3dIndexBuffer,NULL)))
			return false;
		else
		{
			device->SetIndices(d3dIndexBuffer);
			if(FAILED(d3dIndexBuffer->Lock(0,0,&IndexBufferArray,D3DLOCK_DISCARD)))
				return false;
			else
			{
				memcpy(IndexBufferArray,IndexBuffer.data(),IndexBuffer.size()*sizeof(int));
				d3dIndexBufferOK=true;
			}
		}
	}

	return true;
}

bool dl32GraphicsClass::ActivateRenderBufferLevel(int Index)
{
	if(RenderBuffer[Index]==NULL)
	{
		RenderBuffer[Index]=new vector<DL32BUFFEROBJECT>;
		RenderBufferActiveLevels.push_back(Index);
		return true;
	}
	else
		return false;
}

bool dl32GraphicsClass::DRAW_Triangle(float x0, float y0, float x1,float y1, float x2, float y2,dl32Color color,bool fill,int Z)
{
	if(!Working) return false;

	DL32BUFFEROBJECT Object;
	dl322DPoint NewVerts[3];

	if(Z>=DL32GC_MINZLEVEL && Z<=DL32GC_MAXZLEVEL)
	{
		DL32VERTEXTEXTURED verts[3];

		NewVerts[0]=Camera.Apply(x0,y0);
		NewVerts[1]=Camera.Apply(x1,y1);
		NewVerts[2]=Camera.Apply(x2,y2);


		verts[0]=DL32VERTEXTEXTURED(NewVerts[0].x,NewVerts[0].y,Z,color,0,-1,-1);
		verts[1]=DL32VERTEXTEXTURED(NewVerts[1].x,NewVerts[1].y,Z,color,0,-1,-1);
		verts[2]=DL32VERTEXTEXTURED(NewVerts[2].x,NewVerts[2].y,Z,color,0,-1,-1);

		Object.StartIndex=VertexBuffer.size();
		Object.VertexCount=3;
		Object.Texture=-1;

		Object.CallType=RBCT_DRAWTRIANGLE;

		VertexBuffer.insert(VertexBuffer.end(),verts,verts+3);

		if(fill)
		{
			Object.PrimitiveType=D3DPT_TRIANGLESTRIP;
			Object.PrimitiveCount=1;
		}
		else
		{
			Object.PrimitiveType=D3DPT_LINESTRIP;
			Object.PrimitiveCount=3;
			VertexBuffer.push_back(verts[0]);
		}

		ActivateRenderBufferLevel(Z-DL32GC_MINZLEVEL);
		RenderBuffer[Z-DL32GC_MINZLEVEL]->push_back(Object);

		return true;
	}
	else
		return false;
}

bool dl32GraphicsClass::DRAW_Triangle(dl32Vertex V0, dl32Vertex V1, dl32Vertex V2,bool fill,int Z)
{	
	if(!Working) return false;

	DL32BUFFEROBJECT Object;

	if(Z>=DL32GC_MINZLEVEL && Z<=DL32GC_MAXZLEVEL)
	{
		DL32VERTEXTEXTURED verts[3];
		dl322DPoint NewVerts[3];

		NewVerts[0]=Camera.Apply(V0);
		NewVerts[1]=Camera.Apply(V1);
		NewVerts[2]=Camera.Apply(V2);

		verts[0]=DL32VERTEXTEXTURED(NewVerts[0].x,NewVerts[0].y,Z,V0.color,0,-1,-1);
		verts[1]=DL32VERTEXTEXTURED(NewVerts[1].x,NewVerts[1].y,Z,V1.color,0,-1,-1);
		verts[2]=DL32VERTEXTEXTURED(NewVerts[2].x,NewVerts[2].y,Z,V2.color,0,-1,-1);

		Object.StartIndex=VertexBuffer.size();
		Object.VertexCount=3;
		Object.Texture=-1;

		Object.CallType=RBCT_DRAWTRIANGLE;

		VertexBuffer.insert(VertexBuffer.end(),verts,verts+3);

		if(fill)
		{
			Object.PrimitiveType=D3DPT_TRIANGLESTRIP;
			Object.PrimitiveCount=1;
		}
		else
		{
			Object.PrimitiveType=D3DPT_LINESTRIP;
			Object.PrimitiveCount=3;
			VertexBuffer.push_back(verts[0]);
		}

		ActivateRenderBufferLevel(Z-DL32GC_MINZLEVEL);
		RenderBuffer[Z-DL32GC_MINZLEVEL]->push_back(Object);

		return true;
	}
	else
		return false;
}

bool dl32GraphicsClass::DRAW_Polygon(const dl32Vertex Verts[],int Count,bool fill,int Z)
{
	if(!Working) return false;

	if(Z>=DL32GC_MINZLEVEL && Z<=DL32GC_MAXZLEVEL && Count>=3)
	{
		DL32BUFFEROBJECT Object;
		dl322DPoint Baricenter,NewVert,InitVert;
		D3DCOLOR CenterColor=0;

		Object.StartIndex=VertexBuffer.size();

		for(int i=0;i<Count;++i)
		{
			NewVert=Camera.Apply(Verts[i]);
			if(i==0) InitVert=NewVert;

			Baricenter.x+=NewVert.x;
			Baricenter.y+=NewVert.y;
			CenterColor+=Verts[i].color;

			VertexBuffer.push_back(DL32VERTEXTEXTURED(NewVert.x,NewVert.y,Z,Verts[i].color,0,-1,-1));
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
			VertexBuffer.insert(VertexBuffer.begin()+VertexBuffer.size()-Count,DL32VERTEXTEXTURED(Baricenter.x,Baricenter.y,Z,CenterColor,0,-1,-1));
			VertexBuffer.push_back(DL32VERTEXTEXTURED(InitVert.x,InitVert.y,Z,Verts[0].color,0,-1,-1));
		}
		else
		{
			Object.PrimitiveType=D3DPT_LINESTRIP;
			Object.PrimitiveCount=Count;
			Object.VertexCount=Count+1;
			VertexBuffer.push_back(DL32VERTEXTEXTURED(InitVert.x,InitVert.y,Z,Verts[0].color,0,-1,-1));
		}

		ActivateRenderBufferLevel(Z-DL32GC_MINZLEVEL);
		RenderBuffer[Z-DL32GC_MINZLEVEL]->push_back(Object);

		return true;
	}
	else
		return false;
}

bool dl32GraphicsClass::DRAW_Polygon(const dl322DPoint Verts[],int Count,dl32Color color,bool fill,int Z)
{
	if(!Working) return false;

	if(Z>=DL32GC_MINZLEVEL && Z<=DL32GC_MAXZLEVEL && Count>=3)
	{
		DL32BUFFEROBJECT Object;
		dl322DPoint Baricenter,NewVert,InitVert;

		Object.StartIndex=VertexBuffer.size();

		for(int i=0;i<Count;++i)
		{
			NewVert=Camera.Apply(Verts[i]);
			if(i==0) InitVert=NewVert;

			Baricenter.x+=NewVert.x;
			Baricenter.y+=NewVert.y;

			VertexBuffer.push_back(DL32VERTEXTEXTURED(NewVert.x,NewVert.y,Z,color,0,-1,-1));
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
			VertexBuffer.insert(VertexBuffer.begin()+VertexBuffer.size()-Count,DL32VERTEXTEXTURED(Baricenter.x,Baricenter.y,Z,color,0,-1,-1));
			VertexBuffer.push_back(DL32VERTEXTEXTURED(InitVert.x,InitVert.y,Z,color,0,-1,-1));
		}
		else
		{
			Object.PrimitiveType=D3DPT_LINESTRIP;
			Object.PrimitiveCount=Count;
			Object.VertexCount=Count+1;
			VertexBuffer.push_back(DL32VERTEXTEXTURED(InitVert.x,InitVert.y,Z,color,0,-1,-1));
		}

		ActivateRenderBufferLevel(Z-DL32GC_MINZLEVEL);
		RenderBuffer[Z-DL32GC_MINZLEVEL]->push_back(Object);

		return true;
	}
	else
		return false;
}

bool dl32GraphicsClass::DRAW_Box(float x,float y,float width, float height,dl32Color color,bool fill,int Z)
{
	dl322DPointTrapezoid Verts={dl322DPoint(x,y),dl322DPoint(x+width,y),dl322DPoint(x+width,y+height),dl322DPoint(x,y+height)};

	return DRAW_Trapezoid(Verts,color,fill,Z);
}

bool dl32GraphicsClass::DRAW_VertexMap(int texture,const dl32VertexTrapezoid verts,int Z)
{
	if(!Working) return false;

	if(Z>=DL32GC_MINZLEVEL && Z<=DL32GC_MAXZLEVEL && texture>=0 && texture < Textures.size())
	{
		DL32BUFFEROBJECT Object;
		DL32VERTEXTEXTURED Verts[4];
		dl322DPoint NewVerts[4];

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
		Object.StartIndex=VertexBuffer.size();
		Object.VertexCount=4;
		Object.Texture=texture;

		Object.CallType=RBCT_DRAWVERTEXMAP;

		VertexBuffer.insert(VertexBuffer.end(),Verts,Verts+4);

		ActivateRenderBufferLevel(Z-DL32GC_MINZLEVEL);
		RenderBuffer[Z-DL32GC_MINZLEVEL]->push_back(Object);

		return true;
	}
	else
		return false;
}

bool dl32GraphicsClass::DRAW_Mesh(dl32Mesh Mesh,int Z)
{
	if(!Working) return false;

	if(Z>=DL32GC_MINZLEVEL && Z<=DL32GC_MAXZLEVEL)
	{
		DL32BUFFEROBJECT Object;
		int BaseIndex=VertexBuffer.size();
		int StripVertexCount;
		int StripPrimitiveCount;

		Mesh.Transform(Camera);

		for(int i=0;i<Mesh.verts.size();++i)
			Mesh.verts[i].z=(Z-DL32GC_MINZLEVEL+1)*DL32D3DVERTEX_ZVALUE;

		VertexBuffer.insert(VertexBuffer.end(),Mesh.verts.begin(),Mesh.verts.end());

		for(int i=0;i<Mesh.patches.size();++i)
		{
			//StripVertexCount=Mesh.verts.size();
			StripVertexCount=Mesh.verts.size();
			StripPrimitiveCount=(Mesh.patches[i].width-1)*2;

			for(int j=0;j<Mesh.patches[i].height-1;++j)
			{
				Object.BaseIndex=BaseIndex;
				Object.StartIndex=IndexBuffer.size()+(j*Mesh.patches[i].width*2);
				Object.PrimitiveType=D3DPT_TRIANGLESTRIP;
				Object.VertexCount=StripVertexCount;
				Object.PrimitiveCount=StripPrimitiveCount;
				Object.Texture=Mesh.patches[i].texture;

				Object.CallType=RBCT_MESH;

				ActivateRenderBufferLevel(Z-DL32GC_MINZLEVEL);
				RenderBuffer[Z-DL32GC_MINZLEVEL]->push_back(Object);
			}

			IndexBuffer.insert(IndexBuffer.end(),Mesh.indexes[i].begin(),Mesh.indexes[i].end());
		}

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

	if(FAILED(D3DXCreateTextureFromFileEx(device,Filename.c_str(),D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,Format,D3DPOOL_MANAGED,Filter,Filter,ColorKey,&ImageInfo,NULL,&Texture.Texture)))
		return DL32RETURN_ERRORCODE;
	else
	{
		Texture.OriginalWidth=ImageInfo.Width;
		Texture.OriginalHeight=ImageInfo.Height;
		Texture.ColorKey=ColorKey;
		Texture.Index=Textures.size();
		Texture.Format=Format;

		Textures.push_back(Texture);

		return Texture.Index;
	}
}

