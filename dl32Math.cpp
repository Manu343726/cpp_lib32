#include "dl32Math.h"
#include <cmath>

inline float Min(float a,float b)
{
	return a<b ? a:b;
}

//Geometria (2D):

// dl322DPoint: ////////////////////////////////
float dl322DPoint::operator[](int coord)throw(dl32OutOfRangeException)
{
	switch(coord)
	{
	case 0:
		return x;
	case 1:
		return y;
	default:
		throw dl32OutOfRangeException(dl32Range(2),coord,"Point2D::Operator[](int coord): 'coord' must be 0 or 1");
	}
}

dl322DPoint dl322DPoint::Div(dl322DPoint Point, float divisor)throw(dl32DividedByCeroException)
{
	if (divisor!=0)
	{
		Point.x/=divisor;
		Point.y/=divisor;		
		return Point;
	}
	else
		throw dl32DividedByCeroException("dl322DPoint::Div(dl322DPoint point,float divisor): 'divisor' must be different from 0");
}

dl322DPoint dl322DPoint::Baricenter(dl322DPoint PointList[],int PointCount,int TypeSize)
{
	dl322DPoint Return;

	for(int i=0;i<PointCount;++i)
	{
		Return.x+=PointList[i].x;
		Return.y+=PointList[i].y;
	}

	return dl322DPoint(Return.x/PointCount,Return.y/PointCount);
}

// dl322DVector: ////////////////////////////////
void dl322DVector::Normalize()
{
	float m=sqrt(x*x+y*y);

	if(m>0)
	{
		x/=m;
		y/=m;
	}
}

// dl322DLine: //////////////////////////////////////////////
dl322DLine::dl322DLine()
{
	a=0;
	b=0;
	c=0;
}

dl322DLine::dl322DLine(float A,float B,float C)
{
	a=A;
	b=B;
	c=C;
	direction.x=-b;
	direction.y=a;
	position.x=0;
	position.y=-c/B;
}

dl322DLine::dl322DLine(dl322DPoint P1,dl322DPoint P2)
{
	direction.x=P2.x-P1.x;
	direction.y=P2.y-P1.y;
	direction.Normalize();
	position=P1;
	//Ecuacion general: 
	//Ax+By+C=0
	//A*Px+B*Py+C=0 (Si P pertenece a la recta)
	//Por tanto:
	a=direction.y;
	b=-direction.x;
	c=-(position.x*a+position.x*b);
}

dl322DLine::dl322DLine(dl322DPoint Position,dl322DVector Direction)
{
	position=Position;
	direction=Direction;
	a=direction.y;
	b=-direction.x;
	//Ecuacion general: 
	//Ax+By+C=0
	//A*Px+B*Py+C=0 (Si P pertenece a la recta)
	//Por tanto:
	c=-(position.x*a+position.x*b);
}

dl322DLine::dl322DLine(dl322DPoint Position,float Slope)
{
	float mod;
	position=Position;

	//Ecuacion Punto-Pendiente:

	//Y-Py=m(X-Px)
	//Y-Py=mX-mPx
	//mX-Y-mPx+Py=0:
	// --> a=m
	// --> b=-1
	// --> c=Py-mPx

	//Normalizando: a=a/mod, b=b/mod, c=c/mod
	//mod=sqrt(a^2+b^2+c^2)
	//mod=sqrt(a*a+b*b+c*c)
	//mod=sqrt(Slope*Slope + 1 + (Py-mPx)^2))
	//mod=sqrt(Slope*Slope + 1 + (Py*Py - 2*PymPx + mPx*mPx))

	mod=sqrt(Slope*Slope+1+(position.y*position.y-2*position.y*Slope*position.x+Slope*position.x*Slope*position.x));
	a=Slope/mod;
	b=-1/mod;
	c=(position.y-Slope*position.x)/mod;
	direction.x=-b;
	direction.y=a;
}

dl322DPoint dl322DLine::GetPoint(float x)
{
	return dl322DPoint(x,(a*x+c)/-b);
}

dl322DPoint dl322DLine::GetPointByParameter(float param)
{
	return dl322DPoint(position.x+direction.x*param,position.y+direction.y*param);
}

bool dl322DLine::BelongTo(float x, float y)
{
	return a*x+b*y+c==0;
}

float dl322DLine::GetRelativePosition(float x, float y)
{
	return a*x+b*y+c;
}

dl322DPoint dl322DLine::GetPosition()
{
	return position;
}

dl322DVector dl322DLine::GetDirection()
{
	return direction;
}

float dl322DLine::GetSlope()
{
	return -b/a;
}

// dl322DAABB: //////////////////////////////////////////////
dl322DAABB::dl322DAABB()
{
	Position.x=0;
	Position.y=0;
	mWidth=0;
	mHeight=0;
}

dl322DAABB::dl322DAABB(float X, float Y, float Width, float Height)
{
	Position.x=X;
	Position.y=Y;
	mWidth=DL32MACRO_TONATURAL(Width);
	mHeight=DL32MACRO_TONATURAL(Height);
}

dl322DAABB::dl322DAABB(dl322DPoint Position, float Width, float Height)
{
	this->Position=Position;
	mWidth=DL32MACRO_TONATURAL(Width);
	mHeight=DL32MACRO_TONATURAL(Height);
}

dl322DAABB::dl322DAABB(dl322DPoint Position, dl322DVector Area)
{
	this->Position=Position;
	mWidth=DL32MACRO_TONATURAL(Area.x);
	mHeight=DL32MACRO_TONATURAL(Area.y);
}

float dl322DAABB::GetWidth()
{
	return mWidth;
}

