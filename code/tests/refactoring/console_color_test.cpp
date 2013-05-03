#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_CONSOLECOLORSTEST 

#include "dl32ConsoleColor.h"

class Foo : public dl32Singleton<Foo>
{
public:
    void make_things_happen() { cout << "oooohhh!!!" << endl; }
};

int main()
{
    Foo::instance().make_things_happen();
    cout << "HOLA!!!" << endl;
}

#endif /* ISCURRENTTEST */
