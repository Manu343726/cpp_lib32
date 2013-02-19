#include "dl32Math.h"
#include "dl32String.h"
#include "dl32Console.h"
#include <cmath>
#include <cfloat>

inline float Min(float a,float b)
{
	return a<b ? a:b;
}

float DL32FLOAT_INVSQRT(float number)
{
	//La gran mayoría lo entiendo, pero me gustaría saber de donde se sacó el número mágico.....
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
	//      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

//Geometria (2D):

// dl322DPoint: ////////////////////////////////
const float dl32Point2D::operator[](int coord) const throw(dl32OutOfRangeException)
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

const dl32Point2D dl32Point2D::Div(const dl32Point2D &Point, float divisor)throw(dl32DividedByCeroException)
{
	if (divisor!=0)
		return dl32Point2D(Point.x/divisor,Point.y/divisor);
	else
		throw dl32DividedByCeroException("dl322DPoint::Div(dl322DPoint point,float divisor): 'divisor' must be different from 0");
}

dl32Point2D dl32Point2D::Baricenter(dl32Point2D PointList[],int PointCount)
{
	dl32Point2D Return;

	for(int i=0;i<PointCount;++i)
	{
		Return.x+=PointList[i].x;
		Return.y+=PointList[i].y;
	}

	return dl32Point2D(Return.x/PointCount,Return.y/PointCount);
}

// dl322DVector: ////////////////////////////////
void dl32Vector2D::Normalize()
{
	float m = DL32FLOAT_INVSQRT(x*x+y*y);

	if(m>0)
	{
		x*=m;
		y*=m;
	}
}

// dl322DLine: //////////////////////////////////////////////
dl32Line2D::dl32Line2D()
{
	a=0;
	b=0;
	c=0;
}

dl32Line2D::dl32Line2D(float A,float B,float C)
{
	a=A;
	b=B;
	c=C;
	direction.x=b;
	direction.y=-a;
	position.x=0;
	position.y=-c/B;
}

dl32Line2D::dl32Line2D(dl32Point2D P1,dl32Point2D P2)
{
	direction.x=P2.x-P1.x;
	direction.y=P2.y-P1.y;
	direction.Normalize();
	position=P1;
	//Ecuacion general: 
	//Ax+By+C=0
	//A*Px+B*Py+C=0 (Si P pertenece a la recta)
	//Por tanto:
	a=-direction.y;
	b=direction.x;
	c=-(position.x*a+position.x*b);
}

dl32Line2D::dl32Line2D(dl32Point2D Position,dl32Vector2D Direction)
{
	position=Position;
	direction=Direction.GetUnitary();
	a=-direction.y;
	b=direction.x;
	//Ecuacion general: 
	//Ax+By+C=0
	//A*Px+B*Py+C=0 (Si P pertenece a la recta)
	//Por tanto:
	c=-(position.x*a+position.x*b);
}

dl32Line2D::dl32Line2D(dl32Point2D Position,float Slope)
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
	direction.x=b;
	direction.y=-a;
}

dl32Point2D dl32Line2D::Intersection(dl32Line2D L1, dl32Line2D L2,bool externalParallelCheck) throw(dl32InfiniteIntersectionException)
{
	if(externalParallelCheck || !dl32Line2D::AreParallel(L1,L2))
	{
		//Resolución del sistema de ecuaciones formado por las ecuaciones parametricas de las rectas:

		dl32Point2D P1,P2;
		dl32Vector2D V1,V2;

		P1 = L1.GetPosition();
		V1 = L1.GetDirection();
		P2 = L2.GetPosition();
		V2 = L2.GetDirection();

		float e1[] = {V1.x,-V2.x,P2.x-P1.x};
		float e2[] = {V1.y,-V2.y,P2.y-P1.y};
		float* ecuations[] = {e1,e2};
		float lambda,omega;

		dl32EcuationsSystem system(ecuations,2,2);

		if(system.GetSolution().Type == SCD)
		{
			lambda = system.GetSolution().Solutions[0];
			omega = system.GetSolution().Solutions[1];
		
			return L1.GetPointByParameter(lambda);
		}
		else
			throw dl32InfiniteIntersectionException("dl32Line2D::Intersection(dl32Line2D L1, dl32Line2D L2): The lines are parallel. (");
	}
	else
		throw dl32InfiniteIntersectionException("dl32Line2D::Intersection(dl32Line2D L1, dl32Line2D L2): The lines are parallel");
}

