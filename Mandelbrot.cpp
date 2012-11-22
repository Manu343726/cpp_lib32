#include "dl32Preprocessor.h"

#if DL32DEBUG_DEBUGTEST == 6
#include "MandelbrotEngine.h"
#include "dl32Window.h"

enum GUIMode
{
	GUIMODE_IDLE,
	GUIMODE_COMPUTING,
	GUIMODE_SELECTING
};

dl32Window* Window; 
MandelbrotEngine* Engine;
float zoom=1;
GUIMode mode=GUIMODE_IDLE;

void PreDrawProc(dl32GraphicsClass* gfx);

void OnMouseWheel(dl32MouseData MouseData);
void OnMouseMove(dl32MouseData MouseData);
void OnMouseDown(dl32MouseData MouseData);

INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
	Window=new dl32Window("dx_lib32 - Fractal de Mandelbrot",0,0,1440,900);
	Window->Graphics->DEVICE_SetPreDrawProc(PreDrawProc);
	Window->MouseWheel.AddHandler(OnMouseWheel);
	Window->MouseMove.AddHandler(OnMouseMove);
	Window->MouseDown.AddHandler(OnMouseDown);

	Engine=new MandelbrotEngine(Window->GetClientArea());
	Engine->Compute();

	dl32Window::Start();

	delete Window;
}

void PreDrawProc(dl32GraphicsClass* gfx)
{
	Engine->Refresh(gfx);
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
	if(mode==GUIMODE_SELECTING)
	{

	}
	else if(mode==GUIMODE_IDLE)
		Window->SetText("dx_lib32 - Fractal de Mandelbrot. [" + Engine->GetNumber(MouseData.Location).ToString() + "]");
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