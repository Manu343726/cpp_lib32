#include "IsometricEngine.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_ISOMETRICENGINE_ENGINETEST
	
dl32GraphicsClass* gfx;
dl32Window* window;

IsometricTilemap* tilemap;

dl32String ToString(dl32Point2D point);

void OnIdle();
void OnKeyDown(dl32KeyboardData keydata);
void OnKeyUp(dl32KeyboardData data);
void OnMouseWheel(dl32MouseData data);
void OnMouseDown(dl32MouseData data);
void OnMouseDoubleClick(dl32MouseData data);
void OnMouseUp(dl32MouseData data);
void OnMouseMove(dl32MouseData data);

const int TILEMAP_WIDTH = 128;                              //Ancho del tilemap (tiles)
const int TILEMAP_HEIGHT = 128 ;                             //Alto del tilemap (tiles)
const int TILEMAP_TILE_WIDTH = 32;	                        //Ancho de un tile 
const int TILEMAP_TILE_HEIGHT = 32;                        //Alto de un tile
const int TILEMAP_TILECOUNT = TILEMAP_WIDTH*TILEMAP_HEIGHT; //Total de tiles en el tilemap

const int TILESET_TILE_WIDTH = 64;                                    //Ancho de un tile en el tileset (pixels)
const int TILESET_TILE_HEIGHT = 64;                                   //Alto de un tile en el tileset (pixels)
const int TILESET_WIDTH_PIXELS = 1024;                                //Ancho del tileset (pixels)
const int TILESET_HEIGHT_PIXELS = 1024;                               //Alto del tileset (pixels)
const int TILESET_WIDTH = TILESET_WIDTH_PIXELS/TILESET_TILE_WIDTH;    //Ancho del tileset (tiles)
const int TILESET_HEIGHT = TILESET_HEIGHT_PIXELS/TILESET_TILE_HEIGHT; //Alto del tileset (tiles)
const float TILESET_TILE_UNIT_WIDTH = TILESET_TILE_WIDTH/(float)TILESET_WIDTH_PIXELS;    //Ancho en coordenadas de textura de un tile
const float TILESET_TILE_UNIT_HEIGHT = TILESET_TILE_HEIGHT/(float)TILESET_HEIGHT_PIXELS; //Alto en coordenadas de textura de un tile
#define TILESET_TILE_COORDX(x) (x*TILESET_TILE_UNIT_WIDTH)
#define TILESET_TILE_COORDY(y) (y*TILESET_TILE_UNIT_HEIGHT)

const int WINDOW_POSITION_X = 0;
const int WINDOW_POSITION_Y = 0;
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

int selectedTile=0;
dl32Point2D pickedTile = dl32Point2D(-1,-1);
bool keyPressed = false;
dl32Point2D beginTile,endTile;
dl32Point2D origin(-1,-1),v,inertia;

INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, INT )
{
	Console.Open("dx_lib32 C++ - Isometric tile engine (Debug)");

	try{
		window = new dl32Window("dx_lib32 C++ - Isometric tile engine",WINDOW_POSITION_X,WINDOW_POSITION_Y,WINDOW_WIDTH,WINDOW_HEIGHT);
		gfx = new dl32GraphicsClass(window,true);

		gfx->CAMERA_Enable();

		window->Idle.AddHandler(OnIdle);
		window->KeyDown.AddHandler(OnKeyDown);
		window->KeyUp.AddHandler(OnKeyUp);
		window->MouseWheel.AddHandler(OnMouseWheel);
		window->MouseDown.AddHandler(OnMouseDown);
		window->MouseDoubleClick.AddHandler(OnMouseDoubleClick);
		window->MouseUp.AddHandler(OnMouseUp);
		window->MouseMove.AddHandler(OnMouseMove);

		tilemap = new IsometricTilemap(TILEMAP_WIDTH,TILEMAP_HEIGHT,WINDOW_WIDTH,WINDOW_HEIGHT,TILEMAP_TILE_WIDTH,TILEMAP_TILE_HEIGHT);
		tilemap->setTileset(gfx->MAP_Load("tileset.png"));

		gfx->FONT_LoadSystemFont("Lucilda Console",10,false,true);
		gfx->FONT_LoadSystemFont("Lucilda Console",3,false,true);

		//Sencillo mapa procedural:
		for(int i=0;i<TILEMAP_WIDTH;++i)
			for(int j=0;j<TILEMAP_HEIGHT;++j)
			{
				tilemap->setTileColor(i,j,DL32COLOR_WHITE);
				tilemap->setTileTexture(i,j,TILESET_TILE_COORDX(0),TILESET_TILE_COORDY(0),TILESET_TILE_COORDX(1),TILESET_TILE_COORDY(1));//hierba

				if(j % 8 == 0)
				{
					tilemap->setTileTexture(i,j,TILESET_TILE_COORDX(1),TILESET_TILE_COORDY(11),TILESET_TILE_COORDX(2),TILESET_TILE_COORDY(12));//Vía de tren (horizontal)
					tilemap->setTileZ(i,j,TILEMAP_TILE_WIDTH/2);
				}
				if(i % 8 == 0)
				{
					tilemap->setTileTexture(i,j,TILESET_TILE_COORDX(2),TILESET_TILE_COORDY(11),TILESET_TILE_COORDX(3),TILESET_TILE_COORDY(12));//Vía de tren (horizontal)
					tilemap->setTileZ(i,j,TILEMAP_TILE_WIDTH/2);
				}
				if(j % 8 == 0 && i % 8 == 0)
				{
					tilemap->setTileTexture(i,j,TILESET_TILE_COORDX(3),TILESET_TILE_COORDY(11),TILESET_TILE_COORDX(4),TILESET_TILE_COORDY(12));//Vía de tren (cruce)
					tilemap->setTileZ(i,j,TILEMAP_TILE_WIDTH/2);
				}
			}

		dl32Window::Start();
	}catch(dl32WindowException)
	{
		DL32MEMORY_SAFEDELETE(window);
	}catch(dl32Direct3DInitFailedException)
	{

	}

	DL32MEMORY_SAFEDELETE(gfx);
	DL32MEMORY_SAFEDELETE(tilemap);
}