dl32Segment2D::dl32Segment2D(dl32Point2D begin, dl32Point2D end)
{
	_begin=begin;
	_end=end;
	_line=dl32Line2D(_begin,_end);
}

// dl322DAABB: //////////////////////////////////////////////

dl32AABB2D::dl32AABB2D(float X, float Y, float Width, float Height)
{
	Position.x=X;
	Position.y=Y;
	_width=DL32MACRO_TONATURAL(Width);
	_height=DL32MACRO_TONATURAL(Height);
}

dl32AABB2D::dl32AABB2D(dl32Point2D Position, float Width, float Height)
{
	this->Position=Position;
	_width=DL32MACRO_TONATURAL(Width);
	_height=DL32MACRO_TONATURAL(Height);
}

dl32AABB2D::dl32AABB2D(dl32Point2D Position, dl32Vector2D Area)
{
	this->Position=Position;
	_width=DL32MACRO_TONATURAL(Area.x);
	_height=DL32MACRO_TONATURAL(Area.y);
}

dl32AABB2D::dl32AABB2D(dl32Point2D pointCloud[],int pointCount)
{
	dl32Point2D cloudCenter = dl32Point2D::Baricenter(pointCloud,pointCount);
	dl32Point2D north, south, east, west;//Extermos de la nube en coordenadas locales

	north.y=pointCloud[0].y;
	south.y=pointCloud[0].y;
	west.x=pointCloud[0].x;
	east.x=pointCloud[0].x;
	
	for(int i=0;i<pointCount;++i)
	{
		if(pointCloud[i].x>east.x)
			east=pointCloud[i];
		if(pointCloud[i].x<west.x)
			west=pointCloud[i];
		if(pointCloud[i].y>south.y)
			south=pointCloud[i];
		if(pointCloud[i].y<north.y)
			north=pointCloud[i];
	}

	Position.x=west.x;
	Position.y=north.y;
	_width=abs(west.x+east.x);
	_height=abs(north.y+south.y);
}

void dl32AABB2D::SetWidth(float ValWidth)
{
	if (ValWidth>0) _width=ValWidth;
}

void dl32AABB2D::SetHeight(float ValHeight)
{
	if (ValHeight>0) _height=ValHeight;
}

void dl32AABB2D::SetArea(const dl32Vector2D &area)
{
	if(area.x>0 && area.y>0)
	{
		_width=area.x;
		_height=area.y;
	}
}

void dl32AABB2D::SetCenter(float X, float Y)
{
	Position.x=X-(_width/2);
	Position.y=Y-(_height/2);
}

void dl32AABB2D::SetCenter(const dl32Point2D &Center)
{
	Position.x=Center.x-(_width/2);
	Position.y=Center.y-(_height/2);
}

bool dl32AABB2D::Collide(const dl32AABB2D &C1, const dl32AABB2D &C2)
{
	dl32AABB2D BigAABB=dl32AABB2D(C1.Position.x-C2.GetWidth(),C1.Position.y-C2.GetHeight(),C1.GetWidth()+C2.GetWidth(),C1.GetHeight()+C2.GetHeight());

	return BigAABB.BelongTo(C2.Position);
}