float dl322DAABB::GetHeight()
{
	return mHeight;
}

void dl322DAABB::SetWidth(float ValWidth)
{
	if (ValWidth>0) mWidth=ValWidth;
}

void dl322DAABB::SetHeight(float ValHeight)
{
	if (ValHeight>0) mHeight=ValHeight;
}

dl322DPoint dl322DAABB::GetCenter()
{
	return dl322DPoint(Position.x+(mWidth/2),Position.y+(mHeight/2));
}

dl322DPoint dl322DAABB::GetUpLeftCorner()
{
	return Position;
}

dl322DPoint dl322DAABB::GetUpRightCorner()
{
	return dl322DPoint(Position.x+mWidth,Position.y);
}

dl322DPoint dl322DAABB::GetDownRightCorner()
{
	return dl322DPoint(Position.x+mWidth,Position.y+mHeight);
}

dl322DPoint dl322DAABB::GetDownLeftCorner()
{
	return dl322DPoint(Position.x,Position.y+mHeight);
}

void dl322DAABB::SetCenter(float X, float Y)
{
	Position.x=X-(mWidth/2);
	Position.y=Y-(mHeight/2);
}

void dl322DAABB::SetCenter(dl322DPoint &Center)
{
	Position.x=Center.x-(mWidth/2);
	Position.y=Center.y-(mHeight/2);
}

bool dl322DAABB::Collide(dl322DAABB C1, dl322DAABB C2)
{
	dl322DAABB BigAABB=dl322DAABB(C1.Position.x-C2.GetWidth(),C1.Position.y-C2.GetHeight(),C1.GetWidth()+C2.GetWidth(),C1.GetHeight()+C2.GetHeight());

	return BigAABB.BelongTo(C2.Position);
}

dl322DOrientation dl322DAABB::Orientation(dl322DAABB Origin, dl322DAABB AABB)
{
	dl322DOrientation Retorno;
	bool ArribaIzquierda=Origin.BelongTo(AABB.GetUpLeftCorner());
	bool ArribaDerecha=Origin.BelongTo(AABB.GetUpRightCorner());
	bool AbajoDerecha=Origin.BelongTo(AABB.GetDownRightCorner());
	bool AbajoIzquierda=Origin.BelongTo(AABB.GetDownLeftCorner());

	if (ArribaIzquierda && ArribaDerecha)
		Retorno=SOUTH;
	else if(AbajoIzquierda && AbajoDerecha)
		Retorno=NORTH;
	else if (ArribaDerecha && AbajoDerecha)
		Retorno=WEST;
	else if (ArribaIzquierda && AbajoIzquierda)
		Retorno=EAST;
	else if(ArribaIzquierda)
		Retorno=SOUTHEAST;
	else if (ArribaDerecha)
		Retorno=SOUTHWEST;
	else if (AbajoDerecha)
		Retorno=NORTHWEST;
	else if (AbajoIzquierda)
		Retorno=NORTHEAST;
	else
		Retorno=OTHER;

	return Retorno;
}

//Matrices 2D (Matrices 3x3, transformaciones 2D):
//////////////////////////////////////////////////

dl323x3Matrix::dl323x3Matrix()
{
	m11=0;m12=0;m13=0;
	m21=0;m22=0;m23=0;
	m31=0;m32=0;m33=0;
}

dl323x3Matrix::dl323x3Matrix(float m11,float m12,float m13,
							  float m21,float m22,float m23,
						      float m31,float m32,float m33)
{
	this->m11=m11;this->m12=m12;this->m13=m13;
	this->m21=m21;this->m22=m22;this->m23=m23;
	this->m31=m31;this->m32=m32;this->m33=m33;
}

