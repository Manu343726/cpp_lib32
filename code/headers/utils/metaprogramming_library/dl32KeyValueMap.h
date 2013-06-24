/* 
 * File:   dl32KeyValueMap.h
 * Author: Manu343726
 *
 * Created on 17 de junio de 2013, 23:55
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
 * You should have received a copy of the GNU Lesser General Public License     *
 * along with cpp_lib32 project. If not, see <http://www.gnu.org/licenses/>.    *
 *******************************************************************************/

#ifndef DL32KEYVALUEMAP_H
#define	DL32KEYVALUEMAP_H

#include "dl32TMPCore.h"
#include "dl32TypeList.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 compile-time key-value pair
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename KEY , typename VALUE>
struct dl32KeyValuePair
{
    using key = KEY;
    using value = VALUE;
};

template<typename KEY_TYPE , KEY_TYPE KEY , typename VALUE_TYPE , VALUE_TYPE VALUE>
using dl32KeyValuePairOfWrappers = dl32KeyValuePair<dl32ValueWrapper<KEY_TYPE,KEY>,dl32ValueWrapper<VALUE_TYPE,VALUE>>; ///< An alias to key-value pairs wich keys and values are compile-time value wrappers.

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 compile-time key-value map (Hashmap-like container) [FORWARD DECLARATION]
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename... PAIRS>
struct dl32KeyValueMap;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 compile-time key-value map (Hashmap-like container)
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename... KEYS , typename... VALUES>
struct dl32KeyValueMap<dl32KeyValuePair<KEYS,VALUES>...>
{
private:
    using _keys   = dl32TypeList<KEYS...>;
    using _values = dl32TypeList<VALUES...>;
    using _pairs  = dl32TypeList<dl32KeyValuePair<KEYS,VALUES>...>;
    
    template<typename PAIRS_LIST>
    struct _from_list_to_map;
    
    template<typename... PAIRS>
    struct _from_list_to_map<dl32TypeList<PAIRS...>>
    {
        using result = dl32KeyValueMap<PAIRS...>;
    };
    
    template<typename PAIR>
    struct _set_pair;
    
    template<typename KEY , typename VALUE>
    struct _set_pair<dl32KeyValuePair<KEY,VALUE>>
    {
        static const int index_of_key = dl32IndexOf<KEY,_keys>::value;
        
        using push_back_result = dl32TypeList<dl32KeyValuePair<KEYS,VALUES>...,dl32KeyValuePair<KEY,VALUE>>;
        using result = typename _from_list_to_map<typename dl32tmp_if< index_of_key >= 0 , typename dl32Replace<index_of_key,_pairs,dl32KeyValuePair<KEY,VALUE>>::result , push_back_result >::type>::result;
    };
    
    template<typename KEY>
    struct _erase_key
    {
        static const int index_of_key = dl32IndexOf<KEY,_keys>::value;
        using result = typename _from_list_to_map<typename dl32tmp_if< index_of_key >= 0 , typename dl32Remove<index_of_key , _pairs>::result , _pairs>::type>::result;
    };
    
    template<typename KEY>
    struct _get_pair
    {
        static_assert( dl32Contains<KEY,_keys>::value , "The map not contains a pair with that key" );
        static const int index_of_key = dl32IndexOf<KEY,_keys>::value;
        
        using result = typename dl32TypeAt<index_of_key,_pairs>::type;
    };
    
public:
    template<typename KEY_VALUE_PAIR>
    using set_pair = typename _set_pair<KEY_VALUE_PAIR>::result; ///< Stores a key-value pair on the map. If the map already has that key, the value is replaced.
    
    template<typename KEY>
    using get_pair = typename _get_pair<KEY>::result; ///< Gets a key-value pair associated with the passed key. If that key is not used in the map, the operation fails.
    
    template<typename KEY>
    using get_value = typename _get_pair<KEY>::result::value; ///< Gets the value associated with the passed key. If that key is not used in the map, the operation fails.
};

#endif	/* DL32KEYVALUEMAP_H */

