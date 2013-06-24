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

#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_CONSOLECOLORSTEST

#include "dl32ConsoleColor.h"
#include "dl32Singleton.h"

class Foo : public dl32Singleton<Foo>
{
public:
    void make_things_happen() { cout << "oooohhh!!!" << endl; }
};

void test_console_color_settings_api_bysingleton()
{
    cout << "Before push" << endl;
    
    dl32ConsoleColorSettings::instance().push_style();
    
    dl32ConsoleColorSettings::instance().change_foreground( dl32ConsoleColor::LIGHTRED );
    cout << "Red!!!" << endl;
    dl32ConsoleColorSettings::instance().change_foreground( dl32ConsoleColor::LIGHTGREEN );
    cout << "Green!!!" << endl;
    dl32ConsoleColorSettings::instance().change_foreground( dl32ConsoleColor::LIGHTBLUE );
    cout << "Blue!!!" << endl;
    
    dl32ConsoleColorSettings::instance().change_foreground( dl32ConsoleColor::BLACK );

    dl32ConsoleColorSettings::instance().change_background( dl32ConsoleColor::LIGHTRED );
    cout << "Red!!!" << endl;
    dl32ConsoleColorSettings::instance().change_background( dl32ConsoleColor::LIGHTGREEN );
    cout << "Green!!!" << endl;
    dl32ConsoleColorSettings::instance().change_background( dl32ConsoleColor::LIGHTBLUE );
    cout << "Blue!!!" << endl;
    
    dl32ConsoleColorSettings::instance().pop_style();
    cout << "After pop" << endl;
    
    dl32ConsoleColorSettings::instance().set_autopush( true );
    cout << "Auto-push enabled" << endl;
    
    dl32ConsoleColorSettings::instance().change_foreground( dl32ConsoleColor::LIGHTRED );
    cout << "Red!!!" << endl;
    dl32ConsoleColorSettings::instance().change_foreground( dl32ConsoleColor::LIGHTGREEN );
    cout << "Green!!!" << endl;
    dl32ConsoleColorSettings::instance().change_foreground( dl32ConsoleColor::LIGHTBLUE );
    cout << "Blue!!!" << endl;
    
    dl32ConsoleColorSettings::instance().pop_style();
    cout << "After pop" << endl;
    dl32ConsoleColorSettings::instance().pop_style();
    cout << "After pop" << endl;
    dl32ConsoleColorSettings::instance().pop_style();
    cout << "After pop" << endl;
    
    dl32ConsoleColorSettings::instance().set_autopush( false );
    cout << "Auto-push disabled" << endl;
}

void test_console_color_settings_api_explicit()
{
    cout << "Before push" << dl32PushStyle() << endl;
    
    cout << dl32ChangeForegroundColor( dl32ConsoleColor::LIGHTRED )   << "Red!!!"   << endl;
    cout << dl32ChangeForegroundColor( dl32ConsoleColor::LIGHTGREEN ) << "Green!!!" << endl;
    cout << dl32ChangeForegroundColor( dl32ConsoleColor::LIGHTBLUE )  << "Blue!!!"  << endl;
    
    cout << dl32ChangeForegroundColor( dl32ConsoleColor::BLACK );
    
    cout << dl32ChangeBackgroundColor( dl32ConsoleColor::LIGHTRED )   << "Red!!!"   << endl;
    cout << dl32ChangeBackgroundColor( dl32ConsoleColor::LIGHTGREEN ) << "Green!!!" << endl;
    cout << dl32ChangeBackgroundColor( dl32ConsoleColor::LIGHTBLUE )  << "Blue!!!"  << endl;
    
    cout << dl32PopStyle() << "After pop" << endl;
    
    cout << dl32SetAutoPush<true>() << "Auto-push enabled" << endl;
    cout << dl32ChangeForegroundColor( dl32ConsoleColor::LIGHTRED )   << "Red!!!"   << endl;
    cout << dl32ChangeForegroundColor( dl32ConsoleColor::LIGHTGREEN ) << "Green!!!" << endl;
    cout << dl32ChangeForegroundColor( dl32ConsoleColor::LIGHTBLUE )  << "Blue!!!"  << endl;
    
    cout << dl32PopStyle() << "After pop" << endl;
    cout << dl32PopStyle() << "After pop" << endl;
    cout << dl32PopStyle() << "After pop" << endl;
    
    cout << dl32SetAutoPush<false>() << "Auto-push disabled" << endl;
}

