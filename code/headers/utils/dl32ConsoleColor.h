/* 
 * File:   dl32ConsoleColor.h
 * Author: Manu343726
 *
 * Created on 3 de mayo de 2013, 11:52
 */

#ifndef DL32CONSOLECOLOR_H
#define	DL32CONSOLECOLOR_H

#include "dl32Config.h"
#include "dl32Singleton.h"
#include "dl32Typing.h"

#include <iostream>
#include <vector>
using namespace std;

#ifdef WIN32

#include <windows.h>

typedef WORD dl32ConsoleStyle; //En la versión de windows, los styles son los valores de CONSOLE_SCREEN_BUFFER_INFO::wAttributes (Ver implementación))

#else

typedef unsigned int dl32ConsoleStyle; //Por ejemplo, no lo he mirado

#endif /* WIN32 */

enum class dl32ConsoleColor : int
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

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides methods for change colors of the standard output stream.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
class dl32ConsoleColorSettings : public dl32Singleton<dl32ConsoleColorSettings>
{ 
private:
#if WIN32 
    const HANDLE _handle;
    
    std::vector<dl32ConsoleStyle> _styles_stack;
    
    dl32ConsoleStyle _get_style();
    void             _set_style( dl32ConsoleStyle style );
    void             _update_style( dl32ConsoleStyle style );
    void _push_style();
    void _pop_style();
#else
    /* TODO Linux version */
#endif
    
    bool _styles_stack_autopush;
public:  
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Default ctor. Initialices the console-style stack.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    dl32ConsoleColorSettings();
    
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
    void change_background( dl32ConsoleColor color );
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Changes the foreground color of the standard output stream.
    ///
    /// @ param color New foreground color.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void change_foreground( dl32ConsoleColor color );
    
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
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Base class for color change providers.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
struct dl32ColorChange
{
    const dl32ConsoleColor color;

    dl32ColorChange( dl32ConsoleColor color ) : color(color) {}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides an interface to set foreground color through the output stream operator (<<).
///        For example: cout << dl32ChangeForegroundColor( dl32ConsoleColor::RED ) << "HELLO WORLD!";
///        Prints "HELLO WORLD!" with red text. 
///
/// @author	Manu343726
/// 
/// @remarks Note that this changes only affects to text writed after the change.
///////////////////////////////////////////////////////////////////////////////////////////////////////
struct dl32ChangeForegroundColor : public dl32ColorChange
{
    /* ctor */
    dl32ChangeForegroundColor( dl32ConsoleColor color ) : dl32ColorChange( color ) {}
    
    /* output operator */
    ostream& operator<< (ostream& os)
    {
        dl32ConsoleColorSettings.instance().change_foreground( change.color );
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides an interface to set background color through the output stream operator (<<).
///        For example: cout << dl32ChangeForegroundColor( dl32ConsoleColor::RED ) << "HELLO WORLD!";
///        Prints "HELLO WORLD!" with red background. 
///
/// @author	Manu343726
/// 
/// @remarks Note that this changes only affects to text writed after the change.
///////////////////////////////////////////////////////////////////////////////////////////////////////
struct dl32ChangeBackgroundColor : public dl32ColorChange
{
    /* ctor */
    dl32ChangeBackgroundColor( dl32ConsoleColor color ) : dl32ColorChange( color ) {}
    
    /* output operator */
    ostream& operator<< (ostream& os)
    {
        dl32ConsoleColorSettings.instance().change_foreground( change.color );
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides an interface to set the autopush mode through the output stream operator (<<).
///        For example: cout << dl32SetAutoPush<false>(); sets the autopush mode to false.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<bool VALUE>
struct dl32SetAutoPush : dl32ValueWrapper<bool,VALUE>
{
    ostream& operator<< (ostream& os)
    {
        dl32ConsoleColorSettings.instance().set_autopush( VALUE );
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides an interface to pùsh the current style through the output stream operator (<<).
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
struct dl32PushStyle
{
    ostream& operator<< (ostream& os)
    {
        dl32ConsoleColorSettings.instance().push_style();
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides an interface to pop the current style through the output stream operator (<<).
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
struct dl32PopStyle
{
    ostream& operator<< (ostream& os)
    {
        dl32ConsoleColorSettings.instance().pop_style();
    }
};

#endif	/* DL32CONSOLECOLOR_H */

