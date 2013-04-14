#ifndef DL32CONSOLE_H
#define DL32CONSOLE_H
#include "dl32Math.h"
#include "dl32Memory.h"

#include <Windows.h>
#include <string>

using namespace std;

enum dl32ConsoleType
{
	DL32CT_STDOUT,
	DL32CT_STDERR
};

enum dl32ConsolePalette
{
	DL32CP_BLACK=0x00000000,
	DL32CP_BLUE=0x00000001,
	DL32CP_GREEN=0x00000002,
	DL32CP_AQUA=0x00000003,
	DL32CP_RED=0x00000004,
	DL32CP_PURPLE=0x00000005,
	DL32CP_YELLOW=0x00000006,
	DL32CP_WHITE=0x00000007,
	DL32CP_GRAY=0x00000008,
	DL32CP_LIGHTBLUE=0x00000009,
	DL32CP_LIGHTGREEN=0x0000000A,
	DL32CP_LIGHTAQUA=0x0000000B,
	DL32CP_LIGHTRED=0x0000000C,
	DL32CP_LIGHTPURPLE=0x0000000D,
	DL32CP_LIGHTYELLOW=0x0000000E,
	DL32CP_BRIGHTWHITE=0x0000000F,
	DL32CP_EMPTYVALUE=-1
};

#define DL32CONSOLESETTINGS_COLORMASK_NOCOLOR 0x11111100
#define DL32CONSOLESETTINGS_COLORMASK_COLORONLY 0x00000011
#define DL32CONSOLESETTINGS_COLORMASK_NOFOREGROUND 0x11111110
#define DL32CONSOLESETTINGS_COLORMASK_FOREGROUNDONLY 0x00000001
#define DL32CONSOLESETTINGS_COLORMASK_NOBACKGROUND 0x11111101
#define DL32CONSOLESETTINGS_COLORMASK_BACKGROUNDONLY 0x00000010

//DECLARACI�N E IMPLEMENTACI�N (INLINE) DE EXCEPCIONES:
///////////////////////////////////////////////////////
class dl32ConsoleException:public dl32Exception
{
public:
	dl32ConsoleException(const char* message = DEFAULTEXCEPTIONMESSAGE(dl32ConsoleException)):dl32Exception(message){}
};

class dl32ClosedConsoleException:public dl32ConsoleException
{
public:
	dl32ClosedConsoleException(const char* message = DEFAULTEXCEPTIONMESSAGE(dl32ClosedConsoleException)):dl32ConsoleException(message){}
};

class dl32OpenedConsoleException:public dl32ConsoleException
{
public:
	dl32OpenedConsoleException(const char* message = DEFAULTEXCEPTIONMESSAGE(dl32OpenedConsoleException)):dl32ConsoleException(message){}
};

class dl32ConsoleOpenFailed:public dl32ConsoleException
{
public:
	dl32ConsoleOpenFailed(const char* message = DEFAULTEXCEPTIONMESSAGE(dl32ConsoleOpenFailed)):dl32ConsoleException(message){}
};

class dl32ConsoleSettingsFailed:public dl32ConsoleException
{
public:
	dl32ConsoleSettingsFailed(const char* message = DEFAULTEXCEPTIONMESSAGE(dl32ConsoleSettingsFailed)):dl32ConsoleException(message){}
};

class dl32ConsoleSingletonException:public dl32ConsoleException
{
public:
	dl32ConsoleSingletonException(const char* message = DEFAULTEXCEPTIONMESSAGE(dl32ConsoleSingletonException)):dl32ConsoleException(message){}
};

//CLASES DEL M�DULO PROPIAMENTE DICHO:
//////////////////////////////////////

class dl32Console
{
private:
	static dl32Console* instance;
	HANDLE handle;
	DWORD colorsettings;
	dl32ConsoleType type;
	bool ready;

	bool _processing;//Para prevenir recursiones infinitas
	
	dl32Console() throw (dl32ConsoleSingletonException);
	~dl32Console();
public:
	void Open(dl32String Title,dl32ConsoleType Type=DL32CT_STDOUT)throw(dl32OpenedConsoleException,dl32ConsoleOpenFailed);
	void Close();
	
	bool Ready(){return ready;}

	void Write(dl32String str,dl32ConsolePalette foregroundcolor=DL32CP_EMPTYVALUE,dl32ConsolePalette backgroundcolor=DL32CP_BLACK)throw(dl32ClosedConsoleException);
	void WriteLine(dl32String str,dl32ConsolePalette foregroundcolor=DL32CP_EMPTYVALUE,dl32ConsolePalette backgroundcolor=DL32CP_BLACK)throw(dl32ClosedConsoleException);
	void Clear()throw(dl32ClosedConsoleException);

	dl32Console& operator<<(dl32String str)throw(dl32ClosedConsoleException,dl32ClosedConsoleException);

	void SetTitle(dl32String Title)throw(dl32ClosedConsoleException,dl32ConsoleSettingsFailed);
	dl32String GetTitle()throw(dl32ClosedConsoleException,dl32ConsoleSettingsFailed);

	void SetForegroundColor(dl32ConsolePalette color)throw(dl32ClosedConsoleException,dl32ConsoleSettingsFailed);
	void SetBackgroundColor(dl32ConsolePalette color)throw(dl32ClosedConsoleException,dl32ConsoleSettingsFailed);
	void SetColors(dl32ConsolePalette foreground,dl32ConsolePalette background)throw(dl32ClosedConsoleException,dl32ConsoleSettingsFailed);

	void SetCursorPosition(dl32Point2D Position)throw(dl32ClosedConsoleException,dl32ConsoleSettingsFailed);
	dl32Point2D GetCursorPosition()throw(dl32ClosedConsoleException,dl32ConsoleSettingsFailed);

	void SetScroll(int FirstLine)throw(dl32ClosedConsoleException,dl32ConsoleSettingsFailed);
	int GetScroll()throw(dl32ClosedConsoleException,dl32ConsoleSettingsFailed);

	static dl32Console console;

	bool Processing(){return _processing;}
};

#define Console dl32Console::console
#endif