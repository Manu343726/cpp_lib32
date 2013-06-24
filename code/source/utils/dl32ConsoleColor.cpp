/*******************************************************************************
* cpp_lib32 project. C++ port of the dx_lib32 project.                         *
*                                                                              *
* Copyright (C) 2012 - 2013, Manuel Sánchez Pérez                              *                     
*                                                                              *
* This file is part of cpp_lib32 project.                                      *
*                                                                              *
* cpp_lib32 project is free software: you can redistribute it and/or modify    *
* it under the terms of the GNU Lesser General Public License as published by  *
* the Free Software Foundation, version 2 of the License.                      *
*                                                                              *
* cpp_lib32 is distributed in the hope that it will be useful,                 *
* but WITHOUT ANY WARRANTY; without even the implied warranty of               * 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
* GNU Lesser General Public License for more details.                          *
*                                                                              *
 You should have received a copy of the GNU Lesser General Public License      *
 along with cpp_lib32 project. If not, see <http://www.gnu.org/licenses/>.     *
*******************************************************************************/

#include "dl32ConsoleColor.h"

/* Useful constants definitions */
const dl32PushStyle push_style; 
const dl32PopStyle  pop_style; 
const dl32SetAutoPush<true>  enable_autopush; 
const dl32SetAutoPush<false> disable_autopush;

/* DWORD text attributes color masks */
#define COLORMASK_NOCOLOR        0x11111100
#define COLORMASK_COLORONLY      0x00000011
#define COLORMASK_NOFOREGROUND   0x11111110
#define COLORMASK_FOREGROUNDONLY 0x00000001
#define COLORMASK_NOBACKGROUND   0x11111101
#define COLORMASK_BACKGROUNDONLY 0x00000010

/* Internal operations */
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

//Ctor
dl32ConsoleColorSettings::dl32ConsoleColorSettings()
{
    _setup_handle();
    _styles_stack_autopush = false;
    _last_change = dl32StyleChange::FOREGROUND;
    _styles_stack.push_back( _get_style() );
}

void dl32ConsoleColorSettings::_setup_handle() throw ( dl32ConsoleHandleSetupFailed )
{
    if(!(_handle = GetStdHandle( STD_OUTPUT_HANDLE ))) 
        throw dl32ConsoleHandleSetupFailed();
}

dl32ConsoleStyle dl32ConsoleColorSettings::_get_style()
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo( _handle , &info );
    return info.wAttributes;
}

void dl32ConsoleColorSettings::_set_style(dl32ConsoleStyle style) throw( dl32ConsoleStyleChangeFailed )
{
    if( !SetConsoleTextAttribute( _handle , style ) )
        throw dl32ConsoleStyleChangeFailed();
}

void dl32ConsoleColorSettings::_update_style(dl32ConsoleStyle style) throw( dl32ConsoleStyleChangeFailed )
{
    _styles_stack.back() = style;
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

void dl32ConsoleColorSettings::change_background(dl32ConsoleColor color) throw( dl32ConsoleStyleChangeFailed )
{
    if( _styles_stack_autopush ) _push_style();
    
    _update_style( _change_background_only( _styles_stack.back() , (DWORD)color ) );
    
    _last_change = dl32StyleChange::BACKGROUND;
}

void dl32ConsoleColorSettings::change_foreground(dl32ConsoleColor color) throw( dl32ConsoleStyleChangeFailed )
{
    if( _styles_stack_autopush ) _push_style();
    
    _update_style( _change_foreground_only( _styles_stack.back() , (DWORD)color ) );
    
    _last_change = dl32StyleChange::FOREGROUND;
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