dl323x3Matrix Get3x3Unity()
{
	return dl323x3Matrix(1,0,0,
						 0,1,0,
						 0,0,1);
}

 dl323x3Matrix dl323x3Matrix::Add(dl323x3Matrix m1,dl323x3Matrix m2)
{
	return dl323x3Matrix(m1.m11+m2.m11,m1.m12+m1.m12,m1.m13+m2.m13,
						 m1.m21+m2.m21,m1.m22+m1.m22,m1.m23+m2.m23,
						 m1.m31+m2.m31,m1.m32+m1.m32,m1.m33+m2.m33);
}

 dl323x3Matrix dl323x3Matrix::Sub(dl323x3Matrix m1,dl323x3Matrix m2)
{
	return dl323x3Matrix(m1.m11-m2.m11,m1.m12-m1.m12,m1.m13-m2.m13,
						 m1.m21-m2.m21,m1.m22-m1.m22,m1.m23-m2.m23,
						 m1.m31-m2.m31,m1.m32-m1.m32,m1.m33-m2.m33);
}

 dl323x3Matrix dl323x3Matrix::Mul(dl323x3Matrix m1,dl323x3Matrix m2)
{
	return dl323x3Matrix(m1.m11*m2.m11+m1.m12*m2.m21+m1.m13*m2.m31,
						 m1.m11*m2.m12+m1.m12*m2.m22+m1.m13*m2.m32,
						 m1.m11*m2.m13+m1.m12*m2.m23+m1.m13*m2.m33,

						 m1.m21*m2.m11+m1.m22*m2.m21+m1.m23*m2.m31,
						 m1.m21*m2.m12+m1.m22*m2.m22+m1.m23*m2.m32,
						 m1.m21*m2.m13+m1.m22*m2.m23+m1.m23*m2.m33,
						 
						 m1.m31*m2.m11+m1.m32*m2.m21+m1.m33*m2.m31,
						 m1.m31*m2.m12+m1.m32*m2.m22+m1.m33*m2.m32,
						 m1.m31*m2.m13+m1.m32*m2.m23+m1.m33*m2.m33);
}

 dl323x3Matrix dl323x3Matrix::Mul(dl323x3Matrix matrix,float mul)
{
	return dl323x3Matrix(matrix.m11*mul,matrix.m12*mul,matrix.m13*mul,
						 matrix.m21*mul,matrix.m22*mul,matrix.m23*mul,
						 matrix.m31*mul,matrix.m32*mul,matrix.m33*mul);
}

 float dl323x3Matrix::GetDeterminant(dl323x3Matrix matrix)
{
	return (matrix.m11*matrix.m22*matrix.m33+
		   matrix.m12*matrix.m23*matrix.m31+
		   matrix.m21*matrix.m33*matrix.m13-
		   matrix.m13*matrix.m22*matrix.m31-
		   matrix.m12*matrix.m21*matrix.m33-
		   matrix.m11*matrix.m23*matrix.m32);
}  
 dl323x3Matrix dl323x3Matrix::GetInverse(dl323x3Matrix matrix)throw(dl32InvalidMatrixOperationException)
 {
	 float det=dl323x3Matrix::GetDeterminant(matrix);

	 if(det!=0)
		 return dl323x3Matrix((matrix.m22*matrix.m33-matrix.m23*matrix.m32)/det,-(matrix.m12*matrix.m33-matrix.m13*matrix.m32)/det,(matrix.m12*matrix.m23-matrix.m13*matrix.m22)/det,
							  -(matrix.m21*matrix.m33-matrix.m23*matrix.m31)/det,(matrix.m11*matrix.m33-matrix.m13*matrix.m31)/det,-(matrix.m11*matrix.m23-matrix.m13*matrix.m21)/det,
							  (matrix.m21*matrix.m32-matrix.m22*matrix.m31)/det,-(matrix.m11*matrix.m32-matrix.m12*matrix.m31)/det,(matrix.m11*matrix.m22-matrix.m12*matrix.m21)/det);
	 else
		 throw dl32InvalidMatrixOperationException("dl323x3Matrix::GetInverse(dl323x3Matrix matrix): 'matrix' is no-invertible");
 }

dl322DTransform::dl322DTransform()
{
	m11=0;m12=0;m13=0;
	m21=0;m22=0;m23=0;
	m31=0;m32=0;m33=0;
}

dl322DTransform::dl322DTransform(float m11,float m12,float m13,
							  float m21,float m22,float m23,
						      float m31,float m32,float m33)
{
	this->m11=m11;this->m12=m12;this->m13=m13;
	this->m21=m21;this->m22=m22;this->m23=m23;
	this->m31=m31;this->m32=m32;this->m33=m33;
}

dl322DTransform::dl322DTransform(dl323x3Matrix &matrix)
{
	this->m11=matrix.m11;this->m12=matrix.m12;this->m13=matrix.m13;
	this->m21=matrix.m21;this->m22=matrix.m22;this->m23=matrix.m23;
	this->m31=matrix.m31;this->m32=matrix.m32;this->m33=matrix.m33;
}

void dl322DTransform::Apply(dl322DPoint *point)
{
	*point=dl322DPoint(m11*point->x+m12*point->y+m13,
		  m21*point->x+m22*point->y+m23);
}

void dl322DTransform::Apply(float *x,float *y)
{
	float xx,yy;
	xx=m11*(*x)+m12*(*y)+m13;
	yy=m21*(*x)+m22*(*y)+m23;
	*x=xx;
	*y=yy;
}

dl322DTransform& dl322DTransform::Rotation(float center_x,float center_y,float angle)
{
		return dl322DTransform(dl323x3Matrix::Mul(dl323x3Matrix::Mul(dl322DTransform::Traslation(center_x,center_y),dl322DTransform::Rotation(angle)),dl322DTransform::Traslation(-center_x,-center_y)));
};

dl322DTransform& dl322DTransform::Rotation(dl322DPoint center,float angle)
{
		return dl322DTransform(dl323x3Matrix::Mul(dl323x3Matrix::Mul(dl322DTransform::Traslation(center.x,center.y),dl322DTransform::Rotation(angle)),dl322DTransform::Traslation(-center.x,-center.y)));
};

dl323DPoint dl323DPoint::Div(dl323DPoint Point,float divisor)throw(dl32DividedByCeroException)
{
	if(divisor!=0)
	{
		Point.x/=divisor;
		Point.y/=divisor;
		Point.z/=divisor;
		return Point;
	}
	else
		throw dl32DividedByCeroException("dl323DPoint::Div(dl323DPoint point,float divisor): 'divisor' must be different from 0");
}

dl324x4Matrix::dl324x4Matrix()
{
	m11=0;m12=0;m13=0;m14=0;
	m21=0;m22=0;m23=0;m24=0;
	m31=0;m32=0;m33=0;m34=0;
	m41=0;m42=0;m43=0;m44=0;
}

dl324x4Matrix::dl324x4Matrix(float m11,float m12,float m13,float m14,
							  float m21,float m22,float m23,float m24,
							  float m31,float m32,float m33,float m34,
							  float m41,float m42,float m43,float m44)
{
	this->m11=m11;this->m12=m12;this->m13=m13;this->m14=m14;
	this->m21=m21;this->m22=m22;this->m23=m23;this->m24=m24;
	this->m31=m31;this->m32=m32;this->m33=m33;this->m34=m34;
	this->m41=m41;this->m42=m42;this->m43=m43;this->m44=m44;
}

