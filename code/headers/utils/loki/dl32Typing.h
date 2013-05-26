/* 
 * File:   dl32Typing.h
 * Author: Manu343726
 *
 * Created on 27 de abril de 2013, 21:13
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

#ifndef DL32TYPING_H
#define	DL32TYPING_H

#include "dl32Config.h"

//Nota: Soy plenamente consciente de que todo ésto (O por lo menos la gran mayoría) está implementado desde C++11 en la STL ( http://en.cppreference.com/w/cpp/types ), lo hago por mero entretenimiento.

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
    typedef T type;
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

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 type checking basic data.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template< typename T , typename U>
class dl32TypeCheckingHelper
{
public:
    ///< Small size unit type.
    typedef char dl32TypeSize_Small;

    ///< Big size unit type.
    typedef class { char _dummy[2]; } dl32TypeSize_Big;
    
    ///< Compile-time size units types size.
    enum
    {
        _sizeof_Small = sizeof( dl32TypeSize_Small ),
        _sizeof_Big   = sizeof( dl32TypeSize_Big )
    };

protected:    
    static dl32TypeSize_Small _testTtoU( U );   //U args calls use this.
    static dl32TypeSize_Big   _testTtoU( ... ); //Non-U args calls use this.
    static dl32TypeSize_Small _testUtoT( T );   //T args calls use this.
    static dl32TypeSize_Big   _testUtoT( ... ); //Non-T args calls use this.
    
    static T _makeT();
    static U _makeU();
};

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
    static const bool result = _same_type<T,U>::result;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 implicit type conversion checker.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U>
struct dl32Conversion : public dl32TypeCheckingHelper<T,U>
{
    enum { result = sizeof( dl32TypeCheckingHelper<T,U>::_testTtoU( dl32TypeCheckingHelper<T,U>::_makeT() ) ) == dl32TypeCheckingHelper<T,U>::_sizeof_Small };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 inheritance checker.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U>
struct dl32SuperclassSubclass
{
    enum { result = dl32Conversion<const U* , const T*>::result && !dl32SameType<const T* , const void*>::result && !dl32SameType<const T , const U>::result };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 compile-time type data.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U>
class dl32TypeChecking
{
public:
    enum
    { 
        //Implicit type-conversion checking:
        conversion_exists = dl32Conversion<T,U>::result, ///< Checks if a conversion from T to U is alowed. Its value is 1 (TRUE) if a implicit conversion is alowed). 0 (FALSE) if is not alowed.)
        
        //Basic type-checking:
        same_type = dl32SameType<T,U>::result, ///< Checks if the two types provided are the same type.
        
        //Inheritance checking:
        superclass_subclass = dl32SuperclassSubclass<T,U>::result ///< Checks if T is a superclass of U (U is a subclass of T).
    };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 type selection.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<bool flag , typename T , typename U>
class dl32Select
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

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Implementation of a loki-like (C++ 98/03 style) type list
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename HEAD ,typename TAIL>
class dl32Loki_like_TypeList 
{
public:
  typedef HEAD head;
  typedef TAIL tail;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Pushes back a type in a loki-style typelist.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename LOKI_STYLE_TYPELIST , typename T>
struct dl32LokiStyle_pushback
{
private:
    template<typename TYPELIST , typename U>
    struct _push_back;
    
    template<typename U>
    struct _push_back<dl32NoType , U>
    {
        using result = dl32Loki_like_TypeList<U,dl32NoType>;
    };
    
    template<typename HEAD , typename TAIL>
    struct _push_back<dl32NoType , dl32Loki_like_TypeList<HEAD , TAIL>>
    {
        using result = dl32Loki_like_TypeList<HEAD,TAIL>;
    };
    
    template<typename HEAD,typename TAIL , typename U>
    struct _push_back<dl32Loki_like_TypeList<HEAD,TAIL> , U>
    {
        using result = dl32Loki_like_TypeList<HEAD , typename _push_back<TAIL , U>::result>;
    };
    
public:
    using result = typename _push_back<LOKI_STYLE_TYPELIST , T>::result;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 loki-like typelist wrapper
/// @details This class wrapps recursive-based typelist and provides a variadic template interface
///
/// @author	Manu343726
///
/// @remarks Forward declaration (Needed for dl32IndexOf<...> and others).
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename... Ts>
struct dl32TypeList;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This alias represents an empty dl32TypeList.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
using dl32EmptyTypelist = dl32TypeList<>;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Implements a linear search of a type in a given type list (Type list in loki-format).
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<int index, typename TYPELIST>
class dl32TypeAt
{
private:
    template<int _index, typename _TYPELIST>
    struct _type_at{ using value = typename _type_at<_index-1,_TYPELIST>::value; };
    
    template<typename _TYPELIST>
    struct _type_at<0,_TYPELIST>{ using value = typename _TYPELIST::head; };

public:
    using value = typename _type_at<index,TYPELIST>::value;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Gets the index of a given type in a given typelist.
/// @details Member "value" is a integer containing the index of T in TYPELIST. 
///          If TYPELIST not contains T, value is -1.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename TYPELIST>
struct dl32IndexOf
{
private:
    /* Forward declaration */
    template<typename U , typename LOKI_TYPELIST>
    struct _index_of;
    
    /* Base-case for typelist end (type U is not finded in TYPELIST) */
    template<typename U>
    struct _index_of<U , dl32NoType>
    {
        enum { value = -1 };
    };

    /* Base-case for finded case (U is in TYPELIST). Note that the list used in the 
       specialitation is a list with U as head and TYPELIST::tail as tail.       */
    template<typename U , typename TAIL>
    struct _index_of<U, dl32Loki_like_TypeList<U,TAIL>>
    {
        enum { value = 0 };
    };

    /* Recoursive-case specialitation */
    template<typename U , typename HEAD , typename TAIL>
    struct _index_of<U,dl32Loki_like_TypeList<HEAD,TAIL>>
    {
    private:
        enum { temp = _index_of<U,TAIL>::value };
    public:
        enum { value = ( temp == -1 ? -1 : temp + 1 ) }; 
    };
    
