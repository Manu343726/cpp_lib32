#include "Particle engine.h"

void dl32ParticleEmmitter::Emmit(dl32ParticleData *data,dl32Particle *particle)
{
	float angle=RANDOMFLOAT(0,PI2);
	particle->x=position.x;
	particle->y=position.y;
	data->speed=dl32Vector2D(startspeed*cos(angle),startspeed*sin(angle));
	data->acceleration=dl32Vector2D();
	particle->color=RANDOM_COLOR;
	data->life=0;
}

dl32ParticleSystem::dl32ParticleSystem(dl32GraphicsClass *gfx,int particlecount,dl32AABB2D space)
{
	this->gfx=gfx;
	this->particlecount=particlecount;
	this->data=new dl32ParticleData[particlecount];
	this->particles=new dl32Particle[particlecount];
	this->space=space;
	this->mode=EBMODE_INFINITE;
}

dl32ParticleSystem::~dl32ParticleSystem()
{
	delete particles;
	delete data;
}

void dl32ParticleSystem::Init()
{
	if(emmitters.size()>0)
	{
		int particlesperemmitter=particlecount/emmitters.size();
		for(int i=0;i<emmitters.size();++i)
			for(int j=0;j<particlesperemmitter;++j)
				emmitters[i].Emmit(&data[j+i*particlesperemmitter],&particles[j+i*particlesperemmitter]);
	}	
}

void dl32ParticleSystem::Frame()
{
	dl32Vector2D force,distance;
	float value,length;
	int j;
	bool killed;
	int emmitter;

	for(int i=0;i<particlecount;++i)
	{
		killed=false;

		if(space.BelongTo(particles[i]))
		{
			if(data[i].life<data[i].totallife)
			{
				j=0;
				force.x=0;
				force.y=0;

				while(!killed && j<holes.size())
				{
					distance=dl32Vector2D(particles[i],holes[j].position);
					length=distance.GetLength();

					if(length<=holes[j].deadzone)
						killed=true;
					else
					{
						value=holes[j].mass/(length*length);
						distance.Normalize();
						distance=distance*value;
						force+=distance;
					}

					j++;
				}

				if(!killed)
				{
					data[i].acceleration=force;
					data[i].speed+=force;
					particles[i].x+=data[i].speed.x;
					particles[i].y+=data[i].speed.y;
					data[i].life++;
				}
				else
				{
					emmitter=RANDOM(0,emmitters.size());
					emmitters[emmitter].Emmit(&data[i],&particles[i]);
				}
			}
			else
			{
				emmitter=RANDOM(0,emmitters.size());
				emmitters[emmitter].Emmit(&data[i],&particles[i]);
			}
		}
		else
		{
			if(mode==EBMODE_REFLECTION)
			{
				if(particles[i].x<space.GetLeft()) data[i].speed.x=abs(data[i].speed.x);
				if(particles[i].x>space.GetRight()) data[i].speed.x=-abs(data[i].speed.x);
				if(particles[i].y<space.GetTop()) data[i].speed.y=abs(data[i].speed.y);
				if(particles[i].y>space.GetBottom()) data[i].speed.y=-abs(data[i].speed.y);
				
				particles[i].x+=data[i].speed.x;
				particles[i].y+=data[i].speed.y;
				data[i].life++;
			}
			else
			{
				emmitter=RANDOM(0,emmitters.size());
				emmitters[emmitter].Emmit(&data[i],&particles[i]);
			}
		}
	}

	gfx->DRAW_Pixels(particles,particlecount);
	gfx->Frame();
}

bool dl32ParticleSystem::MoveEmmitter(int Emitter,dl32Point2D position)
{
	if(Emitter>=0 && Emitter<emmitters.size())
	{
		emmitters[Emitter].position=position;
		return true;
	}
	else
		return false;
}

bool dl32ParticleSystem::MoveHole(int Hole,dl32Point2D position)
{
	if(Hole>=0 && Hole<holes.size())
	{
		holes[Hole].position=position;
		return true;
	}
	else
		return false;
}