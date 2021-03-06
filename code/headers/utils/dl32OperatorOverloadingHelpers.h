/* 
 * File:   dl32OperatorOverloadingHelpers.h
 * Author: Manu343726
 *
 * Created on 25 de mayo de 2013, 13:14
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

#ifndef DL32OPERATOROVERLOADINGHELPERS_H
#define	DL32OPERATOROVERLOADINGHELPERS_H

#include "dl32MetaprogrammingLibrary.h"

/****************************************************************************
 * This header provides a set of operator overloading helpers based on CRTP *
 *                                                                          *
 * This helpers provides complete operator overloading based on a specific  *
 * operator that must be implemented by the user of the helper (The class   *
 * that inherits from the helper).                                          *
 * For example: dl32EqualityHelper provides an implementation of inequality *
 * ( operator!= ) based on the equality criteria provided by the user       *
 * ( operator== ).                                                          *
 ***************************************************************************/

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides inequelity criteria for types T vs U based on user-defined equality criteria
///        for types T vs U.
///        Also, this helper provides the complete set of inverse comparisons to ensure symmetry.
///
/// @author	Manu343726
///
/// @details User class must implement operator==(const T& t , const U& u).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U = dl32NoType>
struct dl32EqualityHelper
{ 
    friend bool operator!=(const T& t , const T& u) { return !(t == u); }
    
