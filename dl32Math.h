#ifndef MATH_H
#define MATH_H

#include <vector>
#include "dl32Config.h"
#include "dl32Exceptions.h"

using namespace std;

#define DL32MACRO_TONATURAL(x) (x)>=0 ? (x) : 0


#define PI2 6.283185307
#define PI 3.141592654
#define PI_2 1.570796326
#define PI_4 0.785398163

#define _PI2 -6.283185307
#define _PI -3.141592654
#define _PI_2 -1.570796326
#define _PI_4 -0.785398163

float MATH_Min(float a,float b);

float DL32FLOAT_INVSQRT(float x);

//DECLARACIÓN E IMPLEMENTACIÓN (INLINE) DE EXCEPCIONES:
///////////////////////////////////////////////////////

//////////////////////////////////////
///A base for dx_lib32 math exceptions
//////////////////////////////////////
class dl32MathException:public dl32Exception
{
public:
	dl32MathException(char* message = DEFAULTEXCEPTIONMESSAGE(dl32MathException)):dl32Exception(message){};
};

///////////////////////////////////////
///Represents a math overflow exception
///////////////////////////////////////
class dl32OverflowException:public dl32MathException
{
public:
	dl32OverflowException(char* message = DEFAULTEXCEPTIONMESSAGE(dl32OverflowException)):dl32MathException(message){};
};

class dl32DividedByCeroException:public dl32OverflowException
{
public:
	dl32DividedByCeroException(char* message = DEFAULTEXCEPTIONMESSAGE(dl32DividedByCeroException)):dl32OverflowException(message){};
};

class dl32InvalidMatrixOperationException:dl32MathException
{
public:
	dl32InvalidMatrixOperationException(char* message = DEFAULTEXCEPTIONMESSAGE(dl32InvalidMatrixOperationException)):dl32MathException(message){};
};

class dl32InfiniteIntersectionException : dl32MathException
{
public:
	dl32InfiniteIntersectionException(char* message = DEFAULTEXCEPTIONMESSAGE(dl32InfiniteIntersectionException)):dl32MathException(message){};
};

//CLASES DEL MÓDULO PROPIAMENTE DICHO:
//////////////////////////////////////

////////////////////////////////
///Represents a 3x3 float matrix
////////////////////////////////
struct dl32Matrix3x3
{
	union
	{
		struct  
		{
			float m11,m12,m13;
			float m21,m22,m23;
			float m31,m32,m33;
		};
		float m[3][3];
	};

	dl32Matrix3x3();
	dl32Matrix3x3(float m11,float m12,float m13,
		float m21,float m22,float m23,
		float m31,float m32,float m33);

	float* operator[](int index){return m[index];};

	float GetDeterminant() const {return GetDeterminant(*this);};
	dl32Matrix3x3 GetInverse() const {return GetInverse(*this);};

	static dl32Matrix3x3& GetUnity()
	{
		return dl32Matrix3x3(1,0,0,
			0,1,0,
			0,0,1);
	};

	static dl32Matrix3x3 Add(const dl32Matrix3x3 &m1,const dl32Matrix3x3 &m2)
	{
		return dl32Matrix3x3(m1.m11+m2.m11,m1.m12+m1.m12,m1.m13+m2.m13,
			m1.m21+m2.m21,m1.m22+m1.m22,m1.m23+m2.m23,
			m1.m31+m2.m31,m1.m32+m1.m32,m1.m33+m2.m33);
	};
	static dl32Matrix3x3 Sub(const dl32Matrix3x3 &m1,const dl32Matrix3x3 &m2)
	{
		return dl32Matrix3x3(m1.m11-m2.m11,m1.m12-m1.m12,m1.m13-m2.m13,
			m1.m21-m2.m21,m1.m22-m1.m22,m1.m23-m2.m23,
			m1.m31-m2.m31,m1.m32-m1.m32,m1.m33-m2.m33);
	};
	static dl32Matrix3x3 Mul(const dl32Matrix3x3 &m1,const dl32Matrix3x3 &m2)
	{
		return dl32Matrix3x3(m1.m11*m2.m11+m1.m12*m2.m21+m1.m13*m2.m31,
			m1.m11*m2.m12+m1.m12*m2.m22+m1.m13*m2.m32,
			m1.m11*m2.m13+m1.m12*m2.m23+m1.m13*m2.m33,

			m1.m21*m2.m11+m1.m22*m2.m21+m1.m23*m2.m31,
			m1.m21*m2.m12+m1.m22*m2.m22+m1.m23*m2.m32,
			m1.m21*m2.m13+m1.m22*m2.m23+m1.m23*m2.m33,

			m1.m31*m2.m11+m1.m32*m2.m21+m1.m33*m2.m31,
			m1.m31*m2.m12+m1.m32*m2.m22+m1.m33*m2.m32,
			m1.m31*m2.m13+m1.m32*m2.m23+m1.m33*m2.m33);
	};
	static dl32Matrix3x3 Mul(const dl32Matrix3x3 &matrix,float mul)
	{
		return dl32Matrix3x3(matrix.m11*mul,matrix.m12*mul,matrix.m13*mul,
			matrix.m21*mul,matrix.m22*mul,matrix.m23*mul,
			matrix.m31*mul,matrix.m32*mul,matrix.m33*mul);
	};

	static float GetDeterminant(const dl32Matrix3x3 &matrix);
	static dl32Matrix3x3 GetInverse(const dl32Matrix3x3 &matrix)throw(dl32InvalidMatrixOperationException);