public:
    typedef dl32ValueWrapper<int, _index_of<T,TYPELIST>::value> value; ///< The index of the type T in the typelist TYPELIST.
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Checks if a type is contained in a given loki-style typelist.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename TYPELIST>
struct dl32Contains
{
    enum { value = dl32IndexOf<T,TYPELIST>::value >= 0 };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Creates a new typelist with the first typelist elements followed by the second typelist elements. 
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename TYPELIST1 , typename TYPELIST2>
class dl32Merge
{
private:
    template<typename TYPELIST_1 , typename TYPELIST_2>
    struct _merge;
    
    template<typename... TYPES_1 , typename... TYPES_2>
    struct _merge< dl32TypeList<TYPES_1...> , dl32TypeList<TYPES_2...> >
    {
        using result = dl32TypeList<TYPES_1... , TYPES_2...>;
    };
    
public:
    using result = typename _merge<TYPELIST1,TYPELIST2>::result;
};

template<typename TYPELIST , unsigned int index>
class dl32Split
{
private:
    template<typename LOKI_STYLE_TYPELIST>
    struct _split;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 loki-like typelist wrapper
/// @details This class wrapps recursive-based typelist and provides a variadic template interface
///
/// @author	Manu343726
///
/// @remarks Recoursive-case template specialitation.
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename HEAD , typename... TAIL>
struct dl32TypeList<HEAD,TAIL...>
{
    enum { size = sizeof...(TAIL) + 1 }; ///< Size of the typelist (Number of types stored in).
    typedef dl32Loki_like_TypeList<HEAD, typename dl32TypeList<TAIL...>::value> value; ///< The typelist in loki-style (The unwrapped value)
    
    template<int index>
    using type_at = typename dl32TypeAt<index,value>::value; ///< Gets the index-th type of the list. If index is out of range, a compilation error will be generated ("dl32Notype not has member 'head'").
    
    template<typename T>
    using index_of = typename dl32IndexOf<T,value>::value; ///< Gets the position of a given type in the list. If the type is not in the list, dl32NoType will be returned.
    
    template<typename T>
    using contains = dl32Contains<T,dl32TypeList<HEAD,TAIL...>>; ///< Checks if the typelist contains a specified type.
    
    template<typename T>
    using push_back = dl32TypeList<HEAD,TAIL...,T>; ///< Pushes back a new type to the typelist (Returns new typelist).
    
    template<typename T>
    using push_front = dl32TypeList<T,HEAD,TAIL...>; ///< Pushes front a new type to the typelist (Returns new typelist).
    
    using pop_front = dl32TypeList<TAIL...>; ///< Pops the begining type of the typelist (Returns new typelist).
    
