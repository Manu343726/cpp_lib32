#include "IsometricEngine.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_ISOMETRICENGINE_ENGINETEST

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
	_cameraModified=true;

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

void IsometricTilemap::moveCamera(float dx, float dy)
{
	_camerax+=dx;
	_cameray+=dy;
	_viewport_left+=dx;
	_viewport_top+=dy;
	_viewport_right+=dx;
	_viewport_bottom+=dy;

	_cameraModified=true;
}

void IsometricTilemap::zoom(float scale)
{
	dl32Point2D center((_viewport_left+(_screenWidth/2))*scale,(_viewport_top+(_screenHeight/2))*scale);

	for(int i=0;i<=_width;++i)
		for(int j=0;j<=_height;++j)
		{
			_vertex[i][j].x*=scale;
			_vertex[i][j].y*=scale;
		}

	setCamera(center);
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
	_vertex[x][y].y -= dz;     _z[x][y] += dz;
	_vertex[x+1][y].y -= dz;   _z[x+1][y] += dz;
	_vertex[x+1][y+1].y -= dz; _z[x+1][y+1] += dz;
	_vertex[x][y+1].y -= dz;   _z[x][y+1] += dz; 
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

dl32Point2D IsometricTilemap::getTileCenter(int x, int y) const
{
	return dl32Point2D((_vertex[x][y].x + _vertex[x+1][y].x + _vertex[x+1][y+1].x + _vertex[x][y+1].x)/4,(_vertex[x][y].y + _vertex[x+1][y].y + _vertex[x+1][y+1].y + _vertex[x][y+1].y)/4);
}

void IsometricTilemap::smooth(int x1,int y1, int x2,int y2)
{
	for(int i=x1;i<=x2;++i)
		for(int j=y1;j<=y2;++j)
			levelTile(i,j);
}

inline bool IsometricTilemap::_culling(dl32Point2D v1,dl32Point2D v2,dl32Point2D v3,dl32Point2D v4)
{
	return (v1.x >= -1 && v1.x < _screenWidth && v1.y >= -1 && v1.y < _screenHeight) ||
		   (v2.x >= -1 && v2.x < _screenWidth && v2.y >= -1 && v2.y < _screenHeight) ||
		   (v3.x >= -1 && v3.x < _screenWidth && v3.y >= -1 && v3.y < _screenHeight) ||
		   (v4.x >= -1 && v4.x < _screenWidth && v4.y >= -1 && v4.y < _screenHeight);
}

//AQUI ESTA EL MEOLLO DE LA CUESTION
void IsometricTilemap::draw(dl32GraphicsClass* gfx)
{
	_tile tile;
	dl32Vertex verts[4];

	//dl32Point2D* bottomRightBorder,*bottomLeftBorder;

	//bottomLeftBorder=new dl32Point2D[_width+3];
	//bottomRightBorder=new dl32Point2D[_height+3];

	if(_cameraModified)
		_updateVertex();

	//bottomLeftBorder[0]=_vertex[0][_height];
	//bottomLeftBorder[0].y+=_tileWidth*10;
	//bottomLeftBorder[_width+2]=_vertex[_width][_height];
	//bottomLeftBorder[_width+2].y+=_tileWidth*10;

	//bottomRightBorder[0]=_vertex[_width][0];
	//bottomRightBorder[0].y+=_tileWidth*10;
	//bottomRightBorder[_height+2]=bottomLeftBorder[_width+2];

	//for(int i=0;i<=_width;++i)
	//	bottomLeftBorder[i+1] = _vertex[i][_height];
	//for(int i=0;i<=_height;++i)
	//	bottomRightBorder[i+1] = _vertex[_width][i];

	for(int i=0;i<_width;++i)
		for(int j=0;j<_height;++j)
		{
			tile = _tiles[i][j];

			if(tile.object<0 && tile.visible)
			{
				verts[0]=dl32Vertex(_vertex[i][j],tile.color,0,tile.tx1,tile.ty1);
				verts[1]=dl32Vertex(_vertex[i+1][j],tile.color,0,tile.tx2,tile.ty1);
				verts[2]=dl32Vertex(_vertex[i+1][j+1],tile.color,0,tile.tx2,tile.ty2);
				verts[3]=dl32Vertex(_vertex[i][j+1],tile.color,0,tile.tx1,tile.ty2);

				gfx->DRAW_VertexMap(_tileset,verts,0,true);
			}
		}

	//gfx->DRAW_Polygon(bottomLeftBorder,_width+3,COLOR_FromRGB(55,55,55));
	//gfx->DRAW_Polygon(bottomRightBorder,_height+3,COLOR_FromRGB(30,30,30));

	//DL32MEMORY_SAFEDELETE_ARRAY(bottomLeftBorder);
	//DL32MEMORY_SAFEDELETE_ARRAY(bottomRightBorder);
}

void IsometricTilemap::_updateVertex()
{
	bool *lastRowVisible = new bool[_height+1];
	bool lastVisible,visible;

	_visibleCount = 0;

	for(int i=0;i<=_width;++i)
		for(int j=0;j<=_height;++j)
		{
			_vertex[i][j].x+=_camerax; 
			_vertex[i][j].y+=_cameray;

			visible = _vertex[i][j].x >= 0 && _vertex[i][j].x < _screenWidth && _vertex[i][j].y >= 0 && _vertex[i][j].y < _screenHeight;

			if(i>0 && j>0)
				_tiles[i-1][j-1].visible = lastRowVisible[j-1] || lastRowVisible[j] || lastVisible || visible;
			
			lastRowVisible[j] = visible;
			lastVisible = visible;

			_visibleCount+=(int)visible;
		}

	_camerax=0;
	_cameray=0;
	_cameraModified=false;

	delete lastRowVisible;
}

dl32Point2D IsometricTilemap::pick(float x, float y)
{
	dl32Point3D coords;

	for(int i=_width-1;i>=0;--i)
		for(int j=_height-1;j>=0;--j)
			if(_tiles[i][j].visible && _tiles[i][j].object<0)
			{
				coords = baricentricCoordinates(_vertex[i][j], _vertex[i+1][j], _vertex[i][j+1],x,y);//Primer triángulo

				if(coords.x >= 0 && coords.x <= 1 && coords.y>=0 && coords.y<=1 && coords.z>=0 && coords.z<=1)
					return dl32Point2D(i,j);
				else
				{
					coords = baricentricCoordinates(_vertex[i+1][j], _vertex[i+1][j+1], _vertex[i][j+1],x,y);//Segundo triángulo

					if(coords.x >= 0 && coords.x <= 1 && coords.y>=0 && coords.y<=1 && coords.z>=0 && coords.z<=1)
						return dl32Point2D(i,j);
				}
			}

	return dl32Point2D(-1,-1);
}

dl32Point3D IsometricTilemap::baricentricCoordinates(dl32Point2D v1, dl32Point2D v2, dl32Point2D v3, float x, float y)
{
	dl32Point3D coords;
	float det = ((v2.y-v3.y)*(v1.x-v3.x) + (v3.x-v2.x)*(v1.y-v3.y));

	coords.x = ((v2.y-v3.y)*(x-v3.x) + (v3.x-v2.x)*(y-v3.y)) / det;
	coords.y = ((v3.y-v1.y)*(x-v3.x) + (v1.x-v3.x)*(y-v3.y)) / det;
	coords.z = 1-coords.x-coords.y;

	return coords;
}

#endif