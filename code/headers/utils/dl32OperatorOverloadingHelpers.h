/* 
 * File:   dl32OperatorOverloadingHelpers.h
 * Author: Manu343726
 *
 * Created on 25 de mayo de 2013, 13:14
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

#ifndef DL32OPERATOROVERLOADINGHELPERS_H
#define	DL32OPERATOROVERLOADINGHELPERS_H

/****************************************************************************
 * This header provides a set of operator overloading helpers based on CRTP *
 * (Specifically the Barton-Nackman trick)                                  *
 *                                                                          *
 * This helpers provide complete operator overloading based on a specific   *
 * operator that must be implemented by the user of the helper (The class   *
 * that inherits from the helper).                                          *
 * For example: equality_ops provides an implementation of inequality       *
 * ( operator!= ) based on the equality criteria provided by the user       *
 * ( operator== ).                                                          *
 *                                                                          *
 * The different set of operators are implemented in an easily-inlineable   *
 * way*. In addition, the arithmetic operators are implemented using a      *
 * NRVO-friendly implementation (Copy, execute, and return copy).           *
 *                                                                          *
 * NOTE: Note that the inline specifier has nothing to do with the compiler *
 * optimization known as "inlining", its only a linkage specifier.          *
 * The compiler is free to decide what functions have be inlined and what   *
 * have not. Manual aided optimizations (Such as the "register" storage     *
 * specifier) are completely igonered nowadays (Modern compilers are better *
 * than humans for that) and commonly deprecated.                           *
 ***************************************************************************/

namespace dl32
{
	namespace { struct binary_sentinel {}; }

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This helper provides inequelity criteria for types T vs U based on user-defined equality criteria
	///        for types T vs U.
	///        Also, this helper provides the complete set of inverse comparisons to ensure symmetry.
	///
	/// @tparam T Type of the first element of the equality test.
	/// @tparam U Type of the first element of the equality test. If is not specified, the operators are
	///           generated for T vs T tests only (See template specialization).
	///
	/// @author	Manu343726
	///
	/// @details An implementation of the operator==(const T& t , const U& u) overload must exist.
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T , typename U = binary_sentinel>
	struct equality_ops
	{ 
		friend bool operator!=(const T& t , const T& u) { return !(t == u); }
    