	dl32Matrix3x3 operator+(const dl32Matrix3x3 &matrix) const {return Add(*this,matrix);};
	dl32Matrix3x3 operator-(const dl32Matrix3x3 &matrix) const {return Sub(*this,matrix);};
	dl32Matrix3x3 operator*(const dl32Matrix3x3 &matrix) const {return Mul(*this,matrix);};
	dl32Matrix3x3 operator*(float mul) const {return Mul(*this,mul);};
};

////////////////////////////////
///Represents a 4x4 float matrix
////////////////////////////////
struct dl32Matrix4x4
{
	union
	{
		struct
		{
			float m11,m12,m13,m14;
			float m21,m22,m23,m24;
			float m31,m32,m33,m34;
			float m41,m42,m43,m44;
		};
		float m[4][4];
	};

	dl32Matrix4x4();
	dl32Matrix4x4(float m11,float m12,float m13,float m14,
		float m21,float m22,float m23,float m24,
		float m31,float m32,float m33,float m34,
		float m41,float m42,float m43,float m44);

	float* operator[](int index){return m[index];};

	static dl32Matrix4x4 GetUnity()
	{
		return dl32Matrix4x4(1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1);
	};

	static dl32Matrix4x4 Add(dl32Matrix4x4 m1,dl32Matrix4x4 m2);
	static dl32Matrix4x4 Sub(dl32Matrix4x4 m1,dl32Matrix4x4 m2);
	static dl32Matrix4x4 Mul(dl32Matrix4x4 m1,dl32Matrix4x4 m2);
	static dl32Matrix4x4 Mul(dl32Matrix4x4 matrix,float mul);

	static float GetDeterminant(dl32Matrix4x4 &matrix);

	dl32Matrix4x4 operator+(dl32Matrix4x4 matrix){return Add(*this,matrix);};
	dl32Matrix4x4 operator-(dl32Matrix4x4 matrix){return Sub(*this,matrix);};
	dl32Matrix4x4 operator*(dl32Matrix4x4 matrix){return Mul(*this,matrix);};
	dl32Matrix4x4 operator*(float mul){return Mul(*this,mul);};
};

///////////////////////////////////////
///Represents a row of a generic matrix
///////////////////////////////////////
typedef float *dl32MatrixRow;

//////////////////////////////////////////
///Represets a pointer to a float variable
//////////////////////////////////////////
typedef float *dl32FloatPointer;

//////////////////////////////////////////
///Represents a column of a generic matrix
//////////////////////////////////////////
typedef float **dl32MatrixColumn;

////////////////////////////////////
///Represents a float generic matrix
////////////////////////////////////
class dl32Matrix
{
	friend void CopyRef(dl32Matrix &matrix);
	friend dl32Matrix Add(dl32Matrix m1,dl32Matrix m2);
	friend dl32Matrix Sub(dl32Matrix m1,dl32Matrix m2);
	friend dl32Matrix Mul(dl32Matrix matrix,float mul);
	friend dl32Matrix Mul(dl32Matrix m1,dl32Matrix m2);
protected:
	int rows;
	int columns;

	float **Array;

	bool NotInitialized;

	void RowMul(int row, float mul);
	void RowAdd(int source_row,int aux_row,float aux_mul=1);
	void RowSwap(int row1,int row2);
	void ColumnMul(int column,float mul);
	void ColumnAdd(int source_column,int aux_column,float aux_mul=1);
	void ColumnSwap(int column1,int column2);
public:
	dl32Matrix();
	dl32Matrix(dl32Matrix &matrix);
	dl32Matrix(int rows, int columns);
	dl32Matrix(dl32Matrix3x3 &matrix);
	dl32Matrix(dl32Matrix4x4 &matrix);

	dl32Matrix& operator=(dl32Matrix &matrix);

	void CopyRef(dl32Matrix &matrix);
	void Dispose();
	~dl32Matrix();

	bool Ready(){return Array!=NULL;};

	float At(int row,int column)throw(dl32OutOfRangeException);
	dl32MatrixRow operator[](int row)throw(dl32OutOfRangeException);

	int GetColumnsCount(){return columns;};
	int GetRowsCount(){return rows;};
	bool IsSquare(){return rows==columns;};

	dl32MatrixColumn GetColumn(int column)throw(dl32OutOfRangeException);
	dl32MatrixRow GetRow(int row)throw(dl32OutOfRangeException);

	dl32Matrix GetSubMatrix(int row,int column,int rows,int columns)throw(dl32OutOfRangeException);
	dl32Matrix GetSubMatrix(int row,int column,int size)throw(dl32OutOfRangeException){return GetSubMatrix(row,column,size,size);};
	dl32Matrix GetSubMatrix(int row,int column)throw(dl32OutOfRangeException);

	float GetMinor(int row,int column)throw(dl32OutOfRangeException){return GetSubMatrix(row,column).GetDeterminant();};
	float GetMinor(int row,int column,int size)throw(dl32OutOfRangeException){return GetSubMatrix(row,column,size).GetDeterminant();};
	float GetAdjugate(int row,int column)throw(dl32OutOfRangeException){return pow(float(-1),float(row+column))*GetSubMatrix(row,column).GetDeterminant();};
	dl32Matrix GetAdjugate();

	float GetDeterminant();
	dl32Matrix GetTranspose();
	void Transpose();
	dl32Matrix GetInverse();

	int GetRange(){return GetRange(*this);};
	int GetRange(dl32Matrix &FinalMatrix);

