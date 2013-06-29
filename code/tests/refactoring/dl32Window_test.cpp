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
    bool visible = window.visible();
    
    window.hide();
    visible = window.visible();
    
    window.show();
    visible = window.visible();
    
    window.hide();
    visible = window.visible();
    
    window.show();
    visible = window.visible();
    
    const float PI = 3.141592654;
    const float step = 0.0001;
    const dl32Point2Di center = dl32Point2Di(100,100);
    const unsigned int radious = 50;
    
    for(float angle = 0 ; angle <= 2*PI ; angle += step)
    {
        window.position( center.x + (int)(std::cos( angle )*radious) , center.y + (int)(std::sin( angle )*radious) );
        
        window.title( window.position().to_string() );
        
        if( angle + step >= 2*PI)
            angle = 0;
    }
    
    return 0;
}

#endif

