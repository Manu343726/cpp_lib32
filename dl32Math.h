#ifndef MATH_H
#define MATH_H

#include <vector>
#include "dl32Preprocessor.h"
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
	dl32Point2D(float ValX,float ValY){x=ValX;y=ValY;};
	float operator [](int coord)throw (dl32OutOfRangeException);

	dl32Point2D operator()(){x=0;y=0;return *this;};
	dl32Point2D operator()(float x,float y){this->x=x;this->y=y;return *this;};

	static dl32Point2D Add(dl32Point2D P1,dl32Point2D P2){return dl32Point2D(P1.x+P2.x,P1.y+P2.y);};
	static dl32Point2D Sub(dl32Point2D P1,dl32Point2D P2){return dl32Point2D(P1.x-P2.x,P1.y-P2.y);};
	static dl32Point2D Mul(dl32Point2D Point,float Factor){return dl32Point2D(Point.x*Factor,Point.y*Factor);};
	static dl32Point2D Div(dl32Point2D Point,float Divisor)throw(dl32DividedByCeroException);

	static dl32Point2D Baricenter(dl32Point2D PointList[],int PointCount,int TypeSize=sizeof(dl32Point2D));

	dl32Point2D operator+(dl32Point2D point){return Add(*this,point);};
	dl32Point2D operator-(){return dl32Point2D(-x,-y);};
	dl32Point2D operator-(dl32Point2D point){return Sub(*this,point);};
	dl32Point2D operator*(float factor){return Mul(*this,factor);};
	dl32Point2D operator/(float div)throw(dl32DividedByCeroException){return Div(*this,div);};

	bool operator==(dl32Point2D point){return x==point.x && y==point.y;};
	bool operator!=(dl32Point2D point){return x!=point.x || y!=point.y;};
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

	float GetLength(){return sqrt(x*x+y*y);};
	void SetLength(float length){Normalize();x*=length;y*=length;};
	float GetSquareLenght(){return x*x+y*y;};
	void Normalize();
	dl32Vector2D GetUnitary(){float m=sqrt(x*x+y*y);return dl32Vector2D(x/m,y/m);};
	dl32Vector2D GetNormal(bool Up=true){return (Up ? dl32Vector2D(-y,x) : dl32Vector2D(y,-x));};

	static dl32Vector2D Add(dl32Vector2D P1,dl32Vector2D P2){return dl32Vector2D(P1.x+P2.x,P1.y+P2.y);};
	static dl32Vector2D Sub(dl32Vector2D P1,dl32Vector2D P2){return dl32Vector2D(P1.x-P2.x,P1.y-P2.y);};
	static dl32Vector2D Mul(dl32Vector2D Vector,float Factor){return dl32Vector2D(Vector.x*Factor,Vector.y*Factor);};
	static dl32Vector2D Div(dl32Vector2D Vector,float Divisor)throw(dl32DividedByCeroException);

	dl32Vector2D operator+(dl32Vector2D Vector){return Add(*this,Vector);};
	dl32Vector2D operator-(){return dl32Vector2D(-x,-y);};
	dl32Vector2D operator-(dl32Vector2D Vector){return Sub(*this,Vector);};
	dl32Vector2D operator*(float factor){return Mul(*this,factor);};
	dl32Vector2D operator/(float div)throw(dl32DividedByCeroException){return Div(*this,div);};

	dl32Vector2D operator+=(dl32Vector2D vector){*this=Add(*this,vector);return *this;};
	dl32Vector2D operator-=(dl32Vector2D vector){*this=Sub(*this,vector);return *this;};

	static float Mul(dl32Vector2D V1,dl32Vector2D V2){return V1.x*V2.x+V1.y*V2.y;};
	static float Angle(dl32Vector2D V1, dl32Vector2D V2){return acos(Mul(V1,V2)/(V1.GetLength()*V2.GetLength()));};

	float operator*(dl32Vector2D vector){return Mul(*this,vector);};
	float operator^(dl32Vector2D vector){return Angle(*this,vector);};
};

////////////////////////////////
///Represents a line in 2D space
////////////////////////////////
class dl32Line2D
{
protected:
	dl32Point2D position;
	dl32Vector2D direction;
	float a,b,c;
public:
	dl32Line2D();
	dl32Line2D(float A,float B,float C);
	dl32Line2D(dl32Point2D P1,dl32Point2D P2);
	dl32Line2D(dl32Point2D Position,dl32Vector2D Direction);
	dl32Line2D(dl32Point2D Position,float Slope);

