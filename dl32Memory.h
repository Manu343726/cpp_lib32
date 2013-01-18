#ifndef DL32MEMORY
#define DL32MEMORY

#include "dl32Config.h"
#ifndef NULL 
#define NULL 0 
#endif

#define DL32MEMORY_SAFEDELETE(x) if(x!=NULL){delete x; x=NULL;}
#define DL32MEMORY_SAFEDELETE_ARRAY(x) if(x!=NULL){delete[] x; x=NULL;}

template <class T>
class dl32SmartPointer
{
private:
	T* _ptr;
public:
	dl32SmartPointer(T* ptr){_ptr=ptr;};
	~dl32SmartPointer(){DL32MEMORY_SAFEDELETE(_ptr);};
	
	T* operator ->(){return _ptr;};
};
#endif