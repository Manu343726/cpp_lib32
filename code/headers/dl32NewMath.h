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

#include "utils/dl32OperatorOverloadingHelpers.h"

#include <cmath>
#include <limits>

namespace dl32
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief cpp_lib32 vector implementation types settings.
	///
	/// @author	Manu343726
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	enum vector_impl
	{
		HOMEBREW, ///< Homebrew vector2d/3d implementation.
		DIRECT3D, ///< Implementation based on Direct3D algebra utilities API.
		OPENGL    ///< Implementation based on OpenGL algebra utilities API.
	};

	namespace internal
	{
		const dl32::vector_impl current_vector_impl = dl32::vector_impl::HOMEBREW; ///< Vector implementation configuration.

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// @brief This class provides a homebrew implementation of a 2d vector and its most common algebraic
		///        operations.
		///
		/// @tparam IMPLEMENTER Class that uses (inherits from) this implementation (CRTP subclass)
		/// @tparam T Vector implementation coordinates type. Its default value is float.
		/// @tparam BASIC_ALGEBRA_ONLY Defines the set of algebra operations to be implemented. If is
		///                            is set to true, the class includes addition substraction, multiplication,
		///                            and division. If is set to false, it includes addition and substraction
		///                            only.
		///
		/// @author	Manu343726
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		template<typename IMPLEMENTER , typename T = float , bool BASIC_ALGEBRA_ONLY = false>
		struct vector_2d_impl_homebrew
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
    
			//Restrict coordinates to floating-point and integrals only: (Waiting for C++14 concepts lite...)
			static_assert( std::is_floating_point<T>::value || std::is_integral<T>::value , "2d vectors must have integral or floating-point types as coordinates");
    
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			/// @brief Default constructor. Set all coordinates to zero.
			///
			/// @author	Manu343726
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			vector_2d_impl_homebrew() : x(0) , y(0) {}
    
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			/// @brief Constructor. Creates a vector with the specified coordinates.
			///
			/// @param _x First coordinate of the vector.
			/// @param _y Second coordinate of the vector.
			///
			/// @author	Manu343726
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			vector_2d_impl_homebrew(const T& _x ,const T& _y) : x(_x) , y(_y) {}
    
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
			/// @remarks Template parameter T_DUMMY_SFINAE_BRIDGE is designed only to make SFINAE
			///          work with class template parameter T. Please don't use it.
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			template<typename T_DUMMY_SFINAE_BRIDGE = T>
			static typename std::enable_if<std::is_floating_point<T_DUMMY_SFINAE_BRIDGE>::value , bool>::type 
			equal(const IMPLEMENTER& m1 , const IMPLEMENTER m2) 
			{ 
				return dl32::floating_point_helper<T>::are_equal( m1.x , m2.x ) && 
					   dl32::floating_point_helper<T>::are_equal( m1.y , m2.y ); 
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
			/// @remarks Template parameter T_DUMMY_SFINAE_BRIDGE is designed only to make SFINAE
			///          work with class template parameter T. Please don't use it.
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			template<typename T_DUMMY_SFINAE_BRIDGE = T>
			static typename std::enable_if<std::is_integral<T_DUMMY_SFINAE_BRIDGE>::value , bool>::type 
			operator ==(const IMPLEMENTER& m1 , const IMPLEMENTER m2) 
			{ 
				return m1.x == m2.x && m1.y == m2.y; 
			}
    
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			/// @brief Addition implementation. This function performs addition of a provided vector to this instance.
			///
			/// @author	Manu343726
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			IMPLEMENTER& operator+=(const IMPLEMENTER& m)
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
			IMPLEMENTER& operator-=(const IMPLEMENTER& m)
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
			/// @remarks Template parameter BASIC_ALGEBRA_ONLY_DUMMY_SFINAE_BRIDGE is designed only to make SFINAE
			///          work with class template parameter BASIC_ALGEBRA_ONLY. Please don't use it.
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			template<bool BASIC_ALGEBRA_ONLY_DUMMY_SFINAE_BRIDGE = BASIC_ALGEBRA_ONLY>
			typename std::enable_if<!BASIC_ALGEBRA_ONLY_DUMMY_SFINAE_BRIDGE , IMPLEMENTER&>::type 
			operator*=(const coord_type& n)
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
			typename std::enable_if<!BASIC_ALGEBRA_ONLY_DUMMY_SFINAE_BRIDGE , IMPLEMENTER&>::type 
			operator/=(const coord_type& n)
			{
				this->x /= n;
				this->y /= n;
        
				return static_cast<IMPLEMENTER&>(*this);
			}
    
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			/// @brief Gets a string representation of the 2d vector.
			///
			/// @author	Manu343726
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			std::string to_string()
			{
				stringstream ss;
        
				ss << "{" << x << "," << y << "}";
        
				return ss.str();
			}
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// @brief This class provides a Direct3D algebra utilities API based implementation of a 2d vector and 
		///        its most common algebraic operations. NOT IMPLEMENTED. PROVISIONAL DEFINITION.
		///
		/// @author	Manu343726
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		template<typename IMPLEMENTER , typename T = float , bool BASIC_ALGEBRA_ONLY = false>
		struct vector_2d_implt_direct3d{};

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// @brief This class provides a OpenGL algebra utilities API based implementation of a 2d vector and 
		///        its most common algebraic operations. NOT IMPLEMENTED. PROVISIONAL DEFINITION.
		///
		/// @author	Manu343726
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		template<typename IMPLEMENTER , typename T = float , bool BASIC_ALGEBRA_ONLY = false>
		struct vector_2d_implementation_opengl{};
	}

	//Forward declaration for implicit cast from dl32oint2D to dl32Vector2D.
	template<typename T = float>
	struct vector_2d;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This class represents a point in 2d space.
	///
	/// @tparam T Type used for point coordinates.
	/// @author	Manu343726
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T = float>
	struct point_2d : public dl32::internal::vector_2d_impl_homebrew<dl32::point_2d<T>,T,true> ,
		              public dl32::equality_ops<dl32::point_2d<T>>,     //Binary operator !=, based on ==.
		              public dl32::basic_algebra_ops<dl32::point_2d<T>> //Binary operators + and -, based on += and -=.
	{
    
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// @brief Default constructor. Initialices coordinates x and y to zero.
		///
		/// @author	Manu343726
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		point_2d() : dl32::internal::vector_2d_impl_homebrew<dl32::point_2d<T>,T,true>() {}
    
    
		point_2d(const T& _x ,const T& _y) : dl32::internal::vector_2d_impl_homebrew<dl32::point_2d<T>,T,true>(_x,_y) {}
    
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// @brief Implicit cast to 2d vector.
		///
		/// @return A 2d vector (with the same coordinate type) with x and y as coordinates.
		/// @author	Manu343726
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		operator vector_2d<T>() const {return vector_2d<T>{ this->x , this->y }; }
	};

	using point_2di = dl32::point_2d<int>;    ///< Alias for integer 2d points.
	using point_2df = dl32::point_2d<float>;  ///< Alias for single-precision 2d points.
	using point_2dd = dl32::point_2d<double>; ///< Alias for double-precision 2d points.

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This class represents a vector in 2d space.
	///
	/// @tparam T Type used for point coordinates.
	/// @author	Manu343726
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	struct vector_2d : public dl32::internal::vector_2d_impl_homebrew<dl32::vector_2d<T>,T,true> ,
		               public dl32::equality_ops<dl32::vector_2d<T>>,        //Binary operator!=(dl32::vector_2d<T>,dl32::vector_2d<T>)
					   public dl32::basic_algebra_ops<dl32::vector_2d<T>>,   //Binary operator+ (dl32::vector_2d<T>,dl32::vector_2d<T>) , binary operator-(dl32::vector_2d<T>,dl32::vector_2d<T>)
					   public dl32::multiplication_op<dl32::vector_2d<T>,T>, //Binary operator* (dl32::vector_2d<T>,T)
				       public dl32::division_op<dl32::vector_2d<T>,T>        //Binary operator/ (dl32::vector_2d<T>,T)
	{
		vector_2d()                                              : dl32::internal::vector_2d_impl_homebrew<dl32::vector_2d<T>,T,true>()                            {}
		vector_2d(const T& _x ,const T& _y)                      : dl32::internal::vector_2d_impl_homebrew<dl32::vector_2d<T>,T,true>(_x,_y)                       {}
		vector_2d(const dl32Point2D<T>& p1 , dl32Point2D<T>& p2) : dl32::internal::vector_2d_impl_homebrew<dl32::vector_2d<T>,T,true>( p2.x - p1.x , p2.y - p1.y ) {}
    
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// @brief Implicit cast to 2d point.
		///
		/// @return A 2d point (with the same coordinate type) with x and y as coordinates.
		/// @author	Manu343726
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		operator dl32::point_2d<T>() const {return dl32::point_2d<T>( this->x , this->y ); }
    
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

	using vector_2di = dl32::vector_2d<int>;    ///< Alias for integral 2d vectors.
	using vector_2df = dl32::vector_2d<float>;  ///< Alias for single-precision 2d vectors.
	using vector_2dd = dl32::vector_2d<double>; ///< Alias for double-precision 2d vectors.
}

#endif	/* DL32NEWMATH_H */

