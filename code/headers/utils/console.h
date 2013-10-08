/* 
 * File:   dl32ConsoleColor.h
 * Author: Manu343726
 *
 * Created on 3 de mayo de 2013, 11:52
 */

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

/****************************************************************************************************************************************
 * This API provides a set of easy-to-use methods for change the style of the standard output stream.                                   *
 * All operations are performed by the dl32ConsoleColorSettings singleton, but the API provides a more                                  *
 * confortable syntax to use its methods, all based in a stream syntax:                                                                 *
 *                                                                                                                                      *
 * a) Explicit operations:                                                                                                              *
 *     - User specifies the operations in a explicit manner, through a set of types. (See dl32ChangeForegroungColor class,              *
 *       dl32ChangeBackgroundColor class , dl32PushStyle class , dl32PopStyle class , and dl32SetAutoPush class bellow)                 *
 *     - Example: cout << dl32ChangeForegroundColor( dl32ConsoleColor::GREEN ) << "Ohhh, i'm green" << endl;                            *
 *                                                                                                                                      *
 *     #Also, the API provides a set of constants to avoid instantations. For example: cout << push_style; instead of                   *
 *      cout << dl32PushStyle();                                                                                                        *
 *                                                                                                                                      *
 * b) Implicit operations:                                                                                                              *
 *     - User specifies the style-change using a set of values stored in enums (See dl32StyleChange enum operator<< overload bellow).   *
 *     - Example: cout << dl32StyleChange::FOREGROUND << dl32ConsoleColor::GREEN << "Ohhh, i'm green" << endl;                          *
 *                                                                                                                                      *
 *                                                                                                                                      *
 * An example of use is provided in the console_color_test:                                                                             *
 * (https://github.com/Manu343726/cpp_lib32/blob/BigRefactoring/code/tests/refactoring/console_color_test.cpp)                          * 
 *                                                                                                                                      *
 ***************************************************************************************************************************************/

#pragma once

#ifndef DL32CONSOLECOLOR_H
#define	DL32CONSOLECOLOR_H

#include "generic_singleton.h"

#include <iostream>
#include <vector>
#include <cstdlib>

namespace dl32
{
	namespace console
	{

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// @brief Set of console colors.
		///
		/// @author	Manu343726
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		enum color
		{
			BLACK       = 0x00000000,
			BLUE        = 0x00000001,
			GREEN       = 0x00000002,
			AQUA        = 0x00000003,
			RED         = 0x00000004,
			PURPLE      = 0x00000005,
			YELLOW      = 0x00000006,
			WHITE       = 0x00000007,
			GRAY        = 0x00000008,
			LIGHTBLUE   = 0x00000009,
			LIGHTGREEN  = 0x0000000A,
			LIGHTAQUA   = 0x0000000B,
			LIGHTRED    = 0x0000000C,
			LIGHTPURPLE = 0x0000000D,
			LIGHTYELLOW = 0x0000000E,
			BRIGHTWHITE = 0x0000000F,
			EMPTYVALUE  = -1
		};

#if defined(_WIN32)

		namespace windows
		{ 
			#include <Windows.h> 
		}

		typedef windows::HANDLE standard_output_handle;
		typedef windows::DWORD  console_style;
#elif defined(__linux__)

