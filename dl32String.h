#ifndef STRING_H
#define STRING_H

#include <string>
#include "dl32Exceptions.h"

using namespace std;

#define DL32STRINGDEFAULTS_MAXSIZE 100
#define DL32STRINGDEFAULTS_DECIMALS 3
#define DL32TRING_ENDLINE '\n'
#define DL32STRING_FINALSYMBOL NULL

const int DL32CONSTS_0CHARCODE = int('0');

class dl32String
{
	friend class dl32String;
private: 
	char *Array;
	int size;
protected:
	void LongCasting(long Number);
	void HexLongCasting(long Number);
	void FloatCasting(float Number, int Decimals=DL32STRINGDEFAULTS_DECIMALS);
public:
	dl32String();
	dl32String(char Str);
	dl32String(const char Str[]);
	dl32String(char Str[],int Size);
	dl32String(string &Str);
	dl32String(dl32String &Str);
	dl32String(const string &Str);
	dl32String(const dl32String &Str);
	dl32String(int Number,bool Hex=false);
	dl32String(long Number,bool Hex=false);
	dl32String(float Number,int Decimals=DL32STRINGDEFAULTS_DECIMALS);
	dl32String(double Number,int Decimals=DL32STRINGDEFAULTS_DECIMALS);
	dl32String(void* memoryaddress);

	~dl32String();

	bool Ready();

	char* c_str();
	char* Copy();
	char At(int pos)throw(dl32OutOfRangeException);
	char operator[](int pos)throw(dl32OutOfRangeException){return At(pos);};

	dl32String& operator=(dl32String &Str);

	int GetLength(){return size;};

	static dl32String Concat(dl32String &str1,dl32String &str2);
	void Concat(dl32String &str);

	dl32String& operator+=(dl32String &str);
	
	void Erase(int StartIndex,int Lenght=1);
	dl32String GetSubString(int StartIndex,int Lenght=1);
};

dl32String operator+(dl32String &str1,dl32String &str2);

dl32String operator+(char str1[],dl32String &str2);
dl32String operator+(dl32String &str1,char str2[]);

dl32String operator+(dl32String &str,long &number);
dl32String operator+(long &number,dl32String &str);
dl32String operator+(dl32String &str,int &number);
dl32String operator+(int &number,dl32String &str);
dl32String operator+(dl32String &str,double &number);
dl32String operator+(double &number,dl32String &str);
dl32String operator+(dl32String &str,float &number);
dl32String operator+(float &number,dl32String &str);

//dl32String& operator+(char str[],long &number);
//dl32String& operator+(long &number,char str[]);
//dl32String& operator+(char str[],int &number);
//dl32String& operator+(int &number,char str);
//dl32String& operator+(char str[],double &number);
//dl32String& operator+(double &number,char str[]);
//dl32String& operator+(char str[],float &number);
//dl32String& operator+(float &number,char str[]);

#endif