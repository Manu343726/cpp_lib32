/* 
 * File:   dl32Matrix.h
 * Author: Manu343726
 *
 * Created on 19 de junio de 2013, 13:48
 */

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
    
    dl32Matrix<T,WIDTH,HEIGHT>& operator*=(const T& other)
    {
        for(unsigned int i = 0 ; i < HEIGHT ; ++i)
            for(unsigned int j = 0 ; j < WIDTH ; ++j)
                (*this)[i][j] *= other;
                
        return *this;
    }
    
    dl32Matrix<T,WIDTH,HEIGHT>& operator/=(const T& other)
    {
        for(unsigned int i = 0 ; i < HEIGHT ; ++i)
            for(unsigned int j = 0 ; j < WIDTH ; ++j)
                (*this)[i][j] /= other;
                
        return *this;
    }
};

#endif	/* DL32MATRIX_H */

