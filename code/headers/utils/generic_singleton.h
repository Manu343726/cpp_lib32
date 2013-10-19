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

namespace dl32
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This class provides a generic implementation of the singleton dessign pattern based on CRTP.
	///
	/// @tparam T Class that implements the singleton.
	/// @author	Manu343726
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	class singleton
	{
	public:
		typedef T* PointerToInstance;
		typedef T& ReferenceToInstance;
		typedef singleton<T>* RealPointerToInstance;

	private:
		singleton(const singleton&) {}
		singleton& operator=(const singleton&) {}
    
		static RealPointerToInstance _instance;

	protected:
		singleton() {}
		virtual ~singleton() {} //Correct polymorphic delete
	public:
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// @brief Gets a reference to the singleton instance.
		///
		/// @author	Manu343726
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		static ReferenceToInstance instance()
		{
			if( _instance == nullptr)
			{
				/* Register a function to delete the instance at runtime-exit to avoid memory-leaks */
				std::atexit( [](){ delete _instance; } );

				_instance = new T;
			}
        
			return *(static_cast<T*>(_instance));
		}
	};

	/* Definition of the instance pointer */
	template<typename T>
	typename singleton<T>::RealPointerToInstance dl32::singleton<T>::_instance = nullptr;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This macro provides a "shortcut" to hide all ctors and assignment operator to performs 
	///        the singleton dessing pattern implementation.
	///
	/// @details Put this macro at the top of your class/struct (Next to the open brace), with your class 
	///          name as argumment.
	///
	/// @author	Manu343726
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	#define MAKE_SINGLETON_EX( class_name , class_type )                                     \
				friend class dl32::singleton< class_type >;                                  \
			private:                                                                         \
				class_name(); /* ctor implementation must be in class_name implementation */ \
																							 \
				class_name(const class_type&);                                               \
				class_type& operator=(const class_type&);  

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This macro provides a "shortcut" to hide all ctors and assignment operator to performs 
	///        the singleton dessing pattern implementation.
	///
	/// @details Put this macro at the top of your class/struct (Next to the open brace), with your class 
	///          name as argumment.
	///
	/// @author	Manu343726
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	#define MAKE_SINGLETON( class_name ) MAKE_SINGLETON_EX( class_name , class_name )       
}
#endif	/* DL32SINGLETON_H */

