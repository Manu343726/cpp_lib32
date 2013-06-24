/* 
 * File:   dl32TypeTraits.h
 * Author: Manu343726
 *
 * Created on 6 de junio de 2013, 12:26
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

#ifndef DL32TYPETRAITS_H
#define	DL32TYPETRAITS_H

#include "dl32TypeList.h"
#include "dl32TMPCore.h"

namespace _dl32 /* cpp_lib32 private namespace (Implementation-only types defined here) */
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief cpp_lib32 type checking helper utils.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    template< typename T , typename U>
    class dl32TypeCheckingHelper
    {
    private:
        class _dummy_bigger_than_char_class { char _dummy[2]; };
    public:
        ///< Small size unit type.
        using dl32TypeSize_Small = char;

        ///< Big size unit type.
        using dl32TypeSize_Big = _dummy_bigger_than_char_class;

        //Compile-time constants with sizeof of size units:
        static const unsigned int _sizeof_Small = sizeof( dl32TypeSize_Small ); ///< Small size unit
        static const unsigned int _sizeof_Big   = sizeof( dl32TypeSize_Big ); ///< Big size unit

        static dl32TypeSize_Small _testTtoU( U );   //U args calls use this.
        static dl32TypeSize_Big   _testTtoU( ... ); //Non-U args calls use this.
        static dl32TypeSize_Small _testUtoT( T );   //T args calls use this.
        static dl32TypeSize_Big   _testUtoT( ... ); //Non-T args calls use this.

        //Helper functions (Not implemented, used only for type retrieval)
        static T _makeT();
        static U _makeU();
    };

}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 type comparator. 
/// @detailts Use dl32SameType<T,U>::value to check if T and U are the same type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U>
class dl32SameType
{
private:
    template<typename _T , typename _U>
    struct _same_type { static const bool result = false; };
    
