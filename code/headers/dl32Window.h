#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include "dl32String.h"
#include "dl32Math.h"
#include "dl32Exceptions.h"

#include <unordered_set>
using namespace std;


#define DL32WINDOWSDEFAULTS_LEFT CW_USEDEFAULT
#define DL32WINDOWSDEFAULTS_TOP CW_USEDEFAULT
#define DL32WINDOWSDEFAULTS_WIDTH 800
#define DL32WINDOWSDEFAULTS_HEIGHT 600

#define DL32WINDOWFLAGS_MAXTEXTLENGHT 100

const LPCSTR DL32WINDOWCLASS = "DXLIB32WINDOWCLASS";


class dl32WindowException:dl32Exception
{
private:
	HWND _hwnd;
public:
	dl32WindowException(HWND hwnd = (HWND)NULL,char* message = DEFAULTEXCEPTIONMESSAGE(dl32WindowException)):dl32Exception(message){_hwnd=hwnd;}
	HWND GethWnd(){return _hwnd;}
};

class dl32UnhandledWindowMessage: public dl32WindowException
{
private:
	UINT _msg;
public:
	dl32UnhandledWindowMessage(UINT msg,HWND hwnd, char* message):dl32WindowException(hwnd,message){_msg=msg;}
	UINT GetMessage(){return _msg;}
};

class dl32WindowCreationFailedException: public dl32WindowException
{
public:
	dl32WindowCreationFailedException(HWND hwnd,char* message):dl32WindowException(hwnd,message){}
};

class dl32WindowClassRegistrationFailedException: public dl32WindowException
{
public:
	dl32WindowClassRegistrationFailedException(HWND hwnd,char* message):dl32WindowException(hwnd,message){}
};



enum KEYCAPTURESTATE
{
	KEYCAPTURESTATE_KEYDOWN,
	KEYCAPTURESTATE_KEYUP,
	KEYCAPTURESTATE_NOCAPTURE
};

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
	int GetStoppedCount(){return _stoppedCount;}
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

	static dl32MouseData GetMouseData(const MSG &Message);
	static dl32KeyStrokeData GetKeyboardData(const MSG &Message);
	static dl32MouseData GetMouseData(const WPARAM &wParam,const LPARAM &lParam);
	static dl32KeyStrokeData GetKeyboardData(const WPARAM &wParam,const LPARAM &lParam);
public:
	dl32Window();
	dl32Window(string Title,int Left=DL32WINDOWSDEFAULTS_LEFT,int Top=DL32WINDOWSDEFAULTS_TOP,int Width=DL32WINDOWSDEFAULTS_WIDTH,int Height=DL32WINDOWSDEFAULTS_HEIGHT) throw(dl32WindowCreationFailedException);
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

	static dl32Vector2D GetWindowSize(float width,float height, bool flat);

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
	dl32MouseClickEvent MouseClick;
	dl32MouseDoubleClickEvent MouseDoubleClick;
	dl32MouseEnterEvent MouseEnter;
	dl32MouseLeaveEvent MouseLeave;
	dl32MouseMoveEvent MouseMove;
	dl32MouseDownEvent MouseDown;
	dl32MouseWheelEvent MouseWheel;
	dl32MouseUpEvent MouseUp;
	dl32KeyDownEvent KeyDown;
	dl32KeyUpEvent KeyUp;
	dl32NonArgsEvent Idle;
	dl32WindowCloseEvent Close;

	void DeleteWindow(){_mustBeDeleted=true;}

	static int DEBUG_GetMessageLoopStoppedCount(){return Messagging.GetStoppedCount();}
};
#endif