dl32Orientation2D dl32AABB2D::Orientation(const dl32AABB2D &Origin, const dl32AABB2D &AABB)
{
	dl32Orientation2D Retorno;
	bool ArribaIzquierda=Origin.BelongTo(AABB.GetUpLeftCorner());
	bool ArribaDerecha=Origin.BelongTo(AABB.GetUpRightCorner());
	bool AbajoDerecha=Origin.BelongTo(AABB.GetDownRightCorner());
	bool AbajoIzquierda=Origin.BelongTo(AABB.GetDownLeftCorner());

	if(ArribaIzquierda && ArribaDerecha && AbajoDerecha && AbajoIzquierda)
		Retorno=INSIDE;
	else if (ArribaIzquierda && ArribaDerecha  && !AbajoDerecha && !AbajoIzquierda)
		Retorno=SOUTH;
	else if(AbajoIzquierda && AbajoDerecha && !ArribaDerecha && !ArribaIzquierda)
		Retorno=NORTH;
	else if (ArribaDerecha && AbajoDerecha && !ArribaIzquierda && !AbajoIzquierda)
		Retorno=WEST;
	else if (ArribaIzquierda && AbajoIzquierda && !ArribaDerecha && !AbajoDerecha)
		Retorno=EAST;
	else if(ArribaIzquierda && !ArribaDerecha && !AbajoDerecha && !AbajoIzquierda)
		Retorno=SOUTHEAST;
	else if (ArribaDerecha && !ArribaIzquierda && !AbajoDerecha && !AbajoIzquierda)
		Retorno=SOUTHWEST;
	else if (AbajoDerecha && !ArribaIzquierda && !ArribaDerecha && !AbajoIzquierda)
		Retorno=NORTHWEST;
	else if (AbajoIzquierda && !ArribaIzquierda && !ArribaDerecha && !AbajoDerecha)
		Retorno=NORTHEAST;
	else
		Retorno=OUTSIDE;

	return Retorno;
}

dl32OBB2D::dl32OBB2D(dl32Point2D center, dl32Vector2D area, float rotation)
{
	_aabb.SetArea(area);
	_toLocal=dl32Transformation2D::Rotation(rotation)+dl32Transformation2D::Translation(center.x,center.y);
	_toWorld=_toLocal.GetInverse();
}

dl32OBB2D::dl32OBB2D(dl32Vector2D area, dl32Transformation2D WorldToLocal)
{
	_aabb.SetArea(area);
	_toLocal=WorldToLocal;
	_toWorld=_toLocal.GetInverse();
}

dl32OBB2D::dl32OBB2D(dl32Point2D pointCloud[],int pointCount)
{
	dl32Point2D cloudCenter = dl32Point2D::Baricenter(pointCloud,pointCount);//Centro de la nube en coordenadas de mundo
	dl32Vector2D aux;//Vector auxiliar para la búsqueda del eje x local (En coordenadas de mundo)
	dl32Vector2D xAxis;//El eje x local en coordenadas de mundo

	//FASE 1: Encontrar el eje x óptimo, en coordenadas de mundo, para el OBB que envuelve la nube:
	///////////////////////////////////////////////////////////////////////////////////////////////

	/******************************************************************************************************************************************
	* Éste método consiste en obtener el eje x como el sumatorio de los radiovectores de todos los vértices respecto al baricentro de la nube.*
	* El problema es que si se suman los radiovectores, seguramente se anulen unos con otros, ya que muchos tendrán sentidos opuestos.		  *
	* Como lo que nos interesa es la dirección, no el sentido, sumamos los vectores siempre en la misma dirección, es decir, si el sumatorio  *
	* actual y el radiovector son opuestos, se suma el opuesto del radiovector																  *
	******************************************************************************************************************************************/

	for(int i=0;i<pointCount;++i)
	{
		aux=dl32Vector2D(cloudCenter,pointCloud[i]);//El radiovector actual

		if(aux*xAxis>0)//Producto escalar: Es positivo si ambos vectores apuntan en el mismo sentido
			xAxis+=aux;
		else
			xAxis-=aux;
	}

	xAxis.Normalize();//Normalizamos el eje x

	//FASE 2: Calculamos las transformaciones:
	//////////////////////////////////////////

	/***************************************************************************************************************************************
	* La transformación de local a mundo (_toWorld) es la concatenación de la rotación que lleva el eje x del mundo al eje x local (xAxis) *
	* más una traslación que lleve el origen de coordenadas al centro de la nube (cloudCenter). 										   *
	* Por supuesto, la transformación de mundo a local (_toLocal) es la transformación inversa.											   *
	***************************************************************************************************************************************/

	//NOTA: Como el eje x es unitario, el seno del ángulo de rotación es la coordenada y del eje, y el coseno la coordenada x:
	_toWorld=dl32Transformation2D::Rotation(xAxis.y,xAxis.x)+dl32Transformation2D::Translation(cloudCenter.x,cloudCenter.y);
	_toLocal=dl32Transformation2D::Translation(-cloudCenter.x,-cloudCenter.y)+dl32Transformation2D::Rotation(-xAxis.y,xAxis.x);

	//FASE 3: Generamos el AABB en coordenadas locales:
	///////////////////////////////////////////////////

	dl32Point2D north, south, east, west;//Extermos de la nube en coordenadas locales

	north.y=cloudCenter.y;
	south.y=-cloudCenter.y;
	west.x=cloudCenter.x;
	east.x=-cloudCenter.x;

	for(int i=0;i<pointCount;++i)
	{
		dl32Point2D p=_toLocal.Apply(pointCloud[i]);

		if(p.x>east.x)
			east=p;

		if(p.x<west.x)
			west=p;

		if(p.y>south.y)
			south=p;

		if(p.y<north.y)
			north=p;
	}

	_aabb=dl32AABB2D(west.x,north.y,abs(east.x-west.x),abs(south.y-north.y));
}

