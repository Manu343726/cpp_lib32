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

#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_NEWMATH

#include "dl32NewMath.h"
#include "dl32Matrix.h"

#include <iostream>
using namespace std;

struct A{};

int main()
{
   dl32Point2Df p1(1,1) , p2(2,2) , p3(3,3);
   dl32Vector2Df v1(1,1) , v2(2,2) , v3(3,3);
   dl32Matrix<int,4,4> m1 , m2 , m3 , m4 , m5;
   
   p3 = p2 + p3;
   v1 = v2 + v3*3;
   
   for(unsigned int i = 0 ; i < 4 ; ++i)
       for(unsigned int j = 0 ; j < 4 ; ++j)
       {
           m1[i][j] = 1;
           m2[i][j] = 1; //Caché catapum!
       }
   
   //Tras inicializarlas de esa manera, m1 y m2 valen:
   /* 1 1 1 1
    * 1 1 1 1
    * 1 1 1 1
    * 1 1 1 1   
    */
   
   //Creamos submatrices de m1 y m2 (Nòtese que es submatriz por referencia, es decir, trabaja sobre la matriz original, no sobre una copia):
   auto sub1 = m1.get_submatrix<reference_submatrix>( {1,2} , {1,2} );//Filas, columnas
   auto sub2 = m2.get_submatrix<reference_submatrix>( 1,1,2,2 );//Fila inicial, columna inicial, fila final, columna final
   sub1 += sub2*3;//Operas como si fueran matrices normales, pero en realidad estás haciendo la operación solo en un trozo (submatriz) de la matriz
   
   //Tras esa operación, m1 vale:
   /* 1 1 1 1
    * 1 4 4 1
    * 1 4 4 1
    * 1 1 1 1   
    */
   
   //Ojo, si en cambio, trabajas con operaciones binarias de toda la vida, el resultado también es una referencia (Una referencia a la matriz del primer operando, es decir c = a + b es lo mismo que a += b. Eso tengo que buscar una solución) (Tampoco se puede hacer magia)
   auto sub3 = sub1 + 3*sub2;
   
   //Operaciones normales de matrices:
   m3 = (m1 + m2 - m1)*2;
   m4 = 2*(m1 + m2 - m1);
   
   //La operación equivalente sin valores temporales intermedios (Más eficiente):
   m3 << MATRIX_OP_ASSIGN << m1 << MATRIX_OP_ADD << m2 << MATRIX_OP_SUB << m1 << MATRIX_OP_MUL << 2;
   
   if( m3.complete_interval == m4.complete_interval )
       cout << "ok" << endl;
   else
       cout << "mmmm...." << endl;
   
   return 0;
}

#endif /* DL32TESTS_CURRENTTEST */