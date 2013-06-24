#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_MANDELBROTFRACTAL_SIMPLETEST
#include "MandelbrotEngine.h"
#include "dl32Window.h"

enum GUIMode
{
	GUIMODE_IDLE,
	GUIMODE_COMPUTING,
	GUIMODE_SELECTING
}

dl32Window* Window; 
dl32GraphicsClass* gfx;
MandelbrotEngine* Engine;
float zoom=1;
GUIMode mode=GUIMODE_IDLE;

void OnIdle();

void OnMouseWheel(dl32MouseData MouseData);
void OnMouseMove(dl32MouseData MouseData);
void OnMouseDown(dl32MouseData MouseData);

INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
	Window=new dl32Window("dx_lib32 - Fractal de Mandelbrot",0,0,1440,900);
	gfx = new dl32GraphicsClass(Window);

	Window->MouseWheel.AddHandler(OnMouseWheel);
	Window->MouseMove.AddHandler(OnMouseMove);
	Window->MouseDown.AddHandler(OnMouseDown);

	Window->Idle.AddHandler(OnIdle);

	Engine=new MandelbrotEngine(Window->GetClientArea());
	Engine->Compute();

	dl32Window::Start();

	DL32MEMORY_SAFEDELETE(gfx);
}

void OnIdle()
{
	Engine->Refresh(gfx);
	Window->SetText("dx_lib32 - Mandelbrot Set (" + (dl32String)gfx->FPS() + " FPS)");
}

void OnMouseWheel(dl32MouseData MouseData)
{
	if(mode==GUIMODE_SELECTING)
	{
		if(MouseData.Delta>0)
			zoom+=0.001;
		else if(MouseData.Delta<0)
			zoom-=0.001;
	}
}

void OnMouseMove(dl32MouseData MouseData)
{
	//if(mode==GUIMODE_SELECTING)
	//{

	//}
	//else if(mode==GUIMODE_IDLE)
	//	//Window->SetText("dx_lib32 - Fractal de Mandelbrot. [" + Engine->GetNumber(MouseData.Location).ToString() + "]");
}

void OnMouseDown(dl32MouseData MouseData)
{
	if(mode==GUIMODE_SELECTING)
	{
		if(MouseData.Delta>0)
			zoom+=0.001;

	}
}
#endif