	static dl32Matrix Add(dl32Matrix m1,dl32Matrix m2)throw(dl32InvalidMatrixOperationException);
	static dl32Matrix Sub(dl32Matrix m1,dl32Matrix m2)throw(dl32InvalidMatrixOperationException);
	static dl32Matrix Mul(dl32Matrix matrix,float mul)throw(dl32InvalidMatrixOperationException);
	static dl32Matrix Mul(dl32Matrix m1,dl32Matrix m2)throw(dl32InvalidMatrixOperationException);

	dl32Matrix operator+(dl32Matrix matrix)throw(dl32InvalidMatrixOperationException){return Add(*this,matrix);};
	dl32Matrix operator-(dl32Matrix matrix)throw(dl32InvalidMatrixOperationException){return Sub(*this,matrix);};
	dl32Matrix operator*(dl32Matrix matrix)throw(dl32InvalidMatrixOperationException){return Mul(*this,matrix);};
	dl32Matrix operator*(float factor)throw(dl32InvalidMatrixOperationException){return Mul(*this,factor);};

	bool operator==(dl32Matrix &matrix){return Array==matrix.Array;};
	bool operator!=(dl32Matrix &matrix){return Array!=matrix.Array;};
};


//////////////////////////////////////////////////////////
///Represents a solution type of a linear ecuations system
//////////////////////////////////////////////////////////
enum dl32SystemSolutionType
{
	SCD,
	SCI,
	SI
};

/////////////////////////////////////////////////////
///Represents a solution of a linear ecuations system
/////////////////////////////////////////////////////
struct dl32SystemSolution
{
	dl32SystemSolutionType Type;
	float *Solutions;
};

////////////////////////////////////////////////////////////////
///Represents a linear ecuation as a group of float coefficients
////////////////////////////////////////////////////////////////
typedef float *dl32LinearEcuation;

////////////////////////////////////////////////////////////////
///Represents a linear ecuation as a group of float coefficients
////////////////////////////////////////////////////////////////
typedef dl32LinearEcuation *dl32LinearEcuationArray;

////////////////////////////////////////////////////////////////
///Represents a linear ecuation as a group of float coefficients
////////////////////////////////////////////////////////////////
typedef vector<dl32LinearEcuation> dl32LinearEcuationList;

///////////////////////////////////////
///Represents a linear ecuations system
///////////////////////////////////////
class dl32EcuationsSystem
{
	friend class dl32EcuationsSystem;
private: 
	dl32Matrix mainmatrix;
	dl32Matrix auxmatrix;
	dl32Matrix gaussmatrix;
	int mainrange;
	int auxrange;
	int incs;
	int ecs;
	dl32SystemSolution Solution;

	bool Solved;
	void Solve();
public:
	dl32EcuationsSystem();//OK
	dl32EcuationsSystem(dl32EcuationsSystem &System);//OK
	dl32EcuationsSystem(dl32Matrix MainMatrix);//OK
	dl32EcuationsSystem(dl32LinearEcuationArray Ecuations,int EcuationsCount,int IncsCount);//OK
	//dl32EcuationsSystem(dl32LinearEcuationList Ecuations,int IncsCount);//FALTA IMPLEMENTACION

	void Dispose();
	~dl32EcuationsSystem();

	dl32EcuationsSystem& operator=(dl32EcuationsSystem &System);

	int GetMainMatrixRange(){return mainrange;};
	int GetAuxMatrixRange(){return auxrange;};
	dl32SystemSolution GetSolution(){return Solution;};

	dl32Matrix GetGaussMatrix(){return gaussmatrix;};
};

/////////////////////////////////
///Represents a point in 2D space
/////////////////////////////////
struct dl32Point2D
{
	union
	{
		struct
		{
			float x;
			float y;
		};
		float c[2];
	};

	dl32Point2D(){x=0;y=0;};
	dl32Point2D(float ValX,float ValY)                                               {x=ValX;y=ValY;};
	dl32Point2D(float* coordsArray)                                                  {x=coordsArray[0];y=coordsArray[1];};

	const float operator [](int coord) const throw (dl32OutOfRangeException);

	dl32Point2D& operator()()                                                        {x=0;y=0;return *this;};
	dl32Point2D& operator()(float x,float y)                                         {this->x=x;this->y=y;return *this;};

	static const dl32Point2D Add(const dl32Point2D &P1,const dl32Point2D &P2)        {return dl32Point2D(P1.x+P2.x,P1.y+P2.y);};
	static const dl32Point2D Sub(const dl32Point2D &P1,const dl32Point2D &P2)        {return dl32Point2D(P1.x-P2.x,P1.y-P2.y);};
	static const dl32Point2D Mul(const dl32Point2D &Point,float Factor)              {return dl32Point2D(Point.x*Factor,Point.y*Factor);};
	static const dl32Point2D Div(const dl32Point2D &Point,float Divisor)throw(dl32DividedByCeroException);

	static dl32Point2D Baricenter(dl32Point2D PointList[],int PointCount);

	const dl32Point2D operator+(const dl32Point2D &point) const                       {return Add(*this,point);};
	const dl32Point2D operator-()                         const                       {return dl32Point2D(-x,-y);};
	const dl32Point2D operator-(const dl32Point2D &point) const                       {return Sub(*this,point);};
	const dl32Point2D operator*(float factor) const                                   {return Mul(*this,factor);};
	const dl32Point2D operator/(float div)    const throw(dl32DividedByCeroException) {return Div(*this,div);};

	bool operator==(dl32Point2D point)                                                {return DL32FLOAT_EQ(x,point.x) && DL32FLOAT_EQ(y,point.y);};
	bool operator!=(dl32Point2D point)                                                {return DL32FLOAT_NE(x,point.x) || DL32FLOAT_NE(y,point.y);};

