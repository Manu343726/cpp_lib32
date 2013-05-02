#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_IS_DESIGN_TEST

#include "dl32Typing.h"
#include <iostream>
#include <typeinfo>
#include <string>
#include <vector>

using namespace std;

enum AppData_Type { APPDATATYPE_PERSONAL , APPDATATYPE_ESPACIOS , APPDATATYPE_NOTYPE_CENTINEL /* ojo, éste siempre debe ser el último */ };

class Personal
{
    enum { type = APPDATATYPE_PERSONAL };
    
    /* ... */
};

class Espacio
{
    enum { type = APPDATATYPE_PERSONAL };
        
    /* ... */
};

class ResultadosPersonal { ResultadosPersonal( const Personal&) { /* ... */ } };
class ResultadosEspacios { ResultadosEspacios( const Espacio&) { /* ... */ } };

class CriteriosPersonal { /* ... */ };
class CriteriosEspacios { /* ... */ };

typedef dl32TypeList<Personal , Espacio> TiposDatos;
typedef dl32TypeList<ResultadosPersonal , ResultadosEspacios> TiposResultados;
typedef dl32TypeList<CriteriosPersonal , CriteriosEspacios> TiposCriterios;

struct VectorWrapper
{
    virtual ~VectorWrapper();
};

template<typename T>
struct GenericVectorWrapper : public VectorWrapper
{
    std::vector<T> vector;
};

class SearchEngine
{
private:
    std::vector<VectorWrapper*> _data;
    
public:
    SearchEngine()
    {
        for(unsigned int i = 0 ; i < APPDATATYPE_NOTYPE_CENTINEL ; ++i)
            _data.push_back( new GenericVectorWrapper< TiposDatos::type_at<i> >() );
    }
    
    ~SearchEngine()
    {
        for(unsigned int i = 0 ; i < APPDATATYPE_NOTYPE_CENTINEL ; ++i)
            delete _data[i];
    }
    
    template<T> //T es el tipo del dato que quieres buscar (Personal o pacientes)
    TiposResultados::type_at<T::type>&& search(TiposCriterios::type_at<T::type>)
    {
        std::vector<T>* data; //Puntero al vector correspondiente;
        data = dynamic_cast<vector<T>*>(_data[T::type]); //Intentamos "sonsacárselo" al array de vectores.
        
        if( data )
                return TiposResultados::type_at<T::type>( data[0][0] );//Realmente tendrías en cuenta los criterios de búsqueda. Devuelvo el primero por simplicidad en el ejemplo.
        else
            throw; //Ésto jamás debería pasar, salvo que por algún tipo de casualidad hayas usado un tipo que no es de los nuestros, pero da la casualidad de que tiene un miembro "type"
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
 * Y ya está! Nada de nuevas clases idénticas a las que ya hay que básicamente son un wrapper de un vector (ArrayList en java)
 */

#endif