void dl32OBB2D::ApplyTransformation(dl32Transformation2D transformation)
{
	_toLocal=_toLocal+transformation;//Se encadenan las dos transformaciones (Premultiplica la actual por la transformación)
	_toWorld=_toWorld*transformation;//Multiplicar la actual por la transformación es equivalente a premultiplicar por la inversa de la transformación
}

bool dl32OBB2D::Collide(dl32OBB2D O1, dl32OBB2D O2)
{
	dl32AABB2D A1,A2;
	dl32Point2D O1corners[4], O2corners[4];

	/*********************************************************************************************************************************************************
	* El método consiste en testear la colisión a través de los AABBs correspondientes a los dos OBBs en sus coordenadas locales.                            *
	* Para ello, se hacen dos test de colisión, uno en las coordenadas locales de un OBB, y otro en las coordenadas locales del otro OBB.                    *
	* Si ambos test detectan colisión, los OBBs colisionan.																									 *                                                                                                                               *
	* Es necesario calcular dos AABBs extra: Un AABB correspondiente al AABB del OBB1 en el espacio local del OBB2 (A1), y otro correspondiente al AABB del  *
	* OBB2 en coordenadas locales de OBB1 (A2).                                                                                                              *
	*********************************************************************************************************************************************************/

	//Coordenadas de las esquinas del AABB de O1 en coordenadas de O2:
	O1corners[0]=O2.WorldToLocal().Apply(O1.GetUpLeftCorner());
	O1corners[1]=O2.WorldToLocal().Apply(O1.GetUpRightCorner());
	O1corners[2]=O2.WorldToLocal().Apply(O1.GetDownRightCorner());
	O1corners[3]=O2.WorldToLocal().Apply(O1.GetDownLeftCorner());

	A1=dl32AABB2D(O1corners,4);

	if(O2.Collide(A1))
	{
		//Coordenadas de las esquinas del AABB de O2 en coordenadas de O1:
		O2corners[0]=O1.WorldToLocal().Apply(O2.GetUpLeftCorner());
		O2corners[1]=O1.WorldToLocal().Apply(O2.GetUpRightCorner());
		O2corners[2]=O1.WorldToLocal().Apply(O2.GetDownRightCorner());
		O2corners[3]=O1.WorldToLocal().Apply(O2.GetDownLeftCorner());

		A2=dl32AABB2D(O2corners,4);

		return O1.Collide(A2);
	}
	else
		return false;
}

