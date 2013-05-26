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

#include <cmath>
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

const unsigned int _DL32MATH_VECTOR_IMPLEMENTATION_INTEGRAL = (unsigned int)_DL32MATH_VECTOR_IMPLEMENTATION;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides a homebrew implementation of a 2d vector and its most common algebraic
///        operations.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename IMPLEMENTER , typename T = float , bool BASIC_ALGEBRA_ONLY = false>
struct _vector_2d_implementation_homebrew
{
public:
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
    
    static_assert( dl32TypeTraits<T>::isIntegral || dl32TypeTraits<T>::isFloatingPoint , "2d vectors must have integral or floating-point types as coordinates");
    
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
    ///
    /// @remarks Template parameter T_DUMMY_SFINAE_BRIDGE is designed only for make SFINAE
    ///          working with class template parameter T. Please don't use it.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename T_DUMMY_SFINAE_BRIDGE = T>
    static typename dl32EnableIf<dl32TypeTraits<T_DUMMY_SFINAE_BRIDGE>::isFloatingPoint , bool>::type 
    equal(const IMPLEMENTER& m1 , const IMPLEMENTER m2) 
    { 
        return dl32FloatingPointHelper<T>::are_equal( m1.x , m2.x ) && 
               dl32FloatingPointHelper<T>::are_equal( m1.y , m2.y ); 
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Equality operator for two 2d vectors. 
    /// 
    /// @param m1 First vector to be compared.
    /// @param m2 Second vector to be compared.
    ///
    /// @return Returns true if coordinates of m1 and m2 are the same. Returns false in other case.
    ///
    /// @author	Manu343726
    ///
    /// @remarks Template parameter T_DUMMY_SFINAE_BRIDGE is designed only for make SFINAE
    ///          working with class template parameter T. Please don't use it.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename T_DUMMY_SFINAE_BRIDGE = T>
    static typename dl32EnableIf<dl32TypeTraits<T_DUMMY_SFINAE_BRIDGE>::isIntegral , bool>::type 
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
    ///
    /// @author	Manu343726
    ///
    /// @remarks Template parameter BASIC_ALGEBRA_ONLY_DUMMY_SFINAE_BRIDGE is designed only for make SFINAE
    ///          working with class template parameter BASIC_ALGEBRA_ONLY. Please don't use it.
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
    ///
    /// @author	Manu343726
    ///
    /// @remarks Template parameter BASIC_ALGEBRA_ONLY_DUMMY_SFINAE_BRIDGE is designed only for make SFINAE
    ///          working with class template parameter BASIC_ALGEBRA_ONLY. Please don't use it.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    template<bool BASIC_ALGEBRA_ONLY_DUMMY_SFINAE_BRIDGE = BASIC_ALGEBRA_ONLY>
    typename dl32EnableIf<!BASIC_ALGEBRA_ONLY_DUMMY_SFINAE_BRIDGE , my_type&>::type 
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
                                                    _vector_2d_implementation_opengl  <IMPLEMENTER,T,BASIC_ALGEBRA_ONLY>>;
    
public:
    using current_implementation = _vector_2d_implementation_homebrew<IMPLEMENTER,T,BASIC_ALGEBRA_ONLY>; ///< Gets the current vector 2d implementation.
};

///< Operator overloading helpers for vectors with only basic algebra (Addition and substraction).
template<typename VECTOR_IMPLEMENTATION , typename T = float>
using vector_implementation_operators_basic_algebra = dl32BasicAlgebraHelper<VECTOR_IMPLEMENTATION>;

///< Operator overloading helpers for vectors with complete algebra (Addition, substraction, multiplication, and division).
template<typename VECTOR_IMPLEMENTATION , typename T = float>
using vector_implementation_operators_algebra = typename dl32TypeList<dl32BasicAlgebraHelper<VECTOR_IMPLEMENTATION> , dl32MultiplicationHelper<VECTOR_IMPLEMENTATION,T> , dl32DivisionHelper<VECTOR_IMPLEMENTATION,T>>::public_inheritance_from_types;

///< Vector operation overloading selector.
template<typename VECTOR_IMPLEMENTATION , bool BASIC_ALGEBRA_ONLY , typename T = float>
using vector_implementation_operators = typename dl32Select<BASIC_ALGEBRA_ONLY , vector_implementation_operators_basic_algebra<VECTOR_IMPLEMENTATION,T> , vector_implementation_operators_algebra<VECTOR_IMPLEMENTATION,T>>::type;


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
    using my_implementation = typename dl32Vector2dImplementationsManager<dl32Point2D<T>,T,true>::current_implementation; ///< Alias to the implementer type.
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Default constructor. Initialices coordinates x and y to zero.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32Point2D() : my_implementation() {}
    
    
    dl32Point2D(const T& _x ,const T& _y) : my_implementation(_x,_y) {}
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if two dl32Point2Ds are equal.
    ///
    /// @author	Manu343726
    ///
    /// @return True if thw two points are equal. False if not.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    friend bool operator==(const dl32Point2D& p1 , const dl32Point2D& p2) { return my_implementation::equal( p1 , p2 ); }
    
    dl32Point2D& operator+=(const dl32Point2D& p) { return this->add       ( p ); }
    dl32Point2D& operator-=(const dl32Point2D& p) { return this->substract ( p ); }
};

using dl32Point2Df = dl32Point2D<float>; ///< Alias for single-precision 2d points.
using dl32Point2Dd = dl32Point2D<double>; ///< Alias for double-precision 2d points.

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class represents a vector in 2d space.
///
/// @tparam T Type used for point coordinates.
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T = float>
struct dl32Vector2D : public dl32Vector2dImplementationsManager<dl32Vector2D<T>,T,false>::current_implementation
{
    using my_implementation = typename dl32Vector2dImplementationsManager<dl32Vector2D<T>,T,false>::current_implementation; ///< Alias to the implementer type.
    
    dl32Vector2D() : my_implementation() {}
    dl32Vector2D(const T& _x ,const T& _y) : my_implementation(_x,_y) {}
    dl32Vector2D(const dl32Point2D<T>& p1 , dl32Point2D<T>& p2) : my_implementation( p2.x - p1.x , p2.y - p1.y ) {}
    
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

using dl32Vector2Df = dl32Vector2D<float>; ///< Alias for single-precision 2d points.
using dl32Vector2Dd = dl32Vector2D<double>; ///< Alias for double-precision 2d points.

#endif	/* DL32NEWMATH_H */

