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

#include <vector>
#include <unordered_map>
#include <algorithm>

DL32EXCEPTION_SUBCLASS_NODOC(dl32InvalidMatrixOperationException)

//Forwrad declaration for submatrix:
template<typename T , unsigned int ROWS , unsigned int COLUMNS>
class dl32Matrix;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This type stores an interval of rows and columns of a matrix (DEPRECATED, USING dl32SubMatrixBounds INSTEAD).
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
    
    dl32MatrixInterval( const dl32MatrixInterval&& other ) : begin_row( other.begin_row ) , begin_column( other.begin_column ) , end_row( other.end_row ) , end_column( other.end_column )
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
    /// @param interval Matrix interval to be checked.
    ///
    /// @return Returns true if begin_row is less or equal to end_row and begin_column is less or equal 
    ///         to end_column. Returns false otherwise.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool is_valid(const dl32MatrixInterval& interval) 
    {
        return interval.begin_row <= interval.end_row && interval.begin_column <= interval.end_column;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if a specified matrix interval is valid. 
    /// @details A matrix interval is valid if the beginning is less than or equal to the end. 
    ///
    /// @author	Manu343726
    ///
    /// @param interval Matrix interval to be checked.
    /// @param matrix_rows Number of rows of the matrix.
    /// @param matrix_columns Number of columns of the matrix.
    ///
    /// @return Returns true if begin_row is less or equal to end_row and begin_column is less or equal 
    ///         to end_column, and the rows and columns are bounded in the matrix size. 
    ///         Returns false otherwise.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool is_valid(const dl32MatrixInterval& interval , unsigned int matrix_rows , unsigned int matrix_columns) 
    {
        return interval.begin_row >= 0 && interval.begin_column >= 0 && interval.end_row < matrix_rows && interval.end_column < matrix_columns &&
               interval.begin_row <= interval.end_row && interval.begin_column <= interval.end_column;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if a specified matrix interval is valid. 
    /// @details A matrix interval is valid if the beginning is less than or equal to the end. 
    ///
    /// @author	Manu343726
    ///
    /// @tparam T Elements type of the matrix. This parameter could be deduced by the compiler.
    /// @tparam ROWS Number of rows of the matrix. This parameter could be deduced by the compiler.
    /// @tparam COLUMNS Number of columns of the matrix. This parameter could be deduced by the compiler.
    ///
    /// @param interval Matrix interval to be checked.
    /// @param matrix Matrix that the interval will be checked with.
    ///
    /// @return Returns true if begin_row is less or equal to end_row and begin_column is less or equal 
    ///         to end_column, and the rows and columns are bounded in the matrix size. 
    ///         Returns false otherwise.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename T , unsigned int ROWS , unsigned int COLUMNS>
    static bool is_valid(const dl32MatrixInterval& interval , dl32Matrix<T,ROWS,COLUMNS> matrix) 
    {
        return is_valid(interval,ROWS,COLUMNS);
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if this matrix interval is valid. 
    /// @details A matrix interval is valid if the beginning is less than or equal to the end. 
    ///
    /// @author	Manu343726
    ///
    /// @param matrix_rows Number of rows of the matrix.
    /// @param matrix_columns Number of columns of the matrix.
    ///
    /// @return Returns true if begin_row is less or equal to end_row and begin_column is less or equal 
    ///         to end_column, and the rows and columns are bounded in the matrix size. 
    ///         Returns false otherwise.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    bool is_valid(unsigned int matrix_rows , unsigned int matrix_columns) const
    {
        return is_valid(*this,matrix_rows,matrix_columns);
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if this matrix interval is valid. 
    /// @details A matrix interval is valid if the beginning is less than or equal to the end. 
    ///
    /// @author	Manu343726
    ///
    /// @tparam T Elements type of the matrix. This parameter could be deduced by the compiler.
    /// @tparam ROWS Number of rows of the matrix. This parameter could be deduced by the compiler.
    /// @tparam COLUMNS Number of columns of the matrix. This parameter could be deduced by the compiler.
    ///
    /// @param interval Matrix interval to be checked.
    /// @param matrix Matrix that the interval will be checked with.
    ///
    /// @return Returns true if begin_row is less or equal to end_row and begin_column is less or equal 
    ///         to end_column, and the rows and columns are bounded in the matrix size. 
    ///         Returns false otherwise.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename T , unsigned int ROWS , unsigned int COLUMNS>
    bool is_valid(dl32Matrix<T,ROWS,COLUMNS> matrix) const
    {
        return is_valid(*this,ROWS,COLUMNS);
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if this matrix interval is valid. 
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

using submatrix_map = std::unordered_map<unsigned int , unsigned int>; ///< An alias to the type used to map submatrix coordinates to matrix coordinates.


///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Represents the bounds (The set of rows and columns of the original matrix) that forms part of 
///        a submatrix.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
class dl32SubMatrixBounds : public dl32ComparisonHelpers<dl32SubMatrixBounds>
{
private:
    submatrix_map _rows;
    submatrix_map _columns;
   
public:
    dl32SubMatrixBounds(unsigned int begin_row , unsigned int begin_column , unsigned int end_row , unsigned int end_column)
    {
        _rows.rehash( (end_row - begin_row) + 1 );
        _columns.rehash( (end_column - begin_column) + 1 );
        
        for(unsigned int i = begin_row ; i <= end_row ; ++i)
            _rows[i - begin_row] = i;
        
        for(unsigned int i = begin_column ; i <= end_column ; ++i)
            _columns[i - begin_column] = i;
    }
    
    dl32SubMatrixBounds(const dl32MatrixInterval& interval) : dl32SubMatrixBounds( interval.begin_row , interval.begin_column , interval.end_row , interval.end_column ) {}
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Creates a submatrix bounds with the specified rows and columns.
    ///
    /// @author	Manu343726
    ///
    /// @param rows Set of rows of the original matrix that forms part of the submatrix.
    /// @param columns  Set of columns of the original matrix that forms part of the submatrix.
    /// @param exclude_passed_lines If is set to true, the bounds are filled with the entire matrix lines 
    ///                             (Rows and columns) excluding the passed lines. 
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32SubMatrixBounds(const std::vector<unsigned int>& rows , const std::vector<unsigned int>& columns , bool exclude_passed_lines = false)
    {
        _rows.rehash( rows.size() );
        _columns.rehash( columns.size() );
        
        for(unsigned int i = 0 ; i < rows.size() ; ++i)
            _rows[i] = rows[i];
        
        for(unsigned int i = 0 ; i < columns.size() ; ++i)
            _columns[i] = columns[i];
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets the number of rows of this matrix bounds.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned int rows_count() const { return _rows.size(); }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets the number of columns of this matrix bounds.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned int columns_count() const { return _columns.size(); }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if this bounds is square.
    /// @details A matrix bounds is square if has the same number of rows and of columns.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    bool is_square() const { return rows_count() == columns_count(); }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets the number of elements holded in the matrix bounds
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned int area() const { return columns_count() * rows_count(); }
    
    bool operator==(const dl32SubMatrixBounds& other) const
    {
        return _rows == other._rows && _columns == other._columns;
    }
    
    bool operator>(const dl32SubMatrixBounds& other) const
    {
        return area() > other.area();
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if a specified matrix bounds is valid. 
    /// @details A matrix bounds is valid if the associated rows/columns are not out of the size of the original matrix.
    ///
    /// @author	Manu343726
    ///
    /// @param bounds Matrix bounds to be checked.
    /// @param matrix_rows Number of rows of the matrix.
    /// @param matrix_columns Number of columns of the matrix.
    ///
    /// @return Returns true if the rows and columns are bounded in the matrix size. 
    ///         Returns false otherwise.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool is_valid(const dl32SubMatrixBounds& bounds , unsigned int matrix_rows , unsigned int matrix_columns) 
    {
        return std::find_if_not( bounds._rows.begin() , bounds._rows.end()       , [=](const submatrix_map::value_type& pair) { return pair.second < matrix_rows; } )    == bounds._columns.end() &&
               std::find_if_not( bounds._columns.begin() , bounds._columns.end() , [=](const submatrix_map::value_type& pair) { return pair.second < matrix_columns; } ) == bounds._columns.end();
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if a specified matrix bounds is valid. 
    /// @details A matrix bounds is valid if the beginning is less than or equal to the end. 
    ///
    /// @author	Manu343726
    ///
    /// @tparam T Elements type of the matrix. This parameter could be deduced by the compiler.
    /// @tparam ROWS Number of rows of the matrix. This parameter could be deduced by the compiler.
    /// @tparam COLUMNS Number of columns of the matrix. This parameter could be deduced by the compiler.
    ///
    /// @param bounds Matrix bounds to be checked.
    /// @param matrix Matrix that the bounds will be checked with.
    ///
    /// @return Returns true if the rows and columns are bounded in the matrix size. 
    ///         Returns false otherwise.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename T , unsigned int ROWS , unsigned int COLUMNS>
    static bool is_valid(const dl32SubMatrixBounds& bounds , dl32Matrix<T,ROWS,COLUMNS> matrix) 
    {
        return is_valid(bounds,ROWS,COLUMNS);
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if this matrix bounds is valid. 
    /// @details A matrix bounds is valid if the beginning is less than or equal to the end. 
    ///
    /// @author	Manu343726
    ///
    /// @param matrix_rows Number of rows of the matrix.
    /// @param matrix_columns Number of columns of the matrix.
    ///
    /// @return Returns true if begin_row is less or equal to end_row and begin_column is less or equal 
    ///         to end_column, and the rows and columns are bounded in the matrix size. 
    ///         Returns false otherwise.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    bool is_valid(unsigned int matrix_rows , unsigned int matrix_columns) const
    {
        return is_valid(*this,matrix_rows,matrix_columns);
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if this matrix bounds is valid. 
    /// @details A matrix bounds is valid if the beginning is less than or equal to the end. 
    ///
    /// @author	Manu343726
    ///
    /// @tparam T Elements type of the matrix. This parameter could be deduced by the compiler.
    /// @tparam ROWS Number of rows of the matrix. This parameter could be deduced by the compiler.
    /// @tparam COLUMNS Number of columns of the matrix. This parameter could be deduced by the compiler.
    ///
    /// @param bounds Matrix bounds to be checked.
    /// @param matrix Matrix that the bounds will be checked with.
    ///
    /// @return Returns true if begin_row is less or equal to end_row and begin_column is less or equal 
    ///         to end_column, and the rows and columns are bounded in the matrix size. 
    ///         Returns false otherwise.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename T , unsigned int ROWS , unsigned int COLUMNS>
    bool is_valid(dl32Matrix<T,ROWS,COLUMNS> matrix) const
    {
        return is_valid(*this,ROWS,COLUMNS);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if an addition operation between two matrix boundss is valid. 
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool valid_addition(const dl32SubMatrixBounds& i1 , const dl32SubMatrixBounds& i2)
    {
        return i1.rows_count() == i2.rows_count() && i1.columns_count() == i2.columns_count();
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if a substraction operation between two matrix boundss is valid. 
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool valid_substraction(const dl32SubMatrixBounds& i1 , const dl32SubMatrixBounds& i2)
    {      
        return valid_addition(i1,i2);
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if a multiplication operation between two matrix boundss is valid. 
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool valid_multiplication(const dl32SubMatrixBounds& i1 , const dl32SubMatrixBounds& i2)
    {
        return i1.columns_count() == i2.rows_count();
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if a division operation between two matrix boundss is valid. 
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool valid_division(const dl32SubMatrixBounds& i1 , const dl32SubMatrixBounds& i2)
    {
        /* 
         * La división de matrices se implementa como el producto de la primera matriz con la inversa de la segunda.
         * Solo las matrices cuadradas pueden ser invertibles (Pueden ser, no tiene por que), y esto sumado a los 
         * criterios necesarios para la multiplicación, implica que solo se pueden dividir entre si matrices cuadradas 
         * del mismo tamaño.
         */ 
        return i1.is_square() && i1.rows_count() == i2.rows_count();
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Swaps two specified rows of the submatrix.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void swap_rows(unsigned int r1 , unsigned int r2)
    {
        auto value1 = _rows.at( r1 );
        auto value2 = _rows.at( r2 );
        
        _rows[r1] = value2;
        _rows[r2] = value1;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Swaps two specified columns of the submatrix.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void swap_columns(unsigned int c1 , unsigned int c2)
    {
        auto value1 = _rows.at( c1 );
        auto value2 = _rows.at( c2 );
        
        _columns[c1] = value2;
        _columns[c2] = value1;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets the row of the original matrix associated with the specified submatrix row.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned int row(unsigned int index) const
    { 
        return _rows.at(index);
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets the column of the original matrix associated with the specified submatrix column.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned int column(unsigned int index) const
    { 
        return _columns.at(index);
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Erases the specified row from the submatrix.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned int erase_row(unsigned int index)
    { 
        return _rows.erase( index );
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Erases the specified column from the submatrix.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned int erase_column(unsigned int index)
    { 
        return _columns.erase( index );
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Returns the map used to associate submatrix rows with original matrix rows (True rows)
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    const submatrix_map& rows_map() const { return _rows; }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Returns the map used to associate submatrix columns with original matrix columns (True columns)
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    const submatrix_map& columns_map() const { return _columns; }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Set of supported matrix fluent operations.
/// @details This operations include matrix vs matrix operations and 
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
enum class dl32FluentMatrixOperation
{
    FLUSH,
    ASSIGN,
    ADDITION,
    SUBSTRACTION,
    MULTIPLICATION,
    DIVISION
};

const dl32FluentMatrixOperation MATRIX_OP_FLUSH  = dl32FluentMatrixOperation::FLUSH;          ///< Constant for easy-to-write flush operations.
const dl32FluentMatrixOperation MATRIX_OP_ASSIGN = dl32FluentMatrixOperation::ASSIGN;         ///< Constant for easy-to-write assign operations.
const dl32FluentMatrixOperation MATRIX_OP_ADD    = dl32FluentMatrixOperation::ADDITION;       ///< Constant for easy-to-write adition operations.
const dl32FluentMatrixOperation MATRIX_OP_SUB    = dl32FluentMatrixOperation::SUBSTRACTION;   ///< Constant for easy-to-write substraction operations.
const dl32FluentMatrixOperation MATRIX_OP_MUL    = dl32FluentMatrixOperation::MULTIPLICATION; ///< Constant for easy-to-write multiplication operations.
const dl32FluentMatrixOperation MATRIX_OP_DIV    = dl32FluentMatrixOperation::DIVISION;       ///< Constant for easy-to-write division operations.

template<typename T , typename MATRIX_TYPE>
class dl32FluentMatrixOperator
{
private:
    enum class operationStatus { WAITING_OPERAND , WAITING_OPERATOR };
    
    /* NOTA SOBRE IMPLEMENTACIÓN: ¿Por qué un puntero y no una referencia?
     * *******************************************************************
     * 
     * Esta clase (dl32FluentMatrixOperation) está diseñada para ser utilizada como policy class
     * dentro de una clase que implemente una matriz. Así, una instacia de ésta clase forma parte de
     * los atributos privados de dicha implementación de matriz.
     * 
     * El problema viene debido a que de esa manera las dos clases tienen una referencia en bucle 
     * (La matrix guarda una referencia (instancia) de dl32FluentMatrixOperation, y dl32FluentMatrixOperation 
     * guarda una referencia de la matriz (dl32FluentMatrixOperation::_current_matrix)).
     * Si se utlizara una referencia a la matriz, las llamadas al constructor de copia/operador
     * de asignación de la matriz generarían una recursividad infinita, ya que al asignar la matriz se 
     * aigna el fluent_operator, que a su vez intenta asignar su _current_matrix (Y así sucesivamente).
     * 
     * Para solucionarlo, se utiliza un raw-pointer en lugar de una referencia en _current_matrix.
     */
    
    MATRIX_TYPE*    _current_matrix;
    operationStatus _current_status;
    dl32FluentMatrixOperation _operation;
    
public:
    
    dl32FluentMatrixOperator( MATRIX_TYPE* matrix ) : _current_matrix( matrix ) , _current_status( operationStatus::WAITING_OPERATOR ) {}
    
    dl32FluentMatrixOperator( const dl32FluentMatrixOperator<T,MATRIX_TYPE>& other ) : _current_matrix( other._current_matrix ) ,  _current_status( other._current_status ) , _operation( other._operation ) {}
    
    
    dl32FluentMatrixOperator<T,MATRIX_TYPE>& operator=(const dl32FluentMatrixOperator<T,MATRIX_TYPE>& other )
    {
        _current_status = other._current_status;
        _operation = other._operation;
        
        return *this;
    }
    
    friend dl32FluentMatrixOperator<T,MATRIX_TYPE>& operator<<(dl32FluentMatrixOperator<T,MATRIX_TYPE>& fop , const T& operand)
    {
        if( fop._current_status == operationStatus::WAITING_OPERATOR ) throw dl32InvalidMatrixOperationException("Operator expected, got an operand");
        
        switch( fop._operation )
        {
            case dl32FluentMatrixOperation::ASSIGN:       fop._current_matrix->fill( operand );  fop._current_status = operationStatus::WAITING_OPERATOR; return fop;
            case dl32FluentMatrixOperation::ADDITION:     throw dl32InvalidMatrixOperationException("That operation is not supported.");
            case dl32FluentMatrixOperation::SUBSTRACTION: throw dl32InvalidMatrixOperationException("That operation is not supported.");
            case dl32FluentMatrixOperation::MULTIPLICATION: (*fop._current_matrix) *= operand; fop._current_status = operationStatus::WAITING_OPERATOR; return fop;
            case dl32FluentMatrixOperation::DIVISION:       (*fop._current_matrix) /= operand; fop._current_status = operationStatus::WAITING_OPERATOR; return fop;
        }
    }
    
    friend dl32FluentMatrixOperator<T,MATRIX_TYPE>& operator<<(dl32FluentMatrixOperator<T,MATRIX_TYPE>& fop , const MATRIX_TYPE& operand)
    {
        if( fop._current_status == operationStatus::WAITING_OPERATOR ) throw dl32InvalidMatrixOperationException("Operator expected, got an operand");
        
        switch( fop._operation )
        {
            case dl32FluentMatrixOperation::ASSIGN:       (*fop._current_matrix)  = operand; fop._current_status = operationStatus::WAITING_OPERATOR; return fop;
            case dl32FluentMatrixOperation::ADDITION:     (*fop._current_matrix) += operand; fop._current_status = operationStatus::WAITING_OPERATOR; return fop;
            case dl32FluentMatrixOperation::SUBSTRACTION: (*fop._current_matrix) -= operand; fop._current_status = operationStatus::WAITING_OPERATOR; return fop;
            case dl32FluentMatrixOperation::MULTIPLICATION: throw dl32InvalidMatrixOperationException("That operation is not supported."); /* (PROVISIONAL) fop._current_matrix *= operand; fop._current_status = operationStatus::WAITING_OPERATOR; return fop; */
            case dl32FluentMatrixOperation::DIVISION:       throw dl32InvalidMatrixOperationException("That operation is not supported."); /* (PROVISIONAL) fop._current_matrix /= operand; fop._current_status = operationStatus::WAITING_OPERATOR; return fop; */
        }
    }
    
    friend dl32FluentMatrixOperator<T,MATRIX_TYPE>& operator<<(dl32FluentMatrixOperator<T,MATRIX_TYPE>& fop , dl32FluentMatrixOperation operation)
    {
        if( operation == dl32FluentMatrixOperation::FLUSH)
        {
            fop._current_status = operationStatus::WAITING_OPERATOR;
            return fop;
        }
        
        if( fop._current_status == operationStatus::WAITING_OPERAND ) throw dl32InvalidMatrixOperationException("Operand expected, got an operator");
        
        fop._operation = operation;
        fop._current_status = operationStatus::WAITING_OPERAND;
        return fop;
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
    using my_type = dl32SubMatrix<dl32Matrix<T,MATRIX_ROWS,MATRIX_COLUMNS>,USE_MATRIX_REFERENCE>;
    
private:
    dl32MakeReferenceIf<USE_MATRIX_REFERENCE,matrix_type> _underlying_matrix;
    dl32SubMatrixBounds _bounds;

    dl32FluentMatrixOperator<T,my_type> _fluent_operator;
    
public:
    
    //Proxy class for submatrix row indexing
    class Proxy
    {
    private:
        matrix_type& _matrix;
        const unsigned int _row;
        const submatrix_map& _columns;
        
    public:
        Proxy(matrix_type& matrix , const unsigned int row , const submatrix_map& columns) : _matrix( matrix ) , _row( row ) , _columns( columns ) {}
        
        typename matrix_type::element_type& operator[](unsigned int index)
        {
            return _matrix[_row][_columns.at(index)];
        }
        
        const typename matrix_type::element_type& operator[](unsigned int index) const
        {
            return _matrix[_row][_columns.at(index)];
        }
    };
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Creates a submatrix of the specified matrix (The submatrix fits the entire matrix).
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32SubMatrix(dl32MakeConstIf<!USE_MATRIX_REFERENCE,matrix_type&> matrix) : dl32SubMatrix(matrix,0,0,matrix_type::rows-1,matrix_type::columns-1) {}
    
        ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Creates a submatrix of the specified matrix (The submatrix fits the entire matrix).
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32SubMatrix(dl32MakeConstIf<!USE_MATRIX_REFERENCE,matrix_type&> matrix , const dl32SubMatrixBounds& bounds) : _underlying_matrix( matrix ) , _fluent_operator( this ) , _bounds( bounds ) {}
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Creates a submatrix from the specified bounds of a matrix.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32SubMatrix(dl32MakeConstIf<!USE_MATRIX_REFERENCE,matrix_type&> matrix , unsigned int begin_row , unsigned int begin_column , unsigned int end_row , unsigned int end_column) : _underlying_matrix( matrix ) , _fluent_operator( this ) , _bounds(begin_row,begin_column,end_row,end_column) {}
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Creates a submatrix from the specified bounds of a matrix.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32SubMatrix(dl32MakeConstIf<!USE_MATRIX_REFERENCE,matrix_type&> matrix , const dl32MatrixInterval& interval) : dl32SubMatrix(matrix,interval.begin_row,interval.begin_column,interval.end_row,interval.end_column) {}
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Creates a submatrix from the specified set of rows and columns of the original matrix..
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32SubMatrix(dl32MakeConstIf<!USE_MATRIX_REFERENCE,matrix_type&> matrix , const std::vector<unsigned int>& rows , const std::vector<unsigned int>& columns) : _underlying_matrix( matrix ) , _fluent_operator( this ) , _bounds( rows , columns ) {}
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Sets the value of the element stored at the specified position of the submatrix.
    /// @remarks Coordinates in submatrix space.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    Proxy operator[](unsigned int row)
    {
        return Proxy(_underlying_matrix,row,_bounds.rows_map());
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets the value of the element stored at the specified position of the submatrix.
    /// @remarks Coordinates in submatrix space.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    const Proxy operator[](unsigned int row) const
    {
        return Proxy(_underlying_matrix,row,_bounds.columns_count);
    }
    
    dl32FluentMatrixOperator<T,my_type>& operator<<(dl32FluentMatrixOperation operation)
    {
        return _fluent_operator << operation;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Fills the entire submatrix with the specified value.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void fill(const T& value)
    {
        _underlying_matrix.fill( value , _bounds);
    }
    
    template<unsigned int ROWS , unsigned int COLUMNS , bool OTHER_USES_REFRRENCE>
    my_type& operator+=(const dl32SubMatrix<dl32Matrix<T,ROWS,COLUMNS>,OTHER_USES_REFRRENCE>& other)
    {
        _underlying_matrix.add(other._underlying_matrix,_bounds,other._bounds);
        return *this;
    }
    
    template<unsigned int ROWS , unsigned int COLUMNS , bool OTHER_USES_REFRRENCE>
    my_type& operator-=(const dl32SubMatrix<dl32Matrix<T,ROWS,COLUMNS>,OTHER_USES_REFRRENCE>& other)
    {
        _underlying_matrix.add(other._underlying_matrix,_bounds,other._bounds);
        return *this;
    }
    
    my_type& operator*=(const T& scalar)
    {
        _underlying_matrix.multiply(scalar,_bounds);
        return *this;
    }
    
    my_type& operator/=(const T& scalar)
    {
        _underlying_matrix.divide(scalar,_bounds);
        return *this;
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
    
private:
    bool _check_matrix_operations;
    
    dl32FluentMatrixOperator<T,dl32Matrix<T,ROWS,COLUMNS>> _fluent_operator;
    
public:
    /*********************/
    /* Matrix properties */
    /*********************/
    
    static const unsigned int rows    = ROWS;    ///< Gets the number of rows of this type of matrix.
    static const unsigned int columns = COLUMNS; ///< Gets the number of columns of this type of matrix.

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if this type of matrix is a square matrix.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    constexpr bool is_square() const { return rows == columns; }
    
    static const dl32MatrixInterval complete_interval; ///< Gets the interval that fits the complete matrix.
    static const dl32SubMatrixBounds complete_bounds; ///< Gets the bounds that fits the entire matrix.
    
    using matrix_type = dl32Matrix<T,ROWS,COLUMNS>; //< Gets an alias to the matrix type.
    
    /*********/
    /* ctors */
    /*********/
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Default constructor
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32Matrix(bool check_matrix_operations = false) : _check_matrix_operations( check_matrix_operations ) , _fluent_operator( this ) {}
    
    
    /*****************************************/
    /* Matrix operations validation settings */
    /*****************************************/
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Checks if this matrix performs validation before any algebraic operation.
    ///
    /// @author	Manu343726
    ///
    /// @return Returns true if validation is enabled. Returns false if its disabled.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    bool operations_validation_enabled() const { return _check_matrix_operations; }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Enables matrix operations validation.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void enable_operations_validation() { _check_matrix_operations = true; }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Disanables matrix operations validation.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////    
    void disable_operations_validation() { _check_matrix_operations = false; }
    
    /**************************/
    /* Common algebraic tools */
    /**************************/
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Fills the specified bounds of the matrix with the same value.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void fill(const T& value , const dl32SubMatrixBounds& bounds = complete_bounds)
    {
        for(unsigned int i = 0 ; i < bounds.rows_count() ; ++i)
            for(unsigned int j = 0 ; j < bounds.columns_count() ; ++j)
                (*this)[bounds.row(i)][bounds.column(j)] = value;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Computes the determinant of the matrix.
    ///
    /// @author	Manu343726
    ///
    /// @param bounds Sets the bounds of the submatrix where the determinant will be computed. Its default
    ///               value is complete_bounds, so the determinant of the entire matrix is computed by
    ///               default.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    T determinant(const dl32SubMatrixBounds& bounds = complete_bounds) /* WARNING: NOT FINISHED */
    {
        if( !bounds.is_square() ) throw dl32InvalidMatrixOperationException();
        
        #define MATRIX(x,y) ( (*this)[bounds.row( (x) )][bounds.column( (y) )] )
        
        switch( columns )
        {
            case 1: return MATRIX(0,0); //Sarrus 1x1
            
            case 2: return ( MATRIX(0,0) * MATRIX(1,1) ) - ( MATRIX(0,1) * MATRIX(1,0) ); //Sarrus 2x2
            
            case 3: return ( MATRIX(0,0) * MATRIX(1,1) * MATRIX(2,2) + MATRIX(0,1) * MATRIX(1,2) * MATRIX(2,0) + MATRIX(1,0) * MATRIX(2,1) * MATRIX(0,2) ) -
                           ( MATRIX(0,2) * MATRIX(1,1) * MATRIX(2,0) + MATRIX(0,1) * MATRIX(1,0) * MATRIX(2,2) + MATRIX(1,2) * MATRIX(2,1) * MATRIX(0,0) ); //Sarrus 3x3        
            default:
                return 0;
        }
    }
    
    /*********************/
    /* Fluent operations */
    /*********************/
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Performs a secuence of fluent algebraic operations.
    /// @details Fluent operations are algebraic operations performed allways by the same matrix as target
    ///          (The result of every computation is stored in the same matrix). 
    ///          dl32FluentOperator class and operator<< overload in matrices provides a way to perform this
    ///          type of operations without the cost of intermediate temporary objects as in common binary
    ///          operators.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32FluentMatrixOperator<T,matrix_type>& operator<<(dl32FluentMatrixOperation operation)
    {
        return _fluent_operator << operation;
    }
    
    /*******************************/
    /* Common algebraic operations */
    /*******************************/
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Performs an addition operation between this instance and other matrix. The result is stored 
    ///        at this instance.
    ///
    /// @author	Manu343726
    ///
    /// @tparam OTHER_ROWS    Number of rows of the other matrix type. This parameter can be deduced by the compiler.
    /// @tparam OTHER_COLUMNS Number of columns of the other matrix type. This parameter can be deduced by the compiler.
    /// 
    /// @param other The matrix that the operation will de performed with. 
    /// @param this_bounds Bounds of this matrix where the operation will be performed. Its default value
    ///        is the entire matrix (dl32Matrix<T,ROWS,COLUMNS>::complete_bounds).
    //  @param other_bounds Bounds of the matrix where the operation will be performed. Its default value
    ///        is the entire matrix (dl32Matrix<T,OTHER_ROWS,OTHER_COLUMNS>::complete_bounds).
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    template<unsigned int OTHER_ROWS,unsigned int OTHER_COLUMNS>
    void add(const dl32Matrix<T,OTHER_ROWS,OTHER_COLUMNS>& other , const dl32SubMatrixBounds& this_bounds = complete_bounds , const dl32SubMatrixBounds& other_bounds = dl32Matrix<T,OTHER_ROWS,OTHER_COLUMNS>::complete_bounds)
    {
        if( (_check_matrix_operations || other._check_matrix_operations) && !dl32SubMatrixBounds::valid_addition(this_bounds,other_bounds) ) throw dl32InvalidMatrixOperationException();
        
        for(unsigned int i = 0 ; i < this_bounds.rows_count() ; ++i)
            for(unsigned int j = 0 ; j < this_bounds.columns_count() ; ++j)
                (*this)[this_bounds.row(i)][this_bounds.column(j)] += other[other_bounds.row(i)][other_bounds.column(j)];
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Performs a substraction operation between this instance and other matrix. The result is stored 
    ///        at this instance.
    ///
    /// @author	Manu343726
    ///
    /// @tparam OTHER_ROWS    Number of rows of the other matrix type. This parameter can be deduced by the compiler.
    /// @tparam OTHER_COLUMNS Number of columns of the other matrix type. This parameter can be deduced by the compiler.
    /// 
    /// @param other The matrix that the operation will de performed with. 
    /// @param this_bounds Bounds of this matrix where the operation will be performed. Its default value
    ///        is the entire matrix (dl32Matrix<T,ROWS,COLUMNS>::complete_bounds).
    //  @param other_bounds Bounds of the matrix where the operation will be performed. Its default value
    ///        is the entire matrix (dl32Matrix<T,OTHER_ROWS,OTHER_COLUMNS>::complete_bounds).
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    template<unsigned int OTHER_ROWS,unsigned int OTHER_COLUMNS>
    void substract(const dl32Matrix<T,OTHER_ROWS,OTHER_COLUMNS>& other , const dl32SubMatrixBounds& this_bounds = complete_bounds , const dl32SubMatrixBounds& other_bounds = dl32Matrix<T,OTHER_ROWS,OTHER_COLUMNS>::complete_bounds)
    {
        if( (_check_matrix_operations || other._check_matrix_operations) && !dl32SubMatrixBounds::valid_addition(this_bounds,other_bounds) ) throw dl32InvalidMatrixOperationException();
        
        for(unsigned int i = 0 ; i < this_bounds.rows_count() ; ++i)
            for(unsigned int j = 0 ; j < this_bounds.columns_count() ; ++j)
                (*this)[this_bounds.row(i)][this_bounds.column(j)] -= other[other_bounds.row(i)][other_bounds.column(j)];
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Performs a multiplication operation between this instance and a scalar value. The result is stored 
    ///        at this instance.
    ///
    /// @author	Manu343726
    /// 
    /// @param scalar The scalar value that the operation will be performed with.
    /// @param this_bounds Bounds of this matrix where the operation will be performed. Its default value
    ///        is the entire matrix (dl32Matrix<T,ROWS,COLUMNS>::complete_bounds).
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void multiply(const T& scalar , const dl32SubMatrixBounds& this_bounds = complete_bounds)
    {
        for(unsigned int i = 0 ; i < this_bounds.rows_count() ; ++i)
            for(unsigned int j = 0 ; j < this_bounds.columns_count() ; ++j)
                (*this)[this_bounds.row(i)][this_bounds.column(j)] *= scalar;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Performs a division operation between this instance and a scalar value. The result is stored 
    ///        at this instance.
    ///
    /// @author	Manu343726
    /// 
    /// @param scalar The scalar value that the operation will be performed with.
    /// @param this_bounds Bounds of this matrix where the operation will be performed. Its default value
    ///        is the entire matrix (dl32Matrix<T,ROWS,COLUMNS>::complete_bounds).
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void divide(const T& scalar , const dl32SubMatrixBounds& this_bounds = complete_bounds)
    {
        if( !this_bounds.is_valid(*this) ) throw dl32InvalidMatrixOperationException();
        
        for(unsigned int i = 0 ; i < this_bounds.rows_count() ; ++i)
            for(unsigned int j = 0 ; j < this_bounds.columns_count() ; ++j)
                (*this)[this_bounds.row(i)][this_bounds.column(j)] /= scalar;
    }
    
    dl32Matrix<T,ROWS,COLUMNS>& operator+=(const dl32Matrix<T,ROWS,COLUMNS>& other)
    {
        add(other);
        return *this;
    }
    
    dl32Matrix<T,ROWS,COLUMNS>& operator-=(const dl32Matrix<T,ROWS,COLUMNS>& other)
    {
        substract(other);
        return *this;
    }
    
    dl32Matrix<T,ROWS,COLUMNS>& operator*=(const T& scalar)
    {
        multiply(scalar);       
        return *this;
    }
    
    dl32Matrix<T,ROWS,COLUMNS>& operator/=(const T& scalar)
    {
        divide(scalar);
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
    /// @brief Gets a submatrix of this matrix from the specified bounds.
    ///
    ///@tparam GET_REFERENCE_SUBMATRIX If is set to true, the submatrix is a reference to this matrix.
    ///        If is set to false, the submatrix is an independent copy of this matrix.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    template<bool GET_REFERENCE_SUBMATRIX = false , typename... TARGS> //Lo bueno de hacerlo así (Una función que le puedes pasar hasta n parámetros de n tipos diferentes) es que con ésta sola función puedo usar todas las sobrecargas del constructor de submatrix.
    dl32SubMatrix<matrix_type,GET_REFERENCE_SUBMATRIX> get_submatrix(const TARGS&... args)
    {
        return dl32SubMatrix<matrix_type,GET_REFERENCE_SUBMATRIX>(*this,args...);
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets a submatrix of this matrix from the specified bounds.
    ///
    ///@tparam GET_REFERENCE_SUBMATRIX If is set to true, the submatrix is a reference to this matrix.
    ///        If is set to false, the submatrix is an independent copy of this matrix.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    template<bool GET_REFERENCE_SUBMATRIX = false> //No se puede inferir el tipo de un initializer_list? http://stackoverflow.com/questions/11921335/use-brace-enclosed-initializer-lists-in-a-variadic-template Tengo que poner una sobrecarga explicitamente
    dl32SubMatrix<matrix_type,GET_REFERENCE_SUBMATRIX> get_submatrix(const std::initializer_list<unsigned int>& rows , const std::initializer_list<unsigned int>& columns)
    {
        //La verdad es que no hacía falta el cast, la conversión de initializer_list a vector es implícita
        return dl32SubMatrix<matrix_type,GET_REFERENCE_SUBMATRIX>(*this, std::vector<unsigned int> { rows },std::vector<unsigned int> { columns });
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets the submatrix that fits the entire matrix..
    ///
    ///@tparam GET_REFERENCE_SUBMATRIX If is set to true, the submatrix is a reference to this matrix.
    ///        If is set to false, the submatrix is an independent copy of this matrix.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    template<bool GET_REFERENCE_SUBMATRIX = false>
    dl32SubMatrix<matrix_type,GET_REFERENCE_SUBMATRIX> get_submatrix()
    {
        return dl32SubMatrix<matrix_type,GET_REFERENCE_SUBMATRIX>(*this, complete_bounds);
    }
};

template<typename T , unsigned int ROWS , unsigned int COLUMNS>
const dl32MatrixInterval dl32Matrix<T,ROWS,COLUMNS>::complete_interval = dl32MatrixInterval(0,0,ROWS-1,COLUMNS-1);

template<typename T , unsigned int ROWS , unsigned int COLUMNS>
const dl32SubMatrixBounds dl32Matrix<T,ROWS,COLUMNS>::complete_bounds = dl32SubMatrixBounds(0,0,ROWS-1,COLUMNS-1);

#endif	/* DL32MATRIX_H */

