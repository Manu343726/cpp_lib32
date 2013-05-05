/* 
 * File:   ascii_raster.h
 * Author: Manu343726
 *
 * Created on 6 de mayo de 2013, 0:22
 */

#ifndef ASCII_RASTER_H
#define	ASCII_RASTER_H

#include <windows.h>
#include "screen.h"
#include <iostream>

using namespace std;

class ASCII_Raster
{
private:
    Screen* _screen;
    
    CHAR_INFO console_buffer[ASCII_RASTER_CONSOLEBUFFER_WIDTH * ASCII_RASTER_CONSOLEBUFFER_HEIGHT];
    HANDLE _stdout_handle , _console_buffer_handle;
    
    
    static char _char_pixels_to_ASCII( unsigned int row , unsigned int column , character_code char_code )
    {
        if( char_code > 15 ) cout << std::bitset<5>(char_code) << endl;
        
        switch( char_code )
        {
            case  0 /* 0000 */ : return ' ';
            case  1 /* 0001 */ : return ',';
            case  2 /* 0010 */ : return '.';
            case  3 /* 0011 */ : return '_';
            case  4 /* 0100 */ : return '\'';
            case  5 /* 0101 */ : return '|';
            case  6 /* 0110 */ : return '/';
            case  7 /* 0111 */ : return '/';
            case  8 /* 1000 */ : return '`';
            case  9 /* 1001 */ : return '\\';
            case 10 /* 1010 */ : return '|';
            case 11 /* 1011 */ : return '\\';
            case 12 /* 1100 */ : return '¨';
            case 13 /* 1101 */ : return '\\';
            case 14 /* 1110 */ : return '7';
            case 15 /* 1111 */ : return '#';
            default: return 'X';
        }
    }
public:
    ASCII_Raster( Screen* screen ) : _screen( screen ) {}
    
    char char_pixels_to_ascii( unsigned int char_row , unsigned int char_column ) { return _char_pixels_to_ASCII( char_row , char_column , _screen->get_char( char_row ,char_column ) ); }
    
    void send_to_ostream(ostream& os)
    {
        for(unsigned int i = 0 ; i < ASCII_RASTER_CONSOLEBUFFER_WIDTH ; ++i )
        {
            for(unsigned int j = 0 ; j < ASCII_RASTER_CONSOLEBUFFER_HEIGHT ; ++j )
                os << char_pixels_to_ascii( i , j );
            
            os << endl;
        }
    }
};


#endif	/* ASCII_RASTER_H */

