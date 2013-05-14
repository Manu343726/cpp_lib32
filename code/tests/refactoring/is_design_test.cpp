#include "dl32TestConfig.h"

#if DL32TEST_DEBUGTEST == DL32TEST_REFACTORING_IS_DESIGN_TEST

#include "dl32Typing.h"
#include <iostream>
#include <typeinfo>
#include <string>
#include <vector>

#ifdef __GNUC__
#include<cxxabi.h>

string demangle( const char* name)
{ 
    int status;
    
    return string( abi::__cxa_demangle( name , 0 , 0 , &status ) );
}
#else
string demangle( const char* name)
{ 
    return string( name );
}
#endif
using namespace std;

class Personal{ /* ... */ };
class Espacio{ /* ... */ };

struct ResultadosPersonal { ResultadosPersonal( const Personal&) { /* ... */ } };
struct ResultadosEspacios { ResultadosEspacios( const Espacio&) { /* ... */ } };

class CriteriosPersonal { /* ... */ };
class CriteriosEspacios { /* ... */ };

typedef dl32TypeList<Personal , Espacio> TiposDatos;
typedef dl32TypeList<ResultadosPersonal , ResultadosEspacios> TiposResultados;
typedef dl32TypeList<CriteriosPersonal , CriteriosEspacios> TiposCriterios;

struct VectorWrapper
{
    virtual ~VectorWrapper() {}
};

template<typename T>
struct GenericVectorWrapper : public VectorWrapper
{
    std::vector<T> vector;
    ~GenericVectorWrapper() {};
};

template<int type_index>
struct VectorPusher
{
    static void push_back(std::vector<VectorWrapper*>& data)
    {
        cout << "Pushing type " << type_index << " ..." << endl;
        data.push_back( new GenericVectorWrapper< TiposDatos::type_at< type_index > >() );
        
        VectorPusher<type_index+1>::push_back(data);
    }
};

template<>
struct VectorPusher<TiposDatos::size>
{
    static void push_back(std::vector<VectorWrapper*>& data) { cout << "end pushing" << endl; }
};

class SearchEngine
{
private:
    std::vector<VectorWrapper*> _data;
    
    template<typename T>
    std::vector<T>& get_vector()
    {
        GenericVectorWrapper<T>* data; //Puntero al vector correspondiente;
        data = dynamic_cast<GenericVectorWrapper<T>*>(_data[TiposDatos::index_of<T>::value]); //Intentamos "sonsacárselo" al array de vectores.
        
        if( data )
            return data->vector;
        else
        {
            cout << "dynamic_cast error!!!" << endl;
            throw; //Ésto jamás debería pasar, salvo que por algún tipo de casualidad hayas usado un tipo que no es de los nuestros, pero da la casualidad de que tiene un miembro "type"
        }
    }
    
public:
    SearchEngine()
    {
        VectorPusher<0>::push_back(_data);
    }
    
    ~SearchEngine()
    {
        for(unsigned int i = 0 ; i < TiposDatos::size ; ++i)
            delete _data[i];
    }
    
    template<typename T>
    void addData( T data )
    {
        get_vector<T>().push_back( data );
    }
    
    template<typename T> //T es el tipo del dato que quieres buscar (Personal o pacientes)
    TiposResultados::type_at<TiposDatos::index_of<T>::value> search(typename TiposCriterios::type_at<TiposDatos::index_of<T>::value>)
    {   /* Si el compilador es listo, RVO */
        
        cout << "Searching items of type " << demangle( typeid( T ).name() ) << " using criteria of type " << demangle( typeid( TiposCriterios::type_at<TiposDatos::index_of<T>::value> ).name() ) << " ...." << endl;
        cout << "Returning results of type " << demangle( typeid( TiposResultados::type_at<TiposDatos::index_of<T>::value> ).name() ) << " ...." << endl;
        
        return TiposResultados::type_at<TiposDatos::index_of<T>::value>( get_vector<T>().at(0) );//Realmente tendrías en cuenta los criterios de búsqueda. Devuelvo el primero por simplicidad en el ejemplo.
    }
};

/* De esta manera, lo único que tienes que hacer al añadir un nuevo tipo de dato al sistema, es lo siguiente:
 *
 * 1º Añadir el nuevo tipo a la lista "TiposDatos".
 * 2º Añadir el nuevo tipo de criterio a la lista "TiposCriterios".
 * 3º Añadir el nuevo tipo de resultado a la lista "TiposResultados".
 * 
 * NOTA: Los tipos deben meterse en las tres listas en el mismo orden. MUY IMPORTANTE!
 * 
 * Y ya está! Nada de nuevas clases idénticas a las que ya hay que básicamente son un wrapper de un vector (ArrayList en java)
 */

int main()
{   
    SearchEngine engine;
   
    engine.addData(Espacio());
    engine.addData(Personal());
    
    /* Como ves, los tipos se corresponden perfectamente: */
    
    ResultadosEspacios rs_esp = engine.search<Espacio>(CriteriosEspacios()); //OK: El tipo de criterio correspondiente a Espacio es CriteriosEspacios
    
    ResultadosPersonal rs_pers = engine.search<Personal>(CriteriosPersonal()); //OK: El tipo de criterio correspondiente a Espacio es CriteriosEspacios
    
    // Resultados_espacios rs_esp_2 = engine.search<Espacio>(CriteriosPersonal()); //ERROR: El tipo de criterio correspondiente a Personal es "CriteriosPersonal", no "CriteriosEspacios" (ERROR DE COMPILACION)
    
    // ResultadosEspacios rs_pers_2 = engine.search<Personal>(CriteriosPersonal()); //ERROR: El resultado de la búsqueda es de tipo "ResultadosPersonal", no "ResultadosEspacios" (ERROR DE COMPILACION))

    cout << "exiting without throw (Everything is ok)";
}

#endif