dl324x4Matrix dl324x4Matrix::Add(dl324x4Matrix m1,dl324x4Matrix m2)
{
	return dl324x4Matrix(m1.m11+m2.m11,m1.m12+m2.m12,m1.m13+m2.m13,m1.m14+m2.m14,
						 m1.m21+m2.m21,m1.m22+m2.m22,m1.m23+m2.m23,m1.m24+m2.m24,
						 m1.m31+m2.m31,m1.m32+m2.m32,m1.m33+m2.m33,m1.m34+m2.m34,
						 m1.m41+m2.m41,m1.m42+m2.m42,m1.m43+m2.m43,m1.m44+m2.m44);
}

dl324x4Matrix dl324x4Matrix::Sub(dl324x4Matrix m1,dl324x4Matrix m2)
{
	return dl324x4Matrix(m1.m11-m2.m11,m1.m12-m2.m12,m1.m13-m2.m13,m1.m14-m2.m14,
						 m1.m21-m2.m21,m1.m22-m2.m22,m1.m23-m2.m23,m1.m24-m2.m24,
						 m1.m31-m2.m31,m1.m32-m2.m32,m1.m33-m2.m33,m1.m34-m2.m34,
						 m1.m41-m2.m41,m1.m42-m2.m42,m1.m43-m2.m43,m1.m44-m2.m44);
}

 dl324x4Matrix dl324x4Matrix::Mul(dl324x4Matrix m1,dl324x4Matrix m2)
{
	return dl324x4Matrix(m1.m11*m2.m11+m1.m12*m2.m21+m1.m13*m2.m31+m1.m14*m2.m41,
						 m1.m11*m2.m12+m1.m12*m2.m22+m1.m13*m2.m32+m1.m14*m2.m42,
						 m1.m11*m2.m13+m1.m12*m2.m23+m1.m13*m2.m33+m1.m14*m2.m43,
						 m1.m11*m2.m14+m1.m12*m2.m24+m1.m13*m2.m34+m1.m14*m2.m44,

						 m1.m21*m2.m11+m1.m22*m2.m21+m1.m23*m2.m31+m1.m24*m2.m41,
						 m1.m21*m2.m12+m1.m22*m2.m22+m1.m23*m2.m32+m1.m24*m2.m42,
						 m1.m21*m2.m13+m1.m22*m2.m23+m1.m23*m2.m33+m1.m24*m2.m43,
						 m1.m21*m2.m14+m1.m22*m2.m24+m1.m23*m2.m34+m1.m24*m2.m44,
						 
						 m1.m31*m2.m11+m1.m32*m2.m21+m1.m33*m2.m31+m1.m34*m2.m41,
						 m1.m31*m2.m12+m1.m32*m2.m22+m1.m33*m2.m32+m1.m34*m2.m42,
						 m1.m31*m2.m13+m1.m32*m2.m23+m1.m33*m2.m33+m1.m34*m2.m43,
						 m1.m31*m2.m14+m1.m32*m2.m24+m1.m33*m2.m34+m1.m34*m2.m44,
						 
						 m1.m41*m2.m11+m1.m42*m2.m21+m1.m43*m2.m31+m1.m44*m2.m41,
						 m1.m41*m2.m12+m1.m42*m2.m22+m1.m43*m2.m32+m1.m44*m2.m42,
						 m1.m41*m2.m13+m1.m42*m2.m23+m1.m43*m2.m33+m1.m44*m2.m43,
						 m1.m41*m2.m14+m1.m42*m2.m24+m1.m43*m2.m34+m1.m44*m2.m44);
}

dl324x4Matrix dl324x4Matrix::Mul(dl324x4Matrix matrix,float factor)
{
	return dl324x4Matrix(matrix.m11*factor,matrix.m12*factor,matrix.m13*factor,matrix.m14*factor,
						 matrix.m21*factor,matrix.m22*factor,matrix.m23*factor,matrix.m24*factor,
						 matrix.m31*factor,matrix.m32*factor,matrix.m33*factor,matrix.m34*factor,
						 matrix.m41*factor,matrix.m42*factor,matrix.m43*factor,matrix.m44*factor);
}

float dl324x4Matrix::GetDeterminant(dl324x4Matrix &matrix)
{
	return (matrix.m11*(matrix.m22*(matrix.m33*matrix.m44-matrix.m34*matrix.m43)-matrix.m23*(matrix.m32*matrix.m44-matrix.m34*matrix.m42)+matrix.m24*(matrix.m32*matrix.m43-matrix.m33*matrix.m42))-
			matrix.m12*(matrix.m21*(matrix.m33*matrix.m44-matrix.m34*matrix.m43)-matrix.m23*(matrix.m31*matrix.m44-matrix.m34*matrix.m41)+matrix.m24*(matrix.m31*matrix.m43-matrix.m33*matrix.m41))+
			matrix.m13*(matrix.m21*(matrix.m32*matrix.m44-matrix.m34*matrix.m42)-matrix.m22*(matrix.m31*matrix.m44-matrix.m34*matrix.m44)+matrix.m24*(matrix.m31*matrix.m42-matrix.m32*matrix.m41))-
			matrix.m14*(matrix.m21*(matrix.m32*matrix.m43-matrix.m33*matrix.m42)-matrix.m22*(matrix.m31*matrix.m43-matrix.m33*matrix.m43)+matrix.m23*(matrix.m31*matrix.m42-matrix.m32+matrix.m41)));
}

dl32Quaternion::dl32Quaternion(float a,float b,float c,float d,bool UseMatrix)
{
	Normalize(a,b,c,d,UseMatrix);
}

dl32Quaternion::dl32Quaternion(dl323DVector axis,float angle,bool UseMatrix)
{
	float Sin=sin(angle/2),Cos=cos(angle/2);

	Normalize(Cos,Sin*axis.x,Sin*axis.y,Sin*axis.z,UseMatrix);
}

