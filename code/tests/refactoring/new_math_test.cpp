
#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_NEWMATH

#include "dl32NewMath.h"

#include <iostream>
using namespace std;

struct A{};

int main()
{
   dl32Point2Df p1(1,1) , p2(2,2) , p3(3,3);
   dl32Vector2Df v1(1,1) , v2(2,2) , v3(3,3);
   dl32Vector2D<bool> vbool1, vbool2, vbool3;
   //dl32Vector2D<A> vcabum1 , vcabum2 , vcabum3;
   
   p3 = p2 + p3;
   v1 = v2 + v3*3;
   
   return 0;
}

#endif /* DL32TESTS_CURRENTTEST */