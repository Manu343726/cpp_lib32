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
    Foo::instance().make_things_happen();
    dl32ConsoleColorSettings::instance().pop_style();
    cout << dl32ChangeForegroundColor( dl32ConsoleColor::PURPLE ) << "HOLA!!!" << endl;
}

#endif /* ISCURRENTTEST */
