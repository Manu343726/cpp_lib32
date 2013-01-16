#include "dl32Window.h"
#include <WindowsX.h>
#include "dl32Console.h"

WNDCLASSEX dl32Window::WndClass;
vector<dl32Window*> dl32Window::WindowsList=vector<dl32Window*>();
dl32Window* dl32Window::LastWindowMessaged=NULL;
bool dl32Window::WindowClassNotRegistered=true;
DEBUG_MessageLoopRunning dl32Window::Messagging;
bool dl32Window::MessageProcessed=false;

dl32Window::dl32Window()
{
	ready=false;
	KeyboardCapture=false;
	MouseCapture=false;
	hwnd=0;
	Index=-1;
}

dl32Window::dl32Window(string Text,int Left,int Top,int Width,int Height)throw(dl32WindowCreationFailedException)
{
	if(WindowClassNotRegistered) 
	{
		/* Estructura de la ventana */
		WndClass.lpszClassName = DL32WINDOWCLASS;
		WndClass.lpfnWndProc = MessageProc;      /* Esta función es invocada por Windows */
		WndClass.style = CS_DBLCLKS;                 /* Captura los doble-clicks */
		WndClass.cbSize = sizeof (WNDCLASSEX);

		/* Usar icono y puntero por defector */
		WndClass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
		WndClass.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
		WndClass.hCursor = LoadCursor (NULL, IDC_ARROW);
		WndClass.lpszMenuName = NULL;                 /* Sin menú */
		WndClass.cbClsExtra = 0;                      /* Sin información adicional para la */
		WndClass.cbWndExtra = 0;                      /* clase o la ventana */

		if(!FAILED(RegisterClassEx(&WndClass)))
		{
			WindowClassNotRegistered=false;
			LastWindowMessaged=NULL;
		}
		else
			throw  dl32WindowClassRegistrationFailedException((HWND)NULL,"Failed to register window class");
	}

	if (Width<=0) Width=DL32WINDOWSDEFAULTS_WIDTH;
	if (Height<=0) Height=DL32WINDOWSDEFAULTS_HEIGHT;

	LastWindowMessaged=this;
	hwnd=CreateWindowEx(0,DL32WINDOWCLASS,LPCSTR(Text.c_str()),WS_OVERLAPPEDWINDOW,Left,Top,Width,Height,HWND_DESKTOP,NULL,WndClass.hInstance,NULL);

	_mustBeDeleted=false;

	if(hwnd!=NULL)
	{
		ready=true;
		KeyboardCapture=true;
		MouseCapture=true;

		UpdateArea();

		WindowsList.push_back(this);
		Index=WindowsList.size()-1;

		ShowWindow(hwnd,SW_SHOWDEFAULT);
	}
	else
		throw  dl32WindowCreationFailedException(hwnd,"Failed to create window");
}

dl32Window::~dl32Window()
{
	if(ready)
	{
		ShowWindow(hwnd,SW_HIDE);
		DestroyWindow(hwnd);
		Index=-1;

		ready=false;
	}
}

void dl32Window::Cleanup()
{
	for(unsigned int i=0;i<dl32Window::WindowsList.size();++i)
		delete dl32Window::WindowsList[i];

	dl32Window::WindowsList.clear();

	UnregisterClass(DL32WINDOWCLASS,WndClass.hInstance);
	WindowClassNotRegistered=true;
}

void dl32Window::Start()
{
	MessageLoop();
}

void dl32Window::Exit()
{
	Messagging.StopLoop();
}

bool dl32Window::Started()
{
	return Messagging.Running();
}

void dl32Window::UpdateArea()
{
	RECT area;
	GetWindowRect(hwnd,&area);
	Area=dl32AABB2D(area.left,area.top,area.right-area.left,area.bottom-area.top);
}

inline dl32MouseData dl32Window::GetMouseData(MSG &Message)
{
	return GetMouseData(Message.hwnd,Message.message,Message.wParam,Message.lParam);
}

inline dl32KeyboardData dl32Window::GetKeyboardData(MSG &Message)
{
	return GetKeyboardData(Message.hwnd,Message.message,Message.wParam,Message.lParam);
}

dl32MouseData dl32Window::GetMouseData(HWND &hWnd, UINT &msg, WPARAM &wParam, LPARAM &lParam)
{
	dl32MouseData Data;

	switch(wParam)
	{
	case MK_RBUTTON:
		Data.Button=DL32MOUSEBUTTON_RIGHT;
		break;
	case MK_MBUTTON:
		Data.Button=DL32MOUSEBUTTON_CENTER;
		break;
	case MK_LBUTTON:
		Data.Button=DL32MOUSEBUTTON_LEFT;
		break;
	default:
		Data.Button=DL32MOUSEBUTTON_NONE;
		break;
	}

	Data.Location.x=GET_X_LPARAM(lParam);
	Data.Location.y=GET_Y_LPARAM(lParam);
	Data.Delta=GET_WHEEL_DELTA_WPARAM(wParam);

	return Data;
}

