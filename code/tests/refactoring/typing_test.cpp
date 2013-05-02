#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_TYPINGTEST

#include "dl32Typing.h"
#include <iostream>
#include <typeinfo>
#include <string>

using namespace std;

int main()
{  
    typedef dl32TypeList<char , int , float , double> myList;
    
    /* TypeList indexing test */
    
    if(dl32TypeChecking<float,myList::type_at<2>>::same_type)
        cout << "ok!" << endl;
    else
        cout << "mmm... Cascao raro" << endl;
    
    /* Inheritance checking test */
    
    class Foo{};
    class FooChild : public Foo {};
    
    if(dl32TypeChecking<Foo,FooChild>::superclass_subclass)
        cout << "ok!" << endl;
    else
        cout << "mmm... Cascao raro" << endl;
    
    /* TypeList indexof test */
    
    if(myList::index_of<int>::value == 1)
        cout << "ok!" << endl;
    else
        cout << "mmm... Cascao raro" << endl;
    
    /* Type traits checking test */
    
    cout << std::boolalpha;
    cout << (bool) dl32TypeTraits<int>::hasConst << endl;
    cout << (bool) dl32TypeTraits<int*>::isPointer << endl;
    cout << (bool) dl32TypeTraits<int&>::isReference << endl;
    cout << (bool) dl32TypeTraits<int&>::isRvalue << endl;
    
    /* end */
    
    cin.sync();
    cin.get();
}

#endif