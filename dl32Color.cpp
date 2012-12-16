#include "dl32Color.h"

dl32Color COLOR_Add(dl32Color C1,dl32Color C2)
{
	byte a1,r1,g1,b1,a2,r2,g2,b2;
	int a,r,g,b;

	a1=COLOR_GetAlfa(C1);
	r1=COLOR_GetRed(C1);
	g1=COLOR_GetGreen(C1);
	b1=COLOR_GetBlue(C1);

	a2=COLOR_GetAlfa(C2);
	r2=COLOR_GetRed(C2);
	g2=COLOR_GetGreen(C2);
	b2=COLOR_GetBlue(C2);

	a=a1+a2;
	r=r1+r2;
	g=g1+g2;
	b=b1+b2;

	return COLOR_FromARGB(a>255 ? 255:a,r>255 ? 255:r,g>255 ? 255:g,b>255 ? 255:b);
}

dl32Color COLOR_Sub(dl32Color C1,dl32Color C2)
{
	byte a1,r1,g1,b1,a2,r2,g2,b2;
	int a,r,g,b;

	a1=COLOR_GetAlfa(C1);
	r1=COLOR_GetRed(C1);
	g1=COLOR_GetGreen(C1);
	b1=COLOR_GetBlue(C1);

	a2=COLOR_GetAlfa(C2);
	r2=COLOR_GetRed(C2);
	g2=COLOR_GetGreen(C2);
	b2=COLOR_GetBlue(C2);

	a=a1-a2;
	r=r1-r2;
	g=g1-g2;
	b=b1-b2;

	return COLOR_FromARGB(a<0 ? 0:a,r<0 ? 0:r,g<0 ? 0:g,b<0 ? 0:b);
}

dl32Color COLOR_Blend(dl32Color C1,dl32Color C2)
{
	byte a1,r1,g1,b1,a2,r2,g2,b2;
	int a,r,g,b;

	a1=COLOR_GetAlfa(C1);
	r1=COLOR_GetRed(C1);
	g1=COLOR_GetGreen(C1);
	b1=COLOR_GetBlue(C1);

	a2=COLOR_GetAlfa(C2);
	r2=COLOR_GetRed(C2);
	g2=COLOR_GetGreen(C2);
	b2=COLOR_GetBlue(C2);

	if(r1==0 && g1==0 && b1==0)
		return C2;
	else if(r2==0 && g2==0 && b2==0)
		return C1;
	else
	{
		a=(a1+a2)/2;
		r=(r1+r2)/2;
		g=(g1+g2)/2;
		b=(b1+b2)/2;

		return COLOR_FromARGB(a>255 ? 255:a,r>255 ? 255:r,g>255 ? 255:g,b>255 ? 255:b);
	}
}

dl32Color COLOR_RainbowGradient(DWORD value)
{
	if(value<256)
		return COLOR_FromRGB(255,value,0);//Rojo - Amarillo
	else if(value<512)
		return COLOR_FromRGB(511-value,255,0);//Amarillo-Verde
	else if(value<768)
		return COLOR_FromRGB(0,255,value-767);//Verde-Cian
	else if(value<1024)
		return COLOR_FromRGB(0,1023-value,255);//Cian-Azul
	else if(value<1280)
		return COLOR_FromRGB(value-1279,0,255);//Azul-Magenta
	else
		return DL32COLOR_BLACK;
}