    friend bool operator==(const U& u , const T& t) {return t == u;}
    friend bool operator!=(const U& u , const T& t) { return t != u; }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides inequelity criteria for type T based on user defined equality criteria 
///        for type T.
///
/// @author	Manu343726
///
/// @details User class must implement operator==(const T& t1 , const T& t2).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct dl32EqualityHelper<T,dl32NoType>
{
    friend bool operator!=(const T& t1 , const T& t2) { return !(t1 == t2); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides ccmplete comparison criteria for types T vs U based on user-defined 
///        bigger-than and less-than criteria for types T vs U.
///        Also, this helper provides the complete set of inverse comparisons to ensure symmetry.
///
/// @author	Manu343726
///
/// @details User class must implement operator>(const T& t , const U& u) and 
///          operator<(const T& t , const U& u).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U = dl32NoType>
struct dl32ComparisonHelper
{
    friend bool operator<=(const T& t , const U& u) { return !(t > u); }
    friend bool operator>=(const T& t , const U& u) { return !(t < u); }  
    
    friend bool operator> (const U& u , const T& t) { return t < u; }
    friend bool operator<=(const U& u , const T& t) { return !(u > t); }
    friend bool operator< (const U& u , const T& t) { return t > u; }
    friend bool operator>=(const U& u , const T& t) { return !(t < u); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides complete comparison criteria for type T based on userdefined biger-than 
///        criteria for type T.
///
/// @author	Manu343726
///
/// @details User class must implement operator>(const T& t1 , const T& t2).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct dl32ComparisonHelper<T,dl32NoType>
{
    friend bool operator<=(const T& t1 , const T& t2) { return !(t1 > t2); }
    friend bool operator< (const T& t1 , const T& t2) { return t2 > t1; }
    friend bool operator>=(const T& t1 , const T& t2) { return !(t1 < t2); }   
};

template<typename T , typename U = dl32NoType>
using dl32ComparisonHelpers = dl32TypeList<dl32EqualityHelper<T,U>,dl32ComparisonHelper<T,U>>; ///< Defines the set of helpers that provides comparison operators for types T and U.

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides the complete comparison operators for given types T and U.
///        Also, this helper provides the complete set of inverse comparisons to ensure symmetry.
///
/// @author	Manu343726
///
/// @details User class must implement operator>(const T& t , const U& u), operator<(const T& t , const U& u),
///          and operator==(const T& t , const U& u).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U = dl32NoType>
struct dl32CompleteComparisonHelper : public dl32ComparisonHelpers<T,U>::public_inheritance_from_types {};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides binary addition operator (T operator+(const T& t , const U& u) based on 
///        user-defined autoincrement operator ( T& operator+=(const U& u) ).
///
/// @author	Manu343726
///
/// @details User class must implement T& operator+=(const U& u).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U = dl32NoType , bool ENABLE_SYMMETRY = true>
struct dl32AdditionHelper
{
    friend T operator+(const T& t , const U& u)
    {
        T t2( t );
        t2 += u;
        return t2;
    }
    
    template<bool SFINAE_BRIDGE = ENABLE_SYMMETRY>
    friend typename dl32EnableIf<SFINAE_BRIDGE,T>::type
    operator+(const U& u , const T& t)
    {
        return t+u;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides binary substraction operator (T operator-(const T& t , const T& u) based on 
///        user-defined autodecrement operator ( T& operator-=(const U& u) ).
///
/// @author	Manu343726
///
/// @details User class must implement T& operator-=(const U& u).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U = dl32NoType , bool ENABLE_SYMMETRY = false>
struct dl32SubstractionHelper
{
    friend T operator-(const T& t , const U& u)
    {
        T t2( t );
        t2 -= u;
        return t2;
    }
    
    template<bool SFINAE_BRIDGE = ENABLE_SYMMETRY>
    friend typename dl32EnableIf<SFINAE_BRIDGE,T>::type
    operator-(const U& u , const T& t)
    {
        return t-u;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides binary multiplication operator (T operator*(const T& t , const T& u) based on 
///        user-defined automultiplication operator ( T& operator*=(const U& u) ).
///
/// @author	Manu343726
///
/// @details User class must implement T& operator*=(const U& u).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U = dl32NoType , bool ENABLE_SYMMETRY = true>
struct dl32MultiplicationHelper
{
    friend T operator*(const T& t , const U& u)
    {
        T t2( t );
        t2 *= u;
        return t2;
    }
    
    template<bool SFINAE_BRIDGE = ENABLE_SYMMETRY>
    friend typename dl32EnableIf<SFINAE_BRIDGE,T>::type
    operator*(const U& u , const T& t)
    {
        return t*u;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides binary division operator (T operator/(const T& t , const U& u) based on 
///        user-defined autoincrement operator ( T& operator/=(const U& u) ).
///
/// @author	Manu343726
///
/// @details User class must implement T& operator/=(const U& u).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U = dl32NoType , bool ENABLE_SYMMETRY = false>
struct dl32DivisionHelper
{
    friend T operator/(const T& t , const U& u)
    {
        T t2( t );
        t2 /= u;
        return t2;
    }
    
    template<bool SFINAE_BRIDGE = ENABLE_SYMMETRY>
    friend typename dl32EnableIf<SFINAE_BRIDGE,T>::type
    operator/(const U& u , const T& t)
    {
        return t/u;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides binary addition operator (T operator+(const T& t1 , const T& t2) based on 
///        user-defined autoincrement operator ( T& operator+=(const T& t) ).
///
/// @author	Manu343726
///
/// @details User class must implement T& operator+=(const T& t).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , bool ENABLE_SYMMETRY>
struct dl32AdditionHelper<T,dl32NoType,ENABLE_SYMMETRY>
{
    friend T operator+(const T& t1 , const T& t2)
    {
        T t3( t1 );
        t3 += t2;
        return t3;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides binary substraction operator (T operator-(const T& t1 , const T& t2) based on 
///        user-defined autodecrement operator ( T& operator-=(const T& t) ).
///
/// @author	Manu343726
///
/// @details User class must implement T& operator-=(const T& t).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , bool ENABLE_SYMMETRY>
struct dl32SubstractionHelper<T,dl32NoType,ENABLE_SYMMETRY>
{
    friend T operator-(const T& t1 , const T& t2)
    {
        T t3( t1 );
        t3 -= t2;
        return t3;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides binary multiplication operator (T operator*(const T& t1 , const T& t2) based on 
///        user-defined automultiplication operator ( T& operator*=(const T& t) ).
///
/// @author	Manu343726
///
/// @details User class must implement T& operator*=(const T& t).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , bool ENABLE_SYMMETRY>
struct dl32MultiplicationHelper<T,dl32NoType,ENABLE_SYMMETRY>
{
    friend T operator*(const T& t1 , const T& t2)
    {
        T t3( t1 );
        t3 *= t2;
        return t3;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides binary division operator (T operator/(const T& t1 , const T& t2) based on 
///        user-defined autoincrement operator ( T& operator/=(const T& t) ).
///
/// @author	Manu343726
///
/// @details User class must implement T& operator/=(const T& t).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , bool ENABLE_SYMMETRY>
struct dl32DivisionHelper<T,dl32NoType,ENABLE_SYMMETRY>
{
    friend T operator/(const T& t1 , const T& t2)
    {
        T t3( t1 );
        t3 /= t2;
        return t3;
    }
};

template<typename T , typename U = dl32NoType , bool ENABLE_SYMMETRY = false>
using dl32BasicAlgebraHelpers = dl32TypeList<dl32AdditionHelper<T,U,ENABLE_SYMMETRY>,dl32SubstractionHelper<T,U,ENABLE_SYMMETRY>>; ///< Defines the set of basic algebra (Addition and substraction) helpers.

template<typename T , typename U = dl32NoType , bool ENABLE_SYMMETRY = false>
using dl32AlgebraHelpers = dl32TypeList<dl32AdditionHelper<T,U,ENABLE_SYMMETRY>,dl32SubstractionHelper<T,U,ENABLE_SYMMETRY>,dl32MultiplicationHelper<T,U,ENABLE_SYMMETRY>,dl32DivisionHelper<T,U,ENABLE_SYMMETRY>>; ///< Defines the set of algebra (Addition, substraction, multiplication, and division) helpers.

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides the set of basic algebra operators (Addition and substraction) for types
///        T and U.
///
/// @author	Manu343726
///
/// @details User class (T) must implement operator+=(const U&) and operator-=(const U&).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U = dl32NoType , bool ENABLE_SYMMETRY = false>
struct dl32BasicAlgebraHelper : public dl32BasicAlgebraHelpers<T,U,ENABLE_SYMMETRY>::public_inheritance_from_types {}; 

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides the set of algebra operators (Addition, substraction, multiplication,
///        and division) for types T and U.
///
/// @author	Manu343726
///
/// @details User class (T) must implement operator+=(const U&), operator-=(const U&), operator+=(const U&),
///          and operator/=(const U&).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U = dl32NoType , bool ENABLE_SYMMETRY = false>
struct dl32AlgebraHelper : public dl32BasicAlgebraHelpers<T,U,ENABLE_SYMMETRY>::public_inheritance_from_types {}; 

#endif	/* DL32OPERATOROVERLOADINGHELPERS_H */

