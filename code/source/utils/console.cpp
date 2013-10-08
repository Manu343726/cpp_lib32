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

#include "../../headers/utils/console.h"

namespace console = dl32::console;

/* Useful constants definitions */
const console::push_style_t push_style; 
const console::pop_style_t  pop_style; 
const console::set_autopush_t<true>  enable_autopush; 
const console::set_autopush_t<false> disable_autopush;



#if defined(_WIN32)

#include <Windows.h>

/* DWORD text attributes color masks */

const DWORD COLORMASK_NOCOLOR        = 0x11111100;
const DWORD COLORMASK_COLORONLY      = 0x00000011;
const DWORD COLORMASK_NOFOREGROUND   = 0x11111110;
const DWORD COLORMASK_FOREGROUNDONLY = 0x00000001;
const DWORD COLORMASK_NOBACKGROUND   = 0x11111101;
const DWORD COLORMASK_BACKGROUNDONLY = 0x00000010;

typedef DWORD color_value;

/* Internal operations */

std::pair<WORD,WORD> _get_colors_from_style( console::console_style style )
{
    return std::pair<WORD,WORD>( style & COLORMASK_FOREGROUNDONLY , (style & COLORMASK_BACKGROUNDONLY) >> 4 );
}

console::console_style _change_foreground_only(dl32::console::console_style style , DWORD color )
{
    return (style & COLORMASK_NOFOREGROUND) | color;
}

console::console_style _change_background_only(dl32::console::console_style style , DWORD color )
{
    return (style & COLORMASK_NOBACKGROUND) | (color << 4);
}


void console::style_settings::_setup_handle()
{
    if(!(_handle = GetStdHandle( STD_OUTPUT_HANDLE ))) 
        throw;
}

console::console_style console::style_settings::_get_style()
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo( _handle , &info );
    return info.wAttributes;
}


void console::style_settings::_set_style(console::console_style style)
{
    if( !SetConsoleTextAttribute( _handle , style ) )
        throw;
}
#elif defined(__linux__)

//mmm...

#endif



/* Portable code: */


console::style_settings::style_settings()
{
    _setup_handle();
    _styles_stack_autopush = false;
	_last_change = console::style_change::FOREGROUND;
    _styles_stack.push_back( _get_style() );
}


void console::style_settings::_update_style(console::console_style style)
{
    _styles_stack.back() = style;
    _set_style( style );
}

void console::style_settings::_push_style()
{
    _styles_stack.push_back( _get_style() );
}

void console::style_settings::_pop_style()
{
    if( _styles_stack.size() <= 1 ) return;
    
    _styles_stack.pop_back();
    _set_style( _styles_stack.back() );
}

void console::style_settings::change_background(console::color color)
{
    if( _styles_stack_autopush ) _push_style();
    
    _update_style( _change_background_only( _styles_stack.back() , (color_value)color ) );
    
	_last_change = console::style_change::BACKGROUND;
}

void console::style_settings::change_foreground(console::color color)
{
    if( _styles_stack_autopush ) _push_style();
    
    _update_style( _change_foreground_only( _styles_stack.back() , (color_value)color ) );
    
	_last_change = console::style_change::FOREGROUND;
}

void console::style_settings::push_style()
{
    if( _styles_stack_autopush ) return; //Si autopush está activado, haces un push, y luego cambias el estilo, el estilo actual se mete dos veces en la pila.
    
    _push_style();
}

void console::style_settings::pop_style()
{
    _pop_style();
}
