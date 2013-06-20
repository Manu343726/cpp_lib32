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

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief cpp_lib32 cpp_lib32 matrix
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
public:
    constexpr bool is_square() const { return ROWS == COLUMNS; }
    
    dl32Matrix<T,ROWS,COLUMNS>& operator+=(const dl32Matrix<T,ROWS,COLUMNS>& other)
    {
        for(unsigned int i = 0 ; i < ROWS ; ++i)
            for(unsigned int j = 0 ; j < COLUMNS ; ++j)
                (*this)[i][j] += other[i][j];
                
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
};

#endif	/* DL32MATRIX_H */

