#include "dl32String.h"
#include <cmath>

dl32String::dl32String()
{
	Array=new char[1];
	Array[0]=DL32STRING_FINALSYMBOL;
	size=0;
}

dl32String::~dl32String()
{
	if(Array!=NULL)
	{
		DL32MEMORY_SAFEDELETE_ARRAY(Array);
		size=-1;
	}
}

bool dl32String::Ready()const
{
	return size>=0;
}

dl32String::dl32String(char Str)
{
	Array=new char[2];
	size=1;
	Array[0]=Str;
	Array[1]=DL32STRING_FINALSYMBOL;
}

void dl32String::LongCasting(long Number)
{
	int c,n;

	if(Number>0)
	{
		size=floor(log10(float(Number)))+1;
		Array=new char[size+1];

		c=pow(float(10),float(size-1));

		for(int i=0;i<size;++i)
		{
			n=Number / c;
			Number %= c;
			c/=10;

			Array[i]=char(n+DL32CONSTS_0CHARCODE);
		}
	}
	else
	{
		if(Number==0)
		{
			size=1;
			Array=new char[2];
			Array[0]=DL32CONSTS_0CHARCODE;
			Array[1]=DL32STRING_FINALSYMBOL;
		}
		else
		{
			Number=-Number;

			size=floor(log10(float(Number)))+2;
			Array=new char[size+1];

			c=pow(float(10),float(size-2));

			Array[0]='-';

			for(int i=1;i<size;++i)
			{
				n=Number / c;
				Number %= c;
				c/=10;

				Array[i]=char(n+DL32CONSTS_0CHARCODE);
			}
		}
	}

	Array[size]=DL32STRING_FINALSYMBOL;
}

dl32String::dl32String(const char Str[])
{
	size=strlen(Str);

	Array=new char[size+1];

	for(int i=0;i<size;++i)
		Array[i]=Str[i];
	
	Array[size]=DL32STRING_FINALSYMBOL;
}

dl32String::dl32String(char Str[],int Size)
{
	size=Size;
	Array=Str;
}

dl32String::dl32String(const dl32String &str)
{
	if(this != &str)
	{
		size=str.size;
		Array=new char[size+1];

		for(int i=0;i<=size;++i)
			Array[i]=str.Array[i];
	}
}

dl32String::dl32String(const string &str)
{
	size=str.length();
	Array=new char[size+1];

	for(int i=0;i<size;++i)
		Array[i]=str[i];

	Array[size]=DL32STRING_FINALSYMBOL;
}

dl32String::dl32String(long Number,bool Hex)
{
	if(Hex)
		HexLongCasting(Number);
	else
		LongCasting(Number);
}

dl32String::dl32String(int Number,bool Hex)
{
	if(Hex)
		HexLongCasting(long(Number));
	else
		LongCasting(long(Number));
}

dl32String::dl32String(float Number,int)
{
	LongCasting(long(Number));
}

dl32String::dl32String(double Number,int)
{
	LongCasting(long(Number));
}

dl32String::dl32String(void* memoryaddress)
{
	HexLongCasting(long(memoryaddress));
}

void dl32String::HexLongCasting(long Number)
{
	size=10;//"0x" + ocho digitos hexadecimales (32 bits)
	Array=new char[11];//Lo anterior más el caracter final (Cadena C-style)
	int delta,realdelta;

	Array[0]='0';
	Array[1]='x';
	Array[size]=DL32STRING_FINALSYMBOL;

	for(int i=0;i<8;++i)//8 posiciones (cifras) hexadecimales (32 bits)
	{
		delta=int(Number)>>(i*4) & 0x0000000F;//delta es el numero (en decimal) de ésta posición
		if(delta<=9)
			realdelta=delta;
		else
			realdelta=delta+7;// El caracter 'A' no está después del '9' (Ver tabla ASCII)

		Array[size-i-1]=char(int('0')+realdelta);
	}
}

char* dl32String::c_str()
{
	return Array;
}

char* dl32String::Copy() const
{
	char *copy=new char[size+1];
	strcpy(copy,Array);

	return copy;
}

dl32String dl32String::Concat(const dl32String &str1,const dl32String &str2)
{
	if(str1.Ready() && str2.Ready())
	{
		int size=str1.size+str2.size;
		char *Array=new char[size+1];

		for(int i=0;i<str1.size;++i)
			Array[i]=str1.Array[i];
		for(int i=0;i<str2.size;++i)
			Array[i+str1.size]=str2.Array[i];
		Array[size]=DL32STRING_FINALSYMBOL;

		return dl32String(Array,size);
	}
	else
	{
		if(str1.Ready()) return str1;
		if(str2.Ready()) return str2;
		return dl32String();
	}
}

char dl32String::At(int pos)throw(dl32OutOfRangeException)
{
	if(pos>=0 && pos<size)
		return Array[pos];
	else
		throw dl32OutOfRangeException(dl32Range(size),pos,"dl32String::At(int pos): 'pos' is out of range");
}

dl32String& dl32String::operator=(const dl32String &string)
{
	if(this != &string)
	{
		if(Array!=NULL) delete Array;
		
		size=string.size;
		Array=new char[size+1];

		for(int i=0;i<=size;++i)
			Array[i]=string.Array[i];
	}

	return *this;
}

