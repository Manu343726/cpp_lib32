#if DL32DEBUG_DEBUGTEST == 2

#include <iostream>
#include "Math.h"

using namespace std;

void PrintMatrix(dl32Matrix &matrix);
void PrintMatrix(dl323x3Matrix &matrix);

int main()
{
	dl323x3Matrix m1,m2,m3;
	dl32Matrix mm1,mm2,mm3;

	//Damos formato a la salida. En concreto, valores de dos digitos decimales
	cout.setf(ios::fixed, ios::floatfield);
	cout.precision(2);

	m1=MATH_Get3x3Unity();
	m2=MATH_Get3x3Unity();
	mm1=MATH_Get3x3Unity(10);
	mm2=MATH_Get3x3Unity(10);

	PrintMatrix(m1);
	cout << endl << "(PRODUCTO)" << endl;
	PrintMatrix(m2);
	cout << "=====================" << endl;
	m3=m1*m2;
	PrintMatrix(m3);

	PrintMatrix(mm1);
	cout << endl << "(PRODUCTO)" << endl;
	mm2.Traspose();
	mm2[9][0]=1;
	PrintMatrix(mm2.GetTrasposed());
	cout << "=====================" << endl;
	mm3=mm1*mm2;
	PrintMatrix(mm3);

	mm3=MATH_Get3x3Unity(4)*2;
	cout << endl << mm3.GetDeterminant() << endl;
	cout << endl << mm3.GetRange() << endl;
	mm3.GetRange(mm1);
	PrintMatrix(mm1);

	cout << "Test finalizado. Presione ENTER para continuar...";
	cin.sync();
	cin.get();

	return 0;
}

void PrintMatrix(dl32Matrix &matrix)
{
	for(int i=0;i<matrix.GetRowsCount();++i)
	{
		for(int j=0;j<matrix.GetColumnsCount();++j)
			cout << " " << matrix[i][j] << " ";

		cout << endl;
	}
}

void PrintMatrix(dl323x3Matrix &matrix)
{
	cout << " " << matrix.m11 << "  " << matrix.m12 << "  " <<  matrix.m13 << " " << endl;
	cout << " " << matrix.m21 << "  " << matrix.m22 << "  " <<  matrix.m23 << " " << endl;
	cout << " " << matrix.m31 << "  " << matrix.m32 << "  " <<  matrix.m33 << " " << endl;
}
#endif