dl32Quaternion::dl32Quaternion(dl32CoordinateAxis axis,float angle,bool UseMatrix)
{
	switch(axis)
	{
	case DL32AXIS_X:
		Normalize(cos(angle/2),sin(angle/2),0,0,UseMatrix);
		break;
	case DL32AXIS_Y:
		Normalize(cos(angle/2),0,sin(angle/2),0,UseMatrix);
		break;
	case DL32AXIS_Z:
		Normalize(cos(angle/2),0,0,sin(angle/2),UseMatrix);
		break;
	}
}

void dl32Quaternion::Normalize(const float &a,const float &b,const float &c,const float &d,bool UseMatrix)
{
	float m=sqrt(a*a+b*b+c*c+d*d);

	if(m!=0)
	{
		this->a=a/m;
		this->b=b/m;
		this->c=c/m;
		this->d=d/m;

		if(UseMatrix)
			SetupMatrix();
		else
			UseMatrix=false;
	}
	else
		this->a=0;this->b=0;this->c=0;this->d=0;matrixReady=false;
}

void dl32Quaternion::SetupMatrix()
{
	http://software.intel.com/sites/default/files/m/5/9/4/b/8/37726-293748.pdf
	float xx,yy,zz,xy,xz,yz,ax,ay,az;

	matrixReady=true;

	xx=b*b;
	yy=c*c;
	zz=d*d;
	xy=b*c;
	xz=b*d;
	yz=c*d;
	ax=a*b;
	ay=a*c;
	az=a*d;

	m11=1 - (2 * yy) - (2 * zz); m12=(2 * xy) - (2 * az); m13=(2 * xz) + (2 * ay); m14=0;
	m21=(2 * xy) + (2 * az); m21=1 - (2 * xx) - (2 * zz); m23=(2 * yz) - (2 * ax); m24=0;
    m31=(2 * xz) - (2 * ay); m32=(2 * yz) + (2 * ax); m33=1 - (2 * xx) - (2 * yy); m34=0;
    m41=0; m42=0; m43=0; m44=1;
}

dl323DTransform::dl323DTransform()
{
	m11=1;m12=0;m13=0;m14=0;
	m21=0;m22=1;m23=0;m24=0;
	m31=0;m32=0;m33=1;m34=0;
	m41=0;m42=0;m43=0;m44=1;
}

dl323DTransform::dl323DTransform(float m11,float m12,float m13,float m14,
							  float m21,float m22,float m23,float m24,
							  float m31,float m32,float m33,float m34,
							  float m41,float m42,float m43,float m44)
{
	this->m11=m11;this->m12=m12;this->m13=m13;this->m14=m14;
	this->m21=m11;this->m22=m12;this->m23=m13;this->m24=m14;
	this->m31=m11;this->m32=m12;this->m33=m13;this->m34=m14;
	this->m41=m11;this->m42=m12;this->m43=m13;this->m44=m14;
}

dl323DTransform::dl323DTransform(dl324x4Matrix &matrix)
{
	this->m11=matrix.m11;this->m12=matrix.m12;this->m13=matrix.m13;this->m14=matrix.m14;
	this->m21=matrix.m21;this->m22=matrix.m22;this->m23=matrix.m23;this->m24=matrix.m24;
	this->m31=matrix.m31;this->m32=matrix.m32;this->m33=matrix.m33;this->m34=matrix.m34;
	this->m41=matrix.m41;this->m42=matrix.m42;this->m43=matrix.m43;this->m44=matrix.m44;
}

void dl323DTransform::Apply(dl323DPoint *point)
{
	*point=dl323DPoint(m11*point->x+m12*point->y+m13*point->z+m14,m21*point->x+m22*point->y+m23*point->z+m24,m31*point->x+m32*point->y+m33*point->z+m34);
}

void dl323DTransform::Apply(float *x,float *y,float *z)
{
	float xx,yy,zz;
	xx=m11*(*x)+m12*(*y)+m13*(*z)+m14;
	yy=m21*(*x)+m22*(*y)+m23*(*z)+m24;
	zz=m31*(*x)+m32*(*y)+m33*(*z)+m34;
	*x=xx;
	*y=yy;
	*z=zz;
}


void dl32Matrix::CopyRef(dl32Matrix &matrix)
{
	NotInitialized=true;
	this->rows=matrix.rows;
	this->columns=matrix.columns;
	this->Array=matrix.Array;
}

dl32Matrix::dl32Matrix()
{
	NotInitialized=true;
	Array=NULL;
	rows=0;
	columns=0;
}

dl32Matrix::dl32Matrix(int rows,int columns)
{
	if(rows>0 && columns>0)
	{
		Array=new dl32MatrixRow[rows];

		NotInitialized=false;

		for(int i=0;i<rows;++i)
			Array[i]=new float[columns];

		this->rows=rows;
		this->columns=columns;
	}
	else	
	{
		Array=NULL;
		rows=0;
		columns=0;
	}
}

dl32Matrix::dl32Matrix(dl32Matrix &matrix)
{
	Dispose();

	NotInitialized=false;

	rows=matrix.GetRowsCount();
	columns=matrix.GetColumnsCount();

	Array=new dl32MatrixRow[rows];
	for(int i=0;i<rows;++i)
	{
		Array[i]=new float[columns];
		for(int j=0;j<columns;++j)
			Array[i][j]=matrix[i][j];
	}
}

