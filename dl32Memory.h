#ifndef DL32MEMORY
#define DL32MEMORY

#include "dl32Config.h"
#ifndef NULL 
#define NULL 0 
#endif

#define DL32MEMORY_SAFEDELETE(x) if(x!=NULL){delete x; x=NULL;}
#define DL32MEMORY_SAFEDELETE_ARRAY(x) if(x!=NULL){delete[] x; x=NULL;}

class dl32SmartPointer
{
private:
	void* _ptr;
public:
	dl32SmartPointer(void* ptr){_ptr=ptr;};
	~dl32SmartPointer()
	{
		if(_ptr!=NULL)
		{
			delete _ptr;
			_ptr=NULL;
		}
	}
};
#endif