//Matrices 2D (Matrices 3x3, Transformationaciones 2D):
//////////////////////////////////////////////////

dl32Matrix3x3::dl32Matrix3x3()
{
	m11=0;m12=0;m13=0;
	m21=0;m22=0;m23=0;
	m31=0;m32=0;m33=0;
}

dl32Matrix3x3::dl32Matrix3x3(float m11,float m12,float m13,
							  float m21,float m22,float m23,
						      float m31,float m32,float m33)
{
	this->m11=m11;this->m12=m12;this->m13=m13;
	this->m21=m21;this->m22=m22;this->m23=m23;
	this->m31=m31;this->m32=m32;this->m33=m33;
}

 float dl32Matrix3x3::GetDeterminant(const dl32Matrix3x3 &matrix)
{
	return (matrix.m11*matrix.m22*matrix.m33+
		    matrix.m12*matrix.m23*matrix.m31+
		    matrix.m21*matrix.m33*matrix.m13-
		    matrix.m13*matrix.m22*matrix.m31-
		    matrix.m12*matrix.m21*matrix.m33-
		    matrix.m11*matrix.m23*matrix.m32);
}  
 dl32Matrix3x3 dl32Matrix3x3::GetInverse(const dl32Matrix3x3 &matrix)throw(dl32InvalidMatrixOperationException)
 {
	 float det=dl32Matrix3x3::GetDeterminant(matrix);

	 if(det!=0)
		 return dl32Matrix3x3((matrix.m22*matrix.m33-matrix.m23*matrix.m32)/det,-(matrix.m12*matrix.m33-matrix.m13*matrix.m32)/det,(matrix.m12*matrix.m23-matrix.m13*matrix.m22)/det,
							 -(matrix.m21*matrix.m33-matrix.m23*matrix.m31)/det,(matrix.m11*matrix.m33-matrix.m13*matrix.m31)/det,-(matrix.m11*matrix.m23-matrix.m13*matrix.m21)/det,
							  (matrix.m21*matrix.m32-matrix.m22*matrix.m31)/det,-(matrix.m11*matrix.m32-matrix.m12*matrix.m31)/det,(matrix.m11*matrix.m22-matrix.m12*matrix.m21)/det);
	 else
		 throw dl32InvalidMatrixOperationException("dl323x3Matrix::GetInverse(dl323x3Matrix matrix): 'matrix' is no-invertible");
 }

void dl32Transformation2D::Apply(dl32Point2D *point) const
{
	*point=dl32Point2D(m11*point->x+m12*point->y+m13,
		  m21*point->x+m22*point->y+m23);
}

void dl32Transformation2D::Apply(float *x,float *y) const
{
	float xx,yy;
	xx=m11*(*x)+m12*(*y)+m13;
	yy=m21*(*x)+m22*(*y)+m23;
	*x=xx;
	*y=yy;
}

void dl32Transformation2D::Apply(dl32Point2D pointList[], int pointCount) const
{
	for(int i=0;i<pointCount;++i)
		pointList[i]=Apply(pointList[i]);
}

dl32Transformation2D dl32Transformation2D::Rotation(float center_x,float center_y,float angle)
{
		return dl32Transformation2D(dl32Matrix3x3::Mul(dl32Matrix3x3::Mul(dl32Transformation2D::Translation(center_x,center_y),dl32Transformation2D::Rotation(angle)),dl32Transformation2D::Translation(-center_x,-center_y)));
}

dl32Transformation2D dl32Transformation2D::Rotation(dl32Point2D center,float angle)
{
		return dl32Transformation2D(dl32Matrix3x3::Mul(dl32Matrix3x3::Mul(dl32Transformation2D::Translation(center.x,center.y),dl32Transformation2D::Rotation(angle)),dl32Transformation2D::Translation(-center.x,-center.y)));
}

dl32Point3D dl32Point3D::Div(dl32Point3D Point,float divisor)throw(dl32DividedByCeroException)
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

