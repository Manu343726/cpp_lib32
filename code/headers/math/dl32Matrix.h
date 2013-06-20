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

template<typename T , unsigned int WIDTH , unsigned int HEIGHT>
class dl32Matrix : public dl32Array<T,HEIGHT,WIDTH> , //Ojo: Los arrays bidimensionales son arrays de arrays, es decir, arrays de filas.
                   public dl32BasicAlgebraHelper<dl32Matrix<T,WIDTH,HEIGHT>>,                           //matrix + matrix , matrix-matrix
                   public dl32MultiplicationHelper<dl32Matrix<T,WIDTH,HEIGHT>,T>,                       //matrix * scalar
                   public dl32DivisionHelper<dl32Matrix<T,WIDTH,HEIGHT>,T>,                             //matrix / scalar
                   public dl32MultiplicationHelper<dl32Array<T,WIDTH,HEIGHT>,dl32Array<T,HEIGHT,WIDTH>> //matrix * matrix (Matrices nxm solo se pueden multiplicar con matrices mxn)
{
    
    dl32Matrix<T,WIDTH,HEIGHT>& operator+=(const dl32Matrix<T,WIDTH,HEIGHT>& other)
    {
        for(unsigned int i = 0 ; i < HEIGHT ; ++i)
            for(unsigned int j = 0 ; j < WIDTH ; ++j)
                (*this)[i][j] += other[i][j];
                
        return *this;
    }
    
    dl32Matrix<T,WIDTH,HEIGHT>& operator-=(const dl32Matrix<T,WIDTH,HEIGHT>& other)
    {
        for(unsigned int i = 0 ; i < HEIGHT ; ++i)
            for(unsigned int j = 0 ; j < WIDTH ; ++j)
                (*this)[i][j] -= other[i][j];
                
        return *this;
    }
    
    dl32Matrix<T,WIDTH,HEIGHT>& operator*=(const T& scalar)
    {
        for(unsigned int i = 0 ; i < HEIGHT ; ++i)
            for(unsigned int j = 0 ; j < WIDTH ; ++j)
                (*this)[i][j] *= scalar;
                
        return *this;
    }
    
    dl32Matrix<T,WIDTH,HEIGHT>& operator/=(const T& scalar)
    {
        for(unsigned int i = 0 ; i < HEIGHT ; ++i)
            for(unsigned int j = 0 ; j < WIDTH ; ++j)
                (*this)[i][j] /= scalar;
                
        return *this;
    }
};

#endif	/* DL32MATRIX_H */

