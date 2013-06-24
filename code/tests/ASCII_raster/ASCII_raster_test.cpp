/* 
 * File:   ASCII_raster_test.cpp
 * Author: Manu343726
 *
 * Created on 5 de mayo de 2013, 18:06
 */

#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_ASCII_RASTER_TEST

#include <iostream>
#include <bitset>
#include "graphics/raster.h"
#include "ascii/ascii_raster.h"

using namespace std;

/*
 * 
 */
int main()
{
    Screen screen;
    Raster raster( &screen );
    ASCII_Raster ascii_raster( &screen );
    
    screen.clear();
    //raster.DRAW_Line( point_2d{0,0} , point_2d{SCREEN_WIDTH,SCREEN_HEIGHT} );
    //raster.DRAW_Line( point_2d{SCREEN_WIDTH,SCREEN_HEIGHT-1} , point_2d{0,0} );
    
    screen.send_to_ostream( cout );
    
    for(unsigned int i = 0 ; i < SCREEN_WIDTH ; ++i )
        for(unsigned int j = 0 ; j < SCREEN_HEIGHT ; ++j )
            raster.DRAW_Pixel( i , j );
    screen.send_to_ostream( cout );
    
    screen.clear();
    //raster.DRAW_Line( point_2d{0,0} , point_2d{SCREEN_WIDTH,SCREEN_HEIGHT} );
    
    
    screen.send_to_ostream( cout );
    
    raster.DRAW_Line( point_2d{0,SCREEN_HEIGHT} , point_2d{SCREEN_WIDTH,0} );
    
    screen.send_to_ostream( cout );
    
    ascii_raster.send_to_ostream( cout );
    
    return 0;
}

#endif /* DL32TESTS_CURRENTTEST */