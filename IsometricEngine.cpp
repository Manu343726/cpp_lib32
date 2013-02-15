#include "IsometricEngine.h"

IsometricTilemap::IsometricTilemap(int width,int height,int screenWidth,int screenHeight,int tileWidth,int tileHeight)
{
	_width=width;
	_height=height;
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_tileWidth = tileWidth;
	_tileHeight = tileHeight;

	_camerax=0;
	_cameray=0;
	_zoom=1;
	_viewport_left=0;
	_viewport_top=0;
	_viewport_right=_screenWidth;
	_viewport_bottom=_screenHeight;
	_cameraModified=false;

	_tiles=new _tile*[_width*_height];
	_z=new float*[(_width+1)*(_height+1)];
	_vertex=new dl32Point2D*[(_width+1)*(_height+1)];

	for(int i=0;i<=_width;++i)
	{
		if(i<_width)
			_tiles[i]=new _tile[_height];

		_z[i]=new float[_height+1];
		_vertex[i]=new dl32Point2D[_height+1];
	}

	_setupGeometry();
}

void IsometricTilemap::_setupGeometry()
{
	for(int j=0;j<=_height;++j)
		for(int i=0;i<=_width;++i)
		{
			_vertex[i][j] = project(i*_tileWidth,j*_tileHeight);
			_z[i][j]=0;
		}
}

IsometricTilemap::~IsometricTilemap()
{
	if(_tiles !=NULL)
	{
		for(int i=0;i<=_width;++i)
		{
			if(i<_width)
				DL32MEMORY_SAFEDELETE_ARRAY(_tiles[i]);

			DL32MEMORY_SAFEDELETE_ARRAY(_z[i]);
			DL32MEMORY_SAFEDELETE_ARRAY(_vertex[i]);
		}

		DL32MEMORY_SAFEDELETE_ARRAY(_tiles);
		DL32MEMORY_SAFEDELETE_ARRAY(_z);
		DL32MEMORY_SAFEDELETE_ARRAY(_vertex);
	}
}

void IsometricTilemap::setCamera(float x, float y)
{
	_camerax+=_viewport_left-x;
	_cameray+=_viewport_top-y;
	_viewport_left=x;
	_viewport_top=y;
	_viewport_right=x+_screenWidth;
	_viewport_bottom=y+_screenHeight;

	_cameraModified=true;
}

void IsometricTilemap::moveCamera(float dx, float dy)
{
	_camerax=dx;
	_cameray=dy;
	_viewport_left+=dx;
	_viewport_top+=dy;
	_viewport_right+=dx;
	_viewport_bottom+=dy;

	_cameraModified=true;
}

void IsometricTilemap::setTileZ(int x,int y, float z)
{
	_vertex[x][y].y -= z-_z[x][y];       _z[x][y] = z;
	_vertex[x+1][y].y -= z-_z[x+1][y];   _z[x+1][y] = z;
	_vertex[x+1][y+1].y -= z-_z[x+1][y+1]; _z[x+1][y+1] = z;
	_vertex[x][y+1].y -= z-_z[x][y+1];   _z[x][y+1] = z;
}

void IsometricTilemap::upTile(int x,int y,float dz)
{
	_vertex[x][y].y -= dz;     _z[x][y] = dz;
	_vertex[x+1][y].y -= dz;   _z[x+1][y] = dz;
	_vertex[x+1][y+1].y -= dz; _z[x+1][y+1] = dz;
	_vertex[x][y+1].y -= dz;   _z[x][y+1] = dz; 
}

void IsometricTilemap::levelTile(int x,int y)
{
	float z = (_z[x][y] + _z[x+1][y] + _z[x+1][y+1] + _z[x][y+1])/4;

	_vertex[x][y].y -= z-_z[x][y];         _z[x][y] = z;
	_vertex[x+1][y].y -= z-_z[x+1][y];     _z[x+1][y] = z;
	_vertex[x+1][y+1].y -= z-_z[x+1][y+1]; _z[x+1][y+1] = z;
	_vertex[x][y+1].y -= z-_z[x][y+1];     _z[x][y+1] = z;
}

void IsometricTilemap::setTileTexture(int x,int y, float tx1, float ty1, float tx2, float ty2)
{
	_tiles[x][y].tx1=tx1;
	_tiles[x][y].ty1=ty1;
	_tiles[x][y].tx2=tx2;
	_tiles[x][y].ty2=ty2;
}

void IsometricTilemap::smooth(int x1,int y1, int x2,int y2)
{
	for(int i=x1;i<=x2;++i)
		for(int j=y1;j<=y2;++j)
			levelTile(i,j);
}

inline bool IsometricTilemap::_culling(dl32Vertex verts[4])
{
	return (verts[0].x >= 0 && verts[0].x < _screenWidth && verts[0].y >= 0 && verts[0].y < _screenHeight) ||
		   (verts[1].x >= 0 && verts[1].x < _screenWidth && verts[1].y >= 0 && verts[1].y < _screenHeight) ||
		   (verts[2].x >= 0 && verts[2].x < _screenWidth && verts[2].y >= 0 && verts[2].y < _screenHeight) ||
		   (verts[3].x >= 0 && verts[3].x < _screenWidth && verts[3].y >= 0 && verts[3].y < _screenHeight);
}

//AQUI ESTA EL MEOLLO DE LA CUESTION
void IsometricTilemap::draw(dl32GraphicsClass* gfx)
{
	_tile tile;
	dl32Vertex verts[4];
	int size=(_width)*(_height);

	if(_cameraModified)
		_updateVertex();

	for(int i=0;i<_width;++i)
		for(int j=0;j<_height;++j)
		{
			tile = _tiles[i][j];

			verts[0]=dl32Vertex(_vertex[i][j],tile.color,0,tile.tx1,tile.ty1);
			verts[1]=dl32Vertex(_vertex[i+1][j],tile.color,0,tile.tx2,tile.ty1);
			verts[2]=dl32Vertex(_vertex[i+1][j+1],tile.color,0,tile.tx2,tile.ty2);
			verts[3]=dl32Vertex(_vertex[i][j+1],tile.color,0,tile.tx1,tile.ty2);

			if(false || _culling(verts))
			{
				gfx->DRAW_VertexMap(_tileset,verts,0,true);
			}
		}
}

void IsometricTilemap::_updateVertex()
{
	for(int i=0;i<=_width;++i)
		for(int j=0;j<=_height;++j)
		{
			_vertex[i][j].x+=_camerax; 
			_vertex[i][j].y+= _cameray;
		}

	_camerax=0;
	_cameray=0;
	_cameraModified=false;
}