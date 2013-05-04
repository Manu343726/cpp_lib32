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

#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_EVENTSSYSTEM

#include "dl32Config.h"

#include "dl32Window.h"
#include "dl32Events.h"
#include "dl32Typing.h"


#include <functional>
#include <string>
#include <vector>
#include <iostream>

using namespace std;


void OnMouseMove(dl32MouseMoveEvent::ArgummentsType args)
{
	cout << "MouseMove! " << args.Location.toString() << endl;
}

int main()
{
	auto dispatchFunction = [] ( WINDOWS_PROCEDURE_ARGS ) { return dl32SystemMessagesDataTranslator::getMouseData ( WINDOWS_PROCEDURE_BYPASS ); };

        if( dl32TypeChecking<dl32Point2D , dl32Vector2D>::superclass_subclass)
            cout << "Inheritance!!!";
        
        if( dl32TypeTraits<int>::isPointer )
            cout << "Is a pointer!!!";
        
	dl32SystemEventsManager::instance().setUpEvent<dl32MouseMoveEvent>( dispatchFunction , WM_MOUSEMOVE );

	dl32SystemEventsManager::instance().getEvent<dl32MouseMoveEvent>( WM_MOUSEMOVE ).AddHandler( OnMouseMove );

	dl32Window* window = new dl32Window("cpp_lib32");

	dl32WindowsManager::instance().start();
        
        return 0;
}
#endif
