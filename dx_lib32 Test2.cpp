#include "dl32Preprocessor.h"

#if DL32DEBUG_DEBUGTEST == 2

#include <iostream>
#include "dl32Math.h"

using namespace std;

void PrintMatrix(dl32Matrix &matrix,ostream &os=cout);
void PrintMatrix(dl32Matrix3x3 &matrix,ostream &os=cout);
void PrintMatrix(dl32Matrix4x4 &matrix,ostream &os=cout);

ostream& operator<<(ostream &os,dl32Matrix &matrix);
ostream& operator<<(ostream &os,dl32Matrix3x3 &matrix);
ostream& operator<<(ostream &os,dl32Matrix4x4 &matrix);

int main()
{
	dl32Matrix4x4 m1,m2,m3;
	dl32Matrix3x3 mm1,mm2,mm3;
	dl32Transformation2D t;

	//Damos formato a la salida. En concreto, valores de dos digitos decimales
	cout.setf(ios::fixed, ios::floatfield);
	cout.precision(2);

	m1=dl32Matrix4x4::GetUnity();
	m2=dl32Matrix4x4::GetUnity()*2;
	m3=dl32Matrix4x4::GetUnity()*4;

	cout << m1 << endl << m2 << endl << m3 << endl; 

	cout << "|m1|=" << dl32Matrix4x4::GetDeterminant(m1) << endl;
	cout << "|m2|=" << dl32Matrix4x4::GetDeterminant(m2) << endl;
	cout << "|m3|=" << dl32Matrix4x4::GetDeterminant(m3) << endl;

	mm1=dl32Matrix3x3::GetUnity();
	mm2=dl32Matrix3x3::GetUnity()*2;
	mm3=dl32Matrix3x3::GetUnity()*4;

	t=dl32Transformation2D::Translation(1,1);

	try
	{
		cout << dl32Matrix3x3::GetInverse(mm1) << endl;
		cout << dl32Matrix3x3::GetInverse(mm2) << endl;
		cout << dl32Matrix3x3::GetInverse(mm3) << endl;
		cout << dl32Matrix3x3::GetInverse(t) << endl;
	}
	catch (dl32InvalidMatrixOperationException&)
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

void PrintMatrix(dl32Matrix3x3 &matrix,ostream &os)
{
	os << " " << matrix.m11 << "  " << matrix.m12 << "  " <<  matrix.m13 << " " << endl;
	os << " " << matrix.m21 << "  " << matrix.m22 << "  " <<  matrix.m23 << " " << endl;
	os << " " << matrix.m31 << "  " << matrix.m32 << "  " <<  matrix.m33 << " " << endl;
}

void PrintMatrix(dl32Matrix4x4 &matrix,ostream &os)
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

ostream& operator<<(ostream &os,dl32Matrix3x3 &matrix)
{
	PrintMatrix(matrix,os);
	return os;
}

ostream& operator<<(ostream &os,dl32Matrix4x4 &matrix)
{
	PrintMatrix(matrix,os);
	return os;
}
#endif