	static float GetSignedArea(dl32Point2D P1, dl32Point2D P2, dl32Point2D P3)        {return (P2.x-P1.x)*(P3.y-P1.y) - (P3.x-P1.x)*(P2.y-P1.y);};
};

//////////////////////////////////
///Represents a vector in 2D space
//////////////////////////////////
struct dl32Vector2D:public dl32Point2D
{
	dl32Vector2D(){x=0,y=0;};
	dl32Vector2D(float ValX,float ValY){x=ValX;y=ValY;};
	dl32Vector2D(dl32Point2D P1,dl32Point2D P2){x=P2.x-P1.x;y=P2.y-P1.y;};

	dl32Vector2D operator()(){x=0;y=0;return *this;};
	dl32Vector2D operator()(float x,float y){this->x=x;this->y=y;return *this;};
	dl32Vector2D operator()(dl32Point2D P1,dl32Point2D P2){x=P2.x-P1.x;y=P2.y-P1.y;return *this;};

	float GetLength() const {return sqrt(x*x+y*y);}; 
	void SetLength(float length){Normalize();x*=length;y*=length;};
	float GetSquareLenght() const {return x*x+y*y;}; 
	void Normalize();
	dl32Vector2D GetUnitary(){float m=sqrt(x*x+y*y);return dl32Vector2D(x/m,y/m);};
	dl32Vector2D GetNormal(bool Clockwise=true){return (Clockwise ? dl32Vector2D(-y,x) : dl32Vector2D(y,-x));};

	static const dl32Vector2D Add(const dl32Vector2D &P1,const dl32Vector2D &P2){return dl32Vector2D(P1.x+P2.x,P1.y+P2.y);};
	static const dl32Vector2D Sub(const dl32Vector2D &P1,const dl32Vector2D &P2){return dl32Vector2D(P1.x-P2.x,P1.y-P2.y);};
	static const dl32Vector2D Mul(const dl32Vector2D &Vector,float Factor){return dl32Vector2D(Vector.x*Factor,Vector.y*Factor);};
	static const dl32Vector2D Div(const dl32Vector2D &Vector,float Divisor)throw(dl32DividedByCeroException);

	const dl32Vector2D operator+(const dl32Vector2D &Vector){return Add(*this,Vector);};
	const dl32Vector2D operator-(){return dl32Vector2D(-x,-y);};
	const dl32Vector2D operator-(const dl32Vector2D &Vector){return Sub(*this,Vector);};
	const dl32Vector2D operator*(float factor){return Mul(*this,factor);};
	const dl32Vector2D operator/(float div)throw(dl32DividedByCeroException){return Div(*this,div);};

	dl32Vector2D& operator+=(dl32Vector2D vector){*this=Add(*this,vector);return *this;};
	dl32Vector2D& operator-=(dl32Vector2D vector){*this=Sub(*this,vector);return *this;};

	static float Mul(const dl32Vector2D &V1,const dl32Vector2D &V2){return V1.x*V2.x+V1.y*V2.y;};
	static float Angle(const dl32Vector2D &V1, const dl32Vector2D &V2){return acos(Mul(V1,V2)*DL32FLOAT_INVSQRT(V1.x*V1.x+V1.y*V1.y)*DL32FLOAT_INVSQRT(V2.x*V2.x+V2.y*V2.y));};//Una pequeña optimización...

	float operator*(const dl32Vector2D &vector){return Mul(*this,vector);};
	float operator^(const dl32Vector2D &vector){return Angle(*this,vector);};
};

////////////////////////////////
///Represents a line in 2D space
////////////////////////////////
class dl32Line2D
{
protected:
	dl32Point2D position;
	dl32Vector2D direction;
	float a,b,c;//Ax + By + C = 0 (Ecuación general)
public:
	dl32Line2D();
	dl32Line2D(float A,float B,float C);
	dl32Line2D(dl32Point2D P1,dl32Point2D P2);
	dl32Line2D(dl32Point2D Position,dl32Vector2D Direction);
	dl32Line2D(dl32Point2D Position,float Slope);

	dl32Point2D GetPoint(float x){return dl32Point2D(x,(a*x+c)/-b);};
	dl32Point2D GetPointByParameter(float param){return dl32Point2D(position.x+direction.x*param,position.y+direction.y*param);};

	bool BelongTo(float x,float y){return a*x+b*y+c==0;};
	bool BelongTo(dl32Point2D point) {return BelongTo(point.x,point.y);};
	float GetRelativePosition(float x,float y){return a*x+b*y+c;};
	float GetRelativePosition(dl32Point2D point) {return GetRelativePosition(point.x,point.y);};

	dl32Point2D GetPosition(){return position;};
	dl32Vector2D GetDirection(){return direction;};
	float GetSlope(){return -b/a;};

	static bool AreParallel(dl32Line2D L1, dl32Line2D L2){return L1.GetDirection()==L2.GetDirection() || L1.GetDirection()==-L2.GetDirection();}
	static dl32Point2D Intersection(dl32Line2D L1, dl32Line2D L2,bool externalParallelCheck=false) throw(dl32InfiniteIntersectionException);

	void SetPosition(float x,float y);
	void SetPosition(dl32Point2D position);
	void SetDirection(float x,float y);
	void SetDirection(dl32Vector2D pirection);
	void SetSlope(float slope);
};

class dl32Segment2D
{
private:
	dl32Point2D _begin;
	dl32Point2D _end;
	dl32Line2D _line;

public:
	dl32Segment2D(){};
	dl32Segment2D(dl32Point2D begin, dl32Point2D end);
	dl32Segment2D(dl32Point2D begin, dl32Vector2D direction, float lenght);
};

