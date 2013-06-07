/* 
 * File:   dl32TypeList.h
 * Author: Manu343726
 *
 * Created on 6 de junio de 2013, 12:08
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

#ifndef DL32TYPELIST_H
#define	DL32TYPELIST_H

#include "dl32TMPCore.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 type list.
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
using dl32EmptyTypeList = dl32TypeList<>;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Implements a linear search of a type in a given typelist.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned int index, typename TYPELIST>
class dl32TypeAt
{
    static_assert( index < TYPELIST::size , "Parameter 'index' is out of bounds" );
private:
    
    //Forward declaration:
    template<int _index, typename _TYPELIST>
    struct _type_at;
    
    //Recursive case:
    template<int _index , typename HEAD , typename... TAIL>
    struct _type_at<_index,dl32TypeList<HEAD,TAIL...>>
    { 
        using value = typename _type_at<_index-1,dl32TypeList<TAIL...>>::value; 
    };
    
    //Base case:
    template<typename HEAD , typename... TAIL>
    struct _type_at<0,dl32TypeList<HEAD,TAIL...>>
    { 
        using value = HEAD; 
    };

public:
    using type = typename _type_at<index,TYPELIST>::value;
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
    template<typename U , typename _TYPELIST>
    struct _index_of;
    
    /* Base-case for typelist end (type U is not finded in TYPELIST) */
    template<typename U>
    struct _index_of<U , dl32EmptyTypeList>
    {
        static const int value = -1;
    };

    /* Base-case for finded case (U is in TYPELIST). Note that the list used in the 
       specialitation is a list with U as head.                                     */
    template<typename U , typename... TAIL>
    struct _index_of<U, dl32TypeList<U,TAIL...>>
    {
        static const int value = 0;
    };

    /* Recoursive-case specialitation */
    template<typename U , typename HEAD , typename... TAIL>
    struct _index_of<U,dl32TypeList<HEAD,TAIL...>>
    {
    private:
        static const int temp = _index_of<U,dl32TypeList<TAIL...>>::value;
    public:
        static const int value = ( temp == -1 ? -1 : temp + 1 ); 
    };
    
public:
    using value = dl32UintWrapper<_index_of<T,TYPELIST>::value>; ///< The index of the type T in the typelist TYPELIST.
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Checks if a type is contained in a given loki-style typelist.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename TYPELIST>
struct dl32Contains
{
    static const bool value = dl32IndexOf<T,TYPELIST>::value::value >= 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Creates a new typelist with the first typelist elements followed by the second typelist elements. 
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename TYPELIST1 , typename TYPELIST2> //http://stackoverflow.com/questions/16648144/merge-two-variadic-templates-in-one
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

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Splits a typelist in two typelists, right and left, from the specified index.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned int index , typename TYPELIST>
class dl32Split
{
    static_assert( index >= 0 && index < TYPELIST::size , "Parameter 'index' is out of bounds" );
private:
    //Forward declaration:
    template<unsigned int destiny , unsigned int current_index , typename LEFT_LIST , typename HEAD , typename... TAIL>
    struct _split;
    
    //Recursive case:
    template<unsigned int destiny , unsigned int current_index , typename... LEFT_TYPES , typename HEAD , typename... TAIL>
    struct _split<destiny , current_index , dl32TypeList<LEFT_TYPES...> , HEAD , TAIL...>
    {
        using next_left  = dl32TypeList<LEFT_TYPES...,HEAD>;
        using next_split = _split<destiny , current_index + 1 , next_left , TAIL...>;
        
        using right      = typename next_split::right;
        using left       = typename next_split::left;    
    };
    
    //Base case:
    template<unsigned int destiny , typename... LEFT_TYPES , typename HEAD , typename... TAIL>
    struct _split<destiny,destiny,dl32TypeList<LEFT_TYPES...>,HEAD,TAIL...>
    {
        using right = dl32TypeList<TAIL...>;
        using left  = dl32TypeList<LEFT_TYPES...,HEAD>;
    };
    
    //dl32TypeList template args extractor (Split helper)
    template<unsigned int _index , typename _TYPELIST>
    struct _splitter;
    
    template<unsigned int _index , typename HEAD , typename... TAIL>
    struct _splitter<_index,dl32TypeList<HEAD,TAIL...>>
    {
        using split = _split<_index,0,dl32EmptyTypeList,HEAD,TAIL...>;
        using right = typename split::right;
        using left  = typename split::left;
    };
    
    using splitter = _splitter<index,TYPELIST>;
public:
    using right = typename splitter::right; ///< Typelist containing types before index (Indexth type included).
    using left  = typename splitter::left;  ///< Typelist containing types after index.
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Checks if two typelists are same.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename TYPELIST1 , typename TYPELIST2>
class dl32SameTypeList
{
private:
    //NOTA: Podría haber utilizado dl32SameType< typename TYPELIST1::type_at<index> , typename TYPELIST2::type_at<index> > recursivamente hasta llegar al final de las listas
    //pero eso tendría un coste O(n^2), y no quiero tardar veinte millones de años en compilar.
    //Además así es mucho más sencillo.
    
    //Forward declaration:
    template<typename _TYPELIST1 , typename _TYPELIST2>
    struct _are_same;
    
    template<typename... Ts>
    struct _are_same<dl32TypeList<Ts...>,dl32TypeList<Ts...>>
    {
        static const bool value = true;
    };
    
