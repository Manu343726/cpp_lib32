
#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_EVENTSSYSTEM

#include "dl32Events.h"
#include "dl32Config.h"

#include <functional>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

int main()
{
	auto dispatchFunction = [] ( WINDOWS_PROCEDURE_ARGS ) { return dl32SystemMessagesDataTranslator::getMouseData ( WINDOWS_PROCEDURE_BYPASS ); };
	
	dl32SystemEventsManager::instance().setUpEvent<dl32MouseMoveEvent>( dispatchFunction , WM_MOUSEMOVE );
<<<<<<< HEAD
	
	dl32SystemEventsManager::instance().setUpEvent<dl32MouseDownEvent>( dl32SystemMessagesDataTranslator::getMouseData , WM_RBUTTONDOWN );
	dl32SystemEventsManager::instance().setUpEvent<dl32MouseDownEvent>( dl32SystemMessagesDataTranslator::getMouseData , WM_LBUTTONDOWN );

	auto MouseDownEvent = dl32SystemEventsManager::instance().getEvent<dl32MouseDownEvent>( WM_RBUTTONDOWN );

	MouseDownEvent.AddHandler( [](dl32MouseDownEvent::ArgummentsType) { /* MouseDown!!! */ } );
}

#endif