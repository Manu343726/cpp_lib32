#include "dl32Timing.h"

dl32TimingTreeNode dl32Timing::_root;
vector<int> dl32Timing::_timingStack; 
vector<dl32TimingFrame> dl32Timing::_frames;

dl32TimingTreeNode* dl32Timing::_actual=NULL;

double dl32TimingFrame::getLapse()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	return (end.QuadPart - begin.QuadPart)*1000/frequency.QuadPart;
}

dl32TimingTreeNode::~dl32TimingTreeNode()
{
	for(unsigned int i = 0; i<_subnodes.size(); ++i)
		delete _subnodes[i];

	_frame = -1;
	_parent = NULL;
}

void dl32Timing::push(char* function)
{
	LARGE_INTEGER begin;
	QueryPerformanceCounter(&begin);

	if(_timingStack.size() == 0)
	{
		_frames.clear();
		_root.clear();
		_actual = &_root;
	}

	#if DL32DEBUG_TIMING_PROMPTPUSH
	Console.WriteLine("Starting '" + dl32String(function) + "' ... (stackDepth: " + dl32String((int)_timingStack.size()) + ")",DL32CP_GRAY);
	#endif

	dl32TimingTreeNode* newActual = new dl32TimingTreeNode(_actual,_frames.size());
	_actual->add(newActual);
	_actual = newActual;

	_timingStack.push_back(_frames.size());
	_frames.push_back(dl32TimingFrame(function,begin,_timingStack.size()));
}

void dl32Timing::pop()
{
	if(_timingStack.size() > 0)
	{
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);

		_frames[_timingStack.back()].Finalize(end);

		#if DL32DEBUG_TIMING_PROMPTPOP
			Console.WriteLine("Exiting '" + dl32String(_frames[_timingStack.back()].function) + "' (" + dl32String(_frames[_timingStack.back()].lapse) + " ms)",DL32CP_GRAY);
		#endif

		
		_timingStack.pop_back();

		if(!_actual->isRoot())
			_actual = _actual->parent();
	}
}