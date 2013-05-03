#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_TYPINGTEST

#include "dl32Typing.h"
#include <iostream>
#include <typeinfo>
#include <string>

using namespace std;

#define TESTING_USEASSERT FALSE
#define TESTING_WAIT_AT_END FALSE

#define EXPAND(x) x

#if TESTING_USEASSERT
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <assert.h>
#else
#define assert_data(x) __FILE__ << ", in function " << __FUNCTION__  << " (line " <<  __LINE__ << "):" << " '" << #x << "' --> "
#define assert(x) (x) ? cout << assert_data(x) << "OK!" << endl : cout << assert_data(x) << "ERROR (Cascao raro)" << endl 
#endif /* TESTING_USEASSERT */

int main()
{  
    typedef dl32TypeList<char , int , float , double> myList;
    
    /* TypeList indexing test */
    
    assert( (dl32TypeChecking<float,myList::type_at<2>>::same_type) );
    
    /* Inheritance checking test */
    
    class Foo{};
    class FooChild : public Foo {};
    
    assert( (dl32TypeChecking<Foo,FooChild>::superclass_subclass) );
    
    /* TypeList indexof test */
    
    assert( myList::index_of<int>::value == 1 );
    
    /* Type traits checking test */
    
    //HasConst tests:
    assert( !dl32TypeTraits<int>::hasConst        );
    assert(  dl32TypeTraits<const int>::hasConst  );
    assert(  dl32TypeTraits<const int*>::hasConst );
    assert(  dl32TypeTraits<const int&>::hasConst );
    
    //IsPointer tests:
    assert( !dl32TypeTraits<int>::isPointer        );
    assert( !dl32TypeTraits<const int>::isPointer  );
    assert(  dl32TypeTraits<const int*>::isPointer );
    assert( !dl32TypeTraits<const int&>::isPointer );
    
    //IsReference tests:
    assert( !dl32TypeTraits<int>::isReference        );
    assert( !dl32TypeTraits<const int>::isReference  );
    assert( !dl32TypeTraits<const int*>::isReference );
    assert(  dl32TypeTraits<const int&>::isReference );
    
    /* end */
#if TESTING_WAIT_AT_END
    cin.sync();
    cin.get();
#endif
}

#endif