		friend bool operator==(const U& u , const T& t) {return t == u;}
		friend bool operator!=(const U& u , const T& t) { return t != u; }
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This helper provides inequelity criteria for type T based on user defined equality criteria 
	///        for type T.
	///
	/// @tparam T Type of the first element of the equality test.
	/// @tparam U Type of the first element of the equality test. If is not specified, the operators are
	///           generated for T vs T tests only (See template specialization).
	///
	/// @author	Manu343726
	///
	/// @details An implementation of the operator==(const T& t , const T& u) overload must exist.
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	struct equality_ops<T,binary_sentinel>
	{
		friend bool operator!=(const T& t1 , const T& t2) { return !(t1 == t2); }
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This helper provides ccmplete comparison criteria for types T vs U based on user-defined 
	///        bigger-than and less-than criteria for types T vs U.
	///        Also, this helper provides the complete set of inverse comparisons to ensure symmetry.
	///
	/// @tparam T Type of the first element of the comparison.
	/// @tparam U Type of the first element of the comparison. If is not specified, the operators are
	///           generated for T vs T comparisons only (See template specialization).
	///
	/// @author	Manu343726
	///
	/// @details An implementation of operator>(const T& t , const U& u) and 
	///          operator==(const T& t , const U& u) overloads must exist.
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T , typename U = binary_sentinel>
	struct comparison_ops
	{
		friend bool operator<=(const T& t , const U& u) { return !(t > u); }
		friend bool operator< (const T& t , const U& u) { return !(t > u || t == u); }
		friend bool operator>=(const T& t , const U& u) { return !(t < u); }  
    
		friend bool operator> (const U& u , const T& t) { return t < u; }
		friend bool operator<=(const U& u , const T& t) { return !(u > t); }
		friend bool operator< (const U& u , const T& t) { return t > u; }
		friend bool operator>=(const U& u , const T& t) { return !(t < u); }
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This helper provides complete comparison criteria for type T based on userdefined biger-than 
	///        criteria for type T.
	///
	/// @tparam T Type of the first element of the comparison.
	/// @tparam U Type of the first element of the comparison. If is not specified, the operators are
	///           generated for T vs T comparisons only (See template specialization).
	///
	/// @author	Manu343726
	///
	/// @details An implementation of operator>(const T& t1 , const T& t2) overload must exist.
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	struct comparison_ops<T,binary_sentinel>
	{
		friend bool operator<=(const T& t1 , const T& t2) { return !(t1 > t2); }
		friend bool operator< (const T& t1 , const T& t2) { return t2 > t1; }
		friend bool operator>=(const T& t1 , const T& t2) { return !(t1 < t2); }   
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This helper provides the complete set of comparison operators for the specified types (T vs T,
	///        or T vs U/U vs T).
	///
	/// @author	Manu343726
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T , typename U = binary_sentinel>
	struct complete_comparison_ops : public dl32::eqality_ops<T,U> , 
									 public dl32::comparison_ops<T,U> 
	{};

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This helper provides binary addition operator (T operator+(const T& t , const U& u) based on 
	///        user-defined autoincrement operator ( T& T::operator+=(const U& u) ).
	///
	/// @tparam T Type of the left side of the addition. This is the type that must provide the operator+=
	///           implementation.
	/// @tparam U Type of the right side of the addition. 
	/// @tparam ENABLE_SYMMETRY If is set to true, the helper interprets that the addition operation is
	///                         symmetric for the specified types and provides the corresponding operator+ 
	///                         additional overload.
	///
	/// @author	Manu343726
	///
	/// @details User class (T) must implement T& T::operator+=(const U& u).
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T , typename U = binary_sentinel , bool ENABLE_SYMMETRY = true>
	struct addition_op
	{
		friend T operator+(const T& t , const U& u)
		{
			T t2( t );
			t2 += u;
			return t2;
		}
    
		template<bool SFINAE_BRIDGE = ENABLE_SYMMETRY>
		friend typename std::enable_if<SFINAE_BRIDGE,T>::type
		operator+(const U& u , const T& t)
		{
			return t+u;
		}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This helper provides binary substraction operator (T operator-(const T& t , const T& u) based on 
	///        user-defined autodecrement operator ( T& operator-=(const U& u) ).
	///
	/// @tparam T Type of the left side of the substraction. This is the type that must provide the operator-=
	///           implementation.
	/// @tparam U Type of the right side of the substraction. 
	/// @tparam ENABLE_SYMMETRY If is set to true, the helper interprets that the substraction operation is
	///                         symmetric for the specified types and provides the corresponding operator- 
	///                         additional overload.
	///
	/// @author	Manu343726
	///
	/// @details User class (T) must implement T& T::operator-=(const U& u).
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T , typename U = binary_sentinel , bool ENABLE_SYMMETRY = false>
	struct substraction_op
	{
		friend T operator-(const T& t , const U& u)
		{
			T t2( t );
			t2 -= u;
			return t2;
		}
    
		template<bool SFINAE_BRIDGE = ENABLE_SYMMETRY>
		friend typename std::enable_if<SFINAE_BRIDGE,T>::type
		operator-(const U& u , const T& t)
		{
			return t-u;
		}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This helper provides binary multiplication operator (T operator*(const T& t , const T& u) based on 
	///        user-defined automultiplication operator ( T& operator*=(const U& u) ).
	///
	/// @tparam T Type of the left side of the multiplication. This is the type that must provide the operator+=
	///           implementation.
	/// @tparam U Type of the right side of the multiplication. 
	/// @tparam ENABLE_SYMMETRY If is set to true, the helper interprets that the multiplication operation is
	///                         symmetric for the specified types and provides the corresponding operator* 
	///                         additional overload.
	///
	/// @author	Manu343726
	///
	/// @details User class (T) must implement T& T::operator*=(const U& u).
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T , typename U = binary_sentinel , bool ENABLE_SYMMETRY = true>
	struct multiplication_op
	{
		friend T operator*(const T& t , const U& u)
		{
			T t2( t );
			t2 *= u;
			return t2;
		}
    
		template<bool SFINAE_BRIDGE = ENABLE_SYMMETRY>
		friend typename std::enable_if<SFINAE_BRIDGE,T>::type
		operator*(const U& u , const T& t)
		{
			return t*u;
		}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This helper provides binary division operator (T operator/(const T& t , const U& u) based on 
	///        user-defined autoincrement operator ( T& operator/=(const U& u) ).
	///
	/// @tparam T Type of the left side of the division. This is the type that must provide the operator+=
	///           implementation.
	/// @tparam U Type of the right side of the division. 
	/// @tparam ENABLE_SYMMETRY If is set to true, the helper interprets that the division operation is
	///                         symmetric for the specified types and provides the corresponding operator/ 
	///                         additional overload.
	///
	/// @author	Manu343726
	///
	/// @details User class (T) must implement T& T::operator/=(const U& u).
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T , typename U = binary_sentinel , bool ENABLE_SYMMETRY = false>
	struct division_op
	{
		friend T operator/(const T& t , const U& u)
		{
			T t2( t );
			t2 /= u;
			return t2;
		}
    
		template<bool SFINAE_BRIDGE = ENABLE_SYMMETRY>
		friend typename std::enable_if<SFINAE_BRIDGE,T>::type
		operator/(const U& u , const T& t)
		{
			return t/u;
		}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This helper provides binary addition operator (T operator+(const T& t1 , const T& t2) based on 
	///        user-defined autoincrement operator ( T& operator+=(const T& t) ).
	///
	/// @tparam T Type of the left side of the addition. This is the type that must provide the operator+=
	///           implementation.
	/// @tparam U Type of the right side of the addition. 
	/// @tparam ENABLE_SYMMETRY If is set to true, the helper interprets that the addition operation is
	///                         symmetric for the specified types and provides the corresponding operator+ 
	///                         additional overload.
	///
	/// @author	Manu343726
	///
	/// @details User class (T) must implement T& T::operator+=(const T& t).
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T , bool ENABLE_SYMMETRY>
	struct addition_op<T,binary_sentinel,ENABLE_SYMMETRY>
	{
		friend T operator+(const T& t1 , const T& t2)
		{
			T t3( t1 );
			t3 += t2;
			return t3;
		}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This helper provides binary substraction operator (T operator-(const T& t1 , const T& t2) based on 
	///        user-defined autodecrement operator ( T& operator-=(const T& t) ).
	///
	/// @tparam T Type of the left side of the substraction. This is the type that must provide the operator-=
	///           implementation.
	/// @tparam U Type of the right side of the substraction. 
	/// @tparam ENABLE_SYMMETRY If is set to true, the helper interprets that the substraction operation is
	///                         symmetric for the specified types and provides the corresponding operator- 
	///                         additional overload.
	///
	/// @author	Manu343726
	///
	/// @details User class (T) must implement T& T::operator-=(const T& t).
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T , bool ENABLE_SYMMETRY>
	struct substraction_op<T,binary_sentinel,ENABLE_SYMMETRY>
	{
		friend T operator-(const T& t1 , const T& t2)
		{
			T t3( t1 );
			t3 -= t2;
			return t3;
		}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This helper provides binary multiplication operator (T operator*(const T& t1 , const T& t2) based on 
	///        user-defined automultiplication operator ( T& operator*=(const T& t) ).
	///
	/// @tparam T Type of the left side of the multiplication. This is the type that must provide the operator+=
	///           implementation.
	/// @tparam U Type of the right side of the multiplication. 
	/// @tparam ENABLE_SYMMETRY If is set to true, the helper interprets that the multiplication operation is
	///                         symmetric for the specified types and provides the corresponding operator* 
	///                         additional overload.
	///
	/// @author	Manu343726
	///
	/// @details User class (T) must implement T& T::operator*=(const T& t).
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T , bool ENABLE_SYMMETRY>
	struct multiplication_op<T,binary_sentinel,ENABLE_SYMMETRY>
	{
		friend T operator*(const T& t1 , const T& t2)
		{
			T t3( t1 );
			t3 *= t2;
			return t3;
		}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This helper provides binary division operator (T operator/(const T& t1 , const T& t2) based on 
	///        user-defined autoincrement operator ( T& operator/=(const T& t) ).
	///
	/// @tparam T Type of the left side of the division. This is the type that must provide the operator+=
	///           implementation.
	/// @tparam U Type of the right side of the division. 
	/// @tparam ENABLE_SYMMETRY If is set to true, the helper interprets that the division operation is
	///                         symmetric for the specified types and provides the corresponding operator/ 
	///                         additional overload.
	///
	/// @author	Manu343726
	///
	/// @details User class (T) must implement T& T::operator/=(const T& t).
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T , bool ENABLE_SYMMETRY>
	struct division_op<T,binary_sentinel,ENABLE_SYMMETRY>
	{
		friend T operator/(const T& t1 , const T& t2)
		{
			T t3( t1 );
			t3 /= t2;
			return t3;
		}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This helper provides the set of basic algebra operators (Addition and substraction) for types
	///        T and U.
	///
	/// @author	Manu343726
	///
	/// @details User class (T) must implement operator+=(const U&) and operator-=(const U&).
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T , typename U = dl32NoType , bool ENABLE_SYMMETRY = false>
	struct basic_algebra_ops : public addition_op<T,U,ENABLE_SYMMETRY> , 
							   public substraction_op<T,U,ENABLE_SYMMETRY>
	{};

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This helper provides the set of algebra operators (Addition, substraction, multiplication,
	///        and division) for types T and U.
	///
	/// @author	Manu343726
	///
	/// @details User class (T) must implement T::operator+=(const U&), T::operator-=(const U&), T::operator+=(const U&),
	///          and T::operator/=(const U&).
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T , typename U = dl32NoType , bool ENABLE_SYMMETRY = false>
	struct algebra_ops : public addition_op<T,U,ENABLE_SYMMETRY> , 
						 public substraction_op<T,U,ENABLE_SYMMETRY> , 
						 public multiplication_op<T,U,ENABLE_SYMMETRY> , 
						 public division_op<T,U,ENABLE_SYMMETRY>
	{};

}
#endif	/* DL32OPERATOROVERLOADINGHELPERS_H */

