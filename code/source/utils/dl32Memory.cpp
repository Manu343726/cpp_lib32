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