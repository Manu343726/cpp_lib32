#include "dl32Graphics.h"

class dl32SceneObject
{
protected:
	IDirect3DDevice9* _dev;
	IDirect3DVertexBuffer9* _vb;
	int _vbLenght;
	int _zlevel;

	bool _editting;
public:
	dl32SceneObject(){_dev = NULL; _vb = NULL; _vbLenght = 0;};
	dl32SceneObject(const IDirect3DDevice9* device, const _d3dVertex* vertices, int count);

	virtual void draw() = NULL;

	void begin(){_editting = true;};
	void end(){_editting = false;};
	bool editting(){return _editting;};
};