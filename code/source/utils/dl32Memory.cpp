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

#ifdef _MSC_VER

#include "dl32Memory.h"

_CrtMemState dl32LeakDebugger::_initState;
_CrtMemState dl32LeakDebugger::_lastDifference;
dl32AllocHook dl32LeakDebugger::_UserAllocHook = NULL; 
bool dl32LeakDebugger::_hooking = false;

bool dl32LeakDebugger::Check()
{
	_CrtMemState now;
	_CrtMemCheckpoint(&now);

	return _CrtMemDifference(&_lastDifference,&_initState,&now) == TRUE;
}

void dl32LeakDebugger::SetHook(dl32AllocHook hookFunction)
{
	if((_UserAllocHook == NULL && hookFunction !=NULL))
		_CrtSetAllocHook(_allocHook);
	else if(_UserAllocHook != NULL && hookFunction==NULL)
		_CrtSetAllocHook(NULL);

	_UserAllocHook = hookFunction;
}

int dl32LeakDebugger::_allocHook(int allocType, void* userData, size_t size, int blockType, long RequestNumber, const unsigned char* fileName, int lineNumber)
{
	if(!_hooking)//Para prevenir capturas sucesivas (Posible recursividad infinita)
	{
		_hooking = true;
		_UserAllocHook(allocType,userData,size,blockType,RequestNumber,fileName,lineNumber);
		_hooking = false;
	}

	return TRUE;
}

#endif