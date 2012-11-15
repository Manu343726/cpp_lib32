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
	dl323DTransformation rotation,Translation;
	dl32Quaternion q(dl323DVector::VectorialMul(originalDirection,direction),dl323DVector::Angle(originalDirection,direction));

	this->position=position;
	this->direction=direction;
	rotation=q;
	Translation=dl323DTransformation::Translation(position.x,position.y,position.z);
	SURtoSRC=rotation+Translation;
	Translation=dl323DTransformation::Translation(-position.x,-position.y,-position.z);
	rotation=dl323DTransformation::Rotation(q);
	SRCtoSUR=rotation+Translation;
}

void dl323DCamera::SetPosition(dl323DPoint position)
{
	dl323DTransformation Translation=dl323DTransformation::Translation(this->position,position);
	SURtoSRC.Concat(Translation);
	SRCtoSUR.ReverseConcat(Translation);
	this->position=position;
	Translation.Apply(&LookAt);
}

void dl323DCamera::ApplySURTransformation(dl323DTransformation Transformation)
{

}

