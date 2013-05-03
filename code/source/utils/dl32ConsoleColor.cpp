#include "dl32ConsoleColor.h"

#ifdef WIN32
#include <windows.h>

#define COLORMASK_NOCOLOR        0x11111100
#define COLORMASK_COLORONLY      0x00000011
#define COLORMASK_NOFOREGROUND   0x11111110
#define COLORMASK_FOREGROUNDONLY 0x00000001
#define COLORMASK_NOBACKGROUND   0x11111101
#define COLORMASK_BACKGROUNDONLY 0x00000010

std::pair<WORD,WORD> _get_colors_from_style( dl32ConsoleStyle style );
dl32ConsoleStyle _change_foreground_only(dl32ConsoleStyle style , DWORD color );
dl32ConsoleStyle _change_background_only(dl32ConsoleStyle style , DWORD color );

std::pair<WORD,WORD> _get_colors_from_style( dl32ConsoleStyle style )
{
    return std::pair<WORD,WORD>( style & COLORMASK_FOREGROUNDONLY , (style & COLORMASK_BACKGROUNDONLY) >> 4 );
}

dl32ConsoleStyle _change_foreground_only(dl32ConsoleStyle style , DWORD color )
{
    return (style & COLORMASK_NOFOREGROUND) | color;
}

dl32ConsoleStyle _change_background_only(dl32ConsoleStyle style , DWORD color )
{
    return (style & COLORMASK_NOBACKGROUND) | (color << 4);
}

dl32ConsoleStyle dl32ConsoleColorSettings::_get_style()
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo( _handle , &info );
    return info->wAttributes;
}

void dl32ConsoleColorSettings::_set_style(dl32ConsoleStyle style)
{
    if( !SetConsoleTextAttribute( _handle , style ) )
        throw;
}

void dl32ConsoleColorSettings::_update_style(dl32ConsoleStyle style)
{
    _styles_stack.push_back( style );
    _set_style( style );
}

void dl32ConsoleColorSettings::_push_style()
{
    _styles_stack.push_back( _get_style() );
}

void dl32ConsoleColorSettings::_pop_style()
{
    if( _styles_stack.size() <= 1 ) return;
    
    _styles_stack.pop_back();
    _set_style( _styles_stack.back() );
}

dl32ConsoleColorSettings::dl32ConsoleColorSettings()
{
    _handle = GetStdHandle(STD_OUTPUT_HANDLE);
    _styles_stack_autopush = false;
    
    if( _handle )
        _push_style(); //Inicializamos la pila con el estilo actual.
    else
        throw;
}

void dl32ConsoleColorSettings::change_background(dl32ConsoleColor color)
{
    if( _styles_stack_autopush ) _push_style();
    
    _update_style( _change_background_only( _styles_stack.back() , color ) );
}

void dl32ConsoleColorSettings::change_foreground(dl32ConsoleColor color)
{
    if( _styles_stack_autopush ) _push_style();
    
    _update_style( _change_foreground_only( _styles_stack.back() , color ) );
}

void dl32ConsoleColorSettings::push_style()
{
    if( _styles_stack_autopush ) return; //Si autopush está activado, haces un push, y luego cambias el estilo, el estilo actual se mete dos veces en la pila.
    
    _push_style();
}

void dl32ConsoleColorSettings::pop_style()
{
    _pop_style();
}
#else
/* TODO: Linux version */
#endif /* WIN32 */
