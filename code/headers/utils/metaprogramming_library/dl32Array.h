/* 
 * File:   dl32Array.h
 * Author: Manu343726
 *
 * Created on 18 de junio de 2013, 19:39
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

#ifndef DL32ARRAY_H
#define	DL32ARRAY_H

#include "dl32TMPCore.h"
#include "dl32TypeList.h"
#include "dl32TypeTraits.h"
#include "dl32OperatorOverloadingHelpers.h"
#include "dl32FloatingPointHelper.h"

#include "dl32Exceptions.h"

#include <string>

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 n-dimensional static array
///
/// @author	Manu343726
///
/// @tparam T Type of array elements.
/// @tparam LENGHTS Lenght of every array dimension. 
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , unsigned int... LENGHTS>
class dl32Array;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 n-dimensional static array (One dimension specialization)
///
/// @author	Manu343726
///
/// @tparam T Type of array elements.
/// @tparam LENGHTS Lenght of every array dimension. 
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , unsigned int PRIMARY_DIMENSION_LENGHT>
class dl32Array<T,PRIMARY_DIMENSION_LENGHT>
{
private:
    using _access_return_type = T;
    using _value_comparer_type = typename dl32tmp_if<dl32IsFloatingPointType<T>::value,typename dl32FloatingPointHelper<T>::equals_comparer,std::equal_to<T>>::type;

public:
    static const unsigned int dimensions_count = 1; ///< The number of dimensions of the array.
    using array_type = typename dl32MakeArray<_access_return_type,PRIMARY_DIMENSION_LENGHT>::type; ///< Defines the type of the underlying array.
    using value_type = T; ///< Type of array values.
    using element_type = _access_return_type; ///< Type of array elements;
 
protected:
    array_type _array;
    _value_comparer_type _comparer;
    
public:
    
    dl32Array(const std::initializer_list<_access_return_type>& init_list)
    {
        if(init_list.size() > PRIMARY_DIMENSION_LENGHT) throw dl32OutOfRangeException(PRIMARY_DIMENSION_LENGHT,init_list.size());
        
        std::copy(init_list.begin() , init_list.end() , _array);
    }
    
    const _access_return_type& operator[](unsigned int index) const { return _array[index]; } //Read
          _access_return_type& operator[](unsigned int index)       { return _array[index]; } //Write
    
    bool operator==(const dl32Array<T,PRIMARY_DIMENSION_LENGHT>& other) const
    {
        bool result = true;
        
        for(unsigned int i = 0 ; i < PRIMARY_DIMENSION_LENGHT ; ++i)
        {
            result &= _comparer(_array[i] , other._array[i]);
            
            if(!result) return false;
        }
        
        return true;
    }
    
    bool operator!=(const dl32Array<T,PRIMARY_DIMENSION_LENGHT>& other) const {return ( (*this) == other ); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 n-dimensional static array (n dimensions specialization)
///
/// @author	Manu343726
///
/// @tparam T Type of array elements.
/// @tparam LENGHTS Lenght of every array dimension. 
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , unsigned int PRIMARY_DIMENSION_LENGHT , unsigned int... NEXT_LENGHTS>
class dl32Array<T,PRIMARY_DIMENSION_LENGHT,NEXT_LENGHTS...>
{
private:
    using _access_return_type = typename dl32Array<T,NEXT_LENGHTS...>::array_type;

public:
    static const unsigned int dimensions_count = sizeof...(NEXT_LENGHTS) + 1; ///< The number of dimensions of the array.
    using lenghts    = dl32TypeList<dl32UintWrapper<PRIMARY_DIMENSION_LENGHT>,dl32UintWrapper<NEXT_LENGHTS>...>; ///< List of every array-dimension lenghts.
    using array_type = typename dl32MakeArray<_access_return_type,PRIMARY_DIMENSION_LENGHT>::type; ///< Defines the type of the underlying array.
    using value_type = T; ///< Type of array values.
    using element_type = _access_return_type; ///< Type of array elements;
 
protected:
    array_type _array;
        
public:
    dl32Array() {}
    
    dl32Array(const std::initializer_list<_access_return_type>& init_list)
    {
        if(init_list.size() > PRIMARY_DIMENSION_LENGHT) throw dl32OutOfRangeException(PRIMARY_DIMENSION_LENGHT,init_list.size());
        
        std::copy(init_list.begin() , init_list.end() , _array);
    }
    
    const _access_return_type& operator[](unsigned int index) const { return _array[index]; } //Read
          _access_return_type& operator[](unsigned int index)       { return _array[index]; } //Write
    
    bool operator==(const dl32Array<T,PRIMARY_DIMENSION_LENGHT,NEXT_LENGHTS...>& other)
    {
        bool result = true;
        
        for(unsigned int i = 0 ; i < PRIMARY_DIMENSION_LENGHT ; ++i)
        {
            result &= _array[i] == other._array[i];//Se que está mal. Es provisional
            
            if(!result) return false;
        }
        
        return true;
    }
    
    bool operator!=(const dl32Array<T,PRIMARY_DIMENSION_LENGHT>& other) const {return ( (*this) == other ); }
};

#endif	/* DL32ARRAY_H */

