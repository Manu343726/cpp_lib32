#include "dl32Graphics.h"
#include "dl32Math.h"
#include "dl32Console.h"

struct sprite
{
	int texture;
	dl32AABB2D area;
};

struct _tile
{
        dl32Color color;
	float tx1,ty1;
	float tx2,ty2;
	bool visible;

	int object;

	_tile(){color=DL32COLOR_DARKGREEN;visible=true;tx1=-1;ty1=-1;tx2=-1;ty2=-1;object=-1;}
	_tile(int Texture,dl32Color Color, bool Visible = true, float Tx1 = -1, float Ty1 = -1,float Tx2=-1,float Ty2=-1){color=Color;visible=Visible;tx1=Tx1;ty1=Ty1;tx2=Tx2;ty2=Ty2;object=-1;}
};

struct _sprite
{
	float x,y,width,height;
	float tx1,ty1,tx2,ty2;

	_sprite(int px, int py, int Width, int Height){x=px-(Width/2);y=py-Height;}
};

class IsometricTilemap
{	
#define      SQRT2         1.4142135623730950488016887242097
#define      SQRT3         1.7320508075688772935274463415059
#define      SQRT6         2.4494897427831780981972840747059
#define		 SQRT2__2      0.70710678118654752440084436210485
#define	    _SQRT2__2     -0.70710678118654752440084436210485
#define		 SQRT2__3      0.27216552697590867757747600830065
#define	    _SQRT2__3     -0.27216552697590867757747600830065
#define      SQRT3__SQRT6  0.70710678118654752440084436210485
#define     _SQRT3__SQRT6 -0.70710678118654752440084436210485
#define    __SQRT6         0.40824829046386301636621401245098
#define TWO__SQRT6         0.81649658092772603273242802490196

private:
	bool _culling(dl32Point2D v1,dl32Point2D v2,dl32Point2D v3,dl32Point2D v4);
	int _width,_height;
	int _screenWidth,_screenHeight;
	float _camerax,_cameray;
	float _viewport_left,_viewport_top,_viewport_right,_viewport_bottom;
	bool _cameraModified;
	float _zoom;

	int _tileWidth,_tileHeight;

	_tile** _tiles;
	int _tileset;


	float** _z; //Altura de los vertices del terreno
	dl32Point2D** _vertex; //Coordenadas de los vertices del tilemap (Ya proyectados)

	void _setupGeometry();

	IsometricTilemap(const IsometricTilemap &tilemap);
	IsometricTilemap& operator=(const IsometricTilemap &tilemap);

	//virtual void _culling() = NULL;

	void _updateVertex();
	int _visibleCount;
public:
	IsometricTilemap()                                     {_tiles=NULL;_z=NULL;}
	IsometricTilemap(int width,int height,int screenWidth,int screenHeight,int tileWidth,int tileHieght);
	~IsometricTilemap();

	void setTileset(int texture)                            {_tileset=texture;}

	dl32Point2D getCamera()                           const {return dl32Point2D(_camerax,_cameray);}
	void setCamera(float x,float y)                         {moveCamera(_screenWidth/2-x,_screenHeight/2-y);}
	void setCamera(dl32Point2D camera)                      {setCamera(camera.x,camera.y);}

	void moveCamera(float dx,float dy);
	void moveCamera(dl32Point2D move)                       {moveCamera(move.x,move.y);}
	void zoom(float scale);

	void setTileZ(int x, int y,float z);
	void upTile(int x, int y,float dz);
	void upVertex(int x, int y,float dz)                    {_z[x][y] += dz;}
	void setVertexZ(int x, int y,float z)                   {_z[x][y] = z;}
	float getVertexZ(int x, int y)                          {return _z[x][y];}
	dl32Point2D getTileCenter(int x, int y)        const;
	dl32Point2D getTileCenter(dl32Point2D tile)    const    {return getTileCenter((int)tile.x,(int)tile.y);} 

	dl32Color getTileColor(int x, int y)           const    {return _tiles[x][y].color;}
	void setTileColor(int x, int y,dl32Color color)         {_tiles[x][y].color = color;}
	void setTileTexture(int x, int y,float tx1,float ty1,float tx2, float ty2);

	void levelTile(int x, int y);
	void smooth(int x1, int y1,int x2, int y2);
	void smooth(dl32Point2D begin, dl32Point2D end)         {smooth(begin.x,begin.y,end.x,end.y);}

	void draw(dl32GraphicsClass* gfx);

	dl32Point2D pick(float x, float y);
	dl32Point2D pick(dl32Point2D coords)                    {return pick(coords.x,coords.y);}

	dl32Point3D baricentricCoordinates(dl32Point2D v1, dl32Point2D v2, dl32Point2D v3, float x, float y);

	static dl32Point2D project(float x, float y)            {return dl32Point2D(-(SQRT2__2*(x-y)),-(SQRT2__3-__SQRT6*(x+y)));}
	static dl32Point2D project(float x, float y, float z)   {return dl32Point2D(-(SQRT2__2*(x-y)),-(SQRT2__3+z-__SQRT6*(x+y)));}
	static dl32Point2D project(dl32Point2D coords)          {return project(coords.x,coords.y);}
	static dl32Point2D project(dl32Point2D coords, float z) {return project(coords.x,coords.y,z);}
	static dl32Point2D project(dl32Point3D coords)		    {return project(coords.x,coords.y,coords.z);}

	int visibleTilesCount()                           const {return _visibleCount;}
};