/* 
 * File:   raster.h
 * Author: Manu343726
 *
 * Created on 4 de mayo de 2013, 20:54
 */

#ifndef RASTER_H
#define	RASTER_H

#include "screen.h"
#include "dl32Math.h"
#include <iostream>
using namespace std;

typedef dl32Point2D point_2d;
typedef dl32AABB2D box;

struct Raster
{
private:
    Screen* _screen;
    const box _viewport;
    
    void _draw_line( float x0 , float y0 , float x1 , float y1 )
    {cout << __FUNCTION__ << endl;
        /* Bresenham's algorithm (No soporta antialiasing, que para el caso es perfecto. Y es rápido) */
        
        float dx = abs( x1 - x0 );
        float dy = abs( y1 - y0 );
        float error = dx - dy;
        float sx = x0 < x1 ? 1 : -1;
        float sy = y0 < y1 ? 1 : -1;
        
        _screen->set_pixel_at( (unsigned int) y0 , (unsigned int) x0 , true);
        
        while( x0 != x1 || y0 != y1 )
        {                 
            float error_doubled = error * 2;
            
            if( error_doubled > -dy )
            {
                error -= dy;
                x0 += sx;
            }
            
            if( error_doubled < dx )
            {
                error += dx;
                y0 += sy;
            }
            
            _screen->set_pixel_at( (unsigned int) y0 , (unsigned int) x0 , true);
        }
    }
public:
    Raster(Screen* screen) : _screen(screen) , _viewport( 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT ) { _screen->clear(); }
    
    inline void DRAW_Pixel( unsigned int x , unsigned int y )
    {
        if( _viewport.BelongTo( x , y ) )
            _screen->set_pixel_at( y , x , true);
    }
    
    inline void DRAW_Pixel( point_2d pixel ) { DRAW_Pixel( (unsigned int) pixel.x , (unsigned int) pixel.y ); }
    
    void DRAW_Line( point_2d begin , point_2d end )
    {
        box::ClippingResult results = _viewport.clip( begin , end ); cout << __FUNCTION__ << endl;
        
        if( !results.rejected )
            _draw_line( results.begin.x , results.begin.y , results.end.x , results.end.y );
    }
};

#endif	/* RASTER_H */