    template <typename TYPELIST>
    using merge = typename dl32Merge<dl32TypeList<HEAD,TAIL...> , TYPELIST>::result; ///< Creates a new typelist with this typelist elements followed by the provided typelist elements. 

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Inheriting type provides public inheritance from all types contained at the typelist.
    /// @details Note that typelist types must be non-basic types, must be inheritable types.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    struct public_inheritance_from_types : public HEAD , public TAIL... {};
};
    
///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 loki-like typelist wrapper
/// @details This class wrapps recursive-based typelist and provides a variadic template interface
///
/// @author	Manu343726
///
/// @remarks Base-case template specialitation.
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct dl32TypeList<>
{
  typedef dl32NoType value;
  
  enum { size = 0 };
  
  template<int index>
  using type_at = dl32NoType; ///< Gets the index-th type of the list. If index is out of range, a compilation error will be generated ("dl32Notype not has member 'head'").
  
  template<typename T>
  using index_of = typename dl32IndexOf<T,dl32NoType>::value; ///< Gets the position of a given type in the list. If the type is not in the list, dl32NoType will be returned.

  template<typename T>
  using push_back = dl32TypeList<T>; ///< Pushes back a new type to the typelist (Returns new typelist).
  
  template<typename T>
  using push_front = dl32TypeList<T>; ///< Pushes front a new type to the typelist (Returns new typelist).
  
  template<typename T , unsigned int index>
  using insert = typename dl32EnableIf<index == 0 , dl32TypeList<T>>::type; ///< Inserts a new type in the typelist at the specified position (Returns a new typelist).
  
  template <typename TYPELIST>
  using merge = TYPELIST; ///< Creates a new typelist with this typelist elements followed by the provided typelist elements. 
};

///< List of C++ integral types.
using dl32IntegralTypes = dl32TypeList<unsigned char , char , unsigned int , int , unsigned long int , long int>;

///< List of C++ floating-point types.
using dl32FloatingPointTypes = dl32TypeList<float , double , long double>;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class returns a type T without the const specifier (Equivalent to static_cast<T>(const T)).
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class dl32WithoutConst
{
private:
    template<typename _T>
    struct _remove_const{ using result = _T; };
    
    template<typename _T>
    struct _remove_const<const _T>{ using result = _T; };
public:
    using result = typename _remove_const<T>::result;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Type info provider.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class dl32TypeTraits
{
private:
#define TRAITS( name , type , checkingType , typedefName ) \
    template<typename type>                                \
    struct name                                            \
    {                                                      \
        enum { value = FALSE };                            \
        typedef dl32NoType typedefName;                    \
    };                                                     \
    template<typename type>                                \
    struct name<checkingType>                              \
    {                                                      \
        enum { value = TRUE };                             \
        typedef type typedefName;                          \
    };                        
    
#define TRAITS_FULL_EXPLICITTRAITNAME(attribute_name , trait_name , type , checking_type , typedef_name ) \
        TRAITS( trait_name , type , checking_type , typedef_name );                                       \
        public:                                                                                           \
                enum { attribute_name = trait_name<T>::value };                                           \
                typedef typename trait_name<T>::typedef_name typedef_name;                                \
        private:
    
#define TRAITS_FULL_IMPLICITTRAITNAME( attribute_name , unique_id , type , checking_type , typedef_name ) TRAITS_FULL_EXPLICITTRAITNAME(attribute_name , CONCAT( _trait_ , unique_id ) , type , checking_type , typedef_name )
    
#define TRAITS_FULL( attribute_name , type , checking_type , typedef_name ) TRAITS_FULL_IMPLICITTRAITNAME( attribute_name , __COUNTER__ /* unique class identifier */ , type , checking_type , typedef_name )
    
    TRAITS_FULL( isPointer   , U , U*      , PointeeType )    //Checks if T is a pointer
    TRAITS_FULL( isReference , U , U&      , ReferencedType ) //Checks if T is a reference
    TRAITS_FULL( hasConst    , U , const U , NonConstType )   //Checks if T is const
    TRAITS_FULL( isRvalue    , U , U&&     , MovedType )      //Checks if T is a rvalue
 
public:
    typedef T OriginalType;
    
    static const bool isVoid = dl32SameType<void , NonConstType>::result; ///< Checks if T is void.
    static const bool isFloatingPoint = dl32FloatingPointTypes::contains<typename dl32WithoutConst<T>::result>::value; ///< Checks if T is a floating-point type.
    static const bool isIntegral      = dl32IntegralTypes     ::contains<typename dl32WithoutConst<T>::result>::value; ///< Checks if T is a integral type.
};
#endif	/* DL32TYPING_H */

