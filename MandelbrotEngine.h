#ifndef MANDELBROT_H
#define MANDELBROT_H

#include "dl32Graphics.h"

#define MANDELBROT_DEFAULTSTEP 0.003
#define MANDELBROT_DEFAULTITERATIONS 100
#define MANDELBROT_DEFAULTLIMIT 10

struct ComplexNumber
{
	float a,b;//Z = a + bi

	ComplexNumber(){a=0;b=0;};
	ComplexNumber(float real){a=real;b=0;};
	ComplexNumber(float real,float imaginary){a=real;b=imaginary;};

	ComplexNumber conjugate(){*this=conjugate(*this);return *this;};
	ComplexNumber add(ComplexNumber number){*this=add(*this,number);return *this;};
	ComplexNumber sub(ComplexNumber number){*this=sub(*this,number);return *this;};
	ComplexNumber mul(ComplexNumber number){*this=mul(*this,number);return *this;};

	float length(){return length(*this);};
	float squarelength(){return squarelength(*this);};

	ComplexNumber operator+(ComplexNumber number){return add(*this,number);};
	ComplexNumber operator-(ComplexNumber number){return sub(*this,number);};
	ComplexNumber operator*(ComplexNumber number){return mul(*this,number);};
	ComplexNumber operator+=(ComplexNumber number){return add(number);};
	ComplexNumber operator-=(ComplexNumber number){return sub(number);};
	ComplexNumber operator*=(ComplexNumber number){return mul(number);};

	static ComplexNumber conjugate(ComplexNumber number){return ComplexNumber(number.a,-number.b);};
	static ComplexNumber add(ComplexNumber n1,ComplexNumber n2){return ComplexNumber(n1.a+n2.a,n1.b+n2.b);};
	static ComplexNumber sub(ComplexNumber n1,ComplexNumber n2){return ComplexNumber(n1.a-n2.a,n1.b-n2.b);};
	static ComplexNumber mul(ComplexNumber n1,ComplexNumber n2){return ComplexNumber(n1.a*n2.a-n1.b*n2.b,n1.b*n2.a+n1.a*n2.b);};
	
	static float length(ComplexNumber number){return sqrt(number.a*number.a+number.b*number.b);};
	static float squarelength(ComplexNumber number){return number.a*number.a+number.b*number.b;};

	dl32String ToString(){return "(" + dl32String(a) + (b<0 ? "-" : "") + dl32String(b) + ")";};
};

class MandelbrotEngine
{
private:
	dl32Color** screen;
	ComplexNumber origin;
	int width,height;

	float stepx,stepy;
	float limit;
	int iterations;
public:
	MandelbrotEngine(){screen=NULL;};
	MandelbrotEngine(dl322DAABB screenarea,float step=MANDELBROT_DEFAULTSTEP, int iterations=MANDELBROT_DEFAULTITERATIONS, float limit=MANDELBROT_DEFAULTLIMIT);
	~MandelbrotEngine();

	void TranslateX(int tx);
	void TrnaslateY(int ty);
	void SetCenter(ComplexNumber number);

	void Compute();
	void Refresh(dl32GraphicsClass* gfx);

	ComplexNumber GetNumber(dl322DPoint pixel){return ComplexNumber(origin.a+pixel.x*stepx,origin.b+pixel.y*stepy);};
};

#endif