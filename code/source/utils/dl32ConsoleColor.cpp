#include "dl32ConsoleColor.h"

#ifdef WIN32
#include <windows.h>

#define COLORMASK_NOCOLOR        0x11111100
#define COLORMASK_COLORONLY      0x00000011
#define COLORMASK_NOFOREGROUND   0x11111110
#define COLORMASK_FOREGROUNDONLY 0x00000001
#define COLORMASK_NOBACKGROUND   0x11111101
#define COLORMASK_BACKGROUNDONLY 0x00000010

dl32ConsoleStyle dl32ConsoleColorSettings::_get_style()
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    
    
}

dl32ConsoleColorSettings::dl32ConsoleColorSettings()
{
    _handle = GetStdHandle(STD_OUTPUT_HANDLE);
    
    if( _handle )
    {
        
    }
}
#else
/* TODO: Linux version */
#endif /* WIN32 */
