/* 
 * File:   dl32Window_test.cpp
 * Author: manu343726
 *
 * Created on 29 de junio de 2013, 15:29
 */

#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_WINDOWTEST

#include "dl32Window.h"

#include <iostream>

int main()
{
    dl32Window window("cpp_lib32 portable window test");
    
    window.title();
    
    return 0;
}

#endif