	dl32Point2D GetPoint(float x);
	dl32Point2D GetPointByParameter(float param);

	bool BelongTo(float x,float y);
	bool BelongTo(dl32Point2D point) {return BelongTo(point.x,point.y);};
	float GetRelativePosition(float x,float y);
	float GetRelativePosition(dl32Point2D point) {return GetRelativePosition(point.x,point.y);};

	dl32Point2D GetPosition();
	dl32Vector2D GetDirection();
	float GetSlope();

	void SetPosition(float x,float y);
	void SetPosition(dl32Point2D position);
	void SetDirection(float x,float y);
	void SetDirection(dl32Vector2D pirection);
	void SetSlope(float slope);
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
	friend class dl32AABB2D;
private:
	float mWidth;
	float mHeight;
public:
	dl32Point2D Position; //Coordenadas de la esquina superior izquierda del AABB

	dl32AABB2D();
	dl32AABB2D(float X, float Y, float Width, float Height);
	dl32AABB2D(dl32Point2D Position, float Width, float Height);
	dl32AABB2D(dl32Point2D Position, dl32Vector2D Area);

	//Getters:
	float GetWidth();
	float GetHeight();
	dl32Point2D GetCenter();
	dl32Point2D GetUpLeftCorner();
	dl32Point2D GetUpRightCorner();
	dl32Point2D GetDownRightCorner();
	dl32Point2D GetDownLeftCorner();

	float GetLeft(){return Position.x;};
	float GetTop(){return Position.y;};
	float GetRight(){return Position.x+mWidth;};
	float GetBottom(){return Position.y+mHeight;};

	//Setters:
	void SetWidth(float Width);
	void SetHeight(float Height);
	void SetCenter(float X, float Y);
	void SetCenter(dl32Point2D &Center);

	bool BelongTo(dl32Point2D &Point){return BelongTo(Point.x,Point.y);};
	bool BelongTo(float X,float Y){return (X>=Position.x && X<=(Position.x+mWidth) && Y>=Position.y && Y<=(Position.y+mHeight));};

	static bool Collide(dl32AABB2D A1,dl32AABB2D A2);
	static dl32Orientation2D Orientation(dl32AABB2D A1,dl32AABB2D A2);

	bool operator&(dl32AABB2D &AABB){return Collide(*this,AABB);};
	bool operator&(dl32Point2D &point){return BelongTo(point);};
	bool operator ==(dl32AABB2D &AABB){return Position==AABB.Position && mWidth==AABB.mWidth && mHeight==AABB.mHeight;};
	bool operator !=(dl32AABB2D &AABB){return Position!=AABB.Position || mWidth!=AABB.mWidth || mHeight!=AABB.mHeight;};

	bool Collide(dl32AABB2D AABB){return Collide(*this,AABB);};
	dl32Orientation2D Orientation(dl32AABB2D AABB){return Orientation(*this,AABB);};
};

////////////////////////////////
///Represents a 3x3 float matrix
////////////////////////////////
typedef struct dl323x3Matrix
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

	dl323x3Matrix();
	dl323x3Matrix(float m11,float m12,float m13,
				  float m21,float m22,float m23,
				  float m31,float m32,float m33);

	float* operator[](int index){return m[index];};

	float GetDeterminant(){return GetDeterminant(*this);};
	dl323x3Matrix GetInverse(){return GetInverse(*this);};

	static dl323x3Matrix& GetUnity()
	{
		return dl323x3Matrix(1,0,0,
							 0,1,0,
							 0,0,1);
	};
	
	static dl323x3Matrix Add(dl323x3Matrix m1,dl323x3Matrix m2);
	static dl323x3Matrix Sub(dl323x3Matrix m1,dl323x3Matrix m2);
	static dl323x3Matrix Mul(dl323x3Matrix m1,dl323x3Matrix m2);
	static dl323x3Matrix Mul(dl323x3Matrix matrix,float mul);

	static float GetDeterminant(dl323x3Matrix matrix);
	static dl323x3Matrix GetInverse(dl323x3Matrix matrix)throw(dl32InvalidMatrixOperationException);

	dl323x3Matrix operator+(dl323x3Matrix matrix){return Add(*this,matrix);};
	dl323x3Matrix operator-(dl323x3Matrix matrix){return Sub(*this,matrix);};
	dl323x3Matrix operator*(dl323x3Matrix matrix){return Mul(*this,matrix);};
	dl323x3Matrix operator*(float mul){return Mul(*this,mul);};
};
////////////////////////////////////////////////////
///Represents a geometric Transformationation in 2D space
////////////////////////////////////////////////////
class dl32Transformation2D: public dl323x3Matrix
{
public:
	dl32Transformation2D():dl323x3Matrix(){};
	dl32Transformation2D(dl323x3Matrix &matrix):dl323x3Matrix(matrix){};
	dl32Transformation2D(float m11,float m12,float m13,
				    float m21,float m22,float m23,
				    float m31,float m32,float m33):dl323x3Matrix(m11,m12,m13,
																 m21,m22,m23,
																 m31,m32,m33){};

