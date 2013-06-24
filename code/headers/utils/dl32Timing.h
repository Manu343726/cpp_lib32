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

#ifndef DL32TIMING_H
#define DL32TIMING_H

#include "dl32Config.h"

#ifdef WIN32

#include <Windows.h>
#include <string>
#include <vector>

using namespace std;

struct dl32TimingFrame
{
	LARGE_INTEGER begin;
	LARGE_INTEGER end;
	double lapse;
	int depth;
	string function;

	dl32TimingFrame(char* function,LARGE_INTEGER begin, int depth){this->begin = begin; this->depth = depth; this->function = function;}
	void Finalize(LARGE_INTEGER end){this->end = end; lapse = getLapse();}

private:
	double getLapse();
};

class dl32TimingTreeNode
{
private:
	vector<dl32TimingTreeNode*> _subnodes;
	int _frame;
	dl32TimingTreeNode* _parent;
public: 
	dl32TimingTreeNode() : _parent(NULL) , _frame(-1) {}
	dl32TimingTreeNode(dl32TimingTreeNode* parent, int frame) : _parent(parent) , _frame(frame) {}
	~dl32TimingTreeNode();
	
	bool isRoot() const {return _parent == NULL;}
	bool isLeaf() const {return _subnodes.size() == 0;}
	int frame()   const {return _frame;}

	dl32TimingTreeNode* parent() const {return _parent;}

	bool isValid(){return _frame >= 0;}
	void clear(){_subnodes.clear(); _frame = 0; _parent = NULL;}

	void add(dl32TimingTreeNode* node){_subnodes.push_back(node);}
};

//NOTA: Ahora que lo veo igual deber�a haber hecho un singleton en vez de tanta mierda est�tica...
class dl32Timing
{
private:
	static dl32TimingTreeNode _root;
	static vector<int> _timingStack; 
	static vector<dl32TimingFrame> _frames;

	static dl32TimingTreeNode* _actual;
	
	dl32Timing(){}
public:
	static void push(char* function = "Unspecified function");
	static void pop();

	static dl32TimingFrame frame(int index){return _frames.at(index);}
	static vector<dl32TimingFrame> getFrames(){return _frames;}
	static dl32TimingTreeNode getTimingTree(){return _root;}
};

#if DL32DEBUG_TIMING
#define DL32TIMING_BEGIN dl32Timing::push(__FUNCTION__);
#define DL32TIMING_END dl32Timing::pop();
#else
#define DL32TIMING_BEGIN
#define DL32TIMING_END
#endif

#else
#define DL32TIMING_BEGIN
#define DL32TIMING_END
#endif /* WIN32 */
#endif /* INCLUDE GUARDS */