dl32Matrix& dl32Matrix::operator=(dl32Matrix &matrix)
{
	if(this != &matrix)
	{
		Dispose();

		NotInitialized=false;

		rows=matrix.GetRowsCount();
		columns=matrix.GetColumnsCount();

		Array=new dl32MatrixRow[rows];
		for(int i=0;i<rows;++i)
		{
			Array[i]=new float[columns];
			for(int j=0;j<columns;++j)
				Array[i][j]=matrix[i][j];
		}
	}

	return *this;
}

dl32Matrix::dl32Matrix(dl323x3Matrix &Matrix)
{
	rows=3;
	columns=3;

	NotInitialized=false;

	Array=new dl32MatrixRow[3];
	Array[0]=new float[3];
	Array[1]=new float[3];
	Array[2]=new float[3];

	Array[0][0]=Matrix.m[0][0];
	Array[0][1]=Matrix.m[0][1];
	Array[0][2]=Matrix.m[0][2];

	Array[1][0]=Matrix.m[1][0];
	Array[1][1]=Matrix.m[1][1];
	Array[1][2]=Matrix.m[1][2];

	Array[2][0]=Matrix.m[2][0];
	Array[2][1]=Matrix.m[2][1];
	Array[2][2]=Matrix.m[2][2];
}

dl32Matrix::dl32Matrix(dl324x4Matrix &Matrix)
{
	rows=3;
	columns=3;

	NotInitialized=false;

	Array=new dl32MatrixRow[4];
	Array[0]=new float[4];
	Array[1]=new float[4];
	Array[2]=new float[4];
	Array[3]=new float[4];

	Array[0][0]=Matrix.m[0][0];
	Array[0][1]=Matrix.m[0][1];
	Array[0][2]=Matrix.m[0][2];
	Array[0][3]=Matrix.m[0][3];

	Array[1][0]=Matrix.m[1][0];
	Array[1][1]=Matrix.m[1][1];
	Array[1][2]=Matrix.m[1][2];
	Array[1][3]=Matrix.m[1][3];

	Array[2][0]=Matrix.m[2][0];
	Array[2][1]=Matrix.m[2][1];
	Array[2][2]=Matrix.m[2][2];
	Array[2][3]=Matrix.m[2][3];

	Array[3][0]=Matrix.m[3][0];
	Array[3][1]=Matrix.m[3][1];
	Array[3][2]=Matrix.m[3][2];
	Array[3][3]=Matrix.m[3][3];
}

void dl32Matrix::Dispose()
{
	if(Array!=NULL && !NotInitialized)
	{
		try
		{		
			for(int i=0;i<rows;++i)
				delete [] Array[i];

			delete [] Array;
		}
		catch(...)
		{
			NotInitialized=false;
		}

		NotInitialized=false;
		Array=NULL;
		columns=0;
		rows=0;
	}
}

dl32Matrix::~dl32Matrix()
{
	Dispose();
}

float dl32Matrix::At(int row,int column)throw(dl32OutOfRangeException)
{
	if(row>=0 && row<rows)
		if(column>=0 && column<columns)
			return Array[row][column];
		else
			throw dl32OutOfRangeException(dl32Range(columns),column,"dl32Matrix::At(int row,int column): 'column' is out of range");
	else
		throw dl32OutOfRangeException(dl32Range(rows),row,"dl32Matrix::At(int row,int column): 'row' is out of range");
}

dl32MatrixRow dl32Matrix::operator[](int row)throw(dl32OutOfRangeException)
{
	if(row>=0 && row<rows)
		return Array[row];
	else
		throw dl32OutOfRangeException(dl32Range(rows),row,"dl32Matrix::operator[](int row): 'row' is out of range");
}

//NOTA: Ésta función devuelve un puntero a la fila, de manera que tenemos acceso directo a los elementos de ésta
dl32MatrixRow dl32Matrix::GetRow(int row)throw(dl32OutOfRangeException)
{
	if(row>=0 && row<rows)
		return Array[row];
	else
		throw dl32OutOfRangeException(dl32Range(rows),row,"dl32Matrix::GetRow(int row): 'row' is out of ramge");
}

//NOTA: Ésta función devuelve un array de punteros a los elementos de la columna, para tener acceso directo a éstos
//El array es creado dinámicamente, así que no hay que olvidar eliminarlo al terminar de utilizarlo, para no provocar rmemory-leaks
dl32MatrixColumn dl32Matrix::GetColumn(int column)throw(dl32OutOfRangeException)
{
	if(column>=0 && column<columns)
	{
		dl32MatrixColumn Column=new float*[rows];

		for(int i=0;i<rows;++i)
			Column[i]=Array[i]+column;//Recordemos que Array[i] es un puntero al elemento inicial de la fila

		return Column;
	}
	else
		throw dl32OutOfRangeException(dl32Range(columns),column,"dl32Matrix::GetColumn(int column): 'column' is out of range");
}

void dl32Matrix::RowMul(int row,float mul)
{
	for(int i=0;i<columns;++i)
		Array[row][i]*=mul;
}

void dl32Matrix::RowAdd(int source_row,int aux_row,float aux_mul)throw(dl32OutOfRangeException)
{
	if(source_row!=aux_row)
		for(int i=0;i<columns;++i)
			Array[source_row][i]+=Array[aux_row][i]*aux_mul;
}

void dl32Matrix::RowSwap(int row1,int row2)throw(dl32OutOfRangeException)
{
	dl32MatrixRow aux;

	if(row1!=row2)
	{
		aux=Array[row1];
		Array[row1]=Array[row2];
		Array[row2]=aux;
	}
}

void dl32Matrix::ColumnMul(int column,float mul)throw(dl32OutOfRangeException)
{
	for(int i=0;i<columns;++i)
		Array[i][column]*=mul;
}

