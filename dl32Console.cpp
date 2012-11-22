#include "dl32Console.h"

dl32Console* dl32Console::instance=NULL;
dl32Console dl32Console::console=dl32Console();

void dl32Console::Open(dl32String Title,dl32ConsoleType Type)throw(dl32ConsoleException)
{
	ready=false;
	CONSOLE_SCREEN_BUFFER_INFO info;

	if(instance==NULL)//Singleton
	{
		AllocConsole();
		type=Type;

		if(type==DL32CT_STDOUT)
			handle=GetStdHandle(STD_OUTPUT_HANDLE);
		else
			handle=GetStdHandle(STD_ERROR_HANDLE);

		if(handle!=NULL)
		{
			SetConsoleTitle(Title.c_str());
			instance=this;
			ready=true;
			
			GetConsoleScreenBufferInfo(handle,&info);
			colorsettings=info.wAttributes;
		}
		else
			throw dl32ConsoleException("dl32Console::dl32Console(): Error creating console. Cannot get console handle");
	}
	else
		throw dl32ConsoleException("dl32Console::dl32Console(): Application console is running. Cannot create other console");
}

dl32Console::~dl32Console()
{
	FreeConsole();
	instance=NULL;
}

void dl32Console::Close()
{
	FreeConsole();
	instance=NULL;
}

void dl32Console::Write(dl32String str,dl32ConsolePalette foregroundcolor,dl32ConsolePalette backgroundcolor)throw(dl32ConsoleException)
{
	if(ready)
	{
		if(foregroundcolor!=DL32CP_EMPTYVALUE)
		{
			SetConsoleTextAttribute(handle,(colorsettings & DL32CONSOLESETTINGS_COLORMASK_NOCOLOR) | (foregroundcolor | backgroundcolor<<4));
			WriteConsole(handle,str.c_str(),str.GetLength(),NULL,NULL);
			SetConsoleTextAttribute(handle,colorsettings);
		}
		else
			WriteConsole(handle,str.c_str(),str.GetLength(),NULL,NULL);
	}
	else
		throw dl32ConsoleException("dl32Console::Write(dl32String str): Console is not ready");
}

void dl32Console::WriteLine(dl32String str,dl32ConsolePalette foregroundcolor,dl32ConsolePalette backgroundcolor)throw(dl32ConsoleException)
{
	if(ready)
	{
		char* strArray;
		int length=str.GetLength();
		
		strArray=new char[length+2];
		for(int i=0;i<length;++i)
			strArray[i]=str[i];

		strArray[length]=DL32TRING_ENDLINE;
		strArray[length+1]=DL32STRING_FINALSYMBOL;

		if(foregroundcolor!=DL32CP_EMPTYVALUE)
		{
			SetConsoleTextAttribute(handle,(colorsettings & DL32CONSOLESETTINGS_COLORMASK_NOCOLOR) | (foregroundcolor | backgroundcolor<<4));
			WriteConsole(handle,strArray,length+1,NULL,NULL);
			SetConsoleTextAttribute(handle,colorsettings);
		}
		else
			WriteConsole(handle,strArray,length+1,NULL,NULL);

		delete strArray;
	}
	else
		throw dl32ConsoleException("dl32Console::Write(dl32String str): Console is not ready");
}

dl32Console& dl32Console::operator<<(dl32String str)throw(dl32ConsoleException)
{
	Write(str);
	return *this;
}

void dl32Console::SetTitle(dl32String title)throw(dl32ConsoleException)
{
	if(ready)
		if(!SetConsoleTitle(title.c_str())) throw dl32ConsoleException("dl32Console::SetTitle(dl32String title): An error ocurred setting up title. Please check console status");
	else
		throw dl32ConsoleException("dl32Console::SetTitle(dl32String title): Console is not ready");
}

dl32String dl32Console::GetTitle()throw(dl32ConsoleException)
{
	if(ready)
	{
		LPSTR strpointer;
		int length=GetConsoleTitle(strpointer,DL32STRINGDEFAULTS_MAXSIZE);
		if(length>0)
			return dl32String(strpointer,length);
		else
			throw dl32ConsoleException("dl32Console::GetTitle(): An error ocurred getting title. Please check console status");
	}
	else
		throw dl32ConsoleException("dl32Console::GetTitle(): Console is not ready");
}

void dl32Console::SetForegroundColor(dl32ConsolePalette color)throw(dl32Exception)
{
	if(ready)
	{
		DWORD settings=(colorsettings & DL32CONSOLESETTINGS_COLORMASK_NOFOREGROUND) | color;
		if(SetConsoleTextAttribute(handle,settings))
			colorsettings=settings;
		else
			throw dl32ConsoleException("dl32Console::SetForegroundColor(dl32ConsolePalette color): An error ocurred setting up color. Please check console status");
	}
	else
		throw dl32ConsoleException("dl32Console::SetForegroundColor(dl32ConsolePalette color): Console is not ready");
}

void dl32Console::SetBackgroundColor(dl32ConsolePalette color)throw(dl32Exception)
{
	if(ready)
	{
		DWORD settings=(colorsettings & DL32CONSOLESETTINGS_COLORMASK_NOBACKGROUND) | color<<4;
		if(SetConsoleTextAttribute(handle,settings))
			colorsettings=settings;
		else
			throw dl32ConsoleException("dl32Console::SetBackgroundColor(dl32ConsolePalette color): An error ocurred setting up color. Please check console status");
	}
	else
		throw dl32ConsoleException("dl32Console::SetBackgroundColor(dl32ConsolePalette color): Console is not ready");
}

void dl32Console::SetColors(dl32ConsolePalette foreground,dl32ConsolePalette background)throw(dl32ConsoleException)
{
	if(ready)
	{
		DWORD settings=(colorsettings & DL32CONSOLESETTINGS_COLORMASK_NOCOLOR) | (foreground | background<<4);
		if(SetConsoleTextAttribute(handle,settings))
			colorsettings=settings;
		else
			throw dl32ConsoleException("dl32Console::SetColors(dl32ConsolePalette foreground,dl32ConsolePalette background): An error ocurred setting up colors. Please check console status");
	}
	else
		throw dl32ConsoleException("dl32Console::SetColors(dl32ConsolePalette foreground,dl32ConsolePalette background): Console is not ready");
}