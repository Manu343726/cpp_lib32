#include "dl32Console.h"

dl32Console* dl32Console::instance=NULL;
dl32Console dl32Console::console=dl32Console();

dl32Console::dl32Console()throw(dl32ConsoleSingletonException)
{
	if(instance==NULL)//Singleton
	{
		ready=false;
		colorsettings=0;
		instance=this;
		_processing = false;
	}
	else
		throw dl32ConsoleSingletonException("dl32Console::dl32Console(): Can only create one instance of dl32Console");
}

void dl32Console::Open(dl32String Title,dl32ConsoleType Type)throw(dl32OpenedConsoleException,dl32ConsoleOpenFailed)
{
	ready=false;
	CONSOLE_SCREEN_BUFFER_INFO info;

	if(!ready)
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
			throw dl32ConsoleOpenFailed("dl32Console::dl32Console(): Error creating console. Cannot get console handle");
	}
	else
		throw dl32OpenedConsoleException("dl32Console::dl32Console(): Application console is running. Cannot create other console");
}

dl32Console::~dl32Console()
{
	Close();
	instance=NULL;
}

void dl32Console::Close()
{
	if(ready)
	{
		FreeConsole();
		ready=false;
	}
}

void dl32Console::Write(dl32String str,dl32ConsolePalette foregroundcolor,dl32ConsolePalette backgroundcolor)throw(dl32ClosedConsoleException)
{
	if(!_processing)
		if(DL32DEBUG_DL32CONSOLE_AUTOOPEN || ready)
		{
			_processing = true;

			//Si AutoOpen (Solo para debugging) está activado, abrimos la consola:
			if(!ready) Open(DL32DEBUG_DL32CONSOLE_AUTOOPEN_TITLE);

			if(foregroundcolor!=DL32CP_EMPTYVALUE)
			{
				SetConsoleTextAttribute(handle,(colorsettings & DL32CONSOLESETTINGS_COLORMASK_NOCOLOR) | (foregroundcolor | backgroundcolor<<4));
				WriteConsole(handle,str.c_str(),str.GetLength(),NULL,NULL);
				SetConsoleTextAttribute(handle,colorsettings);
			}
			else
				WriteConsole(handle,str.c_str(),str.GetLength(),NULL,NULL);

			_processing = false;
		}
		else
			throw dl32ClosedConsoleException("dl32Console::Write(dl32String str): Console is not ready");
}

void dl32Console::WriteLine(dl32String str,dl32ConsolePalette foregroundcolor,dl32ConsolePalette backgroundcolor)throw(dl32ClosedConsoleException)
{
	if(!_processing)
		if(DL32DEBUG_DL32CONSOLE_AUTOOPEN || ready)
		{
			_processing = true;

			//Si AutoOpen (Solo para debugging) está activado, abrimos la consola:
			if(!ready) Open(DL32DEBUG_DL32CONSOLE_AUTOOPEN_TITLE);

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

			_processing = false;
		}
		else
			throw dl32ClosedConsoleException("dl32Console::Write(dl32String str): Console is not ready");
}

dl32Console& dl32Console::operator<<(dl32String str)throw(dl32ClosedConsoleException)
{
	Write(str);
	return *this;
}

void dl32Console::SetTitle(dl32String title)throw(dl32ClosedConsoleException,dl32ConsoleSettingsFailed)
{
	if(ready)
		if(!SetConsoleTitle(title.c_str())) throw dl32ConsoleSettingsFailed("dl32Console::SetTitle(dl32String title): An error ocurred setting up title. Please check console status");
	else
		throw dl32ConsoleException("dl32Console::SetTitle(dl32String title): Console is not ready");
}

dl32String dl32Console::GetTitle()throw(dl32ClosedConsoleException,dl32ConsoleSettingsFailed)
{
	if(ready)
	{
		LPSTR strpointer;
		int length=GetConsoleTitle(strpointer,DL32STRINGDEFAULTS_MAXSIZE);
		if(length>0)
			return dl32String(strpointer,length);
		else
			throw dl32ConsoleSettingsFailed("dl32Console::GetTitle(): An error ocurred getting title. Please check console status");
	}
	else
		throw dl32ConsoleException("dl32Console::GetTitle(): Console is not ready");
}

void dl32Console::SetForegroundColor(dl32ConsolePalette color)throw(dl32ClosedConsoleException,dl32ConsoleSettingsFailed)
{
	if(ready)
	{
		DWORD settings=(colorsettings & DL32CONSOLESETTINGS_COLORMASK_NOFOREGROUND) | color;
		if(SetConsoleTextAttribute(handle,settings))
			colorsettings=settings;
		else
			throw dl32ConsoleSettingsFailed("dl32Console::SetForegroundColor(dl32ConsolePalette color): An error ocurred setting up color. Please check console status");
	}
	else
		throw dl32ConsoleException("dl32Console::SetForegroundColor(dl32ConsolePalette color): Console is not ready");
}

void dl32Console::SetBackgroundColor(dl32ConsolePalette color)throw(dl32ClosedConsoleException,dl32ConsoleSettingsFailed)
{
	if(ready)
	{
		DWORD settings=(colorsettings & DL32CONSOLESETTINGS_COLORMASK_NOBACKGROUND) | color<<4;
		if(SetConsoleTextAttribute(handle,settings))
			colorsettings=settings;
		else
			throw dl32ConsoleSettingsFailed("dl32Console::SetBackgroundColor(dl32ConsolePalette color): An error ocurred setting up color. Please check console status");
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
			throw dl32ConsoleSettingsFailed("dl32Console::SetColors(dl32ConsolePalette foreground,dl32ConsolePalette background): An error ocurred setting up colors. Please check console status");
	}
	else
		throw dl32ConsoleException("dl32Console::SetColors(dl32ConsolePalette foreground,dl32ConsolePalette background): Console is not ready");
}