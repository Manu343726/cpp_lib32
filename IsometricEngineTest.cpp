#include "IsometricEngine.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_ISOMETRICENGINE_ENGINETEST
	
dl32GraphicsClass* gfx;
dl32Window* window;

IsometricTilemap* tilemap;

void OnIdle();
void OnKeyDown(dl32KeyboardData keydata);
void OnMouseWheel(dl32MouseData data);
void OnMouseDown(dl32MouseData data);
void OnMouseMove(dl32MouseData data);

const int TILEMAP_WIDTH = 129;                              //Ancho del tilemap (tiles)
const int TILEMAP_HEIGHT = 129 ;                             //Alto del tilemap (tiles)
const int TILEMAP_TILE_WIDTH = 64;	                        //Ancho de un tile 
const int TILEMAP_TILE_HEIGHT = 64;                        //Alto de un tile
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
const int WINDOW_WIDTH = 1440;
const int WINDOW_HEIGHT = 900;

int selectedTile=0;
dl32Point2D pickedTile = dl32Point2D(-1,-1);

INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, INT )
{
	Console.Open("dx_lib32 C++ - Isometric tile engine (Debug)");

	try{
		window = new dl32Window("dx_lib32 C++ - Isometric tile engine",WINDOW_POSITION_X,WINDOW_POSITION_Y,WINDOW_WIDTH,WINDOW_HEIGHT);
		gfx = new dl32GraphicsClass(window);

		gfx->CAMERA_Enable();

		window->Idle.AddHandler(OnIdle);
		window->KeyDown.AddHandler(OnKeyDown);
		window->MouseWheel.AddHandler(OnMouseWheel);
		window->MouseDown.AddHandler(OnMouseDown);
		window->MouseMove.AddHandler(OnMouseMove);

		tilemap = new IsometricTilemap(TILEMAP_WIDTH,TILEMAP_HEIGHT,WINDOW_WIDTH,WINDOW_HEIGHT,TILEMAP_TILE_WIDTH,TILEMAP_TILE_HEIGHT);
		tilemap->setTileset(gfx->MAP_Load("tileset.png"));

		gfx->FONT_LoadSystemFont("Lucilda Console",10,false,true);
		gfx->FONT_LoadSystemFont("Lucilda Console",3,false,true);

		for(int i=0;i<TILEMAP_WIDTH;++i)
			for(int j=0;j<TILEMAP_HEIGHT;++j)
			{
				//tilemap->setTileColor(i,j,COLOR_RainbowGradient(i*TILEMAP_WIDTH+j,0,TILEMAP_WIDTH*TILEMAP_HEIGHT));
				tilemap->setTileColor(i,j,DL32COLOR_WHITE);
				tilemap->setTileTexture(i,j,TILESET_TILE_COORDX(0),TILESET_TILE_COORDY(0),TILESET_TILE_COORDX(1),TILESET_TILE_COORDY(1));//hierba


				if(j==(TILEMAP_HEIGHT/2))
				{
					tilemap->setTileTexture(i,j,TILESET_TILE_COORDX(1),TILESET_TILE_COORDY(11),TILESET_TILE_COORDX(2),TILESET_TILE_COORDY(12));//Vía de tren (horizontal)
					tilemap->setTileZ(i,j,36);
				}
				if(i==(TILEMAP_WIDTH/2))
				{
					tilemap->setTileTexture(i,j,TILESET_TILE_COORDX(2),TILESET_TILE_COORDY(11),TILESET_TILE_COORDX(3),TILESET_TILE_COORDY(12));//Vía de tren (horizontal)
					tilemap->setTileZ(i,j,36);
				}
				if(i==(TILEMAP_WIDTH/2) && j==(TILEMAP_HEIGHT/2))
				{
					tilemap->setTileTexture(i,j,TILESET_TILE_COORDX(3),TILESET_TILE_COORDY(11),TILESET_TILE_COORDX(4),TILESET_TILE_COORDY(12));//Vía de tren (cruce)
					tilemap->setTileZ(i,j,36);
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
	tilemap->draw(gfx);
	gfx->Frame();

	window->SetText("dx_lib32 C++ - Isometric tile engine (" + dl32String(gfx->FPS()) + " FPS)");
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

	if(pickedTile.x != -1 && pickedTile.y != -1)
		if(data.Button == DL32MOUSEBUTTON_RIGHT)
			tilemap->upTile(pickedTile.x,pickedTile.y,-1);
		else if(data.Button == DL32MOUSEBUTTON_LEFT)
			tilemap->upTile(pickedTile.x,pickedTile.y,1);
}

void OnMouseWheel(dl32MouseData data)
{
	if(pickedTile.x != -1 && pickedTile.y != -1)
		tilemap->upTile(pickedTile.x,pickedTile.y,data.Delta/10);
}

void OnMouseDown(dl32MouseData data)
{

}
#endif