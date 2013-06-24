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
#include "dl32TypeTraits.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_TYPINGTEST

#include "dl32MetaprogrammingLibrary.h"
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
    
    assert( (dl32SameType<float , myList::type_at<2>>::value) );
    
    /* Inheritance checking test */
    
    class Foo{};
    class FooChild : public Foo {};
    
    assert( (dl32HaveInheritance<Foo,FooChild>::result) );
    
    /* TypeList indexof test */
    
    assert( myList::index_of<int>::value == 1 );
    
    /* TypeList push_back test */
    
    using new_type_list = myList::push_back<bool>;
    
    assert( (dl32SameType<bool , typename myList::push_back<bool>::type_at<myList::size>>::value) );
    
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
    
    assert( (dl32SameTypeList<dl32TypeList<bool,int,char> , dl32TypeList<bool,char>::insert<1,int>>::result) );
    cout << "insert test. typelist A: " << dl32TypeList<bool,int,char>::to_string() << endl;
    cout << "insert test. typelist B: " << dl32TypeList<bool,char>::to_string() << endl;
    cout << "insert test. Result of insert: " << dl32TypeList<bool,char>::insert<1,int>::to_string() << endl;
    
    /* Type traits checking test */
    
    //HasConst tests:
    assert( !dl32WithoutConst<int>::has_const        ); 
    assert(  dl32WithoutConst<const int>::has_const  ); cout << type_to_string<const int>() << " ==> " << type_to_string<dl32WithoutConst<const int>::result>() << endl;
    assert(  dl32WithoutConst<int* const>::has_const ); cout << type_to_string<int* const>() << " ==> " << type_to_string<dl32WithoutConst<int* const>::result>() << endl;
    assert(  dl32WithoutConst<const int&>::has_const ); cout << type_to_string<const int&>() << " ==> " << type_to_string<dl32WithoutConst<const int&>::result>() << endl;
    
    //IsPointer tests:
    assert( !dl32IsPointer<int>::value        );
    assert( !dl32IsPointer<const int>::value  );
    assert(  dl32IsPointer<const int*>::value );
    assert( !dl32IsPointer<const int&>::value );
    
    //IsReference tests:
    assert( !dl32IsReference<int>::value        );
    assert( !dl32IsReference<const int>::value  );
    assert( !dl32IsReference<const int*>::value );
    assert(  dl32IsReference<const int&>::value );
    
    //IsIntegral tests:
    assert( dl32IsIntegralType<unsigned char>::value );
    assert( dl32IsIntegralType<char>::value );
    assert( dl32IsIntegralType<unsigned int>::value );
    assert( dl32IsIntegralType<int>::value );
    assert( dl32IsIntegralType<unsigned long int>::value );
    assert( dl32IsIntegralType<long int>::value );
    assert( !dl32IsIntegralType<bool>::value );
    assert( !dl32IsIntegralType<float>::value );
    assert( !dl32IsIntegralType<double>::value );
    
    //IsFloatingPoint tests:
    assert( !dl32IsFloatingPointType<unsigned char>::value );
    assert( !dl32IsFloatingPointType<char>::value );
    assert( !dl32IsFloatingPointType<unsigned int>::value );
    assert( !dl32IsFloatingPointType<int>::value );
    assert( !dl32IsFloatingPointType<unsigned long int>::value );
    assert( !dl32IsFloatingPointType<long int>::value );
    assert( !dl32IsFloatingPointType<bool>::value ); cout << dl32IntegralTypes::to_string() << " (index of: " << dl32IntegralTypes::index_of<bool>::value << ")" << endl;
    assert( dl32IsFloatingPointType<float>::value );
    assert( dl32IsFloatingPointType<double>::value );
    
    
    /* Compile-time "hashmap" tests */
    
    //For example: A map that associates integral types with its unsigned types.
    using map = dl32KeyValueMap<dl32KeyValuePair<int , unsigned int> , dl32KeyValuePair<char , unsigned char>>;
    
    cout << type_to_string<int>()  << " ---> " << type_to_string< map::get_value<int> >()  << endl;
    cout << type_to_string<char>() << " ---> " << type_to_string< map::get_value<char> >() << endl;
    
    /* n-dimensional static array test */
    dl32Array<int,1,2,3> my_array;
    
    my_array[0][1][2] = 1234;
    
    cout << my_array[0][1][2] << endl;
    cout << type_to_string<dl32Array<int,1,2,3>::array_type>() << endl;
    cout << dl32Array<int,1,2,3>::lenghts::to_string() << endl;
    
    /* end */
#if TESTING_WAIT_AT_END
    cin.sync();
    cin.get();
#endif
}

#endif