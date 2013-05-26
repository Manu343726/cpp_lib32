
#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_NEWMATH

#include "dl32NewMath.h"

#include <iostream>
using namespace std;


int main()
{
   dl32Point2Df p1(1,1) , p2(2,2) , p3(3,3);
   dl32Vector2Df v1 , v2 , v3;
   
   p3 = p2 + p3;
   
   
   return 0;
}

#endif /* DL32TESTS_CURRENTTEST */