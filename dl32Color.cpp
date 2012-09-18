#include "dl32Color.h"

D3DCOLOR COLOR_FromARGB(byte A,byte R, byte G, byte B){return (A<<24) | (R<<16) | (G<<8) | B;}
D3DCOLOR COLOR_FromRGB(byte R, byte G, byte B){return 0xFF000000 | (R<<16) | (G<<8) | B;}

D3DCOLOR COLOR_GetAlfaColor(D3DCOLOR color){return color & ARGBMASK_ALFAONLY;}
D3DCOLOR COLOR_GetRedColor(D3DCOLOR color){return color & ARGBMASK_REDONLY;}
D3DCOLOR COLOR_GetGreenColor(D3DCOLOR color){return color & ARGBMASK_GREENONLY;}
D3DCOLOR COLOR_GetBlueColor(D3DCOLOR color){return color & ARGBMASK_BLUEONLY;}

byte COLOR_GetAlfa(D3DCOLOR color){return byte((color & ARGBMASK_ALFAONLY) >> 24);}
byte COLOR_GetRed(D3DCOLOR color){return byte((color & ARGBMASK_REDONLY) >> 16);}
byte COLOR_GetGreen(D3DCOLOR color){return byte((color & ARGBMASK_GREENONLY) >> 8);}
byte COLOR_GetBlue(D3DCOLOR color){return byte(color & ARGBMASK_BLUEONLY);}

void COLOR_SetAlfa(D3DCOLOR &color,byte A)
{
	color = (color & ARGBMASK_NOALFA) | (A << 24);
}

void COLOR_SetRed(D3DCOLOR &color,byte R)
{
	color = (color & ARGBMASK_NORED) | (R << 16);
}

void COLOR_SetGreen(D3DCOLOR &color,byte G)
{
	color = (color & ARGBMASK_NOGREEN) | (G << 8);
}

void COLOR_SetBlue(D3DCOLOR &color,byte B)
{
	color = (color & ARGBMASK_NOBLUE) | B;
}

D3DCOLOR COLOR_Add(D3DCOLOR C1,D3DCOLOR C2)
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

D3DCOLOR COLOR_Sub(D3DCOLOR C1,D3DCOLOR C2)
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

D3DCOLOR COLOR_Blend(D3DCOLOR C1,D3DCOLOR C2)
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
