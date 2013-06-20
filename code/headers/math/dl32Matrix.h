/* 
 * File:   dl32Matrix.h
 * Author: Manu343726
 *
 * Created on 19 de junio de 2013, 13:48
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

#ifndef DL32MATRIX_H
#define	DL32MATRIX_H

#include "dl32OperatorOverloadingHelpers.h"
#include "dl32MetaprogrammingLibrary.h"
#include "dl32Exceptions.h"

DL32EXCEPTION_SUBCLASS_NODOC(dl32InvalidMatrixOperationException)

//Forwrad declaration for submatrix:
template<typename T , unsigned int ROWS , unsigned int COLUMNS>
class dl32Matrix;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This type stores an interval of rows and columns of a matrix.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
struct dl32MatrixInterval : public dl32ComparisonHelpers<dl32MatrixInterval> // Binary operators !=,<,>=,<=.
{
    unsigned int begin_row;
    unsigned int end_row;
    unsigned int begin_column;
    unsigned int end_column;
    
    dl32MatrixInterval() : begin_row( 0 ) , begin_column( 0 ) , end_row( 0 ) , end_column( 0 ) {}
    
    dl32MatrixInterval( unsigned int begin_r , unsigned int begin_c , unsigned int end_r , unsigned int end_c ) : begin_row( begin_r ) , begin_column( begin_c ) , end_row( end_r ) , end_column( end_c )
    {
        if( begin_row > end_row || begin_column > end_column ) throw dl32OutOfRangeException("Invalid matrix interval. The beginning must be less than or equal to the end");
    }
    
    dl32MatrixInterval( const dl32MatrixInterval& other ) : begin_row( other.begin_row ) , begin_column( other.begin_column ) , end_row( other.end_row ) , end_column( other.end_column )
    {
        if( begin_row > end_row || begin_column > end_column ) throw dl32OutOfRangeException("Invalid matrix interval. The beginning must be less than or equal to the end");
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets the number of rows of this matrix interval.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned int rows() const { return (end_row - begin_row) + 1; }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets the number of columns of this matrix interval.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned int columns() const { return (end_column - begin_column) + 1; }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if this interval is square.
    /// @details A matrix interval is square if has the same number of rows and of columns.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    bool is_square() const { return rows() == columns(); }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets the number of elements holded in the matrix interval
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned int area() const { return columns() * rows(); }
    
    bool operator==(const dl32MatrixInterval& other) const
    {
        return begin_row == other.begin_row && begin_column == other.begin_column && end_row == other.end_row && end_column == other.end_column;
    }
    
    bool operator>(const dl32MatrixInterval& other) const
    {
        return area() > other.area();
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if a specified matrix interval is valid. 
    /// @details A matrix interval is valid if the beginning is less than or equal to the end. 
    ///
    /// @author	Manu343726
    ///
    /// @return Returns true if begin_row is less or equal to end_row and begin_column is less or equal 
    ///         to end_column. Returns false otherwise.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool is_valid(const dl32MatrixInterval& range) 
    {
        return range.begin_row <= range.end_row && range.begin_column <= range.end_column;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if the matrix interval is valid. 
    /// @details A matrix interval is valid if the beginning is less than or equal to the end. 
    ///
    /// @author	Manu343726
    ///
    /// @return Returns true if begin_row is less or equal to end_row and begin_column is less or equal 
    ///         to end_column. Returns false otherwise.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    bool is_valid() const { return is_valid( *this ); }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if an addition operation between two matrix intervals is valid. 
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool valid_addition(const dl32MatrixInterval& i1 , const dl32MatrixInterval& i2)
    {
        return i1.rows() == i2.rows() && i1.columns() == i2.columns();
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if an addition operation between two matrix intervals is valid. 
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool valid_substraction(const dl32MatrixInterval& i1 , const dl32MatrixInterval& i2)
    {
        return valid_addition(i1,i2);
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if an addition operation between two matrix intervals is valid. 
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool valid_multiplication(const dl32MatrixInterval& i1 , const dl32MatrixInterval& i2)
    {
        return i1.columns() == i2.rows();
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if an division operation between two matrix intervals is valid. 
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool valid_division(const dl32MatrixInterval& i1 , const dl32MatrixInterval& i2)
    {
        /* 
         * La división de matrices se implementa como el producto de la primera matriz con la inversa de la segunda.
         * Solo las matrices cuadradas pueden ser invertibles (Pueden ser, no tiene por que), y esto sumado a los 
         * criterios necesarios para la multiplicación, implica que solo se pueden dividir entre si matrices cuadradas 
         * del mismo tamaño.
         */ 
        return i1.is_square() && i1.rows() == i2.rows();
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This type represents a submatrix.
/// @details A submatrix holds a reference or a copy of a given matrix.
///          It can be used to do aritmetic operations as if the submatrix was a common matrix, but
///          that operations only will be computed using THE INTERVAL OF THE ORIGINAL MATRIX THAT THE 
///          SUBMATRIX HOLDS.
///
/// @author	Manu343726
///
/// @tparam ORIGINAL_MATRIX_TYPE The type of the underlying matrix used by the submatrix.
/// @tparam USE_MATRIX_REFERENCE If is set to true, the submatrix uses a reference to the original matrix,
///         instaed of a copy. This allows to do operations in a specific interval of a matrix.
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ORIGINAL_MATRIX_TYPE , bool USE_MATRIX_REFERENCE = false>
class dl32SubMatrix;

