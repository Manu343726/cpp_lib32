/* 
 * File:   dl32Singleton.h
 * Author: Manu343726
 *
 * Created on 3 de mayo de 2013, 12:44
 */

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

#ifndef DL32SINGLETON_H
#define	DL32SINGLETON_H

#include <cstdlib>
#include <assert.h>
using namespace std;

#if defined(__GXX_EXPERIMENTAL_CXX0X) 

#define NULLPTR nullptr

#else

#define NULLPTR NULL

#endif /* C++11 */

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides a generic implementation of the singleton dessign pattern.
///
/// @tparam T Class that implements the singleton.
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , bool USE_VIRTUAL_SETUP = true>
class dl32Singleton
{
public:
    typedef T* PointerToInstance;
    typedef T& ReferenceToInstance;
    typedef dl32Singleton<T>* RealPointerToInstance;
private:
    dl32Singleton(const dl32Singleton&) {}
    dl32Singleton& operator=(const dl32Singleton&) {}
    
    static RealPointerToInstance _instance;
    
    static void _singleton_instance_deleter() { delete _instance; }
protected:
    static bool _ctor_called_by_getinstance;
    
    dl32Singleton() {}
    virtual ~dl32Singleton() {} //Correct polymorphic delete
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Subclasses must define this setup function. All setup operations of a subclass must be 
    ///        performed here, not at the ctor. This allows to make the subclass ctor "private" (That ctor
    ///        only can be used by the instance() method. See assertion on subclass ctor at MAKE_SINGLETON
    ///        macro.)
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual void _setup_singleton_instance() = 0;
public:
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets a reference to the singleton instance.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static ReferenceToInstance instance()
    {
        if( _instance == NULLPTR)
        {
            /* Register the class at runtime-exit to avoid memory-leaks */
            atexit( _singleton_instance_deleter );
            
            _ctor_called_by_getinstance = true;
            _instance = new T;
            
            if(USE_VIRTUAL_SETUP)
                _instance->_setup_singleton_instance();
            
            _ctor_called_by_getinstance = false;
        }
        
        return *(static_cast<T*>(_instance));
    }
};

/* Definition of the instance pointer */
template<typename T , bool USE_VIRTUAL_SETUP>
typename dl32Singleton<T,USE_VIRTUAL_SETUP>::RealPointerToInstance dl32Singleton<T,USE_VIRTUAL_SETUP>::_instance = NULLPTR;
/* Definition of the subclass ctor call flag */
template<typename T , bool USE_VIRTUAL_SETUP>
bool dl32Singleton<T,USE_VIRTUAL_SETUP>::_ctor_called_by_getinstance = false;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This macro provides a "shortcut" to hide all ctors and assignment operator to performs 
///        the singleton dessing pattern implementation.
///
/// @details Put this macro at the top of your class/struct (Next to the open brace), with your class 
///          name as argumment.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAKE_SINGLETON( class_name , use_virtual_setup )                                                             \
            friend class dl32Singleton< class_name , use_virtual_setup >;                                            \
        private:                                                                                                     \
            class_name() { assert( this->_ctor_called_by_getinstance ); } \
                                                                                                                     \
            class_name(const class_name&) = delete;                                                                  \
            class_name& operator=(const class_name&) = delete;                        

#endif	/* DL32SINGLETON_H */