dl32KeyboardData dl32Window::GetKeyboardData(HWND &hWnd, UINT &msg, WPARAM &wParam, LPARAM &lParam)
{
	dl32KeyboardData Data;

	Data.Key=char(wParam);
	Data.PreviousPressed=((lParam >> 30)%2)!=0;//Miguel y Laura, mi cerebro se licua con las operaciones a bits
	Data.RepeatCount=lParam & 0x0000FFFF;//GRACIAS!!!!

	//NOTA (05/09/12): Que aquí conste que si en algún momento gano dinero con ésto, no os pienso dar un duro. Estáis avisados.

	return Data;
}

dl32Vector2D dl32Window::GetWindowSize(float width, float height, bool flat)
{
	if(flat)
		return dl32Vector2D(width,height);
	else
		return dl32Vector2D(width + GetSystemMetrics(SM_CXBORDER)*2 + GetSystemMetrics(SM_CXEDGE),
							height + GetSystemMetrics(SM_CYBORDER)*2 + GetSystemMetrics(SM_CYEDGE));
}

bool dl32Window::Ready()
{
	return ready;
}

HWND dl32Window::GetHwnd()
{
	return hwnd;
}

bool dl32Window::Visible()
{
	return bool(IsWindowVisible(hwnd));
}

void dl32Window::Show()
{
	ShowWindow(hwnd,SW_SHOWDEFAULT);
}

void dl32Window::Hide()
{
	ShowWindow(hwnd,SW_HIDE);
}

float dl32Window::GetLeft()
{
	return Area.Position.x;
}

float dl32Window::GetTop()
{
	return Area.Position.y;
}

float dl32Window::GetWidth()
{
	return Area.GetWidth();
}

float dl32Window::GetHeight()
{
	return Area.GetHeight();
}

void dl32Window::SetLeft(float Left)
{
	Area.Position.x=Left;
	MoveWindow(hwnd,Left,Area.Position.y,Area.GetWidth(),Area.GetHeight(),true);
}

void dl32Window::SetTop(float Top)
{
	Area.Position.y=Top;
	MoveWindow(hwnd,Area.Position.x,Top,Area.GetWidth(),Area.GetHeight(),true);
}

void dl32Window::SetWidth(float Width)
{
	Area.SetWidth(Width);
	MoveWindow(hwnd,Area.Position.x,Area.Position.y,Width,Area.GetHeight(),true);
}

void dl32Window::SetHeight(float Height)
{
	Area.SetHeight(Height);
	MoveWindow(hwnd,Area.Position.y,Area.Position.y,Area.GetWidth(),Height,true);
}

dl32AABB2D dl32Window::GetClientArea()
{
	return dl32AABB2D(0,0,Area.GetWidth(),Area.GetHeight()); //NOTA: Provisional. (Area es el area total de la ventana)
}

dl32AABB2D dl32Window::GetScreenArea()
{
	return Area;
}

void dl32Window::SetScreenArea(dl32AABB2D Area)
{
	this->Area=Area;
	MoveWindow(hwnd,Area.Position.x,Area.Position.y,Area.GetWidth(),Area.GetHeight(),true);
}

dl32String dl32Window::GetText()
{
	LPSTR strptr=NULL;
	GetWindowText(hwnd,strptr,DL32WINDOWFLAGS_MAXTEXTLENGHT);
	return dl32String(strptr);
}

void dl32Window::SetText(dl32String Text)
{
	SetWindowText(hwnd,Text.c_str());
}

void dl32Window::EraseWindow(dl32Window* window)
{
	if(window->Index>=0 && (unsigned int)window->Index<dl32Window::WindowsList.size())
	{
		WindowsList.erase(WindowsList.begin()+window->Index);
		delete window;	

		for(unsigned int i=window->Index;i<WindowsList.size();++i)
			WindowsList[i]->Index--;
	}
}

bool dl32Window::SearchWindow(HWND hwnd)
{
	int i=0;
	LastWindowMessaged=NULL;

	while(LastWindowMessaged == NULL && (unsigned int)i<WindowsList.size())
	{
		if(WindowsList[i]->hwnd==hwnd)
			LastWindowMessaged=WindowsList[i];
		else
			++i;
	}

	return LastWindowMessaged!=NULL;
}

void dl32Window::MessageLoop()throw(dl32UnhandledWindowMessage)
{
	MSG message;
	bool PreMessaging = true;

	try
	{
		while(Messagging.Running() && WindowsList.size()>0)
		{
			MessageProcessed=false;
			if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);

				if(!dl32Window::MessageProcessed && dl32Window::SearchWindow(message.hwnd))
				{
					dl32Window::LastWindowMessaged->ProcessMessage(message);
		
					if(dl32Window::LastWindowMessaged!=NULL && dl32Window::LastWindowMessaged->_mustBeDeleted)
						dl32Window::EraseWindow(dl32Window::LastWindowMessaged);
				}
			}
			else
			{
				for(unsigned int i=0;i<WindowsList.size();++i)
					WindowsList[i]->Idle.RaiseEvent();
			}
		}

		Cleanup();
	}catch(dl32Exception e)
	{
		MessageBox(GetDesktopWindow(),strcat("UNHANDLED DL32EXCEPTION: ",e.what()),"dx_lib32 - Unhandled Exception (Debug)",MB_ICONERROR);
	}
}

