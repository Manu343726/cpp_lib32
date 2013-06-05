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

#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_TYPINGTEST

#include "dl32Typing.h"
#include <iostream>
#include <typeinfo>
#include <string>

using namespace std;

#define TESTING_USEASSERT FALSE
#define TESTING_WAIT_AT_END FALSE
#define TESTING_USE_COLOR FALSE

#define EXPAND(x) x

#if TESTING_USEASSERT
#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <assert.h>
#else

#define assert_data(x) __FILE__ << ", in function " << __FUNCTION__  << " (line " <<  __LINE__ << "):" << " '" << #x << "' --> "

#if TESTING_USE_COLOR

#include "dl32ConsoleColor.h"

#ifdef assert
#undef assert
#endif /* ASSERT */

#define assert(x) (x) ? cout << assert_data(x) << "OK!" << endl : cout << push_style << dl32ChangeForegroundColor( dl32ConsoleColor::RED ) << assert_data(x) << "ERROR (Cascao raro)" << pop_style << endl 
#else

#ifdef assert
#undef assert
#endif /* ASSERT */

#define assert(x) (x) ? cout << assert_data(x) << "OK!" << endl : cout << assert_data(x) << "ERROR (Cascao raro)" << endl
#endif /* TESTING_USE_COLOR */
#endif /* TESTING_USEASSERT */

int main()
{  
    typedef dl32TypeList<char , int , float , double> myList;
    
    /* TypeList indexing test */
    
    assert( (dl32TypeChecking<float,myList::type_at<2>>::same_type) );
    
    /* Inheritance checking test */
    
    class Foo{};
    class FooChild : public Foo {};
    
    assert( (dl32TypeChecking<Foo,FooChild>::superclass_subclass) );
    
    /* TypeList indexof test */
    
    assert( myList::index_of<int>::value == 1 );
    
    /* TypeList push_back test */
    
    using new_type_list = myList::push_back<bool>;
    
    assert( (dl32SameType<bool , myList::push_back<bool>::type_at<myList::size>>::result) );
    
    /* Typelist merge test */
    
    using list1 = dl32TypeList<char , int , bool>;
    using list2 = dl32TypeList<float , double>;
    using merged_list = list1::merge<list2>;
    
    assert( merged_list::size == list1::size + list2::size );
    
    /* Typelist split test */
    
    using split = dl32Split<2,myList>;
    using right = split::right;
    using left  = split::left;
    
    assert( right::size + left::size == myList::size );
    cout << "original typelist size: " << myList::size << endl;
    cout << "Splitted left-typelist size: " << left::size << endl;
    cout << "Splitted right-typelist size: " << right::size << endl;
    
    assert( (dl32SameTypeList<typename dl32Split<0,dl32TypeList<char,int,bool>>::left  , dl32TypeList<char>>::result) );
    assert( (dl32SameTypeList<typename dl32Split<0,dl32TypeList<char,int,bool>>::right , dl32TypeList<int,bool>>::result) );
    
    assert( (dl32SameTypeList<typename dl32Split<1,dl32TypeList<char,int,bool>>::left  , dl32TypeList<char,int>>::result) );
    assert( (dl32SameTypeList<typename dl32Split<1,dl32TypeList<char,int,bool>>::right , dl32TypeList<bool>>::result) );
    
    assert( (dl32SameTypeList<typename dl32Split<2,dl32TypeList<char,int,bool>>::left  , dl32TypeList<char,int,bool>>::result) );
    assert( (dl32SameTypeList<typename dl32Split<2,dl32TypeList<char,int,bool>>::right , dl32TypeList<>>::result) );
    
    /* Same typelist test */
    
    assert( (dl32SameTypeList<myList,myList>::result) );
    assert( !(dl32SameTypeList<dl32TypeList<int,char>,dl32TypeList<char,int>>::result) );
    
    /* typelist insert test */
    
    assert( (dl32SameTypeList<dl32TypeList<bool,int,char> , dl32TypeList<bool,char>::insert<0,int>>::result) );
    
    /* Type traits checking test */
    
    //HasConst tests:
    assert( !dl32TypeTraits<int>::hasConst        );
    assert(  dl32TypeTraits<const int>::hasConst  );
    assert(  dl32TypeTraits<const int*>::hasConst );
    assert(  dl32TypeTraits<const int&>::hasConst );
    
    //IsPointer tests:
    assert( !dl32TypeTraits<int>::isPointer        );
    assert( !dl32TypeTraits<const int>::isPointer  );
    assert(  dl32TypeTraits<const int*>::isPointer );
    assert( !dl32TypeTraits<const int&>::isPointer );
    
    //IsReference tests:
    assert( !dl32TypeTraits<int>::isReference        );
    assert( !dl32TypeTraits<const int>::isReference  );
    assert( !dl32TypeTraits<const int*>::isReference );
    assert(  dl32TypeTraits<const int&>::isReference );
    
    /* end */
#if TESTING_WAIT_AT_END
    cin.sync();
    cin.get();
#endif
}

#endif