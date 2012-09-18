#include "dl32Preprocessor.h"

#if DL32DEBUG_DEBUGTEST == 2

#include <iostream>
#include "dl32Math.h"

using namespace std;

void PrintMatrix(dl32Matrix &matrix,ostream &os=cout);
void PrintMatrix(dl323x3Matrix &matrix,ostream &os=cout);
void PrintMatrix(dl324x4Matrix &matrix,ostream &os=cout);

ostream& operator<<(ostream &os,dl32Matrix &matrix);
ostream& operator<<(ostream &os,dl323x3Matrix &matrix);
ostream& operator<<(ostream &os,dl324x4Matrix &matrix);

int main()
{
	dl324x4Matrix m1,m2,m3;
	dl323x3Matrix mm1,mm2,mm3;
	dl322DTransform t;

	//Damos formato a la salida. En concreto, valores de dos digitos decimales
	cout.setf(ios::fixed, ios::floatfield);
	cout.precision(2);

	m1=dl324x4Matrix::GetUnity();
	m2=dl324x4Matrix::GetUnity()*2;
	m3=dl324x4Matrix::GetUnity()*4;

	cout << m1 << endl << m2 << endl << m3 << endl; 

	cout << "|m1|=" << dl324x4Matrix::GetDeterminant(m1) << endl;
	cout << "|m2|=" << dl324x4Matrix::GetDeterminant(m2) << endl;
	cout << "|m3|=" << dl324x4Matrix::GetDeterminant(m3) << endl;

	mm1=dl323x3Matrix::GetUnity();
	mm2=dl323x3Matrix::GetUnity()*2;
	mm3=dl323x3Matrix::GetUnity()*4;

	t=dl322DTransform::Traslation(1,1);

	try
	{
		cout << dl323x3Matrix::GetInverse(mm1) << endl;
		cout << dl323x3Matrix::GetInverse(mm2) << endl;
		cout << dl323x3Matrix::GetInverse(mm3) << endl;
		cout << dl323x3Matrix::GetInverse(t) << endl;
	}
	catch (dl32DividedByCeroException&)
	{
		cout << "ERROR: Matriz no invertible" << endl;
	}

	cout << "Test finalizado. Presione ENTER para continuar...";
	cin.sync();
	cin.get();

	return 0;
}

void PrintMatrix(dl32Matrix &matrix,ostream &os)
{
	for(int i=0;i<matrix.GetRowsCount();++i)
	{
		for(int j=0;j<matrix.GetColumnsCount();++j)
			os << " " << matrix[i][j] << " ";

		os << endl;
	}
}

void PrintMatrix(dl323x3Matrix &matrix,ostream &os)
{
	os << " " << matrix.m11 << "  " << matrix.m12 << "  " <<  matrix.m13 << " " << endl;
	os << " " << matrix.m21 << "  " << matrix.m22 << "  " <<  matrix.m23 << " " << endl;
	os << " " << matrix.m31 << "  " << matrix.m32 << "  " <<  matrix.m33 << " " << endl;
}

void PrintMatrix(dl324x4Matrix &matrix,ostream &os)
{
	os << " " << matrix.m11 << "  " << matrix.m12 << "  " <<  matrix.m13 << " " << matrix.m14 << endl;
	os << " " << matrix.m21 << "  " << matrix.m22 << "  " <<  matrix.m23 << " " << matrix.m24 << endl;
	os << " " << matrix.m31 << "  " << matrix.m32 << "  " <<  matrix.m33 << " " << matrix.m34 << endl;
	os << " " << matrix.m41 << "  " << matrix.m42 << "  " <<  matrix.m43 << " " << matrix.m44 << endl;
}

ostream& operator<<(ostream &os,dl32Matrix &matrix)
{
	PrintMatrix(matrix,os);
	return os;
}

ostream& operator<<(ostream &os,dl323x3Matrix &matrix)
{
	PrintMatrix(matrix,os);
	return os;
}

ostream& operator<<(ostream &os,dl324x4Matrix &matrix)
{
	PrintMatrix(matrix,os);
	return os;
}
#endif