/////////////////////////////////////////////////////
///Represents a relative orientation for two 2D AABBs
/////////////////////////////////////////////////////
enum dl32Orientation2D
{
	INSIDE,
	OUTSIDE,
	NORTH,
	SOUTH,
	EAST,
	WEST,
	NORTHEAST,
	SOUTHEAST,
	SOUTHWEST,
	NORTHWEST
};

/////////////////////////////////////////////////////
///Represents a Axis Aligned Bounding Box in 2D space
/////////////////////////////////////////////////////
class dl32AABB2D
{
	friend class dl32AABB2D;//Algo guarro, lo se, pero es para no pasar por tanto getter (Aunque igual con las inline no es necesario...)
private:
	float _width;
	float _height;
public:
	dl32Point2D Position; //Coordenadas de la esquina superior izquierda del AABB

	dl32AABB2D() {_width = 0; _height = 0;};
	dl32AABB2D(float X, float Y, float Width, float Height);
	dl32AABB2D(dl32Point2D Position, float Width, float Height);
	dl32AABB2D(dl32Point2D Position, dl32Vector2D Area);
	dl32AABB2D(dl32Point2D pointCloud[],int pointCount);

	//Getters:
	float GetWidth()				 const {return _width;};
	float GetHeight()				 const {return _height;};

	dl32Point2D GetCenter()          const {return dl32Point2D(Position.x+_width*0.5,Position.y+_height*0.5);};
	dl32Point2D GetUpLeftCorner()    const {return Position;};
	dl32Point2D GetUpRightCorner()   const {return dl32Point2D(Position.x+_width,Position.y);};
	dl32Point2D GetDownRightCorner() const {return dl32Point2D(Position.x+_width,Position.y+_height);};
	dl32Point2D GetDownLeftCorner()  const {return dl32Point2D(Position.x,Position.y+_height);};

	float GetLeft()					 const {return Position.x;};
	float GetTop()					 const {return Position.y;};
	float GetRight()				 const {return Position.x+_width;};
	float GetBottom()				 const {return Position.y+_height;};

	//Setters:
	void SetWidth(float Width);
	void SetHeight(float Height);
	void SetCenter(float X, float Y);
	void SetCenter(const dl32Point2D &Center);
	void SetArea(const dl32Vector2D &area);

	bool BelongTo(const dl32Point2D &Point)		          const {return BelongTo(Point.x,Point.y);};
	bool BelongTo(float X,float Y)				          const {return (X>=Position.x && X<=(Position.x+_width) && Y>=Position.y && Y<=(Position.y+_height));};

	bool operator&(dl32AABB2D &AABB)					        {return Collide(*this,AABB);};
	bool operator&(dl32Point2D &point)					        {return BelongTo(point);};
	bool operator ==(dl32AABB2D &AABB)					        {return Position==AABB.Position && DL32FLOAT_EQ(_width,AABB._width) && DL32FLOAT_EQ(_height,AABB._height);};
	bool operator !=(dl32AABB2D &AABB)					        {return Position!=AABB.Position || DL32FLOAT_NE(_width,AABB._width) || DL32FLOAT_NE(_height,AABB._height);};

	bool Collide(dl32AABB2D AABB)                         const {return Collide(*this,AABB);};
	dl32Orientation2D Orientation(const dl32AABB2D &AABB) const {return Orientation(*this,AABB);};

	static bool Collide(const dl32AABB2D &A1,const dl32AABB2D &A2);
	static dl32Orientation2D Orientation(const dl32AABB2D &A1,const dl32AABB2D &A2);
};

class dl32Polygon2D
{
private:
	vector<dl32Point2D> _verts;

};
////////////////////////////////////////////////////
///Represents a geometric Transformationation in 2D space
////////////////////////////////////////////////////
class dl32Transformation2D: public dl32Matrix3x3
{
public:
	dl32Transformation2D():dl32Matrix3x3(
		1,0,0,
		0,1,0,
		0,0,1)
	{};
	dl32Transformation2D(const dl32Matrix3x3 &matrix):dl32Matrix3x3(matrix){};
	dl32Transformation2D(
		float m11,float m12,float m13,
		float m21,float m22,float m23,
		float m31,float m32,float m33
		):dl32Matrix3x3(
		m11,m12,m13,
		m21,m22,m23,
		m31,m32,m33
		)
	{};

	void Apply(dl32Point2D *Point)																const;
	void Apply(dl32Point2D pointList[], int pointCount)											const;
	void Apply(float *x,float *y)																const;

	dl32Point2D Apply(float x,float y)															const {return dl32Point2D(m11*x+m12*y+m13,m21*x+m22*y+m23);};
	dl32Point2D Apply(const dl32Point2D &point)													const {return dl32Point2D(m11*point.x+m12*point.y+m13,m21*point.x+m22*point.y+m23);};

	void Concat(dl32Transformation2D Transformation)                                                  {*this =Concat(*this,Transformation);};
	dl32Transformation2D operator+(const dl32Transformation2D &Transformation)                        {return Concat(*this,Transformation);};
	static dl32Transformation2D Concat(const dl32Transformation2D &t1,const dl32Transformation2D &t2) {return (dl32Transformation2D)dl32Matrix3x3::Mul(t2,t1);};

	static dl32Transformation2D Translation(float dx,float dy)
	{
		return dl32Transformation2D(
			1,0,dx,
			0,1,dy,
			0,0,1
		);
	};

