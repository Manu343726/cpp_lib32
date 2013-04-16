
#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_EVENTSSYSTEM

#include "dl32Window.h"
#include "dl32Events.h"
#include "dl32Config.h"

#include <functional>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

void OnMouseMove(dl32MouseMoveEvent::ArgummentsType)
{
	cout << "MouseDown!" << endl;
}

int main()
{
	auto dispatchFunction = [] ( WINDOWS_PROCEDURE_ARGS ) { return dl32SystemMessagesDataTranslator::getMouseData ( WINDOWS_PROCEDURE_BYPASS ); };
	
	dl32SystemEventsManager::instance().setUpEvent<dl32MouseMoveEvent>( dispatchFunction , WM_MOUSEMOVE );

	dl32SystemEventsManager::instance().getEvent<dl32MouseMoveEvent>( WM_MOUSEMOVE ).AddHandler( OnMouseMove );

	dl32Window* window = new dl32Window("cpp_lib32");

	dl32WindowsManager::instance().start();
}

#endif