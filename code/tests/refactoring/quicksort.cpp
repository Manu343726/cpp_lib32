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

#include <iostream>
using namespace std;

/* quicksort sorts lists of unsigned ints */
template<unsigned int... Ns>
using uint_list = dl32TypeList<dl32UintWrapper<Ns>...>;
using empty_uint_list = dl32EmptyTypeList;

/* set of unsigned int comparers */
template<typename UINT1, typename UINT2>
struct bigger_than     : public dl32BoolWrapper< (UINT1::value > UINT2::value) > {};
template<typename UINT1, typename UINT2>
struct less_than       : public dl32BoolWrapper< (UINT1::value < UINT2::value) > {};
template<typename UINT1, typename UINT2>
struct bigger_or_equal : public dl32BoolWrapper< UINT1::value >= UINT2::value > {};
template<typename UINT1, typename UINT2>
struct less_or_equal   : public dl32BoolWrapper< UINT1::value <= UINT2::value > {};


/* Comparer wrappers for generic quicksort implementation */
struct bigger_than_comparer
{
    template<typename UINT1 , typename UINT2>
    using comparer = bigger_than<UINT1,UINT2>;
};

struct less_than_comparer
{
    template<typename UINT1 , typename UINT2>
    using comparer = less_than<UINT1,UINT2>;
};

struct bigger_or_equal_comparer
{
    template<typename UINT1 , typename UINT2>
    using comparer = bigger_or_equal<UINT1,UINT2>;
};

struct less_or_equal_comparer
{
    template<typename UINT1 , typename UINT2>
    using comparer = less_or_equal<UINT1,UINT2>;
};


/* Compile-time quicksort implementation */
template<typename UINT_LIST , template <typename UINT1, typename UINT2> class COMPARER = bigger_than_comparer>
class quicksort
{
private:
    //Forward declaration:
    template<unsigned int lenght , typename SUBLIST>
    struct _quicksort;
    
    //Base-case for empty sublists:
    template<typename... Ns>
    struct _quicksort<0,dl32TypeList<Ns...>>
    {
        using result = empty_uint_list;
    };
    
    //Base case for one element sublists:
    template<typename UINT_WRAPPER>
    struct _quicksort<1,dl32TypeList<UINT_WRAPPER>>
    {
        using result = dl32TypeList<UINT_WRAPPER>;
    };
    
    //Base-case for two elements sublists (Simple compare and swap):
    template<typename FIRST , typename LAST >
    struct _quicksort<2,dl32TypeList<FIRST,LAST>>
    {
        using result = typename dl32tmp_if< COMPARER<FIRST,LAST>::value , //CONDITION
                                            dl32TypeList<FIRST,LAST>,               //THEN
                                            dl32TypeList<LAST,FIRST>                //ELSE
                                          >::type;
    };
    
    //Recursive case:
    template<unsigned int lenght , typename... Ns>
    struct _quicksort<lenght,dl32TypeList<Ns...>>
    {
    private:
        /* STEP 1: Reorder the sublist in two sublists: Left sublist, with elements greater than pivot, and right, with the others */
        
        //Forward declaration:
        template<typename PIVOT , typename RIGHT /* initial (or actual) right sublist */ , typename LEFT /* initial (or actual) left sublist */ , typename _UINT_LIST /* original sublist */>
        struct _reorder_sublists;
        
        //Recursive case:
        template<typename PIVOT , typename... RIGHT_UINTS , typename... LEFT_UINTS , typename HEAD , typename... TAIL>
        struct _reorder_sublists<PIVOT,dl32TypeList<RIGHT_UINTS...>,dl32TypeList<LEFT_UINTS...>,dl32TypeList<HEAD,TAIL...>>
        {
            using _next_left  = dl32TypeList<LEFT_UINTS...,HEAD>;  ///< Next left  sublist if HEAD is greather than PIVOT.
            using _next_right = dl32TypeList<HEAD,RIGHT_UINTS...>; ///< Next right sublist if HEAD is less than PIVOT.
            //                                                    CONDITION                  THEN                    ELSE
            using next_left  = typename dl32tmp_if< !COMPARER<PIVOT,HEAD>::value , _next_left  , dl32TypeList<LEFT_UINTS...>>::type;
            using next_right = typename dl32tmp_if<  COMPARER<PIVOT,HEAD>::value , _next_right , dl32TypeList<RIGHT_UINTS...>>::type;
            
            using next_reorder = _reorder_sublists<PIVOT,next_right,next_left,dl32TypeList<TAIL...>>; // "Recursive call" (Iteration)
            
            using right = typename next_reorder::right; //Recursive result return
            using left  = typename next_reorder::left;  //Recursive result return
        };
        
        //Base case (End of the iteration):
        template<typename PIVOT , typename RIGHT , typename LEFT>
        struct _reorder_sublists<PIVOT,RIGHT,LEFT,empty_uint_list>
        {
            using right = RIGHT;
            using left  = LEFT;
        };
        
        template<typename PIVOT>
        using _right_sublist = typename _reorder_sublists<PIVOT,empty_uint_list,empty_uint_list,dl32TypeList<Ns...>>::right; //Right sublist computation
        template<typename PIVOT>
        using _left_sublist  = typename _reorder_sublists<PIVOT,empty_uint_list,empty_uint_list,dl32TypeList<Ns...>>::left;  //Left sublist computation

    private:
        static const unsigned int _half_size   = lenght/2;
        static const unsigned int _pivot_index = _half_size; //"Silly" pivot policy. Random-pivot instead? http://stackoverflow.com/questions/11498304/generate-random-numbers-in-c-at-compile-time
        using _pivot = typename dl32TypeAt<_pivot_index,dl32TypeList<Ns...>>::type;
        
        using _right = _right_sublist<_pivot>; //"Call" to reordered right sublist computation
        using _left  = _left_sublist<_pivot>;  //"Call" to reordered left sublist computation
        
    public:
        /* STEP 2: Recursive "call" to quicksort passing the two generated sublists */
        
        using result = typename dl32Merge< typename _quicksort< _left::size , _left >::result , typename _quicksort< _right::size , _right >::result >::result;
    };

public:
    using result = typename _quicksort<UINT_LIST::size,UINT_LIST>::result; //"Call" to quicksort computation;
};


/* input/output printing tools */

template<typename OUTPUT>
struct print_output;

template<>
struct print_output<empty_uint_list>{ static void print() { cout << endl << endl; } };

template<typename HEAD , typename... TAIL>
struct print_output<dl32TypeList<HEAD,TAIL...>>
{
    static void print() 
    { 
        cout << HEAD::value << (sizeof...(TAIL) > 0 ? "," : ""); 
        print_output<dl32TypeList<TAIL...>>::print(); 
    }
};


/* unsigned int lists generator */

template<unsigned int BEGIN , unsigned int END>
class uint_list_generator
{
private:
    template<unsigned int _CURRENT,unsigned int _END>
    struct _generator
    {
        using result = typename dl32Merge<uint_list<_CURRENT>,typename _generator<(BEGIN <= END ? _CURRENT + 1 : _CURRENT - 1) , _END>::result>::result;
    };
    
    template<unsigned int _END>
    struct _generator<_END,_END>
    {
        using result = uint_list<_END>;
    };
    
public:
    using result  = typename _generator<BEGIN,END>::result;
};

using input = typename uint_list_generator<0,499>::result;
using output = typename quicksort<input>::result;

int main()
{
    print_output<input>::print();
    print_output<output>::print();
    
    return 0;
}

#endif /* CURRENT_TEST */

