/* 
 * File:   dl32TypeName.h
 * Author: Manu343726
 *
 * Created on 15 de junio de 2013, 14:20
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

#pragma once

#ifndef DL32TYPENAME_H
#define	DL32TYPENAME_H

#include <string>
#include <typeinfo>

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Retrieves the demangled version of a symbol name.
///
/// @author	Manu343726
/// @return Returns a std::string containing the demangled name. Returns the same string as the imput if
///         demangling fails.
///////////////////////////////////////////////////////////////////////////////////////////////////////
std::string dl32Demangle(const std::string& name);

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Retrieves the name of a type.
///
/// @author	Manu343726
///
/// @tparam T The type wich name will be retrieved.
/// 
/// @return A std::string containing the demangled version of the type name.
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline std::string type_to_string() { return dl32Demangle( typeid( T ).name() ); }

#endif	/* DL32TYPENAME_H */

