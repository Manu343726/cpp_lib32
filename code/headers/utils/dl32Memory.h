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

#ifndef DL32MEMORY
#define DL32MEMORY

#include "dl32Config.h"

#ifndef NULL 
#define NULL 0 
#endif

#define DL32MEMORY_SAFEDELETE(x) {delete x; x=NULL;}
#define DL32MEMORY_SAFEDELETE_ARRAY(x) {delete[] x; x=NULL;}

template <class T>
class dl32SmartPointer
{
private:
	T* _ptr;
public:
	dl32SmartPointer(T* ptr){_ptr=ptr;}
	~dl32SmartPointer(){DL32MEMORY_SAFEDELETE(_ptr);}
	
	T* operator ->(){return _ptr;}
};

#ifdef _MSC_VER
#include <crtdbg.h>
#define DL32UTILS_LEAKDEBUGGER

typedef _CRT_ALLOC_HOOK dl32AllocHook;

class dl32LeakDebugger
{
private:
	static _CrtMemState _initState;
	static _CrtMemState _lastDifference;
	static dl32AllocHook _UserAllocHook;

	dl32LeakDebugger();

	static int _allocHook(int allocType, void* userData, size_t size, int blockType, long RequestNumber, const unsigned char* fileName, int lineNumber);
	static bool _hooking;
public:
	static void Start(){_CrtMemCheckpoint(&_initState);}
	static bool Check();
	static void SetHook(dl32AllocHook hookFunction);
};
#endif //End of _MSC_VER
#endif //End of DL32MEMORY