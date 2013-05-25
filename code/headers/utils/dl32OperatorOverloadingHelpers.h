/* 
 * File:   dl32OperatorOverloadingHelpers.h
 * Author: Manu343726
 *
 * Created on 25 de mayo de 2013, 13:14
 */

#ifndef DL32OPERATOROVERLOADINGHELPERS_H
#define	DL32OPERATOROVERLOADINGHELPERS_H

#include "dl32Typing.h"

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
/// @brief This helper provides inequelity criteria for type T based on user defined equality criteria 
///        for type T.
///
/// @author	Manu343726
///
/// @details User class must implement operator==(const T& t1 , const T& t2).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct dl32EqualityHelper
{
    bool operator!=(const T& t1 , const T& t2) { return !(t1 == t2); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides inequelity criteria for types T vs U based on user-defined equality criteria
///        for types T vs U.
///        Also, this helper provides the complete set of inverse comparisons to ensure simetry.
///
/// @author	Manu343726
///
/// @details User class must implement operator==(const T& t , const U& u).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U>
struct dl32EqualityHelper
{ 
    bool operator!=(const T& t , const T& u) { return !(t == u); }
    
    bool operator==(const U& u , const T& t) {return t == u;}
    bool operator!=(const U& u , const T& t) { return t != u; }
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
struct dl32ComparisonHelper
{
    bool operator<=(const T& t1 , const T& t2) { return !(t1 > t2); }
    bool operator< (const T& t1 , const T& t2) { return t2 > t1; }
    bool operator>=(const T& t1 , const T& t2) { return !(t1 < t2); }   
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides ccmplete comparison criteria for types T vs U based on user-defined 
///        bigger-than and less-than criteria for types T vs U.
///        Also, this helper provides the complete set of inverse comparisons to ensure simetry.
///
/// @author	Manu343726
///
/// @details User class must implement operator>(const T& t , const U& u) and 
///          operator<(const T& t , const U& u).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U>
struct dl32ComparisonHelper
{
    bool operator<=(const T& t , const U& u) { return !(t > u); }
    bool operator>=(const T& t , const U& u) { return !(t < u); }  
    
    bool operator> (const U& u , const T& t) { return t < u; }
    bool operator<=(const U& u , const T& t) { return !(u > t); }
    bool operator< (const U& u , const T& t) { return t > u; }
    bool operator>=(const U& u , const T& t) { return !(t < u); }
};

template<typename T>
using dl32ComparisonHelpers = dl32TypeList<dl32EqualityHelper<T>,dl32ComparisonHelper<T>>; ///< Defines the set of helpers that provides comparison operators for type T.

template<typename T , typename U>
using dl32ComparisonHelpers = dl32TypeList<dl32EqualityHelper<T,U>,dl32ComparisonHelper<T,U>>; ///< Defines the set of helpers that provides comparison operators for types T and U.

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides the complete comparison operators for a given type T.
///
/// @author	Manu343726
///
/// @details User class must implement operator>(const T& t1 , const T& t2)
///          and operator==(const T& t1 , const T& t2).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct dl32CompleteComparisonHelper : public dl32ComparisonHelpers::public_inheritance_from_types {};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides the complete comparison operators for given types T and U.
///        Also, this helper provides the complete set of inverse comparisons to ensure simetry.
///
/// @author	Manu343726
///
/// @details User class must implement operator>(const T& t , const U& u), operator<(const T& t , const U& u),
///          and operator==(const T& t , const U& u).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U>
struct dl32CompleteComparisonHelper : public dl32ComparisonHelpers::public_inheritance_from_types {};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides binary addition operator (T operator+(const T& t1 , const T& t2) based on 
///        user-defined autoincrement operator ( T& operator+=(const T& t) ).
///
/// @author	Manu343726
///
/// @details User class must implement T& operator+=(const T& t).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct dl32AdditionHelper
{
    T operator+(const T& t1 , const T& t2)
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
template<typename T>
struct dl32SubstractionHelper
{
    T operator-(const T& t1 , const T& t2)
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
template<typename T>
struct dl32MultiplicationHelper
{
    T operator*(const T& t1 , const T& t2)
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
template<typename T>
struct dl32DivisionHelper
{
    T operator/(const T& t1 , const T& t2)
    {
        T t3( t1 );
        t3 /= t2;
        return t3;
    }
};

template<typename T>
using dl32BasicAlgebraHelpers = dl32TypeList<dl32AdditionHelper<T>,dl32SubstractionHelper<T>>; ///< Defines the set of basic algebra (Addition and substraction) helpers.

template<typename T>
using dl32AlgebraHelpers = dl32TypeList<dl32AdditionHelper<T>,dl32SubstractionHelper<T>,dl32MultiplicationHelper<T>,dl32DivisionHelper<T>>; ///< Defines the set of algebra (Addition, substraction, multiplication, and division) helpers.

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides the set of basic algebra operators (Addition and substraction) for a type T.
///
/// @author	Manu343726
///
/// @details User class must implement operator+=(const T&) and operator-=(const T&).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct dl32BasicAlgebraHelper : public dl32BasicAlgebraHelpers::public_inheritance_from_types {}; 

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper provides the set of algebra operators (Addition, substraction, multiplication,
///        and division) for a type T.
///
/// @author	Manu343726
///
/// @details User class must implement operator+=(const T&), operator-=(const T&), operator+=(const T&),
///          and operator/=(const T&).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct dl32AlgebraHelper : public dl32BasicAlgebraHelpers::public_inheritance_from_types {}; 
#endif	/* DL32OPERATOROVERLOADINGHELPERS_H */

