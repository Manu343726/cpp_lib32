#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_GENERAL_ECUATIONSYSTEM

#include <iostream>
#include "dl32Math.h"

using namespace std;

void PrintMyMatrix(dl32Matrix &matrix);
void PrintSystemType(dl32SystemSolutionType Type);
void PrintSolutions(float *Solutions,int SolutionsCount);

int main()
{
	dl32Matrix SystemMatrix;
	dl32EcuationsSystem System;
	dl32SystemSolution Solution;

	int ecs,incs;

	//Damos formato a la salida. En concreto, valores de dos digitos decimales
	cout.setf(ios::fixed, ios::floatfield);
	cout.precision(2);

	cout << "dx_lib32 C++ - Test3 (Sistemas de Ecuaciones Lineales)" << endl;
	cout << "======================================================" << endl << endl;

	cout << "Introduzca el numero de ecuaciones del sistema: ";
	cin >> ecs;
	cout << "Introduzca el numero de incognitas del sistema: ";
	cin >> incs;

	cout << endl;

	SystemMatrix=dl32Matrix(ecs,incs+1);//NOTA: Los argumentos del constructor son el número de filas y el número de columnas de la matriz.

	for(int i=0;i<ecs;++i)
	{
		cout << endl << "ECUACION " << i << ":" << endl;
		for(int j=0;j<incs+1;++j)
		{
			if(j<incs)
				cout << "	Introduzca el término de la incognita " << j << ": ";//NOTA: La incognita 0 es la x, la 1 es la Y, la 2 es la Z, etc, etc.
			else
				cout << "	Introduzca el término independiente: ";

			cin >> SystemMatrix[i][j];
		}
	}

	System=dl32EcuationsSystem(SystemMatrix);
	Solution=System.GetSolution();

	cout << endl << endl;
	PrintMyMatrix(SystemMatrix);
	cout << "-------------------------------" << endl;
	PrintMyMatrix(System.GetGaussMatrix());

	cout << endl << "RESULTADOS:" << endl;
	cout << " - Rango matriz principal: " << System.GetMainMatrixRange() << endl;
	cout << " - Rango matriz ampliada: " << System.GetAuxMatrixRange() << endl;
	cout << " - Tipo: " << Solution.Type;PrintSystemType(Solution.Type);cout <<endl;

	if(Solution.Type==SCD)
	{
		cout << " - Solucion: ";
		PrintSolutions(Solution.Solutions,incs);
	}

	cout << endl << "Test finalizado. Presione ENTER para continuar...";
	cin.sync();
	cin.get();
}

void PrintMyMatrix(dl32Matrix &matrix)
{
	for(int i=0;i<matrix.GetRowsCount();++i)
	{
		for(int j=0;j<matrix.GetColumnsCount();++j)
			cout << " " << matrix[i][j] << " ";

		cout << endl;
	}
}

void PrintSystemType(dl32SystemSolutionType Type)
{
	switch(Type)
	{
	case SCD: 
		cout << "(sistema compatible determinado)";
		break;
	case SCI: 
		cout << "(sistema compatible indeterminado)";
		break;
	case SI: 
		cout << "(sistema incompatible)";
		break;
	}
}

void PrintSolutions(float *Solutions,int SolutionsCount)
{
	cout << "(";

	for(int i=0;i<SolutionsCount;++i)
	{
		cout << Solutions[i];
		if (i<SolutionsCount-1) cout << ",";
	}

	cout << ")" << endl; 
}
#endif