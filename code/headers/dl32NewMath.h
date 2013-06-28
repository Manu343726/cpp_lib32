/* 
 * File:   dl32NewMath.h
 * Author: Manu343726
 *
 * Created on 25 de mayo de 2013, 17:04
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

#ifndef DL32NEWMATH_H
#define	DL32NEWMATH_H

#include "utils/metaprogramming_library/dl32TypeTraits.h"


#include "dl32OperatorOverloadingHelpers.h"
#include "dl32MetaprogrammingLibrary.h"
#include "dl32FloatingPointHelper.h"

#include <cmath>
#include <limits>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 vector implementation types settings.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
enum class dl32VectorImplementation : unsigned int
{
    HOMEBREW, ///< Homebrew vector2d/3d implementation.
    DIRECT3D, ///< Implementation based on Direct3D algebra utilities API.
    OPENGL    ///< Implementation based on OpenGL algebra utilities API.
};

const dl32VectorImplementation _DL32MATH_VECTOR_IMPLEMENTATION = dl32VectorImplementation::HOMEBREW; ///< Vector implementation configuration.

const unsigned int _DL32MATH_VECTOR_IMPLEMENTATION_INTEGRAL = (unsigned int)_DL32MATH_VECTOR_IMPLEMENTATION;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides a homebrew implementation of a 2d vector and its most common algebraic
///        operations.
///
/// @tparam IMPLEMENTER Class that uses (inherits from) this implementation (CRTP subclass)
/// @tparam T Vector implementation coordinates type. Its default value is float.
/// @tparam BASIC_ALGEBRA_ONLY Defines the set of algebra operations to be implemented.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename IMPLEMENTER , typename T = float , bool BASIC_ALGEBRA_ONLY = false>
struct _vector_2d_implementation_homebrew
{
private:
    using my_type = _vector_2d_implementation_homebrew<IMPLEMENTER,T,BASIC_ALGEBRA_ONLY>; //Así es mas facil de escribir
public:
    static const unsigned int dimensions = 2; ///< Dimensional range of the point.
    using coord_type = T; ///< Type used for the coordinates.
    
    union
    {
        struct
        {
            T x; ///< First coordinate of the vector.
            T y; ///< Second coordinate of the vector.
        };
        T coords[dimensions]; ///< Vector coordinates in array-style. First element corresponds with x. Second element with y.
    };
    
    static_assert( dl32IsFloatingPointType<T>::value || dl32IsIntegralType<T>::value , "2d vectors must have integral or floating-point types as coordinates");
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Default constructor. Set all coordinates to zero.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    _vector_2d_implementation_homebrew() : x(0) , y(0) {}
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Constructor. Creates a vector with the specified coordinates.
    ///
    /// @param _x First coordinate of the vector.
    /// @param _y Second coordinate of the vector.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    _vector_2d_implementation_homebrew(const T& _x ,const T& _y) : x(_x) , y(_y) {}
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Equality operator for two 2d vectors. 
    /// @details This version is enabled only for floating-point Ts. 
    /// 
    /// @param m1 First vector to be compared.
    /// @param m2 Second vector to be compared.
    ///
    /// @return Returns true if coordinates of m1 and m2 are the same. Returns false in other case.
    ///
    /// @author	Manu343726
    ///
    /// @remarks Template parameter T_DUMMY_SFINAE_BRIDGE is designed only for make SFINAE
    ///          work with class template parameter T. Please don't use it.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename T_DUMMY_SFINAE_BRIDGE = T>
    static typename dl32EnableIf<dl32IsFloatingPointType<T_DUMMY_SFINAE_BRIDGE>::value , bool>::type 
    equal(const IMPLEMENTER& m1 , const IMPLEMENTER m2) 
    { 
        return dl32FloatingPointHelper<T>::are_equal( m1.x , m2.x ) && 
               dl32FloatingPointHelper<T>::are_equal( m1.y , m2.y ); 
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Equality operator for two 2d vectors. 
    /// @details This version is enabled only for integral Ts. 
    /// 
    /// @param m1 First vector to be compared.
    /// @param m2 Second vector to be compared.
    ///
    /// @return Returns true if coordinates of m1 and m2 are the same. Returns false in other case.
    ///
    /// @author	Manu343726
    ///
    /// @remarks Template parameter T_DUMMY_SFINAE_BRIDGE is designed only for make SFINAE
    ///          work with class template parameter T. Please don't use it.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename T_DUMMY_SFINAE_BRIDGE = T>
    static typename dl32EnableIf<dl32IsIntegralType<T_DUMMY_SFINAE_BRIDGE>::value , bool>::type 
    equal(const IMPLEMENTER& m1 , const IMPLEMENTER m2) 
    { 
        return m1.x == m2.x && m1.y == m2.y; 
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Addition implementation. This function performs addition of a provided vector to this instance.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    IMPLEMENTER& add(const IMPLEMENTER& m)
    {
        this->x += m.x;
        this->y += m.y;
        
        return static_cast<IMPLEMENTER&>(*this);
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Substraction implementation. This function performs substraction of a provided vector to this 
    ///        instance.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    IMPLEMENTER& substract(const IMPLEMENTER& m)
    {
        this->x -= m.x;
        this->y -= m.y;
        
        return static_cast<IMPLEMENTER&>(*this);
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Multiplication implementation. This function performs multiplication of a provided vector to 
    ///        this instance.
    /// @details This function is enabled only for implementers wich use complete algebra ( BASIC_ALGEBRA_ONLY == false )
    ///
    /// @author	Manu343726
    ///
    /// @remarks Template parameter BASIC_ALGEBRA_ONLY_DUMMY_SFINAE_BRIDGE is designed only for make SFINAE
    ///          work with class template parameter BASIC_ALGEBRA_ONLY. Please don't use it.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    template<bool BASIC_ALGEBRA_ONLY_DUMMY_SFINAE_BRIDGE = BASIC_ALGEBRA_ONLY>
    typename dl32EnableIf<!BASIC_ALGEBRA_ONLY_DUMMY_SFINAE_BRIDGE , IMPLEMENTER&>::type 
    multiply(const coord_type& n)
    {
        this->x *= n;
        this->y *= n;
        
        return static_cast<IMPLEMENTER&>(*this);
    }
        
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Division implementation. This function performs division of a provided vector to 
    ///        this instance.
    /// @details This function is enabled only for implementers wich use complete algebra ( BASIC_ALGEBRA_ONLY == false )
    ///
    /// @author	Manu343726
    ///
    /// @remarks Template parameter BASIC_ALGEBRA_ONLY_DUMMY_SFINAE_BRIDGE is designed only for make SFINAE
    ///          work with class template parameter BASIC_ALGEBRA_ONLY. Please don't use it.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    template<bool BASIC_ALGEBRA_ONLY_DUMMY_SFINAE_BRIDGE = BASIC_ALGEBRA_ONLY>
    typename dl32EnableIf<!BASIC_ALGEBRA_ONLY_DUMMY_SFINAE_BRIDGE , IMPLEMENTER&>::type 
    divide(const coord_type& n)
    {
        this->x /= n;
        this->y /= n;
        
        return static_cast<IMPLEMENTER&>(*this);
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides a Direct3D algebra utilities API based implementation of a 2d vector and 
///        its most common algebraic operations. NOT IMPLEMENTED. PROVISIONAL DEFINITION.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename IMPLEMENTER , typename T = float , bool BASIC_ALGEBRA_ONLY = false>
struct _vector_2d_implementation_direct3d{};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides a OpenGL algebra utilities API based implementation of a 2d vector and 
///        its most common algebraic operations. NOT IMPLEMENTED. PROVISIONAL DEFINITION.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename IMPLEMENTER , typename T = float , bool BASIC_ALGEBRA_ONLY = false>
struct _vector_2d_implementation_opengl{};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief 2d vector implementations manager
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename IMPLEMENTER , typename T = float , bool BASIC_ALGEBRA_ONLY = false>
class dl32Vector2dImplementationsManager
{
private:
    
    ///< Set of vector 2d implementations.
    using _vector_2d_implementations = dl32TypeList<_vector_2d_implementation_homebrew<IMPLEMENTER,T,BASIC_ALGEBRA_ONLY> , 
                                                    _vector_2d_implementation_direct3d<IMPLEMENTER,T,BASIC_ALGEBRA_ONLY> , 
                                                    _vector_2d_implementation_opengl  <IMPLEMENTER,T,BASIC_ALGEBRA_ONLY>
                                                   >;
    
public:
    using current_implementation = _vector_2d_implementation_homebrew<IMPLEMENTER,T,BASIC_ALGEBRA_ONLY>; ///< Gets the current vector 2d implementation.
};

//Forward declaration for implicit cast from dl32oint2D to dl32Vector2D.
template<typename T = float>
struct dl32Vector2D;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class represents a point in 2d space.
///
/// @tparam T Type used for point coordinates.
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T = float>
struct dl32Point2D : public dl32Vector2dImplementationsManager<dl32Point2D<T>,T,true>::current_implementation ,
                     public dl32EqualityHelper<dl32Point2D<T>>,    //Binary operator !=, based on ==.
                     public dl32BasicAlgebraHelper<dl32Point2D<T>> //Binary operators + and -, based on += and -=.
{
    using my_implementation = typename dl32Vector2dImplementationsManager<dl32Point2D<T>,T,true>::current_implementation; ///< Alias to the implementation type.
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Default constructor. Initialices coordinates x and y to zero.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32Point2D() : my_implementation() {}
    
    
    dl32Point2D(const T& _x ,const T& _y) : my_implementation(_x,_y) {}
    
    using my_type = dl32Point2D<T>;
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Implicit cast to 2d vector.
    ///
    /// @return A 2d vector (with the same coordinate type) with x and y as coordinates.
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    operator dl32Vector2D<T>() const {return dl32Vector2D<T>( x , y ); }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if two dl32Point2Ds are equal.
    ///
    /// @author	Manu343726
    ///
    /// @return True if thw two points are equal. False if not.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    friend bool operator==(const my_type& p1 , const my_type& p2) { return my_implementation::equal( p1 , p2 ); }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Adds a dl32Point2D to this instance.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    my_type& operator+=(const my_type& p) { return this->add       ( p ); }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Substracts a dl32Point2D from this instance.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    my_type& operator-=(const my_type& p) { return this->substract ( p ); }
};

using dl32Point2Di = dl32Point2D<int>;    ///< Alias for integer 2d points.
using dl32Point2Df = dl32Point2D<float>;  ///< Alias for single-precision 2d points.
using dl32Point2Dd = dl32Point2D<double>; ///< Alias for double-precision 2d points.

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class represents a vector in 2d space.
///
/// @tparam T Type used for point coordinates.
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T = float>
struct dl32Vector2D : public dl32Vector2dImplementationsManager<dl32Vector2D<T>,T,false>::current_implementation,
                      public dl32EqualityHelper<dl32Vector2D<T>>,         //Binary operator!=(dl32Vector2D<T>,dl32Vector2D<T>)
                      public dl32BasicAlgebraHelper<dl32Vector2D<T>>,     //Binary operator+ (dl32Vector2D<T>,dl32Vector2D<T>) , binary operator-(dl32Vector2D<T>,dl32Vector2D<T>)
                      public dl32MultiplicationHelper<dl32Vector2D<T>,T>, //Binary operator* (dl32Vector2D<T>,T)
                      public dl32DivisionHelper<dl32Vector2D<T>,T>        //Binary operator/ (dl32Vector2D<T>,T)
{
    using my_implementation = typename dl32Vector2dImplementationsManager<dl32Vector2D<T>,T,false>::current_implementation; ///< Alias to the implementer type.
    
    dl32Vector2D() : my_implementation() {}
    dl32Vector2D(const T& _x ,const T& _y) : my_implementation(_x,_y) {}
    dl32Vector2D(const dl32Point2D<T>& p1 , dl32Point2D<T>& p2) : my_implementation( p2.x - p1.x , p2.y - p1.y ) {}
    
    using my_type = dl32Vector2D<T>;
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Implicit cast to 2d point.
    ///
    /// @return A 2d point (with the same coordinate type) with x and y as coordinates.
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    operator dl32Point2D<T>() const {return dl32Point2D<T>( x , y ); }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if two dl32Vector2Ds are equal.
    ///
    /// @author	Manu343726
    ///
    /// @return True if thw two points are equal. False if not.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    friend bool operator==(const my_type& p1 , const my_type& p2) { return my_implementation::equal( p1 , p2 ); }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Adds a dl32Vector2D to this instance.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    my_type& operator+=(const my_type& p) { return this->add       ( p ); }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Sobstracts a dl32Vector2D from this instance.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    my_type& operator-=(const my_type& p) { return this->substract ( p ); }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Multiplies this instcance by a scalar.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    my_type& operator*=(const T& s) { return this->multiply ( s ); }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Divides this instance by a scalar.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    my_type& operator/=(const T& s) { return this->divide   ( s ); }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Returns the lenght of the vector
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    T lenght() const { return std::sqrt( this->x*this->x + this->y*this->y ); }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Normalizes this vector (Converts this vector in a unit-lenght vector with the same direction).
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void normalize()
    {
        //Fast Inverse Square Root? Me lo pensaré...
        T lenght = lenght();
        
        this->x /= lenght;
        this->y /= lenght;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Returns a normalized version of this vector.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    my_type get_normalized()
    {
        my_type ret( *this );
        ret.normalize();
        
        return ret;
    }
};

using dl32Vector2Df = dl32Vector2D<float>; ///< Alias for single-precision 2d vectors.
using dl32Vector2Dd = dl32Vector2D<double>; ///< Alias for double-precision 2d vectors.

#endif	/* DL32NEWMATH_H */