void dl32Matrix::ColumnAdd(int source_column,int aux_column,float aux_mul)throw(dl32OutOfRangeException)
{
	if(source_column!=aux_column)
		for(int i=0;i<columns;++i)
			Array[i][source_column]+=Array[i][aux_column]*aux_mul;
}

void dl32Matrix::ColumnSwap(int column1,int column2)throw(dl32OutOfRangeException)
{
	float aux;

	if(column1!=column2)
		for(int i=0;i<rows;++i)
		{
			aux=Array[i][column1];
			Array[i][column1]=Array[i][column2];
			Array[i][column2]=aux;
		}
}

dl32Matrix dl32Matrix::GetSubMatrix(int row,int column,int rows,int columns)throw(dl32OutOfRangeException)
{
	if(row>=0 && row+rows<=this->rows )
		if(column>=0 && column+columns<=this->columns)
		{
			dl32Matrix SubMatrix(rows,columns);

			for(int i=0;i<rows;++i)
				for(int j=0;j<columns;++j)
					SubMatrix[i][j]=Array[i+row][j+column];

			return SubMatrix;
		}
		else
			throw dl32OutOfRangeException(dl32Range(this->columns),column+columns,"dl32Matrix::GetSubMatrix(int row,int column,int rows,int columns): The value column+columns is out of interval");
	else
		throw dl32OutOfRangeException(dl32Range(this->rows),row+rows,"dl32Matrix::GetSubMatrix(int row,int column,int rows,int columns): The value row+rows is out of interval");
}

dl32Matrix dl32Matrix::GetSubMatrix(int row,int column)throw(dl32OutOfRangeException)
{
	if(row>=0 && row<rows)
		if(column>=0 && column<columns)
		{
			dl32Matrix SubMatrix(rows-1,columns-1);
			int i,j,x,y;

			i=0;
			x=0;

			do
			{
				if(i<row)
					x=i;
				else
					if(i>row) x=i-1;
					
				if(i!=row)
				{
					j=0;

					do 
					{
						if(j<column)
							y=j;
						else
							if(j>column) y=j-1;

						if(j!=column)
							SubMatrix[x][y]=Array[i][j];

						++j;
					}while(j<columns);
				}

				++i;
			}while(i<rows);

			return SubMatrix;
		}
		else
			throw dl32OutOfRangeException(dl32Range(rows),row,"dl32Matrix::GetSubMatrix(int row,int column): 'row' is out of range");
	else
		throw dl32OutOfRangeException(dl32Range(columns),column,"dl32Matrix::GetSubMatrix(int row,int column): 'column' is out of range");
}

dl32Matrix dl32Matrix::GetAdjugate()
{
	dl32Matrix Adjugate(rows,columns);

	for(int i=0;i<rows;++i)
		for(int j=0;j<columns;++j)
			Adjugate[i][j]=GetAdjugate(i,j);

	return Adjugate;
}

dl32Matrix dl32Matrix::GetTranspose()
{
	dl32Matrix TransposeMatrix(columns,rows);

	for(int i=0;i<columns;++i)
		for(int j=0;j<rows;++j)
			TransposeMatrix[i][j]=Array[j][i];

	return TransposeMatrix;
}

int dl32Matrix::GetRange(dl32Matrix &FinalMatrix)
{
	FinalMatrix=*this;
	int i=0,j,k,Min=min(rows,columns);
	bool HasPivot=true;
	int Range=0;

	while(HasPivot && i<Min)
	{
		HasPivot=false;
		j=i;

		//FASE 1: Busqueda de pivote:
		while(!HasPivot && j<rows)
		{
			k=i;

			while(!HasPivot && k<columns)
			{
				if(FinalMatrix[j][k]!=0)
				{
					HasPivot=true;
					FinalMatrix.RowSwap(j,i);
					FinalMatrix.ColumnSwap(k,i);
					FinalMatrix.RowMul(i,1/FinalMatrix[i][i]);//NOTA: Es 1/m[i][i] porque ya hemos movido el pivote a la diagonal
				}

				k++;
			}

			j++;
		}

		if(HasPivot)
		{
			Range++;

			//FASE 2: Reducción de columna:
			for(j=i+1;j<Min;++j)
				FinalMatrix.RowAdd(j,i,-FinalMatrix[j][i]); //FILA j = FILA j + (-m[j][i])*FILA i
		}

		i++;
	}

	return Range;
}

dl32Matrix GetUnity(int size)
{
	dl32Matrix Return(size,size);
	
	for(int i=0;i<size;++i)
		for(int j=0;j<size;++j)
			if(i==j)
				Return[i][j]=1;
			else
				Return[i][j]=0;

	return Return;
}

float dl32Matrix::GetDeterminant()
{
	if(rows==columns)
	{
		switch(rows)
		{
		case 1:
			return Array[0][0];
		case 2:
			return Array[0][0]*Array[1][1]-Array[0][1]*Array[1][0];
		case 3:
			return Array[0][0]*Array[1][1]*Array[2][2]+Array[0][1]*Array[1][2]*Array[2][0]+Array[1][0]*Array[2][1]*Array[0][2]-Array[0][2]*Array[1][1]*Array[2][0]-Array[0][1]*Array[1][0]*Array[2][2]-Array[0][0]*Array[2][1]*Array[1][2];
		default:
			float Return=0;

			for(int i=0;i<rows;++i)
				Return+=GetAdjugate(i,0)*Array[i][0];

			return Return;
		}
	}
	else
		return 0;
}