	static dl32Transformation2D Translation(dl32Point2D origin,dl32Point2D destiny)
	{
		return dl32Transformation2D(
			1,0,destiny.x-origin.x,
			0,1,destiny.y-origin.y,
			0,0,1
		);
	};

	static dl32Transformation2D Translation(dl32Vector2D Translation)
	{
		return dl32Transformation2D(
			1,0,Translation.x,
			0,1,Translation.y,
			0,0,1
			);
	};

	static dl32Transformation2D Scale(float scale)
	{
		return dl32Transformation2D(
			scale,0,0,
			0,scale,0,
			0,0,1
			);
	};

	static dl32Transformation2D Scale(float sx,float sy)
	{
		return dl32Transformation2D(
			sx,0,0,
			0,sy,0,
			0,0,1
			);
	};

	static dl32Transformation2D Scale(dl32Vector2D scale)
	{
		return dl32Transformation2D(
			scale.x,0,0,
			0,scale.y,0,
			0,0,1
			);
	};

	static dl32Transformation2D Rotation(float angle)
	{
		return dl32Transformation2D(
			cos(angle),sin(angle),0,
			-sin(angle),cos(angle),0,
			0,0,1
			);
	};

	static dl32Transformation2D Rotation(float sin_angle,float cos_angle)
	{
		return dl32Transformation2D(
			cos_angle,sin_angle,0,
			-sin_angle,cos_angle,0,
			0,0,1
			);
	};

	static dl32Transformation2D Rotation(float center_x,float center_y,float angle);
	static dl32Transformation2D Rotation(dl32Point2D center,float angle);
};


class dl32OBB2D
{
private:
	dl32AABB2D _aabb; //ABB in local space
	dl32Transformation2D _toWorld;
	dl32Transformation2D _toLocal;
public:
	dl32OBB2D(){};//Inicializa a transformaciones unitarias (Ver constructor predeterminado de dl32Transformation2D) La inversa de una unitaria es una unitaria (Gracias Laura) joder que espeso estoy hoy...
	dl32OBB2D(dl32Point2D center, dl32Vector2D area, dl32Vector2D xAxis);
	dl32OBB2D(dl32Point2D center, dl32Vector2D area, float rotation);
	dl32OBB2D(dl32Vector2D area, dl32Transformation2D WorldToLocal);
	dl32OBB2D(dl32Point2D pointCloud[],int pointCount);

	dl32Point2D GetUpLeftCorner()       const {return _toWorld.Apply(_aabb.GetUpLeftCorner());};
	dl32Point2D GetUpRightCorner()      const {return _toWorld.Apply(_aabb.GetUpRightCorner());};
	dl32Point2D GetDownRightCorner()    const {return _toWorld.Apply(_aabb.GetDownRightCorner());};
	dl32Point2D GetDownLeftCorner()     const {return _toWorld.Apply(_aabb.GetDownLeftCorner());};

	dl32Transformation2D WorldToLocal() const {return _toLocal;};
	dl32Transformation2D LocalToWorld() const {return _toWorld;};

	bool BelongTo(dl32Point2D point)    const {return _aabb.BelongTo(_toLocal.Apply(point));};
	bool BelongTo(float x, float y)     const {return _aabb.BelongTo(_toLocal.Apply(x,y));};

	bool Collide(dl32OBB2D OBB)         const {return Collide(*this,OBB);};
	bool Collide(dl32AABB2D localAABB)  const {return dl32AABB2D::Collide(_aabb,localAABB);}
	static bool Collide(dl32OBB2D O1, dl32OBB2D O2);

	//dl32Point2D GetUpLeftCorner(){return _aabb.GetUpLeftCorner();};
	//dl32Point2D GetUpRightCorner(){return _aabb.GetUpRightCorner();};
	//dl32Point2D GetDownRightCorner(){return _aabb.GetDownRightCorner();};
	//dl32Point2D GetDownLeftCorner(){return _aabb.GetDownLeftCorner();};

	void ApplyTransformation(dl32Transformation2D transformation);
};

//Represents a point in 3D space
struct dl32Point3D
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		float c[3];
	};

	dl32Point3D(){x=0;y=0;z=0;};
	dl32Point3D(float ValX,float ValY,float ValZ){x=ValX;y=ValY;z=ValZ;};
	float operator [](int coord)throw(dl32OutOfRangeException);

	static dl32Point3D Add(dl32Point3D P1,dl32Point3D P2){return dl32Point3D(P1.x+P2.x,P1.y+P2.y,P1.z+P2.z);};
	static dl32Point3D Sub(dl32Point3D P1,dl32Point3D P2){return dl32Point3D(P1.x+P2.x,P1.y+P2.y,P1.z+P2.z);};
	static dl32Point3D Mul(dl32Point3D Point,float Factor){return dl32Point3D(Point.x*Factor,Point.y*Factor,Point.z*Factor);};
	static dl32Point3D Div(dl32Point3D Point,float Divisor);

	dl32Point3D operator+(dl32Point3D point){return Add(*this,point);};
	dl32Point3D operator-(){return dl32Point3D(-x,-y,-z);};
	dl32Point3D operator-(dl32Point3D point){return Sub(*this,point);};
	dl32Point3D operator*(float factor){return Mul(*this,factor);};
	dl32Point3D operator/(float div)throw(dl32DividedByCeroException){return Div(*this,div);};
};

//////////////////////////////////
///Represents a vector in 3D space
//////////////////////////////////
struct dl32Vector3D:public dl32Point3D
{
	dl32Vector3D(){x=0;y=0;z=0;};
	dl32Vector3D(float x,float y,float z){this->x=x;this->y=y;this->z=z;};
	dl32Vector3D(dl32Point3D P1,dl32Point3D P2){this->x=P2.x-P1.x;y=P2.y-P1.y;z=P2.z-P1.z;};

