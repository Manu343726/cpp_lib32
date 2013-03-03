#ifndef DL32UTILS3D_H
#define DL32UTILS3D_H

#include "dl32Math.h"
#include "dl32Color.h"
#include "dl32Graphics.h"

class dl323DCamera
{
private:
	dl32Transformation3D SURtoSRC;
	dl32Transformation3D SRCtoSUR;
	dl32Point3D position;
	dl32Vector3D direction;
	dl32Point3D LookAt;
public:
	dl323DCamera();
	dl323DCamera(dl32Point3D position,dl32Vector3D direction);
	dl323DCamera(dl32Point3D position,dl32Point3D LookAt){dl323DCamera(position,dl32Vector3D(position,LookAt));}

	dl32Point3D GetPosition(){return position;}
	dl32Vector3D GetDirection(){return direction;}
	void SetPosition(dl32Point3D position);
	void SetDirection(dl32Vector3D direction);
	void SetLookAt(dl32Point3D LookAt){SetDirection(dl32Vector3D(position,LookAt));}

	void ApplySURtoSRC(dl32Point3D* point){SURtoSRC.Apply(point);}
	dl32Point3D ApplySURtoSRC(dl32Point3D &point){return SURtoSRC.Apply(point);}
	void ApplySRCtoSUR(dl32Point3D* point){SRCtoSUR.Apply(point);}
	dl32Point3D ApplySRCtoSUR(dl32Point3D &point){return SRCtoSUR.Apply(point);}

	void ApplySURTransformation(dl32Transformation3D Transformation);
	void ApplySRCTrnasform(dl32Transformation3D Transformation);
};

struct dl323DVertex
{
	dl32Point3D position;
	dl32Vector3D normal;
};

typedef int* dl32FaceIndexes;

struct dl32Face
{
	dl32FaceIndexes indexes;
	dl32Vector3D normal;
};

class dl32Polyedron
{
private:
	vector<dl323DVertex> verts;
	vector<dl32Face> faces;

	void ComputeNormals();
public:
	dl32Polyedron(vector<dl32Point3D> &verts,vector<dl32FaceIndexes> &faces);

	vector<dl32Point2D> Proyect(dl323DCamera &Camera);
};

#endif