dl32Matrix dl32Matrix::Add(dl32Matrix m1,dl32Matrix m2)
{
	if(m1.rows == m2.rows && m1.columns == m2.columns)
	{
		for(int i=0;i<m1.rows;++i)
			for(int j=0;j<m1.columns;++j)
				m1.Array[i][j]+=m2.Array[i][j];

		return m1;
	}
	else
		throw dl32InvalidMatrixOperationException("dl32Matrix::Add(dl32Matrix m1,dl32Matrix m2): 'm1' and 'm2' have not same dimensions");
}

dl32Matrix dl32Matrix::Sub(dl32Matrix m1,dl32Matrix m2)
{
	if(m1.rows == m2.rows && m1.columns == m2.columns)
	{
		for(int i=0;i<m1.rows;++i)
			for(int j=0;j<m1.columns;++j)
				m1.Array[i][j]-=m2.Array[i][j];

		return m1;
	}
	else
		throw dl32InvalidMatrixOperationException("dl32Matrix::Sub(dl32Matrix m1,dl32Matrix m2): 'm1' and 'm2' have not same dimensions");
}

dl32Matrix dl32Matrix::Mul(dl32Matrix matrix,float mul)
{
	for(int i=0;i<matrix.rows;++i)
		for(int j=0;j<matrix.columns;++j)
			matrix.Array[i][j]*=mul;

	return matrix;
}

dl32Matrix dl32Matrix::Mul(dl32Matrix m1,dl32Matrix m2)
{
	if(m1.columns==m2.rows)
	{
		dl32Matrix Return(m1.rows,m2.rows);

		for(int i=0;i<m1.rows;++i)
			for(int j=0;j<m2.columns;++j)
			{
				Return.Array[i][j]=0;
				for(int k=0;k<m2.rows;++k)
					Return.Array[i][j]+=m1.Array[i][k]*m2.Array[k][j];
			}

		return Return;
	}
	else
		throw dl32InvalidMatrixOperationException("dl32Matrix::Mul(dl32Matrix m1,dl32Matrix m2): 'm1' columns count and 'm2' rows count must be equal");
}

dl32EcuationsSystem::dl32EcuationsSystem()
{
	incs=0;
	ecs=0;
	Solution.Solutions=NULL;
	Solved=false;
}

dl32EcuationsSystem::dl32EcuationsSystem(dl32EcuationsSystem &System)
{
	Dispose();

	incs=System.incs;
	ecs=System.ecs;
	mainmatrix=System.mainmatrix;
	auxmatrix=System.auxmatrix;
	gaussmatrix=System.gaussmatrix;
	mainrange=System.mainrange;
	auxrange=System.auxrange;
	Solved=System.Solved;
	Solution.Type=System.Solution.Type;

	if(Solved && Solution.Type==SCD)
	{
		Solution.Solutions=new float[incs];

		for(int i=0;i<incs;++i)
			Solution.Solutions[i]=System.Solution.Solutions[i];
	}
	else
		Solution.Solutions=NULL;
}

dl32EcuationsSystem::dl32EcuationsSystem(dl32Matrix Matrix)
{
	ecs=Matrix.GetRowsCount();
	incs=Matrix.GetColumnsCount()-1;
	auxmatrix=Matrix;
	mainmatrix=auxmatrix.GetSubMatrix(0,0,ecs,incs);
	Solve();	
}

dl32EcuationsSystem::dl32EcuationsSystem(dl32LinearEcuationArray Ecuations,int EcuationsCount,int IncsCount)
{
	incs=IncsCount;
	ecs=EcuationsCount;
	mainmatrix=dl32Matrix(ecs,incs);
	auxmatrix=dl32Matrix(ecs,incs+1);

	for(int i=0;i<ecs;++i)
		for(int j=0;j<=incs;)
		{
			auxmatrix[i][j]=Ecuations[i][j];
			if(j<incs) mainmatrix[i][j]=Ecuations[i][j];
		}

	Solve();
}

dl32EcuationsSystem& dl32EcuationsSystem::operator =(dl32EcuationsSystem &System)
{
	if(this != &System)
	{
		Dispose();

		incs=System.incs;
		ecs=System.ecs;
		mainmatrix=System.mainmatrix;
		auxmatrix=System.auxmatrix;
		gaussmatrix=System.gaussmatrix;
		mainrange=System.mainrange;
		auxrange=System.auxrange;
		Solved=System.Solved;
		Solution.Type=System.Solution.Type;

		if(Solved && Solution.Type==SCD)
		{
			Solution.Solutions=new float[incs];

			for(int i=0;i<incs;++i)
				Solution.Solutions[i]=System.Solution.Solutions[i];
		}
		else
			Solution.Solutions=NULL;
	}
	return *this;
}

void dl32EcuationsSystem::Solve()
{
	mainrange=mainmatrix.GetRange();
	auxrange=auxmatrix.GetRange(gaussmatrix);

	if(mainrange==auxrange)
		if(mainrange<incs)
		{
			Solution.Type=SCI;
			Solution.Solutions=NULL;
		}
		else
		{
			Solution.Type=SCD;
			Solution.Solutions=new float[incs];

			for(int i=ecs-1;i>=0;--i)
			{
				Solution.Solutions[i]=gaussmatrix[i][incs];

				for(int j=i+1;j<incs;++j)
					Solution.Solutions[i]-=gaussmatrix[i][j]*Solution.Solutions[j];

			}
		}
	else
	{
		Solution.Type=SI;
		Solution.Solutions=NULL;//0x01fd5830
	}

	Solved=true;
}

void dl32EcuationsSystem::Dispose()
{
	if(Solved)
		if(Solution.Type==SCD) delete Solution.Solutions;
}

dl32EcuationsSystem::~dl32EcuationsSystem()
{
	Dispose();
}