	void Apply(dl32Point2D *Point);
	void Apply(float *x,float *y);
	dl32Point2D Apply(float x,float y) {return dl32Point2D(m11*x+m12*y+m13,m21*x+m22*y+m23);};
	dl32Point2D Apply(dl32Point2D point) {return dl32Point2D(m11*point.x+m12*point.y+m13,m21*point.x+m22*point.y+m23);};

	void Concat(dl32Transformation2D Transformation) {*this=dl323x3Matrix::Mul(Transformation,*this);};
	dl32Transformation2D operator+(dl32Transformation2D &Transformation){return dl32Transformation2D(dl323x3Matrix::Mul(Transformation,*this));};
	static dl32Transformation2D Concat(dl32Transformation2D &t1,dl32Transformation2D &t2) {return dl32Transformation2D(dl323x3Matrix::Mul(t2,t1));};

	static dl32Transformation2D& Translation(float dx,float dy)
	{
		return dl32Transformation2D(1,0,dx,
							   0,1,dy,
							   0,0,1);
	};

	static dl32Transformation2D& Translation(dl32Point2D origin,dl32Point2D destiny)
	{
		return dl32Transformation2D(1,0,destiny.x-origin.x,
							   0,1,destiny.y-origin.y,
							   0,0,1);
	};
	
	static dl32Transformation2D& Translation(dl32Vector2D Translation)
	{
		return dl32Transformation2D(1,0,Translation.x,
							   0,1,Translation.y,
							   0,0,1);
	};

	static dl32Transformation2D& Scale(float scale)
	{
		return dl32Transformation2D(scale,0,0,
							   0,scale,0,
							   0,0,1);
	};

	static dl32Transformation2D& Scale(float sx,float sy)
	{
			return dl32Transformation2D(sx,0,0,
								   0,sy,0,
								   0,0,1);
	};

	static dl32Transformation2D& Scale(dl32Vector2D scale)
	{
			return dl32Transformation2D(scale.x,0,0,
								   0,scale.y,0,
								   0,0,1);
	};

	static dl32Transformation2D& Rotation(float angle)
	{
			return dl32Transformation2D(cos(angle),sin(angle),0,
								   -sin(angle),cos(angle),0,
								   0,0,1);
	};

	static dl32Transformation2D& Rotation(float sin_angle,float cos_angle)
	{
			return dl32Transformation2D(cos_angle,sin_angle,0,
								   -sin_angle,cos_angle,0,
								   0,0,1);
	};

	static dl32Transformation2D& Rotation(float center_x,float center_y,float angle);
	static dl32Transformation2D& Rotation(dl32Point2D center,float angle);
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

////////////////////////////////
///Represents a 4x4 float matrix
////////////////////////////////
struct dl324x4Matrix
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

	dl324x4Matrix();
	dl324x4Matrix(float m11,float m12,float m13,float m14,
				  float m21,float m22,float m23,float m24,
				  float m31,float m32,float m33,float m34,
				  float m41,float m42,float m43,float m44);

	float* operator[](int index){return m[index];};

	static dl324x4Matrix GetUnity()
	{
		return dl324x4Matrix(1,0,0,0,
							 0,1,0,0,
							 0,0,1,0,
							 0,0,0,1);
	};
	
	static dl324x4Matrix Add(dl324x4Matrix m1,dl324x4Matrix m2);
	static dl324x4Matrix Sub(dl324x4Matrix m1,dl324x4Matrix m2);
	static dl324x4Matrix Mul(dl324x4Matrix m1,dl324x4Matrix m2);
	static dl324x4Matrix Mul(dl324x4Matrix matrix,float mul);

