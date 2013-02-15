#include "IsometricEngine.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_ISOMETRICENGINE_ENGINETEST
	
dl32GraphicsClass* gfx;
dl32Window* window;

int tileset;
IsometricTilemap* tilemap;

void OnIdle();
void OnKeyDown(dl32KeyboardData keydata);
void OnMouseWheel(dl32MouseData data);
void OnMouseDown(dl32MouseData data);

const int TILEMAP_WIDTH = 10;
const int TILEMAP_HEIGHT = 10;
const int TILEMAP_TILE_WIDTH = 50;
const int TILEMAP_TILE_HEIGHT = 50;
const int TILEMAP_TILECOUNT = TILEMAP_WIDTH*TILEMAP_HEIGHT;

const int WINDOW_POSITION_X = 0;
const int WINDOW_POSITION_Y = 0;
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 769;

int selectedTile=0;

INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, INT )
{
	Console.Open("dx_lib32 C++ - Isometric tile engine (Debug)");

	try{
		window = new dl32Window("dx_lib32 C++ - Isomatric tile engine",WINDOW_POSITION_X,WINDOW_POSITION_Y,WINDOW_WIDTH,WINDOW_HEIGHT);
		gfx = new dl32GraphicsClass(window);

		window->Idle.AddHandler(OnIdle);
		window->KeyDown.AddHandler(OnKeyDown);
		window->MouseWheel.AddHandler(OnMouseWheel);
		window->MouseDown.AddHandler(OnMouseDown);

		tilemap = new IsometricTilemap(TILEMAP_WIDTH,TILEMAP_HEIGHT,WINDOW_WIDTH,WINDOW_HEIGHT,TILEMAP_TILE_WIDTH,TILEMAP_TILE_HEIGHT);
		tilemap->setTileset(gfx->MAP_Load("Texture.png"));

		for(int i=0;i<TILEMAP_WIDTH;++i)
			for(int j=0;j<TILEMAP_HEIGHT;++j)
				tilemap->setTileColor(i,j,COLOR_RainbowGradient(i*TILEMAP_WIDTH+j,0,TILEMAP_WIDTH*TILEMAP_HEIGHT));

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

	window->SetText("dx_lib32 C++ - Isomatric tile engine (" + dl32String(gfx->FPS()) + " FPS)");
}

void OnKeyDown(dl32KeyboardData keydata)
{
	switch(keydata.Key)
	{
	case 'a':
		tilemap->moveCamera(-10,0);break;
	case 'd':
		tilemap->moveCamera(10,0);break;
	case 'w':
		tilemap->moveCamera(0,-10);break;
	case 's':
		tilemap->moveCamera(0,10);break;
	}
}

void OnMouseWheel(dl32MouseData data)
{
	tilemap->upTile(selectedTile/TILEMAP_WIDTH,selectedTile % TILEMAP_WIDTH,data.Delta/10);
}

void OnMouseDown(dl32MouseData data)
{
	if(data.Button == DL32MOUSEBUTTON_RIGHT)
		if(selectedTile < TILEMAP_TILECOUNT-1)
			selectedTile++;
		else
			selectedTile=0;
	else if(data.Button == DL32MOUSEBUTTON_LEFT)
		if(selectedTile > 0)
			selectedTile++;
		else
			selectedTile=TILEMAP_TILECOUNT-1;
}
#endif