#ifndef DL32EXCEPTIONS_H
#define DL32EXCEPTIONS_H

#include <exception>

#define DEFAULTEXCEPTIONMESSAGE(exception) "Unexpected " #exception

#define DL32EXCEPTION_SUBCLASS_DEFAULTDOC(exceptionClass) /##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/ \
	                                                      /##/##/ @brief	cpp_lib32 exceptionClass exception.                                                                                                                                                                                                                                                      \
														  /##/##/                                                                                                                                                                                                                                                                                                    \
														  /##/##/ @author	Manu343726                                                                                                                                                                                                                                                                               \
														  /##/##/ @date	11/04/2013                                                                                                                                                                                                                                                                                   \
														  /##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/##/

#define DL32EXCEPTION_SUBCLASS(exception) DL32EXCEPTION_SUBCLASS_DEFAULTDOC(exception) \
										  class exception : public dl32Exception       \
										  {                                            \
										  public:                                      \
											  exception( const MessageType& message = DEFAULTEXCEPTIONMESSAGE(exception) ) : dl32Exception(message) {} \
										  }; 

                                       

//////////////////////////////////////////////
///The base for the dx_lib32 exceptions family
//////////////////////////////////////////////
class dl32Exception:public std::exception
{
public:
	typedef string MessageType;
#ifdef __GNUC__
private:
    const MessageType _message;
public:
	dl32Exception(const string message=DEFAULTEXCEPTIONMESSAGE(dl32Exception)) : _message(message){}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	Gets exception message.
    ///
    /// @author	Manu 343726
    /// @date	11/04/2013
    ///
    /// @return	C-style string containing the exception message.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    const char* what() const {return _message.c_str();}
#else
public:
	dl32Exception(const MessageType& message=DEFAULTEXCEPTIONMESSAGE(dl32Exception)) : exception(message.c_str()) {}
#endif
};

///////////////////////////////////
///dx_lib32 memory issues exception
///////////////////////////////////
class dl32MemoryException:public dl32Exception
{
protected:
	void* pointer;
public:
	dl32MemoryException(const MessageType& message=DEFAULTEXCEPTIONMESSAGE(dl32MemoryException),void* pointer=NULL):dl32Exception(message){this->pointer=pointer;}

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
	dl32Range _range;
	int _index;
public:
	dl32OutOfRangeException(dl32Range range, int index, const MessageType& message=DEFAULTEXCEPTIONMESSAGE(dl32OutOfRangeException)):dl32Exception(message) , _range(range) , _index(index) {}

	dl32Range getRange() const {return _range;}
	int getIndex() const {return _index;}
};
#endif