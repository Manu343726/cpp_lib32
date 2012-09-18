#include "dl32Utils3D.h"
#include "dl32Math.h"

dl323DCamera::dl323DCamera()
{
	direction.z=1;
	LookAt.z=1;
}

dl323DCamera::dl323DCamera(dl323DPoint position,dl323DVector direction)
{
	dl323DVector originalDirection(0,0,1);
	dl323DTransform rotation,traslation;
	dl32Quaternion q(dl323DVector::VectorialMul(originalDirection,direction),dl323DVector::Angle(originalDirection,direction));

	this->position=position;
	this->direction=direction;
	rotation=q;
	traslation=dl323DTransform::Traslation(position.x,position.y,position.z);
	SURtoSRC=rotation+traslation;
	traslation=dl323DTransform::Traslation(-position.x,-position.y,-position.z);
	rotation=dl323DTransform::Rotation(q);
	SRCtoSUR=rotation+traslation;
}

void dl323DCamera::SetPosition(dl323DPoint position)
{
	dl323DTransform traslation=dl323DTransform::Traslation(this->position,position);
	SURtoSRC.Concat(traslation);
	SRCtoSUR.ReverseConcat(traslation);
	this->position=position;
	traslation.Apply(&LookAt);
}

void dl323DCamera::ApplySURTransform(dl323DTransform transform)
{

}