void test_console_color_settings_api_explicit_constants()
{
    cout << "Before push" << push_style << endl;
    
    cout << dl32ChangeForegroundColor( dl32ConsoleColor::LIGHTRED )   << "Red!!!"   << endl;
    cout << dl32ChangeForegroundColor( dl32ConsoleColor::LIGHTGREEN ) << "Green!!!" << endl;
    cout << dl32ChangeForegroundColor( dl32ConsoleColor::LIGHTBLUE )  << "Blue!!!"  << endl;
    
    cout << dl32ChangeForegroundColor( dl32ConsoleColor::BLACK );
    
    cout << dl32ChangeBackgroundColor( dl32ConsoleColor::LIGHTRED )   << "Red!!!"   << endl;
    cout << dl32ChangeBackgroundColor( dl32ConsoleColor::LIGHTGREEN ) << "Green!!!" << endl;
    cout << dl32ChangeBackgroundColor( dl32ConsoleColor::LIGHTBLUE )  << "Blue!!!"  << endl;
    
    cout << pop_style << "After pop" << endl;
    
    cout << enable_autopush << "Auto-push enabled" << endl;
    cout << dl32ChangeForegroundColor( dl32ConsoleColor::LIGHTRED )   << "Red!!!"   << endl;
    cout << dl32ChangeForegroundColor( dl32ConsoleColor::LIGHTGREEN ) << "Green!!!" << endl;
    cout << dl32ChangeForegroundColor( dl32ConsoleColor::LIGHTBLUE )  << "Blue!!!"  << endl;
    
    cout << pop_style << "After pop" << endl;
    cout << pop_style << "After pop" << endl;
    cout << pop_style << "After pop" << endl;
    
    cout << disable_autopush << "Auto-push disabled" << endl;
}

void test_console_color_settings_api_implicit()
{
    cout << "Before push" << push_style << endl;
    
    cout << dl32StyleChange::FOREGROUND << dl32ConsoleColor::LIGHTRED << "Red!!!"   << endl;
    cout << dl32StyleChange::FOREGROUND << dl32ConsoleColor::LIGHTGREEN << "Green!!!"   << endl;
    cout << dl32StyleChange::FOREGROUND << dl32ConsoleColor::LIGHTBLUE << "Blue!!!"   << endl;
    
    cout << dl32StyleChange::FOREGROUND << dl32ConsoleColor::BLACK;
    
    cout << dl32StyleChange::BACKGROUND << dl32ConsoleColor::LIGHTRED << "Red!!!"   << endl;
    cout << dl32StyleChange::BACKGROUND << dl32ConsoleColor::LIGHTGREEN << "Green!!!"   << endl;
    cout << dl32StyleChange::BACKGROUND << dl32ConsoleColor::LIGHTBLUE << "Blue!!!"   << endl;
    
    cout << pop_style << "After pop" << endl;
    
    cout << enable_autopush << "Auto-push enabled" << endl;
    cout << dl32StyleChange::FOREGROUND << dl32ConsoleColor::LIGHTRED << "Red!!!"   << endl;
    cout << dl32StyleChange::FOREGROUND << dl32ConsoleColor::LIGHTGREEN << "Green!!!"   << endl;
    cout << dl32StyleChange::FOREGROUND << dl32ConsoleColor::LIGHTBLUE << "Blue!!!"   << endl;
    
    cout << pop_style << "After pop" << endl;
    cout << pop_style << "After pop" << endl;
    cout << pop_style << "After pop" << endl;
    
    cout << disable_autopush << "Auto-push disabled" << endl;
}

int main()
{
    cout << endl << "USING SINGLETON:" << endl << endl;
    test_console_color_settings_api_bysingleton();
    cout << endl << "USING EXPLICIT SYNTAX:" << endl << endl;
    test_console_color_settings_api_explicit();
    cout << endl << "USING EXPLICIT SYNTAX WITH CONSTANTS:" << endl << endl;
    test_console_color_settings_api_explicit_constants();
    cout << endl << "USING IMPLICIT SYNTAX:" << endl << endl;
    test_console_color_settings_api_implicit();
}

#endif /* ISCURRENTTEST */
