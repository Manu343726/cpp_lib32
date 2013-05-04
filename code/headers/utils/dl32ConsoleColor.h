/* 
 * File:   dl32ConsoleColor.h
 * Author: Manu343726
 *
 * Created on 3 de mayo de 2013, 11:52
 */

#pragma once

#ifndef DL32CONSOLECOLOR_H
#define	DL32CONSOLECOLOR_H

#include "dl32Config.h"
#include "dl32Typing.h"
#include "dl32Exceptions.h"

#include <windows.h>

#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

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

typedef HANDLE dl32StandardOutputHandle;
typedef WORD dl32ConsoleStyle;

DL32EXCEPTION_SUBCLASS_NODOC( dl32ConsoleStyleChangeFailed );
DL32EXCEPTION_SUBCLASS_NODOC( dl32ConsoleHandleSetupFailed );

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Set of console-style change types.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
enum class dl32StyleChange
{
    FOREGROUND,
    BACKGROUND
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides methods for change colors of the standard output stream.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
class dl32ConsoleColorSettings
{ 
private:
    dl32StandardOutputHandle _handle;
    void _setup_handle() throw ( dl32ConsoleHandleSetupFailed ) { if(!(_handle = GetStdHandle( STD_OUTPUT_HANDLE ))) throw dl32ConsoleHandleSetupFailed(); }
    
    std::vector<dl32ConsoleStyle> _styles_stack;
    
    dl32ConsoleStyle _get_style();
    void             _set_style( dl32ConsoleStyle style )    throw( dl32ConsoleStyleChangeFailed );
    void             _update_style( dl32ConsoleStyle style ) throw( dl32ConsoleStyleChangeFailed );
    void _push_style();
    void _pop_style();
    bool _styles_stack_autopush;
    
    dl32StyleChange _last_change;
    
    
    /* singleton */
    static dl32ConsoleColorSettings* _instance;
    static void _singleton_instance_deleter() { delete _instance; }
    ~dl32ConsoleColorSettings() {}
    dl32ConsoleColorSettings(const dl32ConsoleColorSettings&) {}
    dl32ConsoleColorSettings& operator=(const dl32ConsoleColorSettings&) {}
    
    dl32ConsoleColorSettings() throw ( dl32ConsoleHandleSetupFailed )
    {
        _setup_handle();
        _styles_stack_autopush = false;
        _last_change = dl32StyleChange::FOREGROUND;
        _styles_stack.push_back( _get_style() );
    }
public:  
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets a reference to the singleton instance
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static dl32ConsoleColorSettings& instance() 
    {
        if( _instance == nullptr )
        {
            atexit( _singleton_instance_deleter );
            _instance = new dl32ConsoleColorSettings();
        }
        
        return *_instance;
    }
    
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
    void change_background( dl32ConsoleColor color ) throw( dl32ConsoleStyleChangeFailed );
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Changes the foreground color of the standard output stream.
    ///
    /// @ param color New foreground color.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void change_foreground( dl32ConsoleColor color ) throw( dl32ConsoleStyleChangeFailed );
    
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
    dl32StyleChange last_change() { return _last_change; }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Executes the last console-style change with the specified color.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void execute_change( dl32ConsoleColor color )
    {
        switch( _last_change )
        {
            case dl32StyleChange::BACKGROUND: change_background( color ); break;
            case dl32StyleChange::FOREGROUND: change_foreground( color ); break;
        }
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Forces function "execute_change()" to perform an specified change. 
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void set_execute_change( dl32StyleChange change) { _last_change = change; }
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
    //ctor
    dl32ChangeForegroundColor( dl32ConsoleColor color ) : dl32ColorChange( color ) {}
    
    friend std::ostream& operator<<(ostream& os , const dl32ChangeForegroundColor& change)
    {
        dl32ConsoleColorSettings::instance().change_foreground( change.color );

        return os;
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
    //ctor
    dl32ChangeBackgroundColor( dl32ConsoleColor color ) : dl32ColorChange( color ) {}
    
    friend std::ostream& operator<<(ostream& os , const dl32ChangeBackgroundColor& change)
    {
        dl32ConsoleColorSettings::instance().change_background( change.color );

        return os;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides an interface to set the autopush mode through the output stream operator (<<).
///        For example: cout << dl32SetAutoPush<false>(); sets the autopush mode to false.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<bool VALUE>
struct dl32SetAutoPush : public dl32ValueWrapper<bool,VALUE> {};

template<>
struct dl32SetAutoPush<true>
{
    friend std::ostream& operator<<(ostream& os , const dl32SetAutoPush<true>& enable_autopush)
    {
        dl32ConsoleColorSettings::instance().set_autopush( true );

        return os;
    }
};

template<>
struct dl32SetAutoPush<false>
{
    friend std::ostream& operator<<(ostream& os , const dl32SetAutoPush<false>& disable_autopush)
    {
        dl32ConsoleColorSettings::instance().set_autopush( false );

        return os;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides an interface to pùsh the current style through the output stream operator (<<).
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
struct dl32PushStyle 
{
    friend std::ostream& operator<<(ostream& os , const dl32PushStyle& push)
    {
        dl32ConsoleColorSettings::instance().push_style();

        return os;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides an interface to pop the current style through the output stream operator (<<).
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
struct dl32PopStyle 
{
    friend std::ostream& operator<<(ostream& os , const dl32PopStyle& pop)
    {
        dl32ConsoleColorSettings::instance().pop_style();

        return os;
    }
};

inline ostream& operator<<(ostream& os , dl32StyleChange change)
{
    dl32ConsoleColorSettings::instance().set_execute_change( change );
    
    return os;
}

inline ostream& operator<<(ostream& os , dl32ConsoleColor color)
{
    dl32ConsoleColorSettings::instance().execute_change( color );
    
    return os;
}


#endif	/* DL32CONSOLECOLOR_H */

