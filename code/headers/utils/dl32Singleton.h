/* 
 * File:   dl32Singleton.h
 * Author: Manu343726
 *
 * Created on 3 de mayo de 2013, 12:44
 */

#ifndef DL32SINGLETON_H
#define	DL32SINGLETON_H

#include <cstdlib>

#if defined(__GXX_EXPERIMENTAL_CXX0X) 

#define NULLPTR nullptr

#else

#define NULLPTR NULL

#endif /* C++11 */

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides a generic implementation of the singleton dessign pattern.
///
/// @tparam T Class that implements the singleton.
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class dl32Singleton
{
public:
    typedef T* PointerToInstance;
    typedef T& ReferenceToInstance;
    typedef dl32Singleton<T>* RealPointerToInstance;
private:
    dl32Singleton(const dl32Singleton&) {}
    dl32Singleton& operator=(const dl32Singleton&) {}
    
    static RealPointerToInstance _instance;
    
    static void _singleton_instance_deleter() { delete _instance; }
protected:
    dl32Singleton() {}
    virtual ~dl32Singleton() {}; //Correct polymorphic delete
public:
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets a reference to the singleton instance.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    static ReferenceToInstance instance()
    {
        if( _instance == NULLPTR)
        {
            /* Register the class at runtime-exit to avoid memory-leaks */
            atexit( _singleton_instance_deleter );
            
            _instance = new T;
        }
        
        return *(static_cast<T*>(_instance));
    }
};

/* Definition of the instance pointer */
template<typename T>
typename dl32Singleton<T>::RealPointerToInstance dl32Singleton<T>::_instance = NULLPTR;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This macro provides a "shortcut" to hide all ctors and assignments operator to performs 
///        the singleton dessing pattern implementation.
///
/// @details Put this macro in the top of your class/struct (Next to the open brace), with your class 
///          name as argumment.
///
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAKE_SINGLETON( class_name )                    \
            friend class dl32Singleton< class_name >;   \
        private:                                        \
            class_name() { _setup_instance(); }         \
            class_name(const class_name&) {}            \
            class_name& operator=(const class_name&) {} \
            ~class_name {}

#endif	/* DL32SINGLETON_H */