LRESULT WINAPI MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	dl32Window::MessageProcessed = false;

	if((msg == WM_CLOSE || msg == WM_DESTROY || msg == WM_QUIT) && dl32Window::SearchWindow(hWnd))
	{
		dl32Window::LastWindowMessaged->ProcessMessage(hWnd,msg,wParam,lParam);
		
		if(dl32Window::LastWindowMessaged!=NULL && dl32Window::LastWindowMessaged->_mustBeDeleted)
			dl32Window::EraseWindow(dl32Window::LastWindowMessaged);
	}

	if(!dl32Window::MessageProcessed)
		return DefWindowProc(hWnd, msg, wParam, lParam);
	else
		return 0;
}

void dl32Window::ProcessMessage(HWND &hWnd, UINT &msg, WPARAM &wParam, LPARAM &lParam)
{
	bool cancel=false;
	bool* cancelAdress = &cancel;

	switch(msg)
	{
	//NOTA: Error en el procesamiento de WM_MOVE. La ventana deja de dibujarse correctamente. 
	//case WM_MOVE:
	//	UpdateArea();
	//	Move.RaiseEvent(&Area);
	//	LastKeyCaptureState=KEYCAPTURESTATE_NOCAPTURE;
	//	break;
	//case WM_PAINT:
	//	UpdateArea();
	//	Paint.RaiseEvent();
	//	LastKeyCaptureState=KEYCAPTURESTATE_NOCAPTURE;
	//	break;
	case WM_MOUSEMOVE:
		MouseMove.RaiseEvent(&GetMouseData(hWnd,msg,wParam,lParam));
		LastKeyCaptureState=KEYCAPTURESTATE_NOCAPTURE;
		break;
	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
		MouseUp.RaiseEvent(&GetMouseData(hWnd,msg,wParam,lParam));
		LastKeyCaptureState=KEYCAPTURESTATE_NOCAPTURE;
		break;
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
		MouseDown.RaiseEvent(&GetMouseData(hWnd,msg,wParam,lParam));
		LastKeyCaptureState=KEYCAPTURESTATE_NOCAPTURE;
		break;
	case WM_MOUSEWHEEL:
		MouseWheel.RaiseEvent(&GetMouseData(hWnd,msg,wParam,lParam));
		LastKeyCaptureState=KEYCAPTURESTATE_NOCAPTURE;
		break;
	case WM_KEYDOWN:
		LastKeyCaptureState=KEYCAPTURESTATE_KEYDOWN;
		break;
	case WM_KEYUP:
		LastKeyCaptureState=KEYCAPTURESTATE_KEYUP;
		break;
	case WM_CHAR:
		switch(LastKeyCaptureState)
		{
		case KEYCAPTURESTATE_KEYDOWN:
			KeyDown.RaiseEvent(&GetKeyboardData(hWnd,msg,wParam,lParam));
			break;
		case KEYCAPTURESTATE_KEYUP:
			KeyUp.RaiseEvent(&GetKeyboardData(hWnd,msg,wParam,lParam));
			break;
		}

		LastKeyCaptureState=KEYCAPTURESTATE_NOCAPTURE;
		break;
	case WM_QUIT:
		if(DL32DEBUG_DL32WINDOW_PROMPTPREPROCESSEDWMSGS) Console.WriteLine("PRE-PROCESSED WINDOWS MESSAGE: WM_QUIT",DL32CP_PURPLE,DL32CP_BLACK);
		break;
	case WM_CLOSE:
		if(DL32DEBUG_DL32WINDOW_PROMPTPREPROCESSEDWMSGS) Console.WriteLine("PRE-PROCESSED WINDOWS MESSAGE: WM_CLOSE",DL32CP_PURPLE,DL32CP_BLACK);

		Close.RaiseEvent(&cancelAdress);
		
		if(!cancel)
		{
			this->DeleteWindow();
		}
		else
			this->Show();

		LastKeyCaptureState=KEYCAPTURESTATE_NOCAPTURE;
		break;
	case WM_DESTROY:
		if(DL32DEBUG_DL32WINDOW_PROMPTPREPROCESSEDWMSGS) Console.WriteLine("PRE-PROCESSED WINDOWS MESSAGE: WM_DESTROY",DL32CP_PURPLE,DL32CP_BLACK);
		break;
	default:
		if(DL32DEBUG_DL32WINDOW_PROMPTUNHANDLEDWMSGS) Console.WriteLine("UNKNOWN MESSAGE: " + dl32String(long(msg),true),DL32CP_WHITE,DL32CP_BLACK);
		Idle.RaiseEvent();
	}

	dl32Window::MessageProcessed=true;
}

inline void dl32Window::ProcessMessage(MSG &Message)
{
	return ProcessMessage(Message.hwnd,Message.message,Message.wParam,Message.lParam);
}