		typedef void* file_descriptor; //Working on it.... Provisional
		typedef file_descriptor standard_output_handle;
		typedef unsigned int   console_style;
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// @brief Set of console-style change types.
		///
		/// @author	Manu343726
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		enum class style_change
		{
			FOREGROUND,
			BACKGROUND
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// @brief This class provides methods for change colors of the standard output stream.
		///
		/// @author	Manu343726
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		class style_settings : public dl32::singleton<style_settings>
		{ 
			MAKE_SINGLETON( style_settings )
		private:
			standard_output_handle _handle;
			void _setup_handle();
    
			std::vector<console_style> _styles_stack;
    
			dl32::console::console_style _get_style();
			void                         _set_style( dl32::console::console_style style );
			void                         _update_style( dl32::console::console_style style );
			void _push_style();
			void _pop_style();
			bool _styles_stack_autopush;
    
			dl32::console::style_change _last_change;

		public:  
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			/// @brief Sets the autopush mode.
			/// @details If auto_push is true, the new console style is pushed automatically when "change_background"
			///          or "change_foreground" are called.
			///
			/// @author	Manu343726
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			void set_autopush( bool value ) { _styles_stack_autopush = value; }
    
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			/// @brief Changes the background color of the standard output stream.
			///
			/// @ param color New background color.
			///
			/// @author	Manu343726
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			void change_background( dl32::console::color color );
    
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			/// @brief Changes the foreground color of the standard output stream.
			///
			/// @ param color New foreground color.
			///
			/// @author	Manu343726
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			void change_foreground( dl32::console::color color );
    
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			/// @brief Pushes the current console style to the stack (The current style is not changed).
			///
			/// @author	Manu343726
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			void push_style();
    
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			/// @brief Pops the current console style. Current state is changed to the last stacked state.
			///
			/// @author	Manu343726
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			void pop_style();
    
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			/// @brief Gets the last console-style change performed.
			///
			/// @author	Manu343726
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			dl32::console::style_change last_change() { return _last_change; }
    
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			/// @brief Executes the last console-style change with the specified color.
			///
			/// @author	Manu343726
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			void execute_change( dl32::console::color color )
			{
				switch( _last_change )
				{
					case dl32::console::style_change::BACKGROUND: change_background( color ); break;
					case dl32::console::style_change::FOREGROUND: change_foreground( color ); break;
				}
			}
    
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			/// @brief Forces function "execute_change()" to perform an specified change. 
			///
			/// @author	Manu343726
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			void set_execute_change( dl32::console::style_change change) { _last_change = change; }
		};


		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// @brief Base class for color change providers.
		///
		/// @author	Manu343726
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		struct color_change
		{
			const dl32::console::color color;

			color_change( dl32::console::color _color ) : color(_color) {}
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// @brief This class provides an interface to set foreground color through the output stream operator (<<).
		///        For example: cout << change_foreground_t( console::color::RED ) << "HELLO WORLD!";
		///        Prints "HELLO WORLD!" with red text. 
		///
		/// @author	Manu343726
		/// 
		/// @remarks Note that this changes only affects to text writed after the change.
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		struct change_foreground_t : public color_change
		{
			change_foreground_t( dl32::console::color color ) : color_change( color ) {}
    
			friend std::ostream& operator<<(std::ostream& os , const change_foreground_t& change)
			{
				dl32::console::style_settings::instance().change_foreground( change.color );

				return os;
			}
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// @brief This class provides an interface to set background color through the output stream operator (<<).
		///        For example: cout << change_background_t( console::color::RED ) << "HELLO WORLD!";
		///        Prints "HELLO WORLD!" with red background. 
		///
		/// @author	Manu343726
		/// 
		/// @remarks Note that this changes only affects to text writed after the change.
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		struct change_background_t : public color_change
		{
			//ctor
			change_background_t( dl32::console::color color ) : color_change( color ) {}
    
			friend std::ostream& operator<<(std::ostream& os , const change_background_t& change)
			{
				dl32::console::style_settings::instance().change_background( change.color );

				return os;
			}
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// @brief This class provides an interface to set the autopush mode through the output stream operator (<<).
		///        For example: cout << dl32SetAutoPush<true>(); enables autopush.
		///
		/// @author	Manu343726
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		template<bool VALUE>
		struct set_autopush_t : public std::integral_constant<bool,VALUE>
		{
			friend std::ostream& operator<<(std::ostream& os , const set_autopush_t<VALUE>& enable_autopush)
			{
				dl32::console::style_settings::instance().set_autopush( VALUE );

				return os;
			}
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// @brief This class provides an interface to pùsh the current style through the output stream operator (<<).
		///
		/// @author	Manu343726
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		struct push_style_t 
		{
			friend std::ostream& operator<<(std::ostream& os , const push_style_t& push)
			{
				dl32::console::style_settings::instance().push_style();

				return os;
			}
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// @brief This class provides an interface to pop the current style through the output stream operator (<<).
		///
		/// @author	Manu343726
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		struct pop_style_t 
		{
			friend std::ostream& operator<<(std::ostream& os , const pop_style_t& pop)
			{
				dl32::console::style_settings::instance().pop_style();

				return os;
			}
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// @brief Operator << overload to provide a simpler syntax.
		/// @details With these overloads, operations can be performed with the following simpler syntax:
		///          cout << [change] << [color];
		///          Where:
		///           - [change] is a value of the dl32StyleChange enum. It specifies the change that will be 
		///             performed.
		///           - [color] is the console color (A value of the dl32ConsoleColor enum) that will be setted.
		///
		/// @author	Manu343726
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		inline std::ostream& operator<<(std::ostream& os , dl32::console::style_change change)
		{
			dl32::console::style_settings::instance().set_execute_change( change );
    
			return os;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// @brief Operator << overload to provide a simpler syntax.
		/// @details With these overloads, operations can be performed with the following simpler syntax:
		///          cout << [change] << [color];
		///          Where:
		///           - [change] is a value of the dl32StyleChange enum. It specifies the change that will be 
		///             performed.
		///           - [color] is the console color (A value of the dl32ConsoleColor enum) that will be setted.
		///
		/// @author	Manu343726
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		inline std::ostream& operator<<(std::ostream& os , dl32::console::color color)
		{
			dl32::console::style_settings::instance().execute_change( color );
    
			return os;
		}

		/* Set of global useful constants */

		extern const dl32::console::push_style_t push_style; ///< push_style_t constant. Avoids the creation of multiple dummy instances.
		extern const dl32::console::pop_style_t  pop_style;  ///< pop_style_t constant. Avoids the creation of multiple dummy instances.
		extern const dl32::console::set_autopush_t<true>  enable_autopush;  ///< set_autopush_t<true> constant. Avoids the creation of multiple dummy instances.
		extern const dl32::console::set_autopush_t<false> disable_autopush; ///< set_autopush_t<false> constant. Avoids the creation of multiple dummy instances.
	}
}

#endif	/* DL32CONSOLECOLOR_H */