template<typename T , unsigned int MATRIX_ROWS , unsigned int MATRIX_COLUMNS , bool USE_MATRIX_REFERENCE>
class dl32SubMatrix<dl32Matrix<T,MATRIX_ROWS,MATRIX_COLUMNS>,USE_MATRIX_REFERENCE> : public dl32BasicAlgebraHelper<dl32SubMatrix<dl32Matrix<T,MATRIX_ROWS,MATRIX_COLUMNS>,USE_MATRIX_REFERENCE>>,     //matrix + matrix , matrix-matrix
                                                                                     public dl32MultiplicationHelper<dl32SubMatrix<dl32Matrix<T,MATRIX_ROWS,MATRIX_COLUMNS>,USE_MATRIX_REFERENCE>,T>, //matrix * scalar
                                                                                     public dl32DivisionHelper<dl32SubMatrix<dl32Matrix<T,MATRIX_ROWS,MATRIX_COLUMNS>,USE_MATRIX_REFERENCE>,T>        //matrix / scalar 
{
public:
    using matrix_type = dl32Matrix<T,MATRIX_ROWS,MATRIX_COLUMNS>; ///< Gets the type of the underlying matrix.
    
private:
    dl32MakeReferenceIf<USE_MATRIX_REFERENCE,matrix_type> _underlying_matrix;
    const dl32MatrixInterval _interval;
    
public:
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Creates a submatrix of the specified matrix (The submatrix fits the entire matrix).
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32SubMatrix(dl32MakeConstIf<!USE_MATRIX_REFERENCE,matrix_type&> matrix) : _underlying_matrix( matrix ) , _interval( 0 , 0 , MATRIX_ROWS-1 , MATRIX_COLUMNS-1 ) {}
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Creates a submatrix from the specified interval of a matrix.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32SubMatrix(dl32MakeConstIf<!USE_MATRIX_REFERENCE,matrix_type&> matrix , unsigned int begin_row , unsigned int begin_column , unsigned int end_row , unsigned int end_column) : _underlying_matrix( matrix ) , _interval( begin_row , begin_column , end_row , end_column )
    {
        if( end_row >= MATRIX_ROWS && end_column >= MATRIX_COLUMNS ) throw dl32OutOfRangeException( "The submatrix interval is out of range");
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Creates a submatrix from the specified interval of a matrix.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32SubMatrix(dl32MakeConstIf<!USE_MATRIX_REFERENCE,matrix_type&> matrix , const dl32MatrixInterval& interval) : _underlying_matrix( matrix ) , _interval( interval )
    {
        if( _interval.end_row >= MATRIX_ROWS && _interval.end_column >= MATRIX_COLUMNS || !interval.is_valid() ) throw dl32OutOfRangeException( "The submatrix interval is out of range");
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets the submatrix interval.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    const dl32MatrixInterval& get_interval() const { return _interval; }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Sets the value of the element stored at the specified position of the submatrix.
    /// @remarks Coordinates in submatrix space (column 0 is the _interval.begin_column column of the 
    ///          underlying matrix).
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    typename matrix_type::element_type& at(unsigned int row , unsigned int column)
    {
        return _underlying_matrix[_interval.begin_row + row][_interval.begin_column + column];
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets the value of the element stored at the specified position of the submatrix.
    /// @remarks Coordinates in submatrix space (column 0 is the _interval.begin_column column of the 
    ///          underlying matrix).
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    const typename matrix_type::element_type& at(unsigned int row , unsigned int column) const
    {
        return _underlying_matrix[_interval.begin_row + row][_interval.begin_column + column];
    }
    
    template<unsigned int ROWS , unsigned int COLUMNS , bool OTHER_USES_REFRRENCE>
    dl32SubMatrix& operator+=(const dl32SubMatrix<dl32Matrix<T,ROWS,COLUMNS>,OTHER_USES_REFRRENCE>& other)
    {
        if( dl32MatrixInterval::valid_addition(_interval , other._interval) )
        {
            _underlying_matrix.add(other._underlying_matrix,_interval,other._interval);
            return *this;
        }
        else
            throw dl32InvalidMatrixOperationException();
    }
};

const bool reference_submatrix = true;  ///< Boolean constant to make submatrix constructions more readable.
const bool copy_submatrix      = false; ///< Boolean constant to make submatrix constructions more readable.

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 matrix
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T , unsigned int ROWS , unsigned int COLUMNS>
class dl32Matrix : public dl32Array<T,ROWS,COLUMNS> ,
                   public dl32BasicAlgebraHelper<dl32Matrix<T,ROWS,COLUMNS>>,                            //matrix + matrix , matrix-matrix
                   public dl32MultiplicationHelper<dl32Matrix<T,ROWS,COLUMNS>,T>,                        //matrix * scalar
                   public dl32DivisionHelper<dl32Matrix<T,ROWS,COLUMNS>,T>,                              //matrix / scalar
                   public dl32SelectType<ROWS == COLUMNS,dl32DivisionHelper<dl32Matrix<T,ROWS,COLUMNS>>,dl32NoType>::type //matrix / matrix
{
    static_assert(ROWS > 0 && COLUMNS > 0 , "A matrix must have at least onle row and one column");
    
public:
    static const unsigned int rows    = ROWS;    ///< Gets the number of rows of this type of matrix.
    static const unsigned int columns = COLUMNS; ///< Gets the number of columns of this type of matrix.
    
    static const dl32MatrixInterval complete_interval; ///< Gets the interval that fits the complete matrix.
    
    using matrix_type = dl32Matrix<T,ROWS,COLUMNS>; //< Gets an alias to the matrix type.
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if this type of matrix is a square matrix.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    constexpr bool is_square() const { return rows == columns; }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Performs an addition operation between this instance and other matrix. The result is stored 
    ///        at this instance.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void add(const dl32Matrix<T,ROWS,COLUMNS>& other , const dl32MatrixInterval& this_interval = complete_interval , const dl32MatrixInterval& other_interval = complete_interval)
    {
        if( !dl32MatrixInterval::valid_addition(this_interval,other_interval) ) throw dl32InvalidMatrixOperationException();
        
        for(unsigned int i = 0 ; i < this_interval.rows() ; ++i)
            for(unsigned int j = 0 ; j < this_interval.columns() ; ++j)
                (*this)[this_interval.begin_row + i][this_interval.begin_column + j] += other[other_interval.begin_row + i][other_interval.begin_column + j];
    }
    
    dl32Matrix<T,ROWS,COLUMNS>& operator+=(const dl32Matrix<T,ROWS,COLUMNS>& other)
    {
        add(other);
        return *this;
    }
    
    dl32Matrix<T,ROWS,COLUMNS>& operator-=(const dl32Matrix<T,ROWS,COLUMNS>& other)
    {
        for(unsigned int i = 0 ; i < ROWS ; ++i)
            for(unsigned int j = 0 ; j < COLUMNS ; ++j)
                (*this)[i][j] -= other[i][j];
                
        return *this;
    }
    
    dl32Matrix<T,ROWS,COLUMNS>& operator*=(const T& scalar)
    {
        for(unsigned int i = 0 ; i < ROWS ; ++i)
            for(unsigned int j = 0 ; j < COLUMNS ; ++j)
                (*this)[i][j] *= scalar;
                
        return *this;
    }
    
    dl32Matrix<T,ROWS,COLUMNS>& operator/=(const T& scalar)
    {
        for(unsigned int i = 0 ; i < ROWS ; ++i)
            for(unsigned int j = 0 ; j < COLUMNS ; ++j)
                (*this)[i][j] /= scalar;
                
        return *this;
    }
    
    template<unsigned int ROWS1 , unsigned int COLUMNS1 , unsigned int ROWS2 , unsigned int COLUMNS2>
    friend dl32Matrix<T,COLUMNS1,ROWS2> operator*(const dl32Matrix<T,COLUMNS1,ROWS1>& m1 , const dl32Matrix<T,COLUMNS2,ROWS2>& m2)
    {
        static_assert(COLUMNS1 == ROWS2 , "First matrix must have the same number of columns as the number of rows of the second matrix");
        
        dl32Matrix<T,ROWS1,COLUMNS2> result;
        
        for(unsigned int i = 0 ; i < ROWS1 ; ++i)
            for(unsigned int j = 0 ; j < COLUMNS2 ; ++j)
            {
                result[i][j] = 0;
                
                for(unsigned int k = 0 ; k < ROWS2 ; ++k)
                    result[i][j] += m1[i][k] * m2[k][j];
            }
        
        return result; //NRVO, si no move, si no copia. http://stackoverflow.com/questions/4986673/c11-rvalues-and-move-semantics-confusion
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets a submatrix of this matrix from the specified interval.
    ///
    ///@tparam GET_REFERENCE_SUBMATRIX If is set to true, the submatrix is a reference to this matrix.
    ///        If is set to false, the submatrix is an independent copy of this matrix.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    template<bool GET_REFERENCE_SUBMATRIX = false>
    dl32SubMatrix<matrix_type,GET_REFERENCE_SUBMATRIX> get_submatrix(const dl32MatrixInterval& interval)
    {
        return dl32SubMatrix<matrix_type,GET_REFERENCE_SUBMATRIX>(*this,interval);
    }
};

template<typename T , unsigned int ROWS , unsigned int COLUMNS>
const dl32MatrixInterval dl32Matrix<T,ROWS,COLUMNS>::complete_interval = dl32MatrixInterval(0,0,ROWS-1,COLUMNS-1);

#endif	/* DL32MATRIX_H */

