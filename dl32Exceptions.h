#ifndef DL32EXCEPTIONS_H
#define DL32EXCEPTIONS_H

#include <exception>
#include "dl32String.h"

#define DL32DEFAULTEXCEPTIONMESSAGE "Unexpected dl32Exception"
#define DL32DEFAULTMEMORYEXCEPTIONMESSAGE "Unexpected dl32MemoryException"
#define DL32DEFAULTOUTOFINTERVALEXCEPTIONMESSAGE "Unexpected dl32OutOfIntervalException"

class dl32Exception:public std::exception
{
protected:
	dl32String message;
public:
	dl32Exception(dl32String message=DL32DEFAULTEXCEPTIONMESSAGE){this->message=message;};

	const char* what(){return message.c_str();};
	dl32String GetMessage(){return message;};
};

class dl32MemoryException:public dl32Exception
{
protected:
	void* pointer;
public:
	dl32MemoryException(dl32String message=DL32DEFAULTMEMORYEXCEPTIONMESSAGE,void* pointer=NULL):dl32Exception(message){this->pointer=pointer;};

	void* GetPOinter(){return pointer;};
};

struct dl32Range
{
	int max;
	int min;

	dl32Range(){max=0;min=0;};
	dl32Range(int max,int min){this->max=max;this->min=min;};
	dl32Range(int size){max=size-1;min=0;};
};

class dl32OutOfRangeException:public dl32Exception
{
protected:
	dl32Range range;
	int index;
public:
	dl32OutOfRangeException(dl32Range range, int index, dl32String message=DL32DEFAULTOUTOFINTERVALEXCEPTIONMESSAGE):dl32Exception(message){this->range=range;this->index=index;};
};

class dl32MathException:public dl32Exception
{
public:
	dl32MathException(dl32String message):dl32Exception(message){};
};

class dl32OverflowException:public dl32MathException
{
public:
	dl32OverflowException(dl32String message):dl32MathException(message){};
};

class dl32DividedByCeroException:public dl32OverflowException
{
public:
	dl32DividedByCeroException(dl32String message):dl32OverflowException(message){};
};

#endif