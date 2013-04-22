#include "dl32Window.h"
#include <string>

using namespace std;

/// @brief	The cpp_lib32 windows manager singleton instance.
dl32WindowsManager* dl32WindowsManager::_instance = NULL;

/*********************************
* dl32Window static fields setup *
*********************************/
bool dl32Window::_windowClassRegistered = false;
const dl32WindowHandle dl32Window::INVALID_WINDOW_HANDLE = (dl32WindowHandle)NULL;
const WNDCLASS dl32Window::WINDOWCLASS = 
{
	CS_DBLCLKS ,                                        //(Style) Window captures double-click messages
	(WNDPROC)dl32WindowsManager::getWindowProcedure() , //(Window proc)
	0,                                                  //(cbClsExtra) No extra information
	0,                                                  //(cbWndExtra) No extra information
	(HINSTANCE)NULL,                                    //(hInstance)
	LoadIcon (NULL, IDI_APPLICATION),                   //(hIcon)
	LoadCursor (NULL, IDC_ARROW),                       //(hCursor)
	(HBRUSH)NULL,                                       //(hBrush)
	(LPCTSTR)NULL,                                      //(MenuName)
	(LPCTSTR)"cpp_lib32"                                //(WindowClassName)
};


LRESULT CALLBACK dl32WindowsManager::_WindowProcedure( WINDOWS_PROCEDURE_ARGS )
{
	EventsManager.dispatch( WINDOWS_PROCEDURE_BYPASS );

	return DefWindowProc( WINDOWS_PROCEDURE_BYPASS );
}

void dl32Window::_setup(const string& title , uint width , uint height , uint left , uint top , bool isNewWindow)
{
	if(isNewWindow)
	{
		if(!_windowClassRegistered)
		{
			if(RegisterClass( &WINDOWCLASS ) == NULL)
				throw dl32WindowClassRegistrationFailedException();
			else
				_windowClassRegistered = true;
		}

		if( ( _handle = CreateWindow( WINDOWCLASS.lpszClassName , (LPCSTR)title.c_str() , WINDOWCLASS.style , left , top , width , height , NULL , NULL, WINDOWCLASS.hInstance , NULL) ) == dl32Window::INVALID_WINDOW_HANDLE )
			throw dl32WindowCreationFailedException();
	}
	else
		SetWindowLongPtr( _handle , GWL_WNDPROC , dl32WindowsManager::getWindowProcedure()); //Reestablece el procedimiento de ventana (Puede que la ventana no la hayamos creado nosotros y por tanto no use nuestro procedimiento de ventana).

	SetWindowLongPtr( _handle , GWLP_USERDATA , reinterpret_cast<LONG_PTR>(this) ); //Guardamos en el user data de la ventana la instancia de su wrapper.
                                                                                    //As� podemos obtener facilmente la instancia de dl32Window asociada a una ventana ( Ver implementaci�n de dl32WindowsManager::getWindow() )

	//ShowWindow( _handle , SW_SHOW );
}

void dl32WindowsManager::_messageLoop()
{
	while( _processingMessages )
	{
		MSG message;

		if( PeekMessage( &message , NULL , 0 , 0 , PM_REMOVE ) == TRUE )
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
}