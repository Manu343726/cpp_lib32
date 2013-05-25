/* 
 * File:   dl32NewMath.h
 * Author: Manu343726
 *
 * Created on 25 de mayo de 2013, 17:04
 */

#ifndef DL32NEWMATH_H
#define	DL32NEWMATH_H

#include "dl32OperatorOverloadingHelpers.h"
#include "dl32Typing.h"

#include <math.h>
#include <limits>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides a set of useful floating-point functions.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T = float>
struct dl32FloatingPointHelper
{
    using floating_point_type = T; ///< Public alias to the floating-point type in use.
    static const floating_point_type epsilon = numeric_limits<T>::epsilon(); ///< floating-point epsilon used for comparisons.
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if two floating-point numbers are equal.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool are_equal(const T& a , const T& b) { return std::fabs( a - b ) < epsilon; }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if two floating-point numbers are not equal.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool are_not_equal(const T& a , const T& b) { return !are_equal( a , b ); }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if a floating-point number is not equal to zero.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool is_zero(const T& number) { return std::fabs( number ) < epsilon; }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if a floating-point number is not equal to zero.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool is_not_zero(const T& number) { return !is_zero( number ); }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if a floating-point number is bigger or equal than other floating-point number.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool bigger_or_equal(const T& a , const T& b) { return a > b || are_equal( a , b ); }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if a floating-point number is not bigger or equal than other floating-point number.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool not_bigger_or_equal(const T& a , const T& b) { return a < b; }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if a floating-point number is less or equal than other floating-point number.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool less_or_equal(const T& a , const T& b) { return a < b || are_equal( a , b ); }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if a floating-point number is not less or equal than other floating-point number.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool not_less_or_equal(const T& a , const T& b) { return a > b; }
};

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

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides a homebrew implementation of a 2d vector and its most common algebraic
///        operations.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T = float , bool BASIC_ALGEBRA_ONLY = false>
struct _vector_2d_implementation_homebrew : public dl32EqualityHelper<_vector_2d_implementation_homebrew> , 
                                            public dl32Select< BASIC_ALGEBRA_ONLY , dl32BasicAlgebraHelper<_vector_2d_implementation_homebrew> , 
                                                                           typename dl32TypeList< dl32BasicAlgebraHelper<_vector_2d_implementation_homebrew> , 
                                                                                                  dl32MultiplicationHelper<_vector_2d_implementation_homebrew,T> , 
                                                                                                  dl32DivisionHelper<_vector_2d_implementation_homebrew,T>
                                                                                                >::public_inheritance_from_types
                                                             >::type
{
private:
    using my_type = _vector_2d_implementation_homebrew; //Así es mas facil de escribir
public:
    static const unsigned int dimensions = 2; ///< Dimensional range of the point.
    static const my_type zero = my_type(0,0); ///< Addition neutral operand.
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
    /// 
    /// @param m1 First vector to be compared.
    /// @param m2 Second vector to be compared.
    ///
    /// @return Returns true if coordinates of m1 and m2 are the same. Returns false in other case.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    bool operator==(const my_type& m1 , const my_type m2) 
    { 
        return dl32FloatingPointHelper<T>::are_equal( m1.x , m2.x ) && 
               dl32FloatingPointHelper<T>::are_equal( m1.y , m2.y ); 
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Addition implementation. This function performs addition of a provided vector to this instance.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    my_type& operator+=(const my_type& m)
    {
        this->x += m.x;
        this->y += m.y;
        
        return *this;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Substraction implementation. This function performs substraction of a provided vector to this 
    ///        instance.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    my_type& operator-=(const my_type& m)
    {
        this->x -= m.x;
        this->y -= m.y;
        
        return *this;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Multiplication implementation. This function performs multiplication of a provided vector to 
    ///        this instance.
    ///
    /// @author	Manu343726
    ///
    /// @remarks Template parameter BASIC_ALGEBRA_ONLY_DUMMY_SFINAE_BRIDGE is designed only for make SFINAE
    ///          working with class template parameter BASIC_ALGEBRA_ONLY. Please don't use it.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    template<bool BASIC_ALGEBRA_ONLY_DUMMY_SFINAE_BRIDGE = BASIC_ALGEBRA_ONLY>
    typename dl32EnableIf<!BASIC_ALGEBRA_ONLY_DUMMY_SFINAE_BRIDGE , my_type&>::type 
    operator*=(const coord_type& n)
    {
        this->x *= n;
        this->y *= n;
        
        return *this;
    }
        
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Division implementation. This function performs division of a provided vector to 
    ///        this instance.
    ///
    /// @author	Manu343726
    ///
    /// @remarks Template parameter BASIC_ALGEBRA_ONLY_DUMMY_SFINAE_BRIDGE is designed only for make SFINAE
    ///          working with class template parameter BASIC_ALGEBRA_ONLY. Please don't use it.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    template<bool BASIC_ALGEBRA_ONLY_DUMMY_SFINAE_BRIDGE = BASIC_ALGEBRA_ONLY>
    typename dl32EnableIf<!BASIC_ALGEBRA_ONLY_DUMMY_SFINAE_BRIDGE , my_type&>::type 
    operator/=(const coord_type& n)
    {
        this->x /= n;
        this->y /= n;
        
        return *this;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides a Direct3D algebra utilities API based implementation of a 2d vector and 
///        its most common algebraic operations. NOT IMPLEMENTED. PROVISIONAL DEFINITION.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T = float , bool BASIC_ALGEBRA_ONLY = false>
struct _vector_2d_implementation_direct3d{};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides a OpenGL algebra utilities API based implementation of a 2d vector and 
///        its most common algebraic operations. NOT IMPLEMENTED. PROVISIONAL DEFINITION.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T = float , bool BASIC_ALGEBRA_ONLY = false>
struct _vector_2d_implementation_opengl{};

///< The set of 2d vector implementations.
template<typename T = float , bool BASIC_ALGEBRA_ONLY = false>
using _vector_2d_implementations = dl32TypeList<_vector_2d_implementation_homebrew<T,BASIC_ALGEBRA_ONLY> , _vector_2d_implementation_direct3d<T,BASIC_ALGEBRA_ONLY> , _vector_2d_implementation_opengl<T,BASIC_ALGEBRA_ONLY>>;

///< Current implementation of 2d vectors.
template<typename T = float , bool BASIC_ALGEBRA_ONLY = false>
using _vector_2d_implementation_current = _vector_2d_implementations<T,BASIC_ALGEBRA_ONLY>::type_at<(unsigned int)_DL32MATH_VECTOR_IMPLEMENTATION>;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class represents a point in 2d space.
///
/// @tparam T Type used for point coordinates.
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T = float>
struct dl32Point2D : public _vector_2d_implementation_current<T,true> {};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class represents a vector in 2d space.
///
/// @tparam T Type used for point coordinates.
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T = float>
struct dl32Vector2D : public _vector_2d_implementation_current<T,false>
{
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
        T lenght = lenght();
        
        this->x /= lenght;
        this->y /= lenght;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Returns a normalized version of this vector.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32Vector2D get_normalized()
    {
        dl32Vector2D ret( *this );
        ret.normalize();
        
        return ret;
    }
};


#endif	/* DL32NEWMATH_H */

