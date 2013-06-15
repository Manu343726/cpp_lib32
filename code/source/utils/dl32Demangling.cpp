#include "dl32Demangling.h"

#if defined( _MSC_VER )

#include <Dbghelp.h>
    
const unsigned int UNDECORATED_NAME_LENGHT = 512; //No creo que haya nombres mucho más largos

//MSVC demangling implementation
std::string dl32Demangle(const std::string& name)
{
    char output_buffer[UNDECORATED_NAME_LENGHT];

    if( !FAILED( UnDecorateSymbolName( name.c_str() , output_buffer , UNDECORATED_NAME_LENGHT , UNDNAME_COMPLETE ) ) )
    {
        return std::string( output_buffer );
    }
    else
        return std::string( name );
}
#endif /* MSVC */

#if defined ( __GNUC__ )

#include <cxxabi.h>

//GCC demangling implementation
std::string dl32Demangle(  const std::string& name )
{ 
    int status;
    
    return std::string( abi::__cxa_demangle( name.c_str() , 0 , 0 , &status ) );
}
#endif /* GCC */
