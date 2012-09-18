#ifndef DL32UTILS3D_H
#define DL32UTILS3D_H

#include "dl32Math.h"
#include "dl32Color.h"
#include "dl32Graphics.h"

class dl323DCamera
{
private:
	dl323DTransform SURtoSRC;
	dl323DTransform SRCtoSUR;
	dl323DPoint position;
	dl323DVector direction;
	dl323DPoint LookAt;
public:
	dl323DCamera();
	dl323DCamera(dl323DPoint position,dl323DVector direction);
	dl323DCamera(dl323DPoint position,dl323DPoint LookAt){dl323DCamera(position,dl323DVector(position,LookAt));};

	dl323DPoint GetPosition(){return position;};
	dl323DVector GetDirection(){return direction;};
	void SetPosition(dl323DPoint position);
	void SetDirection(dl323DVector direction);
	void SetLookAt(dl323DPoint LookAt){SetDirection(dl323DVector(position,LookAt));};

	void ApplySURtoSRC(dl323DPoint* point){SURtoSRC.Apply(point);};
	dl323DPoint ApplySURtoSRC(dl323DPoint &point){return SURtoSRC.Apply(point);};
	void ApplySRCtoSUR(dl323DPoint* point){SRCtoSUR.Apply(point);};
	dl323DPoint ApplySRCtoSUR(dl323DPoint &point){return SRCtoSUR.Apply(point);};

	void ApplySURTransform(dl323DTransform transform);
	void ApplySRCTrnasform(dl323DTransform transform);
};

struct dl323DVertex
{
	dl323DPoint position;
	dl323DVector normal;
};

typedef int* dl32FaceIndexes;

struct dl32Face
{
	dl32FaceIndexes indexes;
	dl323DVector normal;
};

class dl32Polyedron
{
private:
	vector<dl323DVertex> verts;
	vector<dl32Face> faces;

	void ComputeNormals();
public:
	dl32Polyedron(vector<dl323DPoint> &verts,vector<dl32FaceIndexes> &faces);

	vector<dl322DPoint> Proyect(dl323DCamera &Camera);
};

#endif