    template<typename... Ts , typename... Us>
    struct _are_same<dl32TypeList<Ts...>,dl32TypeList<Us...>>
    {
        static const bool value = false;
    };
public:
    static const bool result = _are_same<TYPELIST1,TYPELIST2>::value; //< True if the two typelists provided are the same. False if not.
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Inserts a set of types in a typelist at the specified position.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned int index  , typename TYPELIST , typename... Ts>
class dl32Insert
{
    static_assert( index >= 0 && index < TYPELIST::size , "Parameter 'index' is out of bounds" );
private:
    template<unsigned int destiny , unsigned int current_index , typename _TYPELIST , typename... LEFT_TYPES>
    struct _insert;
    
    template<unsigned int destiny , unsigned int current_index , typename HEAD , typename... TAIL , typename... LEFT_TYPES>
    struct _insert<destiny , current_index , dl32TypeList<HEAD,TAIL...> , LEFT_TYPES...>
    {
        using result = typename _insert<destiny , current_index+1 , dl32TypeList<TAIL...> , LEFT_TYPES... , HEAD>::result;
    };
    
    template<unsigned int destiny , typename HEAD , typename... TAIL , typename... LEFT_TYPES>
    struct _insert<destiny , destiny , dl32TypeList<HEAD,TAIL...> , LEFT_TYPES...>
    {
        using result = dl32TypeList<LEFT_TYPES...,Ts...,HEAD,TAIL...>;
    };
    
public:
    using result = typename _insert<index,0,TYPELIST,dl32EmptyTypeList>::result; ///< The new typelist as the initial typelist with Ts inserted begining at index.
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Removes the type contained in a specified position of a typelist.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned int index  , typename TYPELIST>
class dl32Remove
{
    static_assert( index >= 0 && index < TYPELIST::size , "Parameter 'index' is out of bounds" );
private:
    template<unsigned int destiny , unsigned int current_index , typename _TYPELIST , typename... LEFT_TYPES>
    struct _remove;
    
    template<unsigned int destiny , unsigned int current_index , typename HEAD , typename... TAIL , typename... LEFT_TYPES>
    struct _remove<destiny , current_index , dl32TypeList<HEAD,TAIL...> , LEFT_TYPES...>
    {
        using result = typename _remove<destiny , current_index+1 , dl32TypeList<TAIL...> , LEFT_TYPES... , HEAD>::result;
    };
    
    template<unsigned int destiny , typename HEAD , typename... TAIL , typename... LEFT_TYPES>
    struct _remove<destiny , destiny , dl32TypeList<HEAD,TAIL...> , LEFT_TYPES...>
    {
        using result = dl32TypeList<LEFT_TYPES...,TAIL...>;
    };
public:
    using result = typename _remove<index,0,TYPELIST,dl32EmptyTypeList>::result; ///< The new typelist as the initial typelist with the type at index erased.
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Replaces the type contained in a specified position of a typelist with an specified type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned int index  , typename TYPELIST , typename T>
class dl32Replace
{
    static_assert( index >= 0 && index < TYPELIST::size , "Parameter 'index' is out of bounds" );
private:
    template<unsigned int destiny , unsigned int current_index , typename _TYPELIST , typename... LEFT_TYPES>
    struct _set;
    
    template<unsigned int destiny , unsigned int current_index , typename HEAD , typename... TAIL , typename... LEFT_TYPES>
    struct _set<destiny , current_index , dl32TypeList<HEAD,TAIL...> , LEFT_TYPES...>
    {
        using result = typename _set<destiny , current_index+1 , dl32TypeList<TAIL...> , LEFT_TYPES... , HEAD>::result;
    };
    
    template<unsigned int destiny , typename HEAD , typename... TAIL , typename... LEFT_TYPES>
    struct _set<destiny , destiny , dl32TypeList<HEAD,TAIL...> , LEFT_TYPES...>
    {
        using result = dl32TypeList<LEFT_TYPES...,T,TAIL...>; //Replaces HEAD with T.
    };
public:
    using result = typename _set<index,0,TYPELIST,dl32EmptyTypeList>::result; ///< The new typelist as the initial typelist with the type at index erased.
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
private:
    using this_list = dl32TypeList<HEAD,TAIL...>;
public:
    static const unsigned int size = sizeof...(TAIL) + 1; ///< Size of the typelist (Number of types stored in).
    
    template<int index>
    using type_at = typename dl32TypeAt<index,this_list>::type; ///< Gets the index-th type of the list. If index is out of range, a compilation error will be generated ("dl32Notype not has member 'head'").
    
    using front = HEAD; ///< Gets the first type stored at the typelist.
    
    template<typename T>
    using index_of = typename dl32IndexOf<T,this_list>::value; ///< Gets the position of a given type in the list. If the type is not in the list, dl32NoType will be returned.
    
    template<typename T>
    using contains = dl32Contains<T,this_list>; ///< Checks if the typelist contains a specified type.
    
    template<typename... Ts>
    using push_back = dl32TypeList<HEAD,TAIL...,Ts...>; ///< Pushes back a new type to the typelist (Returns new typelist).
    
    template<typename... Ts>
    using push_front = dl32TypeList<Ts...,HEAD,TAIL...>; ///< Pushes front a new type to the typelist (Returns new typelist).
    
    template<unsigned int index , typename... Ts>
    using insert = typename dl32Insert<index , this_list , Ts...>::result;
    
    using pop_front = dl32TypeList<TAIL...>; ///< Pops the begining type of the typelist (Returns new typelist).
    
    template <typename TYPELIST>
    using merge = typename dl32Merge<this_list , TYPELIST>::result; ///< Creates a new typelist with this typelist elements followed by the provided typelist elements. 

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Inheriting from this type provides public inheritance from all types contained at the typelist.
    /// @remarks Note that typelist types must be non-basic types, must be inheritable types.
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
  
    static const unsigned int size = 0;
  
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

#endif	/* DL32TYPELIST_H */

