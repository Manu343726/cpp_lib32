#include "dl32Math.h"
#include "dl32Graphics.h"

class Polyline
{
private:
	vector<_d3dVertex> _points;
	vector<int> _indices;
public:
	Polyline(){};
	Polyline(dl32Point2D* vertices,int vertexCount);

	_d3dVertex* GetVertexBuffer(){return _points.data();};
	int* GetIndexBuffer(){return _indices.data();};
};

Polyline::Polyline(dl32Point2D* vertices, int vertexCount)
{
	dl32Point2D* centers = new dl32Point2D[vertexCount-1];

	for(int i=0;i<vertexCount-1;++i)
		centers[i]=(vertices[i]+vertices[i+1])/2;

	dl32Line2D lineAT,lineBT;
	dl32Point2D T0,T1,aT,bT,vP,_vP;

	


}