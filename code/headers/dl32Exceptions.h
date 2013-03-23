#ifndef DL32EXCEPTIONS_H
#define DL32EXCEPTIONS_H

#include <exception>
#include <Windows.h>

#define DEFAULTEXCEPTIONMESSAGE(exceptionClass) "Unexpected " #exceptionClass

//////////////////////////////////////////////
///The base for the dx_lib32 exceptions family
//////////////////////////////////////////////
class dl32Exception:public std::exception
{
private:
    const char* _message;
public:
	dl32Exception(const char* message=DEFAULTEXCEPTIONMESSAGE(dl32Exception)):_message(message){}

        const char* what() const noexcept {return _message;}
	const char* GetMessage() const {return what();}
};

///////////////////////////////////
///dx_lib32 memory issues exception
///////////////////////////////////
class dl32MemoryException:public dl32Exception
{
protected:
	void* pointer;
public:
	dl32MemoryException(const char* message=DEFAULTEXCEPTIONMESSAGE(dl32MemoryException),void* pointer=NULL):dl32Exception(message){this->pointer=pointer;}

	void* GetPointer(){return pointer;}
};

struct dl32Range
{
	int max;
	int min;

	dl32Range(){max=0;min=0;}
	dl32Range(int max,int min){this->max=max;this->min=min;}
	dl32Range(int size){max=size-1;min=0;}
};

////////////////////////////////////////////////////////////////////////////
///Represents a out of range exception, for example an array buffer overflow
////////////////////////////////////////////////////////////////////////////
class dl32OutOfRangeException:public dl32Exception
{
protected:
	dl32Range range;
	int index;
public:
	dl32OutOfRangeException(dl32Range range, int index, const char* message=DEFAULTEXCEPTIONMESSAGE(dl32OutOfRangeException)):dl32Exception(message){this->range=range;this->index=index;}
};
#endif