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

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 type selection.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<bool flag , typename T , typename U>
struct dl32Select { typedef T result; };

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 type selection. (Template specialitation for second type selection). 
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , typename U>
struct dl32Select<false,T,U> { typedef U result; };

template<typename T>
class dl32TypeTraits
{
private:
#define TRAITS( name , type , checkingType , typedefName ) \
    template<typename type>                                \
    struct name                                            \
    {                                                      \
        enum { value = false };                            \
        typedef dl32NoType typedefName;                    \
    };                                                     \
    template<typename type>                                \
    struct name<checkingType>                              \
    {                                                      \
        enum {value = true };                              \
        typedef dl32EmptyType typedefName;                 \
    };                        
    
#define TRAITS_FULL(attribute_name , trait_name , type , checking_type , typedef_name ) \
        TRAITS( trait_name , type , checking_type , typedef_name );                     \
        public:                                                                         \
                enum { attribute_name = trait_name<T>::value };                         \
                typedef typename trait_name<T>::typedef_name typedef_name;              \
        private:
    
    TRAITS_FULL( isPointer   , _pointerTraits    , U , U*      , PointeeType )    //Checks if T is a pointer
    TRAITS_FULL( isReference , _referenceTraits  , U , U&      , ReferencedType ) //Checks if T is a reference
    TRAITS_FULL( hasConst    , _constTraits      , U , const U , NonConstType )    //Checks if T is a pointer
    TRAITS_FULL( isRvalue    , _rvalueTraits     , U , U&&     , MovedType )      //Checks if T is a rvalue
    
    typedef T OriginalType;
};
#endif	/* DL32TYPING_H */

