#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_IS_DESIGN_TEST

#include "dl32Typing.h"
#include <iostream>
#include <typeinfo>
#include <string>
#include <vector>

using namespace std;

enum AppData_Type { APPDATATYPE_PERSONAL = 0 , APPDATATYPE_ESPACIOS = 1 , APPDATATYPE_NOTYPE_CENTINEL = 2 /* ojo, éste siempre debe ser el último */ };

class Personal
{
public:
    enum { type = APPDATATYPE_PERSONAL };
    
    /* ... */
};

class Espacio
{
public:
    enum { type = APPDATATYPE_ESPACIOS };
        
    /* ... */
};

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
struct VectorPusher<APPDATATYPE_NOTYPE_CENTINEL>
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
        data = dynamic_cast<GenericVectorWrapper<T>*>(_data[T::type]); //Intentamos "sonsacárselo" al array de vectores.
        
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
        for(unsigned int i = 0 ; i < APPDATATYPE_NOTYPE_CENTINEL ; ++i)
            delete _data[i];
    }
    
    template<typename T>
    void addData( T data )
    {
        get_vector<T>().push_back( data );
    }
    
    template<typename T> //T es el tipo del dato que quieres buscar (Personal o pacientes)
    TiposResultados::type_at<T::type> search(TiposCriterios::type_at<T::type>)
    {   /* Si el compilador es listo, RVO */
        
        cout << "Searching items of type " << typeid( T ).name() << " using criteria of type " << typeid( TiposCriterios::type_at<T::type> ).name() << " ...." << endl;
        cout << "Returning results of type " << typeid( TiposResultados::type_at<T::type> ).name() << " ...." << endl;
        
        return TiposResultados::type_at<T::type>( get_vector<T>().at(0) );//Realmente tendrías en cuenta los criterios de búsqueda. Devuelvo el primero por simplicidad en el ejemplo.
    }
};

/* De esta manera, lo único que tienes que hacer al añadir un nuevo tipo de dato al sistema, es lo siguiente:
 *
 * 1º Añadir un valor más a la enumeración "AppDataType" (Ojo, siempre antes que el valor centinela).
 * 2º Añadir un atributo a la nueva clase/tipo de dato (Preferiblemete como un enum, para que sea evaluable en tiempo de compilación) llamado "type" con el nuevo valor de la enumeración.
 * 3º Añadir el nuevo tipo a la lista "TiposDatos" (En el mismo orden que en la enumeración).
 * 4º Añadir el tipo de criterio a la lista "TiposCriterios" (En el mismo orden que en la enumeración).
 * 5º Añadir el tipo de resultado a la lista "TiposResultados" (En el mismo orden que en la enumeración).
 * 
 * NOTA: Las tres últimas estoy pensando la manera de automatizarlas.
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