	float GetLength(){return sqrt(x*x+y*y+z*z);};
	void Normalize();
	dl32Vector3D GetUnitary(){float m=sqrt(x*x+y*y+z*z);return dl32Vector3D(x/m,y/m,z/m);};

	static float Mul(dl32Vector3D V1, dl32Vector3D V2){return V1.x*V2.x+V1.y*V2.y+V1.z*V2.z;};
	static dl32Vector3D VectorialMul(dl32Vector3D V1,dl32Vector3D V2){return dl32Vector3D(V1.y*V2.z-V1.z*V2.y,V1.z*V2.x-V1.x*V2.z,V1.x*V2.y-V1.y*V2.x);};
	static float Angle(dl32Vector3D V1,dl32Vector3D V2){return acos(Mul(V1,V2)/(V1.GetLength()*V2.GetLength()));};

	dl32Vector3D operator-(){return dl32Vector3D(-x,-y,-z);};
	float operator*(dl32Vector3D vector){return Mul(*this,vector);};
	dl32Vector3D operator&(dl32Vector3D vector){return VectorialMul(*this,vector);};
	float operator^(dl32Vector3D vector){return Angle(*this,vector);};
};

/////////////////////////////////////////////
///Represents a set of quaternion coordinates
/////////////////////////////////////////////
typedef float dl32QuaternionCoords[4];

///////////////////////////////
///Represents a coordinate axis
///////////////////////////////
enum dl32CoordinateAxis
{
	DL32AXIS_X,// X axis
	DL32AXIS_Y,// Y axis
	DL32AXIS_Z // Z axis
};

//////////////////////////////////
///Represents a unitary quaternion
//////////////////////////////////
class dl32Quaternion:public dl32Matrix4x4
{
	friend dl32Quaternion Add(dl32Quaternion Q1,dl32Quaternion Q2);
	friend dl32Quaternion Sub(dl32Quaternion Q1,dl32Quaternion Q2);
	friend dl32Quaternion Mul(dl32Quaternion Q1,dl32Quaternion Q2);
	friend dl32Quaternion Mul(dl32Quaternion quaternion, float factor);
private:
	float a,b,c,d;//q = a + bi + cj + dk
	bool matrixReady;

	void Normalize(const float &a,const float &b,const float &c,const float &d,bool UseMatrix);
public:
	dl32Quaternion(){a=0;b=0;c=0;d=0;matrixReady=false;};
	dl32Quaternion(float a,float b,float c,float d,bool UseMatrix=false);
	dl32Quaternion(dl32Vector3D Axis,float Angle,bool UseMatrix=false);
	dl32Quaternion(dl32CoordinateAxis axis,float angle,bool UseMatrix=false);

	void SetupMatrix();
	bool MatrixReady(){return matrixReady;};

	float GetLength(){return sqrt(a*a+b*b+c*c+d*d);};
	float GetSquareLenght(){return a*a+b*b+c*c+d*d;};

	float GetA(){return a;};
	float GetB(){return b;};
	float GetC(){return c;};
	float GetD(){return d;};
	void GetCoords(dl32QuaternionCoords Coords){Coords[0]=a;Coords[1]=b,Coords[2]=c;Coords[3]=d;};

	void SetA(float A){a=A;matrixReady=false;};
	void SetB(float B){b=B;matrixReady=false;};
	void SetC(float C){c=C;matrixReady=false;};
	void SetD(float D){d=D;matrixReady=false;};
	void SetCoords(float A,float B,float C,float D,bool UseMatrix){a=A;b=B;c=C;d=D;if(UseMatrix) SetupMatrix();};
	void SetCoords(dl32QuaternionCoords Coords,bool UseMatrix){return SetCoords(Coords[0],Coords[1],Coords[2],Coords[3],UseMatrix);};

	dl32Quaternion GetConjugate(){return dl32Quaternion(a,-b,-c,-d);};
	dl32Quaternion GetInversed(){float m=GetSquareLenght();return dl32Quaternion(a/m,-b/m,-c/m,-d/m);};


	static dl32Quaternion Add(dl32Quaternion Q1,dl32Quaternion Q2){return dl32Quaternion(Q1.a+Q2.a,Q1.b+Q2.b,Q1.c+Q2.c,Q1.d+Q2.d);};
	static dl32Quaternion Sub(dl32Quaternion Q1,dl32Quaternion Q2){return dl32Quaternion(Q1.a-Q2.a,Q1.b-Q2.b,Q1.c-Q2.c,Q1.d-Q2.d);};
	static dl32Quaternion Mul(dl32Quaternion quaternion, float factor){return dl32Quaternion(quaternion.a*factor,quaternion.b*factor,quaternion.c*factor,quaternion.d*factor);};
	static dl32Quaternion Div(dl32Quaternion quaternion, float div)throw(dl32DividedByCeroException);
	static dl32Quaternion Div(dl32Quaternion Q1,dl32Quaternion Q2){return Mul(Q1,Q2.GetInversed());};

	static dl32Quaternion Mul(dl32Quaternion Q1,dl32Quaternion Q2)
	{
		return dl32Quaternion(
			(Q1.a * Q2.a) - (Q1.b * Q2.b) - (Q1.c * Q2.c) - (Q1.d * Q2.d),
			(Q1.a * Q2.b) + (Q1.b * Q2.a) + (Q1.c * Q2.d) - (Q1.d * Q2.c),
			(Q1.a * Q2.c) - (Q1.b * Q2.d) + (Q1.c * Q2.a) + (Q1.d * Q2.b),
			(Q1.a * Q2.d) + (Q1.b + Q2.c) - (Q1.c * Q2.b) + (Q1.d * Q2.a)
			);
	};
};

