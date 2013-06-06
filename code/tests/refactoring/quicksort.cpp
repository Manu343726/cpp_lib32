/* 
 * File:   quicksort.cpp
 * Author: Manu343726
 *
 * Created on 6 de junio de 2013, 16:22
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

#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_TMP_QUICKSORT

#include "dl32MetaprogrammingLibrary.h"

template<unsigned int... Ns>
using uint_list = dl32TypeList<dl32UintWrapper<Ns>...>;

using empty_uint_list = dl32EmptyTypelist;

template<typename UINT1, typename UINT2>
struct equal           : public dl32BoolWrapper< UINT1::value == UINT2::value > {};
template<typename UINT1, typename UINT2>
struct not_equal       : public dl32BoolWrapper< UINT1::value != UINT2::value > {};
template<typename UINT1, typename UINT2>
struct bigger_than     : public dl32BoolWrapper< (UINT1::value > UINT2::value) > {};
template<typename UINT1, typename UINT2>
struct less_than       : public dl32BoolWrapper< (UINT1::value < UINT2::value) > {};
template<typename UINT1, typename UINT2>
struct bigger_or_equal : public dl32BoolWrapper< UINT1::value >= UINT2::value > {};
template<typename UINT1, typename UINT2>
struct less_or_equal   : public dl32BoolWrapper< UINT1::value <= UINT2::value > {};

template<unsigned int... Ns>
class quicksort
{
private:
    template<unsigned int lenght , typename SUBLIST>
    struct _quicksort;
    
    template<unsigned int... Ns>
    struct _quicksort<0,uint_list<Ns...>>
    {
        using result = empty_uint_list;
    };
    
    template<unsigned int... Ns>
    struct _quicksort<1,uint_list<Ns...>>
    {
        using result = uint_list<Ns...>;
    };
    
    template<unsigned int... Ns>
    struct _quicksort<2,uint_list<Ns...>>
    {
    private:
        static const unsigned int left  = uint_list<Ns...>::type_at<0>::value;
        static const unsigned int right = uint_list<Ns...>::type_at<1>::value;
        
    public:
        using result = dl32tmp_if< bigger_or_equal<left,right>::value , 
                                   uint_list<left,right>,
                                   uint_list<right,left>
                                 >::type;
    };
    
    template<unsigned int lenght , unsigned int... Ns>
    struct _quicksort<lenght,uint_list<Ns...>>
    {
    private:
        template<unsigned int PIVOT , typename RIGHT , typename LEFT , unsigned int... Ns>
        struct _reorder_sublists;
        
        template<unsigned int PIVOT , typename RIGHT , typename LEFT , unsigned int HEAD , unsigned int... TAIL>
        struct _reorder_sublists
        {
            using next_reorder = typename dl32tmp_if< bigger_or_equal<PIVOT,HEAD::value>::value ,
                                                      _reorder_sublists<PIVOT,RIGHT::push_back<dl32UintWrapper<HEAD>>,LEFT,TAIL...>,
                                                      _reorder_sublists<PIVOT,RIGHT,LEFT::push_front<dl32UintWrapper<HEAD>>,TAIL...>
                                                    >::type;
            
            using right = typename next_reorder::right;
            using left  = typename next_reorder::left;
        };
        
        template<unsigned int PIVOT , typename RIGHT , typename LEFT>
        struct _reorder_sublists<PIVOT,RIGHT,LEFT>
        {
            using right = RIGHT;
            using left  = LEFT;
        };
        
        template<unsigned int PIVOT>
        using _right_sublist = typename _reorder_sublists<PIVOT,empty_uint_list,empty_uint_list,Ns...>::right;
        template<unsigned int PIVOT>
        using _left_sublist  = typename _reorder_sublists<PIVOT,empty_uint_list,empty_uint_list,Ns...>::left;
    private:
        static const unsigned int _half_size   = sizeof...(Ns)/2;
        static const unsigned int _pivot_index = _half_size;
        static const unsigned int _pivot       = uint_list<Ns...>::type_at<_pivot_index>::value;
        
        using _right = _right_sublist<_pivot>;
        using _left  = _left_sublist<_pivot>;
        
        using result = typename dl32Merge< typename _quicksort< _half_size , _left >::result , typename _quicksort< _half_size , _right > >::result;
    };
    
    
    using _input_list = uint_list<Ns...>;
    using _result = typename _quicksort<_input_list::size,_input_list>::result;
public:
    using result = _result;
};
#endif /* CURRENT_TEST */

