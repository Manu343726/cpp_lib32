#ifndef PARTICLEENGINE_H
#define PARTICLEENGINE_H

#include "dl32Math.h"
#include "dl32Graphics.h"

#define PARTICLELIFE 1000

const int MINRAND=-1;
const int MAXRAND=1;

const int PI10000=31415*2;
#define RANDOMFLOAT(HI,LO) LO + (float)rand()/((float)RAND_MAX/(HI-LO))
#define RANDOM(min,max) ((rand()%(max-min))+min)
#define RANDOM_COLOR COLOR_FromRGB(RANDOM(0,255),RANDOM(0,255),RANDOM(0,255))
#define RANDOM_POINT(minX,minY,maxX,maxY) dl32Point2D(RANDOM(minX,maxX),RANDOM(minY,maxY))

#define PARTICLERANDOMRANGE 0.001
#define PARTICLERANDOM RANDOM(-float(PARTICLERANDOMRANGE),float(PARTICLERANDOMRANGE))
#define PARTICLERANDOMVECTOR(range) dl32Vector2D(RANDOM(-range,range),RANDOM(-range,range))

typedef dl32Pixel dl32Particle;

struct dl32ParticleData
{
	dl32Vector2D speed;
	dl32Vector2D acceleration;
	long totallife;
	long life;

	dl32ParticleData(){totallife=PARTICLELIFE;life=0;}
};

enum EngineBorderMode
{
	EBMODE_REFLECTION,
	EBMODE_INFINITE
};

struct dl32ParticleEmmitter
{
	float startspeed;
	dl32Point2D position;
	dl32Transformation2D rotation;

	dl32ParticleEmmitter(float startspeed,dl32Point2D position){this->rotation=dl32Transformation2D::Rotation(PI/10000);this->startspeed=startspeed;this->position=position;}
	void Emmit(dl32ParticleData *data,dl32Particle *particle);
};

struct dl32ParticleHole
{
	dl32Point2D position;
	float deadzone;
	float mass;

	dl32ParticleHole(dl32Point2D position,float deadzone,float mass){this->position=position;this->deadzone=deadzone;this->mass=mass;}
};

class dl32ParticleSystem
{
private:
	dl32GraphicsClass* gfx;
	dl32Particle *particles;
	dl32ParticleData *data;
	int particlecount;
	vector<dl32ParticleEmmitter> emmitters;
	vector<dl32ParticleHole> holes;
	dl32AABB2D space;
	EngineBorderMode mode;
public:
	dl32ParticleSystem(dl32GraphicsClass *gfx,int particlecount,dl32AABB2D space);
	~dl32ParticleSystem();

	void AddEmmitter(dl32ParticleEmmitter emmitter){emmitters.push_back(emmitter);}
	void AddHole(dl32ParticleHole hole){holes.push_back(hole);}

	bool MoveEmmitter(int Emmitter,dl32Point2D position);
	bool MoveHole(int Hole,dl32Point2D position);

	void Init();
	void Frame();

	int GetTotalEmmitters(){return emmitters.size();}
	int GetTotalHoles(){return holes.size();}

	void SetBorderMode(EngineBorderMode mode){this->mode=mode;}
	EngineBorderMode GetNorderMode(){return mode;}
};

#endif