	static float GetDeterminant(dl324x4Matrix &matrix);

	dl324x4Matrix operator+(dl324x4Matrix matrix){return Add(*this,matrix);};
	dl324x4Matrix operator-(dl324x4Matrix matrix){return Sub(*this,matrix);};
	dl324x4Matrix operator*(dl324x4Matrix matrix){return Mul(*this,matrix);};
	dl324x4Matrix operator*(float mul){return Mul(*this,mul);};
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
class dl32Quaternion:public dl324x4Matrix
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
		return dl32Quaternion((Q1.a * Q2.a) - (Q1.b * Q2.b) - (Q1.c * Q2.c) - (Q1.d * Q2.d),
						      (Q1.a * Q2.b) + (Q1.b * Q2.a) + (Q1.c * Q2.d) - (Q1.d * Q2.c),
						      (Q1.a * Q2.c) - (Q1.b * Q2.d) + (Q1.c * Q2.a) + (Q1.d * Q2.b),
						      (Q1.a * Q2.d) + (Q1.b + Q2.c) - (Q1.c * Q2.b) + (Q1.d * Q2.a));
	};
};

////////////////////////////////////////////////////
///Represents a geometric Transformationation in 3D space
////////////////////////////////////////////////////
class dl32Transformation3D:public dl324x4Matrix
{
public:
	dl32Transformation3D();
	dl32Transformation3D(dl324x4Matrix &matrix);
	dl32Transformation3D(float m11,float m12,float m13,float m14,
					float m21,float m22,float m23,float m24,
					float m31,float m32,float m33,float m34,
					float m41,float m42,float m43,float m44);

	void Apply(dl32Point3D *Point);
	void Apply(float *x,float *y,float *z);
	dl32Point3D Apply(float x,float y,float z) {return dl32Point3D(m11*x+m12*y+m13*z+m14,m21*x+m22*y+m23*z+m24,m31*x+m32*y+m33*z+m34);};
	dl32Point3D Apply(dl32Point3D point) {return dl32Point3D(m11*point.x+m12*point.y+m13*point.z+m14,m21*point.x+m22*point.y+m23*point.z+m24,m31*point.x+m32*point.y+m33*point.z+m34);};

	void Concat(dl32Transformation3D Transformation){*this=dl32Transformation3D(dl324x4Matrix::Mul(Transformation,*this));};
	void ReverseConcat(dl32Transformation3D Transformation){*this=dl32Transformation3D(dl324x4Matrix::Mul(*this,Transformation));};

	static dl32Transformation3D& Concat(dl32Transformation3D T1,dl32Transformation3D T2){return dl32Transformation3D(dl324x4Matrix::Mul(T2,T1));};
	static dl32Transformation3D& ReverseConcat(dl32Transformation3D T1, dl32Transformation3D T2){return dl32Transformation3D(dl324x4Matrix::Mul(T1,T2));};

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
	static dl32Transformation3D& Rotation(dl32Point3D center,dl32Quaternion &quaternion){if(!quaternion.MatrixReady()) quaternion.SetupMatrix(); return dl32Transformation3D(dl324x4Matrix::Mul(dl32Transformation3D::Translation(center.x,center.y,center.z),dl324x4Matrix::Mul(dl32Transformation3D(quaternion),dl32Transformation3D::Translation(-center.x,-center.y,-center.z))));};
	static dl32Transformation3D& Rotation(dl32Vector3D origin,dl32Vector3D destiny){return Rotation(dl32Quaternion(dl32Vector3D::VectorialMul(origin,destiny),dl32Vector3D::Angle(origin,destiny)));};
	static dl32Transformation3D& Rotation(dl32Point3D center,dl32Vector3D origin,dl32Vector3D destiny){return Rotation(center,dl32Quaternion(dl32Vector3D::VectorialMul(origin,destiny),dl32Vector3D::Angle(origin,destiny)));};
	static dl32Transformation3D& Rotation(dl32Vector3D axis,float angle){return dl32Transformation3D(dl32Quaternion(axis,angle));};
	static dl32Transformation3D& Rotation(dl32CoordinateAxis axis,float angle){return dl32Transformation3D(dl32Quaternion(axis,angle));};

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
	dl32Matrix(dl323x3Matrix &matrix);
	dl32Matrix(dl324x4Matrix &matrix);

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