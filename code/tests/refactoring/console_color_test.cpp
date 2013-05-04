#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_CONSOLECOLORSTEST 

#include "dl32ConsoleColor.h"
#include "dl32Singleton.h"

class Foo : public dl32Singleton<Foo>
{
public:
    void make_things_happen() { cout << "oooohhh!!!" << endl; }
};

void test_console_color_settings_api()
{
    cout << "Before push" << dl32PushStyle() << endl;
    
    cout << dl32ChangeForegroundColor( dl32ConsoleColor::LIGHTRED )   << "Red!!!"   << endl;
    cout << dl32ChangeForegroundColor( dl32ConsoleColor::LIGHTGREEN ) << "Green!!!" << endl;
    cout << dl32ChangeForegroundColor( dl32ConsoleColor::LIGHTBLUE )  << "Blue!!!"  << endl;
    
    cout << dl32StyleChange::FOREGROUND << dl32ConsoleColor::BLACK;
    
    cout << dl32ChangeBackgroundColor( dl32ConsoleColor::LIGHTRED )   << "Red!!!"   << endl;
    cout << dl32ChangeBackgroundColor( dl32ConsoleColor::LIGHTGREEN ) << "Green!!!" << endl;
    cout << dl32ChangeBackgroundColor( dl32ConsoleColor::LIGHTBLUE )  << "Blue!!!"  << endl;
    
    cout << dl32PopStyle() << "After pop" << endl;
}

int main()
{
    test_console_color_settings_api();
}

#endif /* ISCURRENTTEST */
