#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_ISOMETRICENGINE_ENGINETEST

#include "Particle engine.h"
#include "dl32Window.h"
#include "dl32Console.h"

dl32ParticleSystem *engine;
dl32Window *Window;
int selected=0;

void PreDrawProc();

void OnMouseMove(dl32MouseData MouseData);
void OnMouseWheel(dl32MouseData MouseData);
void OnKeyDown(dl32KeyStrokeData KeyboardData);

dl32GraphicsClass* gfx;

dl32Pixel *screen;

INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
	Console.Open("dx_lib32 C++ Particle engine test (Debug)");

	Window=new dl32Window("dx_lib32 C++ - Particle engine test",0,0,1440,900);
	gfx=new dl32GraphicsClass(Window);
	engine=new dl32ParticleSystem(gfx,1000,Window->GetClientArea());
	screen=new dl32Pixel[1280*900];

	Window->MouseMove.AddHandler(OnMouseMove);
	Window->MouseWheel.AddHandler(OnMouseWheel);
	Window->KeyDown.AddHandler(OnKeyDown);

	engine->AddEmmitter(dl32ParticleEmmitter(3,dl32Point2D(100,100)));
	//engine->AddEmmitter(dl32ParticleEmmitter(3,dl322DPoint(1000,500)));

	engine->AddHole(dl32ParticleHole(dl32Point2D(400,400),20,10000));

	Window->Idle.AddHandler(PreDrawProc);

	engine->Init();

	dl32Window::Start();

	delete engine;
	delete gfx;
	delete screen;
}

void PreDrawProc()
{
	/*engine->Frame();*/
	Window->SetText("dx_lib32 C++ - Particle engine test (" + dl32String(gfx->FPS()) + " FPS)");

	for(int i=0;i<1280;++i)
		for(int j=0;j<900;++j)
		{
			screen[i*900+j].color=COLOR_RainbowGradient(i);
			screen[i*900+j].x=i;
			screen[i*900+j].y=j;
		}

	gfx->DRAW_Pixels(screen,1280*900);
	gfx->Frame();
}

void OnMouseMove(dl32MouseData MouseData)
{
	switch(MouseData.Button)
	{
	case DL32MOUSEBUTTON_RIGHT:
		engine->MoveHole(selected,MouseData.Location);
		break;
	case DL32MOUSEBUTTON_LEFT:
		engine->MoveEmmitter(selected,MouseData.Location);
		break;
	case DL32MOUSEBUTTON_CENTER:
		engine->AddHole(dl32ParticleHole(MouseData.Location,10,100));
		break;
	}
}

void OnMouseWheel(dl32MouseData MouseData)
{
	if(MouseData.Delta>=0)
		selected++;
	else
		selected--;
}

void OnKeyDown(dl32KeyStrokeData KeyboardData)
{
	switch(KeyboardData.Key)
	{
	case 'q':
		dl32Window::Exit();
		break;
	case 'm':
		engine->SetBorderMode(engine->GetNorderMode()==EBMODE_INFINITE ? EBMODE_REFLECTION : EBMODE_INFINITE);
		break;
	}
}
#endif