void OnIdle()
{
	if(origin.x>=0)
		tilemap->moveCamera(v.x/50,v.y/50);
	else if(abs(v.x)>DL32FLOAT_EPSILON && abs(v.y)>DL32FLOAT_EPSILON)
	{
		v.x*=0.96 + 0.0001 * gfx->FPS();
		v.y*=0.96 + 0.0001 * gfx->FPS();

		tilemap->moveCamera(v.x/50,v.y/50);//Un poco de inercia para darle gracia
	}


	tilemap->draw(gfx);
	gfx->Frame();

	window->SetText("dx_lib32 C++ - Isometric tile engine (" + dl32String(gfx->FPS()) + " FPS " + (dl32String)tilemap->visibleTilesCount() + " tiles) - (Picked tile: " + ToString(pickedTile) + ")");
}

void OnKeyDown(dl32KeyboardData keydata)
{
	switch(keydata.Key)
	{
	case 'a':
		tilemap->moveCamera(10,0);break;
		//gfx->Camera.Traslate(-10,0);break;
	case 'd':
		tilemap->moveCamera(-10,0);break;
		//gfx->Camera.Traslate(10,0);break;
	case 'w':
		tilemap->moveCamera(0,10);break;
		//gfx->Camera.Traslate(0,-10);break;
	case 's':
		tilemap->moveCamera(0,-10);break;
		//gfx->Camera.Traslate(0,10);break;
	case 't':
		if(!keyPressed)
		{
			keyPressed = true;
			beginTile = pickedTile;
		}
		break;
	case 'l':
		if(pickedTile.x>=0)
			tilemap->levelTile(pickedTile.x,pickedTile.y);
		break;
	case '+':
		if(pickedTile.x>=0)
			tilemap->upTile(pickedTile.x,pickedTile.y,10);
		break;
	case '-':
		if(pickedTile.x>=0)
			tilemap->upTile(pickedTile.x,pickedTile.y,-10);
		break;
	case '1':
		tilemap->zoom(0.5);
		break;
	case '2':
		tilemap->zoom(2);
		break;
	case 'q':
		dl32Window::Exit();
		break;
	}
}

void OnKeyUp(dl32KeyboardData data)
{
	if(data.Key == 'T' && keyPressed)
	{
		endTile = pickedTile;
		keyPressed = false;
		tilemap->smooth(beginTile,endTile);
	}
}

void OnMouseMove(dl32MouseData data)
{
	dl32Point2D newPick = tilemap->pick(data.Location);

	if(pickedTile != newPick)
	{
		if(pickedTile.x != -1 && pickedTile.y != -1)
			tilemap->setTileColor(pickedTile.x,pickedTile.y,DL32COLOR_WHITE);
		
		if(newPick.x != -1 && newPick.y != -1)
			tilemap->setTileColor(newPick.x,newPick.y,DL32COLOR_AQUA);

		pickedTile = newPick;
	}

	if(data.Button == DL32MOUSEBUTTON_LEFT)
	{
		if(origin.x<0)
			origin=window->GetClientArea().GetCenter();

		v=origin-data.Location;
	}
}

void OnMouseWheel(dl32MouseData data)
{
	if(pickedTile.x != -1 && pickedTile.y != -1)
		tilemap->upTile(pickedTile.x,pickedTile.y,data.Delta/10);
}

void OnMouseDown(dl32MouseData data)
{

}

void OnMouseDoubleClick(dl32MouseData data)
{
	dl32Point2D distance;

	if(pickedTile.x>=0)
	{
		tilemap->setCamera(tilemap->getTileCenter(pickedTile));
	}
}

void OnMouseUp(dl32MouseData data)
{
	origin.x=-1;
}

dl32String ToString(dl32Point2D point)
{
	return "{X=" + dl32String(point.x) + ",Y=" + dl32String(point.y) + "}";
}
#endif