#ifndef DL32EXCEPTIONS_H
#define DL32EXCEPTIONS_H

#include <exception>
#include <Windows.h>

#define DL32DEFAULTEXCEPTIONMESSAGE "Unexpected dl32Exception"
#define DL32DEFAULTMEMORYEXCEPTIONMESSAGE "Unexpected dl32MemoryException"
#define DL32DEFAULTOUTOFINTERVALEXCEPTIONMESSAGE "Unexpected dl32OutOfIntervalException"

//////////////////////////////////////////////
///The base for the dx_lib32 exceptions family
//////////////////////////////////////////////
class dl32Exception:public std::exception
{
public:
	dl32Exception(char* message=DL32DEFAULTEXCEPTIONMESSAGE):exception(message){};

	const char* GetMessage(){return this->what();};
};

///////////////////////////////////
///dx_lib32 memory issues exception
///////////////////////////////////
class dl32MemoryException:public dl32Exception
{
protected:
	void* pointer;
public:
	dl32MemoryException(char* message=DL32DEFAULTMEMORYEXCEPTIONMESSAGE,void* pointer=NULL):dl32Exception(message){this->pointer=pointer;};

	void* GetPointer(){return pointer;};
};

struct dl32Range
{
	int max;
	int min;

	dl32Range(){max=0;min=0;};
	dl32Range(int max,int min){this->max=max;this->min=min;};
	dl32Range(int size){max=size-1;min=0;};
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
	dl32OutOfRangeException(dl32Range range, int index, char* message=DL32DEFAULTOUTOFINTERVALEXCEPTIONMESSAGE):dl32Exception(message){this->range=range;this->index=index;};
};

//////////////////////////////////////
///A base for dx_lib32 math exceptions
//////////////////////////////////////
class dl32MathException:public dl32Exception
{
public:
	dl32MathException(char* message):dl32Exception(message){};
};

///////////////////////////////////////
///Represents a math overflow exception
///////////////////////////////////////
class dl32OverflowException:public dl32MathException
{
public:
	dl32OverflowException(char* message):dl32MathException(message){};
};

class dl32DividedByCeroException:public dl32OverflowException
{
public:
	dl32DividedByCeroException(char* message):dl32OverflowException(message){};
};

class dl32InvalidMatrixOperationException:dl32MathException
{
public:
	dl32InvalidMatrixOperationException(char* message):dl32MathException(message){};
};

class dl32ConsoleException:public dl32Exception
{
public:
	dl32ConsoleException(char* message):dl32Exception(message){};
};

class dl32Direct3DInitFailedException:public dl32Exception
{
public:
	dl32Direct3DInitFailedException(char* message):dl32Exception(message){};
};

class dl32WindowException:dl32Exception
{
private:
	HWND _hwnd;
public:
	dl32WindowException(HWND hwnd,char* message):dl32Exception(message){_hwnd=hwnd;};
	HWND GethWnd(){return _hwnd;};
};

class dl32UnhandledWindowMessage: public dl32WindowException
{
private:
	UINT _msg;
public:
	dl32UnhandledWindowMessage(UINT msg,HWND hwnd, char* message):dl32WindowException(hwnd,message){_msg=msg;};
	UINT GetMessage(){return _msg;};
};
#endif