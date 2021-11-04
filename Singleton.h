#ifndef SINGLETON_H
#define SINGLETON_H

/*
*   FUTURE NOTE: This implementation of a singelton is not thread safe if you ever venture out that way
*/

template <typename _type>
/**
* The Singleton class defines the `GetInstance` method that serves as an
* alternative to constructor and lets clients access the same instance of this
* class over and over.
*/
class Singleton {
    /**
    * The Singleton's constructor should always be private to prevent direct
    * construction calls with the `new` operator.
    */

    protected:
        Singleton(const _type value): value_(value) {}

        static Singleton* singleton_;
        _type value_;

    public:
        /*
        * Singletons should not be cloneable.
        */
        Singleton(Singleton &other) = delete;
        
        /*
        * Singletons should not be assignable.
        */
        void operator=(const Singleton &) = delete;
        
        /*
        * This is the static method that controls the access to the singleton
        * instance. On the first run, it creates a singleton object and places it
        * into the static field. On subsequent runs, it returns the client existing
        * object stored in the static field.
        */
        static Singleton *GetInstance(const _type& value);
        
        /**
        * Finally, any singleton should define some business logic, which can be
        * executed on its instance.
        */
        void SomeBusinessLogic()
        {
            // ...
        }

        _type value() const{
            return value_;
        } 
};

template <typename _type>
Singleton<_type>* Singleton<_type>::singleton_= nullptr;

/**
* Static methods should be defined outside the class.
*/
template <typename _type>
Singleton<_type> *Singleton<_type>::GetInstance(const _type& value)
{
    /*
    * This is a safer way to create an instance. instance = new Singleton is
    * dangeruous in case two instance threads wants to access at the same time
    */
    if(singleton_ == nullptr){
        singleton_ = new Singleton(value);
    }
    return singleton_;
}   

#endif