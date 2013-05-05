/* 
 * File:   screen.h
 * Author: Manu343726
 *
 * Created on 4 de mayo de 2013, 21:00
 */

#ifndef SCREEN_H
#define	SCREEN_H

#include<cstdint>
#include <functional>
#include <bitset>
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
#include "dl32ConsoleColor.h"

#ifdef assert
#undef assert
#endif

#define ASSERT_PRINT_ONLY_ERRORS TRUE

#if ASSERT_PRINT_ONLY_ERRORS
#define ASSERT_PASSED_ACTION 
#else
#define ASSERT_PASSED_ACTION cout << assert_data(x) << "OK!" << endl
#endif
        
#define assert_data(x) __FILE__ << ", in function " << __FUNCTION__  << " (line " <<  __LINE__ << "):" << " '" << #x << "' --> "
#define assert(x) (x) ? ASSERT_PASSED_ACTION : cout << push_style << dl32ChangeForegroundColor( dl32ConsoleColor::RED ) << assert_data(x) << "ERROR (Cascao raro)" << pop_style << endl 
#endif /* TESTING_USEASSERT */

const unsigned int CHARCODE_PIXELS_MAX = 64;
typedef uint64_t character_code;

const unsigned int ASCII_RASTER_PIXELS_PER_CHAR_WIDTH  = 2;
const unsigned int ASCII_RASTER_PIXELS_PER_CHAR_HEIGHT = 2;

const unsigned int ASCII_RASTER_PIXELS_PER_CHAR_HEIGHT_MAX = CHARCODE_PIXELS_MAX / ASCII_RASTER_PIXELS_PER_CHAR_WIDTH;
const unsigned int ASCII_RASTER_CONSOLEBUFFER_WIDTH  = 20;
const unsigned int ASCII_RASTER_CONSOLEBUFFER_HEIGHT = 20; //Square console screen
const unsigned int SCREEN_WIDTH  = ASCII_RASTER_PIXELS_PER_CHAR_WIDTH * ASCII_RASTER_CONSOLEBUFFER_WIDTH;
const unsigned int SCREEN_HEIGHT = ASCII_RASTER_PIXELS_PER_CHAR_HEIGHT * ASCII_RASTER_CONSOLEBUFFER_HEIGHT;

const unsigned int PIXELS_PER_GROUP   = ASCII_RASTER_PIXELS_PER_CHAR_WIDTH;
typedef std::bitset<PIXELS_PER_GROUP> pixels_group;
typedef std::tuple<pixels_group& , unsigned int> pixels_group_address;

const unsigned int PIXEL_GROUPS_COUNT = ASCII_RASTER_CONSOLEBUFFER_WIDTH;
typedef pixels_group screen_row[PIXEL_GROUPS_COUNT];
typedef screen_row screen_buffer[SCREEN_HEIGHT];

class Screen
{
private:
    screen_buffer _buffer;
    
    static bool discard( unsigned int row , unsigned int column ) { return !( row < SCREEN_HEIGHT && column < SCREEN_WIDTH ); }
public:
    Screen() 
    { 
        static_assert( ASCII_RASTER_PIXELS_PER_CHAR_HEIGHT <= ASCII_RASTER_PIXELS_PER_CHAR_HEIGHT_MAX , "Character codes are packaged in 64 bit integers. So character width x character height must be less or equal to 64." ); 
        static_assert( ( ASCII_RASTER_PIXELS_PER_CHAR_WIDTH & ( ASCII_RASTER_PIXELS_PER_CHAR_WIDTH - 1 ) ) == 0 , "Characters must have a power-of-two pixels width" );
    } 
    
    int get_pixel_at( unsigned int row , unsigned int column )
    {
        return _buffer[row][column/PIXELS_PER_GROUP][PIXELS_PER_GROUP - (column % PIXELS_PER_GROUP)] /* los pixels van de izquierda a derecha, los indices de los bits al reves, de ahí la resta */;
    }
    void set_pixel_at( unsigned int row , unsigned int column , bool value )
    {
        int bit_index = (column % PIXELS_PER_GROUP);
        assert( bit_index < PIXELS_PER_GROUP );
        _buffer[row][column/PIXELS_PER_GROUP][PIXELS_PER_GROUP - (column % PIXELS_PER_GROUP)] = value;
    }
    
    //row y column en coordenadas de caracteres, no de pixels
    character_code get_char( unsigned int row , unsigned int column)
    {
        character_code code = 0x0;
        
        /* Los códigos de caracter indican que pixels están activados (blancos) dentro de dicho caracter (Entendiendo por caracter la
         * zona del screen que corresponde al caracter). Las dimensiones de dicha zona están definidas por las constantes 
         * ASCII_RASTER_PIXELS_PER_CHAR_WIDTH y ASCII_RASTER_PIXELS_PER_CHAR_HEIGHT
         */
        
        //Recorremos fila a fila la zona de la pantalla que se corresponde con el caracter, extrallendo en cada fila los bits (pixels) correspondientes:)
        for( unsigned int i = 0 ; i < ASCII_RASTER_PIXELS_PER_CHAR_HEIGHT ; ++i )
        {
            code |= _buffer[row][column/PIXELS_PER_GROUP].to_ulong() << ( ASCII_RASTER_PIXELS_PER_CHAR_HEIGHT - i - 1 ); //Los códigos de las filas van de izquierda a derecha. La primera fila en la zona más significativa, la última en la menos significativa.
        }
        
        return code;
    }
    
    void clear()
    {
        cout << __FUNCTION__ << endl; 
        
        for( auto i = 0 ; i < SCREEN_HEIGHT ; ++i )
            for( auto j = 0 ; j < PIXEL_GROUPS_COUNT ; ++j )
                _buffer[i][j] = 0x0;
    }
    
    void send_to_ostream(ostream& os , bool raw_mode = false , char white = '#' , char black = '.' )
    {
        if( !raw_mode )
        {
            for(auto i = 0 ; i < SCREEN_HEIGHT ; ++i )
            {            
                for( auto j = 0 ; j < SCREEN_WIDTH ; ++j)
                    if( get_pixel_at( j , i ))
                        os << white;
                    else
                        os << black;

                os << endl;
            }
        
            os << endl;
        }
        else
        {
            for( auto i = 0 ; i < SCREEN_HEIGHT ; ++i )
            {
                for( auto j = 0 ; j < PIXEL_GROUPS_COUNT ; ++j )
                    os << std::bitset<PIXELS_PER_GROUP>( _buffer[i][j] ) << " ";
                
                os << endl;
            }
        }
    }
};

#endif	/* SCREEN_H */