////////////////////////////////////////////////////
///Represents a geometric Transformationation in 3D space
////////////////////////////////////////////////////
class dl32Transformation3D:public dl32Matrix4x4
{
public:
	dl32Transformation3D();
	dl32Transformation3D(dl32Matrix4x4 &matrix);
	dl32Transformation3D(float m11,float m12,float m13,float m14,
		float m21,float m22,float m23,float m24,
		float m31,float m32,float m33,float m34,
		float m41,float m42,float m43,float m44);

	void Apply(dl32Point3D *Point);
	void Apply(float *x,float *y,float *z);
	dl32Point3D Apply(float x,float y,float z) {return dl32Point3D(m11*x+m12*y+m13*z+m14,m21*x+m22*y+m23*z+m24,m31*x+m32*y+m33*z+m34);};
	dl32Point3D Apply(dl32Point3D point) {return dl32Point3D(m11*point.x+m12*point.y+m13*point.z+m14,m21*point.x+m22*point.y+m23*point.z+m24,m31*point.x+m32*point.y+m33*point.z+m34);};

	void Concat(dl32Transformation3D Transformation){*this=dl32Transformation3D(dl32Matrix4x4::Mul(Transformation,*this));};
	void ReverseConcat(dl32Transformation3D Transformation){*this=dl32Transformation3D(dl32Matrix4x4::Mul(*this,Transformation));};

	static dl32Transformation3D& Concat(dl32Transformation3D T1,dl32Transformation3D T2){return dl32Transformation3D(dl32Matrix4x4::Mul(T2,T1));};
	static dl32Transformation3D& ReverseConcat(dl32Transformation3D T1, dl32Transformation3D T2){return dl32Transformation3D(dl32Matrix4x4::Mul(T1,T2));};

	static dl32Transformation3D& Translation(float tx,float ty,float tz)
	{
		return dl32Transformation3D(1,0,0,tx,
			0,1,0,ty,
			0,0,1,tz,
			0,0,0,1);
	};

	static dl32Transformation3D& Translation(dl32Vector3D Translation)
	{
		return dl32Transformation3D(1,0,0,Translation.x,
			0,1,0,Translation.y,
			0,0,1,Translation.z,
			0,0,0,1);
	};

	static dl32Transformation3D& Translation(dl32Point3D origin,dl32Point3D destiny)
	{
		return dl32Transformation3D(1,0,0,destiny.x-origin.x,
			0,1,0,destiny.y-origin.y,
			0,0,1,destiny.z-origin.z,
			0,0,0,1);
	};

	static dl32Transformation3D& Scale(float tx,float ty,float tz)
	{
		return dl32Transformation3D(1,0,0,tx,
			0,1,0,ty,
			0,0,1,tz,
			0,0,0,1);
	};

	static dl32Transformation3D& Scale(dl32Vector3D Translation)
	{
		return dl32Transformation3D(1,0,0,Translation.x,
			0,1,0,Translation.y,
			0,0,1,Translation.z,
			0,0,0,1);
	};

	static dl32Transformation3D& Rotation(dl32Quaternion &rotation){if(!rotation.MatrixReady()) rotation.SetupMatrix();return dl32Transformation3D(rotation);};
	static dl32Transformation3D& Rotation(dl32Point3D center,dl32Quaternion &quaternion){if(!quaternion.MatrixReady()) quaternion.SetupMatrix(); return dl32Transformation3D(dl32Matrix4x4::Mul(dl32Transformation3D::Translation(center.x,center.y,center.z),dl32Matrix4x4::Mul(dl32Transformation3D(quaternion),dl32Transformation3D::Translation(-center.x,-center.y,-center.z))));};
	static dl32Transformation3D& Rotation(dl32Vector3D origin,dl32Vector3D destiny){return Rotation(dl32Quaternion(dl32Vector3D::VectorialMul(origin,destiny),dl32Vector3D::Angle(origin,destiny)));};
	static dl32Transformation3D& Rotation(dl32Point3D center,dl32Vector3D origin,dl32Vector3D destiny){return Rotation(center,dl32Quaternion(dl32Vector3D::VectorialMul(origin,destiny),dl32Vector3D::Angle(origin,destiny)));};
	static dl32Transformation3D& Rotation(dl32Vector3D axis,float angle){return dl32Transformation3D(dl32Quaternion(axis,angle));};
	static dl32Transformation3D& Rotation(dl32CoordinateAxis axis,float angle){return dl32Transformation3D(dl32Quaternion(axis,angle));};

};

struct dl32SplineInterval
{
	float a,b,c,d;//a + bu + cu^2 + du^3, con u en el intervalo [0,1]
	float Interpolate(float u){return a + b*u + c*u*u + d*u*u*u;};
};

class dl32Spline //http://t.co/CrHFdGmB
{
	friend class dl32Spline;
private:
	vector<dl32Point2D> nodes;
	int nodecount;
	vector<dl32SplineInterval> intervalsX;
	vector<dl32SplineInterval> intervalsY;

	void Compute();
public:
	dl32Spline(){nodecount=-1;};
	dl32Spline(dl32Point2D* Nodes,int Count);
	~dl32Spline();

	vector<dl32Point2D> Interpolate(int PointsPerInterval=0);
	int GetIntervalsCount(){return nodecount-1;};
	int GetNodesCount(){return nodecount;};
};
#endif