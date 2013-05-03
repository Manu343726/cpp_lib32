#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_CONSOLECOLORSTEST 

#include "dl32ConsoleColor.h"
#include "dl32Singleton.h"

class Foo : public dl32Singleton<Foo>
{
public:
    void make_things_happen() { cout << "oooohhh!!!" << endl; }
};

int main()
{
    cout << "Before push" << dl32PushStyle() << endl;
    cout << dl32ChangeForegroundColor( dl32ConsoleColor::YELLOW ) << dl32ChangeBackgroundColor( dl32ConsoleColor::BLUE );
    cout << "Old-days syntax highlighting..." << endl;
    cout << "Old-days syntax highlighting..." << endl;
    cout << "Old-days syntax highlighting..." << endl;
    cout << "Old-days syntax highlighting..." << endl;
    cout << dl32PopStyle() << "After pop" << endl;
}

#endif /* ISCURRENTTEST */
