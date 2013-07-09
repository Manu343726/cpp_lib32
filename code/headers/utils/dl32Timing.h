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
#include "dl32Singleton.h"
#include "dl32MetaprogrammingLibrary.h"

#include <chrono>
#include <string>
#include <vector>
#include <memory>

using std::chrono::high_resolution_clock;

template<bool HIGH_RESOLUTION_CLOCK_ENABLED>
using dl32TimingClock = typename dl32SelectType<HIGH_RESOLUTION_CLOCK_ENABLED , std::chrono::high_resolution_clock , std::chrono::steady_clock>::type;

template<bool HIGH_RESOLUTION_CLOCK_ENABLED>
using dl32TimingTimePoint = typename dl32TimingClock<HIGH_RESOLUTION_CLOCK_ENABLED>::time_point;

template<bool HIGH_RESOLUTION_CLOCK_ENABLED>
using dl32TimingDuration = typename dl32TimingClock<HIGH_RESOLUTION_CLOCK_ENABLED>::duration;

template<bool HIGH_RESOLUTION = true> 
class dl32TimingFrame
{
private:
	typename dl32TimingClock<HIGH_RESOLUTION>::time_point _begin;
	typename dl32TimingClock<HIGH_RESOLUTION>::time_point _end;
	int    _depth;
	string _function;
        bool _finished;

public:
	dl32TimingFrame(const dl32TimingTimePoint<HIGH_RESOLUTION>& begin , int depth , const std::string& function_name = "unknown") : _finished( false ) , _depth( depth ) , _function( function_name ) , _begin( begin ) {}
        
        bool running() const { return !_finished; }
        
        const dl32TimingTimePoint<HIGH_RESOLUTION>& begin_point() const { return _begin; }
        const dl32TimingTimePoint<HIGH_RESOLUTION>& end_point()   const { return _end; }
        
        decltype( _end - _begin ) duration() const { return _end - _begin; }
        
        void finish(const dl32TimingTimePoint<HIGH_RESOLUTION>& end_point) { _end = end_point; _finished = true; }
        void finish() { finish( dl32TimingClock<HIGH_RESOLUTION>::now() ); }
};

class dl32TimingTreeNode
{
private:
	std::vector<std::shared_ptr<dl32TimingTreeNode>> _subnodes;
	int _frame;
	std::shared_ptr<dl32TimingTreeNode> _parent;
public: 
	dl32TimingTreeNode() : _parent( nullptr ) , _frame( -1 ) {}
	dl32TimingTreeNode(const std::shared_ptr<dl32TimingTreeNode>& parent, int frame) : _parent( parent ) , _frame( frame ) {}
	~dl32TimingTreeNode() {};

	bool is_root()       const {return _parent == nullptr;}
        bool is_root_child() const { return !is_root() && _parent->is_root(); }
	bool is_leaf()       const {return _subnodes.size() == 0;}
	int frame()          const {return _frame;}

	const std::shared_ptr<dl32TimingTreeNode>& parent() const {return _parent;}

	bool isValid() const {return _frame >= 0;}
	void clear()  {_subnodes.clear(); _frame = -1; _parent = nullptr;}

	void add(const std::shared_ptr<dl32TimingTreeNode>& node){_subnodes.push_back( node );}
        
        const std::vector<std::shared_ptr<dl32TimingTreeNode>>& timing_tree() const { return _subnodes; }
};

template<bool HIGH_RESOLUTION = true>
class dl32Timing : public dl32Singleton<dl32Timing<HIGH_RESOLUTION>>
{
    MAKE_SINGLETON_EX( dl32Timing , dl32Timing<HIGH_RESOLUTION> );
    
private:
	std::shared_ptr<dl32TimingTreeNode> _root;
	std::vector<int>   _stack; 
	std::vector<dl32TimingFrame<HIGH_RESOLUTION>> _frames;

	std::shared_ptr<dl32TimingTreeNode> _current;
        
public:
	void push(const std::string& function_name = "unknown")
        {          
            auto new_current_node = std::make_shared<dl32TimingTreeNode>( _current , _frames.size() );

            _current->add( new_current_node );
            _current = new_current_node;
            
            _stack.push_back( _frames.size() );
            _frames.push_back( dl32TimingFrame<HIGH_RESOLUTION>( dl32TimingClock<HIGH_RESOLUTION>::now() , _stack.size() - 1 , function_name ) );
        }
        
	void pop()
        {
            auto end = dl32TimingClock<HIGH_RESOLUTION>::now();
            
            if( _stack.empty() ) return;
            
            _frames[ _stack.back() ].finish( end );
            
            _stack.pop_back();
            
            if( !_current->is_root() )
                _current = _current->parent();
        }
        
        void reset()
        {
            _stack.clear();
            _frames.clear();
            _root->clear();
            _current = _root;
        }
        
        bool good() const { return _root != nullptr; }

        const dl32TimingFrame<HIGH_RESOLUTION>&         last_frame()     const { return frame( _frames.size() - 1 ); }
	const dl32TimingFrame<HIGH_RESOLUTION>&         frame(int index) const { return _frames.at(index); }
	const vector<dl32TimingFrame<HIGH_RESOLUTION>>& frames()         const { return _frames; }
	const std::shared_ptr<dl32TimingTreeNode>&      timing_tree()    const { return _root; }
};


template<bool HIGH_RESOLUTION>
dl32Timing<HIGH_RESOLUTION>::dl32Timing()
{
    _root = std::make_shared<dl32TimingTreeNode>();
    _current = _root;
}

const bool high_resolution_timing   = true;
const bool normal_resolution_timing = false;


#define DL32TIMING_HIGHRESOLUTION_BEGIN   dl32Timing<high_resolution_timing  >::instance().push(__FUNCTION__);
#define DL32TIMING_NORMALRESOLUTION_BEGIN dl32Timing<normal_resolution_timing>::instance().push(__FUNCTION__);
#define DL32TIMING_HIGHRESOLUTION_END   dl32Timing<high_resolution_timing  >::instance().pop();
#define DL32TIMING_NORMALRESOLUTION_END dl32Timing<normal_resolution_timing>::instance().pop();

#define DL32TIMING_BEGIN DL32TIMING_HIGHRESOLUTION_BEGIN
#define DL32TIMING_END   DL32TIMING_HIGHRESOLUTION_END

#endif /* INCLUDE GUARDS */