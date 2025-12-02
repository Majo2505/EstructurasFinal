#pragma once
#include "Lista8.h"
#include <string>
using namespace std;

template<class T>
class Vertice
{
private:
    string elemento;        // Identificador del nodo
    Lista8<T>* adyacencia;  // Lista de vecinos

public:
    Vertice();
    ~Vertice();
    Lista8<T>& getLista();
    string getElemento() const;
    void setElemento(const string& el);
};

template<class T>
inline Vertice<T>::Vertice()
{
    elemento = "";
    adyacencia = new Lista8<T>();
}

template<class T>
inline Vertice<T>::~Vertice()
{
    if (adyacencia != nullptr)
    {
        delete adyacencia;
        adyacencia = nullptr;
    }
}

template<class T>
inline Lista8<T>& Vertice<T>::getLista()
{
    return *adyacencia;
}

template<class T>
inline string Vertice<T>::getElemento() const
{
    return elemento;
}

template<class T>
inline void Vertice<T>::setElemento(const string& el)
{
    elemento = el;
}