dl32Matrix4x4::dl32Matrix4x4()
{
	m11=0;m12=0;m13=0;m14=0;
	m21=0;m22=0;m23=0;m24=0;
	m31=0;m32=0;m33=0;m34=0;
	m41=0;m42=0;m43=0;m44=0;
}

dl32Matrix4x4::dl32Matrix4x4(float m11,float m12,float m13,float m14,
							  float m21,float m22,float m23,float m24,
							  float m31,float m32,float m33,float m34,
							  float m41,float m42,float m43,float m44)
{
	this->m11=m11;this->m12=m12;this->m13=m13;this->m14=m14;
	this->m21=m21;this->m22=m22;this->m23=m23;this->m24=m24;
	this->m31=m31;this->m32=m32;this->m33=m33;this->m34=m34;
	this->m41=m41;this->m42=m42;this->m43=m43;this->m44=m44;
}

dl32Matrix4x4 dl32Matrix4x4::Add(dl32Matrix4x4 m1,dl32Matrix4x4 m2)
{
	return dl32Matrix4x4(m1.m11+m2.m11,m1.m12+m2.m12,m1.m13+m2.m13,m1.m14+m2.m14,
						 m1.m21+m2.m21,m1.m22+m2.m22,m1.m23+m2.m23,m1.m24+m2.m24,
						 m1.m31+m2.m31,m1.m32+m2.m32,m1.m33+m2.m33,m1.m34+m2.m34,
						 m1.m41+m2.m41,m1.m42+m2.m42,m1.m43+m2.m43,m1.m44+m2.m44);
}

