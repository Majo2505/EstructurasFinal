#pragma once
#include "Tripla.h"

template<class T>
class Lista8
{
private:
    Tripla<T>* primer;
    Tripla<T>* ultimo;

public:
    Lista8();
    ~Lista8();

    bool insertar_Final(T el);
    void mostrar();
    // 1. Fusión O(1): Pega una lista al final de otra sin recorrerla
    void absorberLista(Lista8<T>& otraLista);
};

template<class T>
inline Lista8<T>::Lista8()
{
    primer = NULL;
    ultimo = NULL;
}

template<class T>
inline Lista8<T>::~Lista8()
{
    Tripla<T>* aux = primer;
    while (aux != NULL)
    {
        Tripla<T>* siguiente = aux->getSiguiente();
        delete aux;
        aux = siguiente;
    }
    primer = NULL;
    ultimo = NULL;
}

template<class T>
inline bool Lista8<T>::insertar_Final(T el)
{
    if (ultimo == NULL)
    {
        ultimo = new Tripla<T>(NULL, el, NULL);
        primer = ultimo;
    }
    else
    {
        Tripla<T>* nuevo = new Tripla<T>(ultimo, el, NULL);
        ultimo->setTriplaSig(nuevo);
        ultimo = nuevo;
    }
    return true;
}

template<class T>
inline void Lista8<T>::mostrar()
{
    Tripla<T>* aux = primer;
    while (aux != NULL)
    {
        cout << " " << aux->getElemento();
        aux = aux->getSiguiente();
    }
    cout << endl;
}

// --- FUSIÓN RÁPIDA (O(1)) ---
template<class T>
inline void Lista8<T>::absorberLista(Lista8<T>& otraLista)
{
    if (otraLista.primer == NULL) return;

    if (this->primer == NULL)
    {
        this->primer = otraLista.primer;
        this->ultimo = otraLista.ultimo;
    }
    else
    {
        this->ultimo->setTriplaSig(otraLista.primer);
        otraLista.primer->setTriplaAnt(this->ultimo);
        this->ultimo = otraLista.ultimo;
    }

    otraLista.primer = NULL;
    otraLista.ultimo = NULL;
}