    template<typename _T>
    struct _same_type<_T,_T> { static const bool result = true; };
    
public:
    static const bool value = _same_type<T,U>::result;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper removes the const specifier from a type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class dl32WithoutConst
{
private:    
    template<typename _T>
    struct _without_const
    {
        using type = _T;
    };
    
    template<typename _T>
    struct _without_const<const _T>
    {
        using type = _T;
    };
    
public:
    using result = typename _without_const<T>::type; ///< The type T without the const specifier. If T has not const specifier, this type is T.
    static const bool has_const = !dl32SameType<T,result>::value; ///< True if T has const specifier. False if not.
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This helper removes the volatile specifier from a type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class dl32WithoutVolatile
{
private:
    template<typename _T>
    struct _without_volatile
    {
        using type = _T;
    };
    
    template<typename _T>
    struct _without_volatile<volatile _T>
    {
        using type = _T;
    };
    
public:
    using result = typename _without_volatile<T>::type; ///< The type T without the volatile specifier. If T has not volatile specifier, this type is T.
    static const bool has_volatile = !dl32SameType<T,result>::value; ///< True if T has volatile specifier. False if not.
};

template<typename T>
using dl32WithoutConstVolatile = typename dl32WithoutConst<typename dl32WithoutVolatile<T>::result>::result; ///< An alias for removing const and volatile specifier from a type.

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class checks if a provided type T is a pointer type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class dl32IsPointer
{
private:
    template<typename _T>
    struct _is_pointer
    {
        static const bool value = false;
        using pointee_type = _T;
    };
    
    template<typename _T>
    struct _is_pointer<_T*>
    {
        static const bool value = true;
        using pointee_type = _T;
    };
    
public:
    static const bool value = _is_pointer<dl32WithoutConstVolatile<T>>::value; ///< True if T is a pointer type. False if not.
    using PointeeType = typename _is_pointer<dl32WithoutConstVolatile<T>>::pointee_type; ///< Gets the type that T points to. If T is not a pointer, PointeeType is T.
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class checks if a provided type T is a reference type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class dl32IsReference
{
private:
    template<typename _T>
    struct _is_reference
    {
        static const bool value = false;
        using referenced_type = _T;
    };
    
    template<typename _T>
    struct _is_reference<_T&>
    {
        static const bool value = true;
        using referenced_type = _T;
    };
    
public:
    static const bool value = _is_reference<dl32WithoutConstVolatile<T>>::value; ///< True if T is a reference type. False if not.
    using ReferencedType = typename _is_reference<dl32WithoutConstVolatile<T>>::referenced_type; ///< Gets the type that T references to. If T is not a reference, ReferencedType is T.
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class checks if a provided type T is an static (non-heap allocated) array type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class dl32IsArray
{
public:
    static const unsigned int UNESPECIFIED_ARRAY_LENGHT = 0;
private:
    template<typename _T>
    struct _is_array
    {
        static const bool value = false;
        static const int lenght = UNESPECIFIED_ARRAY_LENGHT;
        using type = _T;
    };
    
    template<typename _T>
    struct _is_array<_T[]>
    {
        static const bool value = true;
        static const int lenght = UNESPECIFIED_ARRAY_LENGHT;
        using type = _T;
    };
    
    template<unsigned int LENGHT , typename _T>
    struct _is_array<_T[LENGHT]>
    {
        static const bool value = true;
        static const int lenght = LENGHT;
        using type = _T;
    };
    
public:
    static const bool value = _is_array<T>::value; ///< True if T is an static array type. False if not.
    static const unsigned int lenght = _is_array<T>::lenght; ///< Gets the lenght of the array. Y the lenght is unespecified (T is declared as U[]) ot if T is not an array type, its value is UNESPECIFIED_ARRAY_LENGHT.  
    using elements_type = typename _is_array<T>::type; //< Gets the type of the elements of the array. If T is not an array, elements_type is T.
};

///< List of C++ integral types.
using dl32IntegralTypes = dl32TypeList<unsigned char , char , unsigned int , int , unsigned long int , long int>;

///< List of C++ floating-point types.
using dl32FloatingPointTypes = dl32TypeList<float , double , long double>;

///< List of C++ basic types.
using dl32BasicTypes = typename dl32Merge<dl32IntegralTypes::push_back<void,bool> , dl32FloatingPointTypes>::result;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class checks if a provided type T is a floating-point type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct dl32IsFloatingPointType
{
    static const bool value = dl32FloatingPointTypes::contains<dl32WithoutConstVolatile<T>>::value; ///< True if T is a floating-point type. False if not.
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class checks if a provided type T is an integral type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct dl32IsIntegralType
{
    static const bool value = dl32IntegralTypes::contains<dl32WithoutConstVolatile<T>>::value; ///< True if T is an integral type. False if not.
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class checks if a provided type T is a C++ basic type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct dl32IsBasicType
{
    static const bool value = dl32BasicTypes::contains<dl32WithoutConstVolatile<T>>::value; ///< True if T is a basic type. False if not.
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class checks if a provided type T is a class.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class dl32IsClass
{
private:
    template<typename U> static typename _dl32::dl32TypeCheckingHelper<T,T>::dl32TypeSize_Small _checker(void(U::*)());
    template<typename U> static typename _dl32::dl32TypeCheckingHelper<T,T>::dl32TypeSize_Big   _checker(...);
public:
    static const bool value = sizeof ( _checker<T>(nullptr)) == _dl32::dl32TypeCheckingHelper<T,T>::_sizeof_Small;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 implicit type cast checker.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U>
struct dl32ImplicitCast
{
    static const bool result = sizeof( _dl32::dl32TypeCheckingHelper<T,U>::_testTtoU( _dl32::dl32TypeCheckingHelper<T,U>::_makeT() ) ) == _dl32::dl32TypeCheckingHelper<T,U>::_sizeof_Small;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 inheritance checker.
///
/// @author	Manu343726
///
/// @tparam T superclass
/// @tparam U subclass
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U>
struct dl32HaveInheritance
{
    static_assert( dl32IsClass<T>::value && dl32IsClass<U>::value , "Template parameters T and U must be classes");
    
    static const bool result = dl32ImplicitCast<const U* , const T*>::result && !dl32SameType<const T* , const void*>::value && !dl32SameType<const T , const U>::value;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Generates an array of the specified type with a specified lenght.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , unsigned int lenght>
struct dl32MakeArray : public dl32TypeWrapper<T[lenght]> {};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Makes a reference type of the original type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct dl32MakeReference : public dl32TypeWrapper<T&> {};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Makes a rvalue reference type of the original type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct dl32MakeRValue : public dl32TypeWrapper<T&&> {};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Makes a pointer type of the original type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct dl32MakePointer : public dl32TypeWrapper<T*> {};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Makes a const type of the original type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct dl32MakeConst : public dl32TypeWrapper<const T> {};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Makes a volatile type of the original type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct dl32MakeVolatile : public dl32TypeWrapper<volatile T> {};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Makes an array type of the original type if a boolean condition is true. If its false,
///        makes the same type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<bool CONDITION , typename T , unsigned int SIZE>
using dl32MakeArrayIf = typename dl32SelectType<CONDITION,typename dl32MakeArray<T,SIZE>::type,T>::type;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Makes a reference type of the original type if a boolean condition is true. If its false,
///        makes the same type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<bool CONDITION , typename T>
using dl32MakeReferenceIf = typename dl32SelectType<CONDITION,typename dl32MakeReference<T>::type,T>::type;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Makes a rvalue reference type of the original type if a boolean condition is true. If its false,
///        makes the same type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<bool CONDITION , typename T>
using dl32MakeRValueIf = typename dl32SelectType<CONDITION,typename dl32MakeRValue<T>::type,T>::type;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Makes a pointer type of the original type if a boolean condition is true. If its false,
///        makes the same type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<bool CONDITION , typename T>
using dl32MakePointerIf = typename dl32SelectType<CONDITION,typename dl32MakePointer<T>::type,T>::type;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Makes a const type of the original type if a boolean condition is true. If its false,
///        makes the same type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<bool CONDITION , typename T>
using dl32MakeConstIf = typename dl32SelectType<CONDITION,typename dl32MakeConst<T>::type,T>::type;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Makes a volatile type of the original type if a boolean condition is true. If its false,
///        makes the same type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<bool CONDITION , typename T>
using dl32MakeVolatileIf = typename dl32SelectType<CONDITION,typename dl32MakeVolatile<T>::type,T>::type;
#endif	/* DL32TYPETRAITS_H */

