#include "dl32Windows.h"
#include <WindowsX.h>

WNDCLASSEX dl32Window::WndClass;
vector<dl32Window*> dl32Window::WindowsList=vector<dl32Window*>();
dl32Window* dl32Window::LastWindowMessaged=NULL;
bool dl32Window::WindowClassNotRegistered=true;
bool dl32Window::Messagging=false;

LRESULT WINAPI MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

dl32Window::dl32Window()
{
	ready=false;
	KeyboardCapture=false;
	MouseCapture=false;
	hwnd=0;
	Index=-1;
	Graphics=NULL;
}

dl32Window::dl32Window(string Text,int Left,int Top,int Width,int Height)
{
	Graphics=NULL;

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
			ready=true;
		}
		else
			ready=false;
	}
	else
		ready=true;

	if (Width<=0) Width=DL32WINDOWSDEFAULTS_WIDTH;
	if (Height<=0) Height=DL32WINDOWSDEFAULTS_HEIGHT;

	LastWindowMessaged=this;
	hwnd=CreateWindowEx(0,DL32WINDOWCLASS,LPCSTR(Text.c_str()),WS_OVERLAPPEDWINDOW,Left,Top,Width,Height,HWND_DESKTOP,NULL,WndClass.hInstance,NULL);

	if(ready)
	{
		if(hwnd!=NULL)
		{
			ready=true;
			KeyboardCapture=true;
			MouseCapture=true;

			UpdateArea();
			Graphics=new dl32GraphicsClass(hwnd,Area.GetWidth(),Area.GetHeight(),true);
			Graphics->Start();

			WindowsList.push_back(this);
			Index=WindowsList.size()-1;

			ShowWindow(hwnd,SW_SHOWDEFAULT);
		}
		else
		{
			DWORD Error;
			Error=GetLastError();
			Error=Error;
			ready=false;
		}
	}
	else
		ready=false;
}

dl32Window::~dl32Window()
{
	if(ready)
	{
		ShowWindow(hwnd,SW_HIDE);
		DestroyWindow(hwnd);
		WindowsList.erase(WindowsList.begin()+Index);
		Index=-1;

		if(Graphics!=NULL)
		{
			Graphics->Exit();
			delete Graphics;
			Graphics=NULL;
		}

		if(WindowsList.size()==0)
		{
			UnregisterClass(DL32WINDOWCLASS,WndClass.hInstance);
			WindowClassNotRegistered=true;
		}
	}
}

void dl32Window::Start()
{
	MessageLoop();
}

void dl32Window::Exit()
{
	Messagging=false;
}

bool dl32Window::Started()
{
	return Messagging;
}

void dl32Window::UpdateArea()
{
	RECT area;
	GetWindowRect(hwnd,&area);
	Area=dl322DAABB(area.left,area.top,area.right-area.left,area.bottom-area.top);
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

bool dl32Window::Ready()
{
	return ready;
}

HWND dl32Window::GetHwnd()
{
	return hwnd;
}

dl32GraphicsClass* dl32Window::GetGraphics()
{
	return Graphics;
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

dl322DAABB dl32Window::GetClientArea()
{
	return dl322DAABB(0,0,Area.GetWidth(),Area.GetHeight()); //NOTA: Provisional. (Area es el area total de la ventana)
}

dl322DAABB dl32Window::GetScreenArea()
{
	return Area;
}

void dl32Window::SetScreenArea(dl322DAABB Area)
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

void dl32Window::MessageLoop()
{
	MSG message;
	Messagging=true;
	int i;

	while(Messagging && WindowsList.size()>0)
	{
		if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
		{
			i=0;
			while(i>=0 && i<WindowsList.size())
			{
				if(WindowsList[i]->hwnd==message.hwnd)
				{
					LastWindowMessaged=WindowsList[i];
					i=-1;
				}
				else
					++i;
			}

			TranslateMessage(&message);
			DispatchMessage(&message);

			LastWindowMessaged->ProcessMessage(message);
			LastWindowMessaged->Graphics->Frame();
		}
		else
		{
			for(int i=0;i<WindowsList.size();++i)
			{
				WindowsList[i]->Idle.RaiseEvent();
				WindowsList[i]->Graphics->Frame();
			}
		}
	}

	Messagging=false;
}

void dl32Window::ProcessMessage(HWND &hWnd, UINT &msg, WPARAM &wParam, LPARAM &lParam)
{
	switch(msg)
	{
	case WM_QUIT:
	case WM_DESTROY:
	case WM_CLOSE:
		this->~dl32Window();
		LastKeyCaptureState=KEYCAPTURESTATE_NOCAPTURE;
		break;
	case WM_MOVE:
		UpdateArea();
		Move.RaiseEvent(&Area);
		LastKeyCaptureState=KEYCAPTURESTATE_NOCAPTURE;
		break;
	case WM_PAINT:
		UpdateArea();
		Paint.RaiseEvent();
		LastKeyCaptureState=KEYCAPTURESTATE_NOCAPTURE;
		break;
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
	default:
		Idle.RaiseEvent();
	}
}

inline void dl32Window::ProcessMessage(MSG &Message)
{
	return ProcessMessage(Message.hwnd,Message.message,Message.wParam,Message.lParam);
}

