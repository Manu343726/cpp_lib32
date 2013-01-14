#include "MandelbrotEngine.h"

MandelbrotEngine::MandelbrotEngine(dl32AABB2D screenarea,float step, int iterations, float limit)
{
	width=screenarea.GetWidth();
	height=screenarea.GetHeight();

	this->stepx=step;
	this->stepy=step*(float(width)/float(height));
	this->limit=limit;
	this->iterations=iterations;
	origin=ComplexNumber(-stepx*width/2,-stepy*height/2);

	screen=new dl32Color*[height];
	for(int i=0;i<height;++i)
		screen[i]=new dl32Color[width];
}

MandelbrotEngine::~MandelbrotEngine()
{
	if(screen!=NULL)
	{
		for(int i=0;i<height;++i)
			delete screen[i];
		
		delete screen;
		screen=NULL;
	}
}

void MandelbrotEngine::Compute()
{
	float squarelenght=0,squarelimit=limit*limit;
	int it;
	ComplexNumber pixelnumber,z;

	for(int i=0;i<width;++i)
		for(int j=0;j<height;++j)
		{
			it=0;
			squarelenght=0;
			pixelnumber=ComplexNumber(origin.a+i*stepx,origin.b+j*stepy);
			z=ComplexNumber();

			while(it<iterations && squarelenght<squarelimit)
			{
				z=z*z+pixelnumber;
				squarelenght=z.squarelength();
				it++;
			}

			if(it<iterations-1 && squarelenght>squarelimit)
				screen[j][i]=COLOR_RainbowGradient(it*DL32COLOR_MAXRAINBOWVALUE/iterations);
			else
				screen[j][i]=DL32COLOR_BLACK;
		}
}

void MandelbrotEngine::Refresh(dl32GraphicsClass* gfx)
{
	gfx->DRAW_Pixels(screen,0,0,width,height);
	gfx->Frame();
}