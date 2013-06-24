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

#include "dl32Timing.h"

#ifdef WIN32

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

		_timingStack.pop_back();

		if(!_actual->isRoot())
			_actual = _actual->parent();
	}
}

#endif /* WIN32 */