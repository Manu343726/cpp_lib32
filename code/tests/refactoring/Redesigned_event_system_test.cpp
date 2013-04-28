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
        
	dl32SystemEventsManager::instance().setUpEvent<dl32MouseMoveEvent>( dispatchFunction , WM_MOUSEMOVE );

	dl32SystemEventsManager::instance().getEvent<dl32MouseMoveEvent>( WM_MOUSEMOVE ).AddHandler( OnMouseMove );

	dl32Window* window = new dl32Window("cpp_lib32");

	dl32WindowsManager::instance().start();
        
        return 0;
}
#endif
