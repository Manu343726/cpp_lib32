#include "dl32Utils3D.h"
#include "dl32Math.h"

dl323DCamera::dl323DCamera()
{
	direction.z=1;
	LookAt.z=1;
}

dl323DCamera::dl323DCamera(dl32Point3D position,dl32Vector3D direction)
{
	dl32Vector3D originalDirection(0,0,1);
	dl32Transformation3D rotation,Translation;
	dl32Quaternion q(dl32Vector3D::VectorialMul(originalDirection,direction),dl32Vector3D::Angle(originalDirection,direction));

	this->position=position;
	this->direction=direction;
	rotation=q;
	Translation=dl32Transformation3D::Translation(position.x,position.y,position.z);
	SURtoSRC=rotation+Translation;
	Translation=dl32Transformation3D::Translation(-position.x,-position.y,-position.z);
	rotation=dl32Transformation3D::Rotation(q);
	SRCtoSUR=rotation+Translation;
}

void dl323DCamera::SetPosition(dl32Point3D position)
{
	dl32Transformation3D Translation=dl32Transformation3D::Translation(this->position,position);
	SURtoSRC.Concat(Translation);
	SRCtoSUR.ReverseConcat(Translation);
	this->position=position;
	Translation.Apply(&LookAt);
}

void dl323DCamera::ApplySURTransformation(dl32Transformation3D Transformation)
{

}