dl32Matrix4x4 dl32Matrix4x4::Sub(dl32Matrix4x4 m1,dl32Matrix4x4 m2)
{
	return dl32Matrix4x4(m1.m11-m2.m11,m1.m12-m2.m12,m1.m13-m2.m13,m1.m14-m2.m14,
						 m1.m21-m2.m21,m1.m22-m2.m22,m1.m23-m2.m23,m1.m24-m2.m24,
						 m1.m31-m2.m31,m1.m32-m2.m32,m1.m33-m2.m33,m1.m34-m2.m34,
						 m1.m41-m2.m41,m1.m42-m2.m42,m1.m43-m2.m43,m1.m44-m2.m44);
}

 dl32Matrix4x4 dl32Matrix4x4::Mul(dl32Matrix4x4 m1,dl32Matrix4x4 m2)
{
	return dl32Matrix4x4(m1.m11*m2.m11+m1.m12*m2.m21+m1.m13*m2.m31+m1.m14*m2.m41,
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

dl32Matrix4x4 dl32Matrix4x4::Mul(dl32Matrix4x4 matrix,float factor)
{
	return dl32Matrix4x4(matrix.m11*factor,matrix.m12*factor,matrix.m13*factor,matrix.m14*factor,
						 matrix.m21*factor,matrix.m22*factor,matrix.m23*factor,matrix.m24*factor,
						 matrix.m31*factor,matrix.m32*factor,matrix.m33*factor,matrix.m34*factor,
						 matrix.m41*factor,matrix.m42*factor,matrix.m43*factor,matrix.m44*factor);
}

float dl32Matrix4x4::GetDeterminant(dl32Matrix4x4 &matrix)
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

dl32Quaternion::dl32Quaternion(dl32Vector3D axis,float angle,bool UseMatrix)
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

dl32Transformation3D::dl32Transformation3D()
{
	m11=1;m12=0;m13=0;m14=0;
	m21=0;m22=1;m23=0;m24=0;
	m31=0;m32=0;m33=1;m34=0;
	m41=0;m42=0;m43=0;m44=1;
}

dl32Transformation3D::dl32Transformation3D(float m11,float m12,float m13,float m14,
							  float m21,float m22,float m23,float m24,
							  float m31,float m32,float m33,float m34,
							  float m41,float m42,float m43,float m44)
{
	this->m11=m11;this->m12=m12;this->m13=m13;this->m14=m14;
	this->m21=m21;this->m22=m22;this->m23=m23;this->m24=m24;
	this->m31=m31;this->m32=m32;this->m33=m33;this->m34=m34;
	this->m41=m41;this->m42=m42;this->m43=m43;this->m44=m44;
}

dl32Transformation3D::dl32Transformation3D(dl32Matrix4x4 &matrix)
{
	this->m11=matrix.m11;this->m12=matrix.m12;this->m13=matrix.m13;this->m14=matrix.m14;
	this->m21=matrix.m21;this->m22=matrix.m22;this->m23=matrix.m23;this->m24=matrix.m24;
	this->m31=matrix.m31;this->m32=matrix.m32;this->m33=matrix.m33;this->m34=matrix.m34;
	this->m41=matrix.m41;this->m42=matrix.m42;this->m43=matrix.m43;this->m44=matrix.m44;
}

void dl32Transformation3D::Apply(dl32Point3D *point)
{
	*point=dl32Point3D(m11*point->x+m12*point->y+m13*point->z+m14,m21*point->x+m22*point->y+m23*point->z+m24,m31*point->x+m32*point->y+m33*point->z+m34);
}

void dl32Transformation3D::Apply(float *x,float *y,float *z)
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
	if(this != &matrix && matrix.Ready())
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

dl32Matrix::dl32Matrix(dl32Matrix3x3 &Matrix)
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

dl32Matrix::dl32Matrix(dl32Matrix4x4 &Matrix)
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
					if(FinalMatrix[i][i]!=1) FinalMatrix.RowMul(i,1/FinalMatrix[i][i]);//NOTA: Es 1/m[i][i] porque ya hemos movido el pivote a la diagonal
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
				if(FinalMatrix[i][j]!=0) FinalMatrix.RowAdd(j,i,-FinalMatrix[j][i]); //FILA j = FILA j + (-m[j][i])*FILA i
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
		for(int j=0;j<=incs;++j)
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
		Solution.Solutions=NULL;
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

void PrintMatrix(dl32Matrix &matrix)
{
	for(int i=0;i<matrix.GetRowsCount();++i)
	{
		for(int j=0;j<matrix.GetColumnsCount();++j)
			Console << " " << dl32String(matrix[i][j],2) << " ";

		Console << dl32endl;
	}
}

dl32Spline::dl32Spline(dl32Point2D* Nodes,int Count)
{
	if(Count>=2)
	{
		nodes.insert(nodes.begin(),Nodes,Nodes+Count);
		nodecount=Count;
		Compute();
	}
}

dl32Spline::~dl32Spline()
{
	nodecount=-1;
}

void dl32Spline::Compute()
{
	dl32EcuationsSystem *X,*Y;
	dl32SystemSolution SolX,SolY;
	dl32LinearEcuationArray ArrayX,ArrayY;

	//FASE 1: Rellenamos el sistema de ecuaciones para calcular las derivadas:

	ArrayX=new dl32LinearEcuation[nodecount];
	ArrayY=new dl32LinearEcuation[nodecount];

	for(int i=0;i<nodecount;++i)
	{
		ArrayX[i]=new float[nodecount+1];
		ArrayY[i]=new float[nodecount+1];

		for(int j=0;j<nodecount;++j)
		{
			ArrayX[i][j]=0;
			ArrayY[i][j]=0;
		}

		if(i==0)
		{
			ArrayX[i][0]=2;ArrayX[i][1]=1;
			ArrayY[i][0]=2;ArrayY[i][1]=1;

			ArrayX[i][nodecount]=3*(nodes[i+1].x-nodes[i].x);
			ArrayY[i][nodecount]=3*(nodes[i+1].y-nodes[i].y);
		}
		else if(i==nodecount-1)
		{
			ArrayX[i][nodecount-2]=1;ArrayX[i][nodecount-1]=2;
			ArrayY[i][nodecount-2]=1;ArrayY[i][nodecount-1]=2;

			ArrayX[i][nodecount]=3*(nodes[i].x-nodes[i-1].x);
			ArrayY[i][nodecount]=3*(nodes[i].y-nodes[i-1].y);
		}
		else
		{
			ArrayX[i][i-1]=1;ArrayX[i][i]=4;ArrayX[i][i+1]=1;
			ArrayY[i][i-1]=1;ArrayY[i][i]=4;ArrayY[i][i+1]=1;

			ArrayX[i][nodecount]=3*(nodes[i+1].x-nodes[i].x);
			ArrayY[i][nodecount]=3*(nodes[i+1].y-nodes[i].y);
		}


	}

	//FASE 2: Calculamos las derivadas (Resolvemos el sistema)
	X=new dl32EcuationsSystem(ArrayX,nodecount,nodecount);
	Y=new dl32EcuationsSystem(ArrayY,nodecount,nodecount);
	SolX=X->GetSolution();
	SolY=Y->GetSolution();

	//FASE 3: Calculamos los coeficientes de los polinomios
	intervalsX.resize(nodecount-1);
	intervalsY.resize(nodecount-1);

	for(int i=0;i<nodecount-1;++i)
	{
		intervalsX[i].a=nodes[i].x;
		intervalsX[i].b=SolX.Solutions[i];
		intervalsX[i].c=3*(nodes[i+1].x-nodes[i].x)-SolX.Solutions[i+1]-2*SolX.Solutions[i];
		intervalsX[i].d=2*(nodes[i].x-nodes[i+1].x)+SolX.Solutions[i]+SolX.Solutions[i+1];

		intervalsY[i].a=nodes[i].y;
		intervalsY[i].b=SolY.Solutions[i];
		intervalsY[i].c=3*(nodes[i+1].y-nodes[i].y)-SolY.Solutions[i+1]-2*SolY.Solutions[i];
		intervalsY[i].d=2*(nodes[i].y-nodes[i+1].y)+SolY.Solutions[i]+SolY.Solutions[i+1];

		#if DL32DEBUG_SPLINE_PROMPTINTERVALDATA
			Console << "dl32Spline - Intervalo " << dl32String(i+1) << " de " << dl32String(nodecount-1) << ":" << dl32endl;
			Console << " ==> X:" << dl32endl;
			Console << "	a (Termino independiente): " << intervalsX[i].a << dl32endl;
			Console << "	b (Termino de X): " << intervalsX[i].b << dl32endl;
			Console << "	c (Termino de X^2): " << intervalsX[i].c << dl32endl;
			Console << "	d (Termino de X^3): " << intervalsX[i].d << dl32endl;
			Console << " ==> Y:" << dl32endl;
			Console << "	a (Termino independiente): " << intervalsY[i].a << dl32endl;
			Console << "	b (Termino de Y): " << intervalsY[i].b << dl32endl;
			Console << "	c (Termino de Y^2): " << intervalsY[i].c << dl32endl;
			Console << "	d (Termino de Y^3): " << intervalsY[i].d << dl32endl;
		#endif
	}

	delete ArrayX;
	delete ArrayY;
	delete X;
	delete Y;
}

vector<dl32Point2D> dl32Spline::Interpolate(int PointsPerInterval)
{
	float LongitudX,LongitudY,Step;
	vector<dl32Point2D> retorno;

	for(int i=0;i<nodecount-1;++i)
	{
		LongitudX=abs(nodes[i+1].x-nodes[i].x);
		LongitudY=abs(nodes[i+1].y-nodes[i].y);

		if(PointsPerInterval<=0)//Si el parámetro es menor o igual que cero, el spline se genera pixel a pixel
			Step=1.0/(LongitudX+LongitudY);//El paso utilizado es la media entre las dos (Elegir uno de ellos como referencia puede provocar problemas con curvas con mucha/poca pendiente) 
		else
			Step=1.0/PointsPerInterval;
		
		for(float u=0;u<1;u+=Step)
			retorno.push_back(dl32Point2D(intervalsX[i].Interpolate(u),intervalsY[i].Interpolate(u)));
	}

	retorno.push_back(nodes[nodecount-1]);

	return retorno;
}
