#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include "dl32String.h"
#include "dl32Math.h"


#define DL32WINDOWSDEFAULTS_LEFT CW_USEDEFAULT
#define DL32WINDOWSDEFAULTS_TOP CW_USEDEFAULT
#define DL32WINDOWSDEFAULTS_WIDTH 800
#define DL32WINDOWSDEFAULTS_HEIGHT 600

#define DL32WINDOWFLAGS_MAXTEXTLENGHT 100

const LPCSTR DL32WINDOWCLASS = "DXLIB32WINDOWCLASS";

template <class PARAMTYPE>
class dl32Event
{
private:
	vector<void (*)(PARAMTYPE)> Handlers;
public:
	void RaiseEvent(PARAMTYPE *P);
	void RaiseEvent(void);//NOTA: Necesario para eventos sin parámetros
	void AddHandler(void (*Handler)(PARAMTYPE));
	void RemoveHandler(void (*Handler)(PARAMTYPE));
};

template <class PARAMTYPE>
void dl32Event<PARAMTYPE>::RaiseEvent(void)
{
	for(int i=0;i<Handlers.size();++i)
		Handlers[i]();
}

template <class PARAMTYPE>
void dl32Event<PARAMTYPE>::RaiseEvent(PARAMTYPE *P)
{
	for(int i=0;i<Handlers.size();++i)
		Handlers[i](*P);
}

template <class PARAMTYPE>
void dl32Event<PARAMTYPE>::AddHandler(void (*Handler)(PARAMTYPE))
{
	Handlers.push_back(Handler);
}

template <class PARAMTYPE>
void dl32Event<PARAMTYPE>::RemoveHandler(void (*Handler)(PARAMTYPE))
{
	Handlers.erase(Handler);
}

enum dl32MouseButton
{
	DL32MOUSEBUTTON_NONE,
	DL32MOUSEBUTTON_RIGHT,
	DL32MOUSEBUTTON_CENTER,
	DL32MOUSEBUTTON_LEFT
};

struct dl32MouseData
{
	dl32Point2D Location;
	dl32MouseButton Button;
	int Delta;
};

typedef char dl32KeyCode;

struct dl32KeyboardData
{
	bool PreviousPressed;
	dl32KeyCode Key;
	int RepeatCount;
};

enum KEYCAPTURESTATE
{
	KEYCAPTURESTATE_KEYDOWN,
	KEYCAPTURESTATE_KEYUP,
	KEYCAPTURESTATE_NOCAPTURE
};

typedef dl32Event<dl32MouseData> dl32MouseEvent;
typedef dl32MouseEvent dl32MouseMoveEvent;
typedef dl32MouseEvent dl32MouseUpEvent;
typedef dl32MouseEvent dl32MouseDownEvent;
typedef dl32MouseEvent dl32MouseWheelEvent;

typedef dl32Event<dl32KeyboardData> dl32KeyboardEvent;
typedef dl32KeyboardEvent dl32KeyPressEvent;
typedef dl32KeyboardEvent dl32KeyDownEvent;
typedef dl32KeyboardEvent dl32KeyUpEvent;

typedef dl32Event<dl32AABB2D> dl32AreaChangedEvent;
typedef dl32AreaChangedEvent dl32MoveEvent;
typedef dl32AreaChangedEvent dl32ResizeEvent;
typedef dl32AreaChangedEvent dl32ResizeBeginEvent;
typedef dl32AreaChangedEvent dl32ResizeEndEvent;

typedef dl32Event<void> dl32VoidEvent;
typedef dl32VoidEvent dl32PaintEvent;

typedef dl32Event<bool*> dl32WindowCloseEvent;


LRESULT WINAPI MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class DEBUG_MessageLoopRunning
{
private:
	bool _running;
	int _stoppedCount;
public:
	DEBUG_MessageLoopRunning(){_running=true;_stoppedCount=0;}
	void StopLoop(){_running=false;_stoppedCount++;}
	bool Running(){return _running;}
	int GetStoppedCount(){return _stoppedCount;};
};

class dl32Window
{
	friend LRESULT WINAPI MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	static WNDCLASSEX WndClass;
	static bool WindowClassNotRegistered;

	static vector<dl32Window*> WindowsList;
	static dl32Window *LastWindowMessaged;
	static bool MessageProcessed;

	static void EraseWindow(dl32Window* window);
	static bool SearchWindow(HWND hwnd);
	static void MessageLoop()throw(dl32UnhandledWindowMessage);
	static DEBUG_MessageLoopRunning Messagging;

	static void Cleanup();
	bool _mustBeDeleted;

	dl32AABB2D Area;
	bool KeyboardCapture;
	bool MouseCapture;

	KEYCAPTURESTATE LastKeyCaptureState;

	int Index;
protected:
	HWND hwnd;
	bool ready;

	void UpdateArea();

	void ProcessMessage(MSG &Message);
	void ProcessMessage(HWND &hWnd, UINT &msg, WPARAM &wParam, LPARAM &lParam);

	static dl32MouseData GetMouseData(MSG &Message);
	static dl32KeyboardData GetKeyboardData(MSG &Message);
	static dl32MouseData GetMouseData(HWND &hWnd, UINT &msg, WPARAM &wParam, LPARAM &lParam);
	static dl32KeyboardData GetKeyboardData(HWND &hWnd, UINT &msg, WPARAM &wParam, LPARAM &lParam);
public:
	dl32Window();
	dl32Window(string Title,int Left=DL32WINDOWSDEFAULTS_LEFT,int Top=DL32WINDOWSDEFAULTS_TOP,int Width=DL32WINDOWSDEFAULTS_WIDTH,int Height=DL32WINDOWSDEFAULTS_HEIGHT);
	~dl32Window();

	static void Start();
	static void Exit();	
	static bool Started();

	bool Ready();

	HWND GetHwnd();

	bool Visible();
	void Show();
	void Hide();

	float GetLeft();
	float GetTop();
	float GetWidth();
	float GetHeight();
	void SetLeft(float Left);
	void SetTop(float Top);
	void SetWidth(float Width);
	void SetHeight(float Height);

	dl32AABB2D GetScreenArea();
	dl32AABB2D GetClientArea();
	void SetScreenArea(dl32AABB2D Area);

	dl32String GetText();
	void SetText(dl32String Text);

	bool KeyboardCaptureEnabled();
	bool MouseCaptureEnabled();	
	void EnableKeyboardCapture();
	void EnableMouseCapture();
	void DisableKeyboardCapture();
	void DisableMouseCapture();

	dl32PaintEvent Paint;
	dl32MoveEvent Move;
	dl32ResizeEvent Resize;
	dl32MouseMoveEvent MouseMove;
	dl32MouseDownEvent MouseDown;
	dl32MouseWheelEvent MouseWheel;
	dl32MouseUpEvent MouseUp;
	dl32KeyDownEvent KeyDown;
	dl32KeyUpEvent KeyUp;
	dl32VoidEvent Idle;
	dl32WindowCloseEvent Close;

	void DeleteWindow(){_mustBeDeleted=true;};

	static int DEBUG_GetMessageLoopStoppedCount(){return Messagging.GetStoppedCount();};
};
#endif