/* 
 * File:   dl32TMPCore.h
 * Author: Manu343726
 *
 * Created on 6 de junio de 2013, 11:09
 */

#ifndef DL32TMPCORE_H
#define	DL32TMPCORE_H

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

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Type representing types without semantic sense (Non-valid types).
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
class dl32NoType {};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Basic type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
class dl32EmptyType {};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 basic generic type-wrapper.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct dl32TypeWrapper
{
    using type = T;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 basic compile-time-constant value wrapper. (Valid only for integral types).
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, T VALUE>
struct dl32ValueWrapper
{
    static const T value = VALUE;
};

template<bool VALUE>
using dl32BoolWrapper = dl32ValueWrapper<bool,VALUE>; ///< Compile-time boolean expression wrapper.
using dl32TrueWrapper = dl32BoolWrapper<true>; ///< Compile time true boolean value wrapper.
using dl32FalseWrapper = dl32BoolWrapper<false>; ///< Compile-time false boolean wrapper.

template<unsigned int VALUE>
using dl32UintWrapper = dl32ValueWrapper<unsigned int , VALUE>; ///< Compile-time unsigned int value wrapper.

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Checks if a type is a compile-time integral value wrapper.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename U>
struct dl32IsValueWrapper : public dl32FalseWrapper {};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Checks if a type is a compile-time integral value wrapper.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename U , U _wrapped_value>
struct dl32IsValueWrapper<dl32ValueWrapper<U,_wrapped_value>> : public dl32TrueWrapper
{
    static const U wrapped_value = _wrapped_value; ///< Wrapped value.
    using type = U; ///< Type of the wrapped value.
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 type selection.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<bool flag , typename T , typename U>
class dl32SelectType
{
private:
    template<bool _flag , typename _T , typename _U>
    struct _select;
    
    template<typename _T , typename _U>
    struct _select<true,_T,_U>{ using type = _T; };
    
    template<typename _T , typename _U>
    struct _select<false,_T,_U>{ using type = _U; };
    
public:
    using type = typename _select<flag,T,U>::type;
};

template<bool CONDITION , typename THEN , typename ELSE>
using dl32tmp_if = dl32SelectType<CONDITION,THEN,ELSE>; ///< Metaprogram if.

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 function overloading enabler.
/// @details This template provides a method to disable certain function overloads by a boolean flag.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<bool flag , typename T>
struct dl32EnableIf {};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 function overloading enabler.
/// @details This template provides a method to disable certain function overloads by a boolean flag.
///
/// @author	Manu343726
///
/// @remarks Template specialitation for enabled case.
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct dl32EnableIf<true,T>{ using type = T; };
#endif	/* DL32TMPCORE_H */

