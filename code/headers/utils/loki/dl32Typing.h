/* 
 * File:   dl32Typing.h
 * Author: Manu343726
 *
 * Created on 27 de abril de 2013, 21:13
 */

#ifndef DL32TYPING_H
#define	DL32TYPING_H

#include "dl32Config.h"

//Nota: Soy plenamente consciente de que todo ésto está implementado desde C++11 en la STL ( http://en.cppreference.com/w/cpp/types ), lo hago por mero entretenimiento.

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 type checking basic data.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template< typename T , typename U>
class dl32TypeChecker_Base
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
/// @detailts Use dl32SameType>T,U>::same_type to check if the and U are the same type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U>
struct dl32SameType
{
    enum { value = false };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 type comparator. (Template specialitation for same types))
/// @detailts Use dl32SameType>T,U>::same_type to check if the and U are the same type.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct dl32SameType<T,T>
{
    enum { value = true };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 implicit type conversion checker.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U>
struct dl32Conversion : public dl32TypeChecker_Base<T,U>
{
    enum { value = sizeof( dl32TypeChecker_Base<T,U>::_testTtoU( dl32TypeChecker_Base<T,U>::_makeT() ) ) == dl32TypeChecker_Base<T,U>::_sizeof_Small };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 inheritance checker.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U>
struct dl32SuperclassSubclass
{
    enum { value = dl32Conversion<const U* , const T*>::value && !dl32SameType<const T* , const void*>::value && !dl32SameType<const T , const U>::value };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 compile-time type data.
/// @details This class 
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
        conversion_exists = dl32Conversion<T,U>::value, ///< Checks if a conversion from T to U is alowed. Its value is 1 (true) if a implicit conversion is alowwed). 0 (False) if is not alowed.)
        
        //Basic type-checking:
        same_type = dl32SameType<T,U>::value, ///< Checks if the two types provided are the same type.
        
        //Inheritance checking:
        superclass_subclass = dl32SuperclassSubclass<T,U>::value ///< Checks if T is a superclass of U (U is a subclass of T).
    };
};


#endif	/* DL32TYPING_H */

