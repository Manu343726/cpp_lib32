/* 
 * File:   dl32Singleton.h
 * Author: Manu343726
 *
 * Created on 3 de mayo de 2013, 12:44
 */

#ifndef DL32SINGLETON_H
#define	DL32SINGLETON_H

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides an abstract singleton implementation.
///
/// @tparam T Class that will implement the singleton dessign pattern.
/// @author	Manu343726
///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class dl32Singleton
{
private:
    dl32Singleton<T>* _instance;
    
    void _singletonDeleter() { delete _instance; }

public:
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gets a reference to the singleton instance.
    ///
    /// @author	Manu343726
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    T& instance()
    {
        if( !_instance )
        {
            atexit( _singletonDeleter() );
            _instance = new T;
        }
        
        return *(static_cast<T*>(_instance));
    }
};

#endif	/* DL32SINGLETON_H */

