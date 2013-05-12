/*******************************************************************************
* cpp_lib32 project. C++ port of the dx_lib32 project.                         *
*                                                                              *
* Copyright (C) 2012 - 2013, Manuel Sánchez Pérez                              *                     
*                                                                              *
* This file is part of cpp_lib32 project.                                      *
*                                                                              *
* cpp_lib32 project is free software: you can redistribute it and/or modify    *
* it under the terms of the GNU Lesser General Public License as published by  *
* the Free Software Foundation, version 2 of the License.                      *
*                                                                              *
* cpp_lib32 is distributed in the hope that it will be useful,                 *
* but WITHOUT ANY WARRANTY; without even the implied warranty of               * 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
* GNU Lesser General Public License for more details.                          *
*                                                                              *
 You should have received a copy of the GNU Lesser General Public License      *
 along with cpp_lib32 project. If not, see <http://www.gnu.org/licenses/>.     *
*******************************************************************************/

#include "dl32Window.h"
#include <string>
#include <unordered_set>
#include <windowsx.h>

using namespace std;

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

//Singleton instance:
dl32SystemEventsManager* dl32SystemEventsManager::_instance = nullptr;

/***************************************************************************
* Deletes event dispatchers pointed by every pointer stored in the hashmap *
***************************************************************************/
dl32SystemEventsManager::~dl32SystemEventsManager()
{
	/*****************************************************************************
	* Hay que recordar que un evento puede envolver ("wrappear") a varios system *
	* messages, lo que implica que diferentes system messages (Diferentes claves *
	* del hashmap) guardar�n el mismo puntero.                                   *
	*                                                                            *
	* Por esta raz�n, hay que tener cuidado de no eliminar dos veces el mismo    *
	* puntero. Para ello, se provee de otra hashmap con clave/valor el puntero   *
	* (Si la clave/puntero esta en el hashmap, este ya ha sido eliminado)        *
	*****************************************************************************/

	unordered_set<dl32EventDispatcher*> deleted;

	for(auto it = _dispatchers.begin() ; it != _dispatchers.end() ; it++)
	{
		if( deleted.find(it->second) == deleted.end() )
		{
			delete it->second;

			deleted.insert(it->second);
		}
	}
}

/*****************************************************************************************************
* B�sicamente comprueba que no existe  ning�n evento en el manager asociado a los mensajes, y de ser *
* as�, inserta un par (systemMessage , eventDispatcher) en la tabla para cada mensaje. Recordemos    *
* que un evento puede ser lanzado por varios system messages diferentes. Para ello el hashmap        *
* contiene una entrada por cada mensaje, todas ellas asociadas al mismo dispatcher (Diferentes       *
* claves, mismo valor).                                                                              *
*****************************************************************************************************/
bool dl32SystemEventsManager::setUpEvent(dl32EventDispatcher* dispatcher, const vector<UINT>& systemMessages)
{
	if(systemMessages.empty()) return false;

	for(auto it = std::begin( systemMessages ) ; it != std::end( systemMessages ) ; it++)
		if( _dispatchers.find(*it) != _dispatchers.end() )
			return false;
		else
			_dispatchers.insert( unordered_map<UINT , dl32EventDispatcher*>::value_type(*it , dispatcher) );

	return true;
}

/****************************************************************************
* Sobrecarga de la funci�n anterior, para un solo system message por evento *
****************************************************************************/
bool dl32SystemEventsManager::setUpEvent(dl32EventDispatcher* dispatcher , UINT systemMessage)
{
	if( _dispatchers.find(systemMessage) == std::end( _dispatchers ) )
	{
		_dispatchers.insert( unordered_map<UINT , dl32EventDispatcher*>::value_type(systemMessage , dispatcher) );

		return true;
	}
	else
		return false;
}

/**********************************************************
* Configura el conjunto de eventos predefinidos en la API *
**********************************************************/
void dl32SystemEventsManager::setUpDefaultEvents()
{
	/* TODO */
}

dl32KeyStrokeData dl32SystemMessagesDataTranslator::getKeyStrokeData( WINDOWS_PROCEDURE_ARGS )
{
	dl32KeyStrokeData Data;

	Data.Key=char(wParam);
	Data.PreviousPressed=(lParam >> 30) & 0x1;//Miguel y Laura, mi cerebro se licua con las operaciones a bits
	Data.RepeatCount=lParam & 0x00007FFF; //00000000000000000111111111111111

	return Data;
}

dl32MouseData dl32SystemMessagesDataTranslator::getMouseData( WINDOWS_PROCEDURE_ARGS )
{
	dl32MouseData Data;

	switch(wParam)
	{
	case MK_RBUTTON:
		Data.Button = dl32MouseButton::RIGHT;
		break;
	case MK_MBUTTON:
		Data.Button = dl32MouseButton::CENTER;
		break;
	case MK_LBUTTON:
		Data.Button = dl32MouseButton::LEFT;
		break;
	default:
		Data.Button = dl32MouseButton::NONE;
		break;
	}

	Data.Location.x = GET_X_LPARAM(lParam);
	Data.Location.y = GET_Y_LPARAM(lParam);
	Data.Delta      = GET_WHEEL_DELTA_WPARAM(wParam);

	return dl32MouseData( Data.Location , Data.Button , Data.Delta );
}

shared_ptr<dl32Window> dl32WindowsManager::createWindow(const string& title, uint width, uint height, uint left, uint top)
{
    return shared_ptr<dl32Window>( new dl32Window( title , width , height , left , top ) );
}

shared_ptr<dl32Window> dl32WindowsManager::createWindow(dl32WindowHandle handle)
{
    return shared_ptr<dl32Window>( new dl32Window( handle ) );
}

LRESULT CALLBACK dl32WindowsManager::_WindowProcedure( WINDOWS_PROCEDURE_ARGS )
{
	EventsManager.dispatch( WINDOWS_PROCEDURE_BYPASS );

	return DefWindowProc( WINDOWS_PROCEDURE_BYPASS );
}

void dl32Window::_setup(const string& title , uint width , uint height , uint left , uint top , bool isNewWindow)
{
    if (isNewWindow) {
        if (!_windowClassRegistered) {
            if (RegisterClass(&WINDOWCLASS) == NULL)
                throw dl32WindowClassRegistrationFailedException();
            else
                _windowClassRegistered = true;
        }

        if ((_handle = CreateWindow(WINDOWCLASS.lpszClassName, (LPCWSTR) title.c_str(), WINDOWCLASS.style, left, top, width, height, NULL, NULL, WINDOWCLASS.hInstance, NULL)) == dl32Window::INVALID_WINDOW_HANDLE)
            throw dl32WindowCreationFailedException();
    } else
        SetWindowLongPtr(_handle, GWL_WNDPROC, dl32WindowsManager::getWindowProcedure()); //Reestablece el procedimiento de ventana (Puede que la ventana no la hayamos creado nosotros y por tanto no use nuestro procedimiento de ventana).

    SetWindowLongPtr(_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR> (this)); //Guardamos en el user data de la ventana la instancia de su wrapper.
    //As� podemos obtener facilmente la instancia de dl32Window asociada a una ventana ( Ver implementaci�n de dl32WindowsManager::getWindow() )

    ShowWindow(_handle, SW_SHOW);
}

void dl32WindowsManager::_messageLoop()
{
    while (_processingMessages) {
        MSG message;

        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE) == TRUE) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    }
}