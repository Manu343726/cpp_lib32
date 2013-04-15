#include "dl32Events.h"

#include <unordered_set>
#include <WindowsX.h>

using namespace std;

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
	* del hashmap) guardarán el mismo puntero.                                   *
	*                                                                            *
	* Por esta razón, hay que tener cuidado de no eliminar dos veces el mismo    *
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
* Básicamente comprueba que no existe  ningún evento en el manager asociado a los mensajes, y de ser *
* así, inserta un par (systemMessage , eventDispatcher) en la tabla para cada mensaje. Recordemos    *
* que un evento puede ser lanzado por varios system messages diferentes. Para ello el hashmap        *
* contiene una entrada por cada mensaje, todas ellas asociadas al mismo dispatcher (Diferentes       *
* claves, mismo valor).                                                                              *
*****************************************************************************************************/
bool dl32SystemEventsManager::setUpEvent(dl32EventDispatcher* dispatcher, const vector<UINT>& systemMessages)
{
	if(systemMessages.empty()) return false;

	for(auto it = systemMessages.begin() ; it != systemMessages.end() ; it++)
		if( _dispatchers.find(*it) != _dispatchers.end() )
			return false;
		else
			_dispatchers.insert( unordered_map<UINT , dl32EventDispatcher*>::value_type(*it , dispatcher) );

	return true;
}

/****************************************************************************
* Sobrecarga de la función anterior, para un solo system message por evento *
****************************************************************************/
bool dl32SystemEventsManager::setUpEvent(dl32EventDispatcher* dispatcher , UINT systemMessage)
{
	if( _dispatchers.find(systemMessage) == _dispatchers.end() )
	{
		_dispatchers.insert( unordered_map<UINT , dl32EventDispatcher*>::value_type(systemMessage , dispatcher) );

		return true;
	}
	else
		return false;
}

/**************************************************************************************************************
* Busca en la lista de event dispatchers el evento correspondiente al system message, y llama a su dispatcher *
**************************************************************************************************************/
void dl32SystemEventsManager::dispatch(HWND window , UINT message , WPARAM wParam , LPARAM lParam)
{
	auto it = _dispatchers.find(message);

	if( it != _dispatchers.end() )
		it->second->dispatch( window , message , wParam , lParam );
}

/**********************************************************
* Configura el conjunto de eventos predefinidos en la API *
**********************************************************/
void dl32SystemEventsManager::setUpDefaultEvents()
{

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

	return Data;
}
