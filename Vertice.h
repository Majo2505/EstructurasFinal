#pragma once
#include "Lista8.h"
#include <string> 
using namespace std;

template<class T>
class Vertice
{
private:
	string elemento;
	// Representa la lista de vecinos conectados a este vértice.
	Lista8<T>* adyacencia;
	string padre;

public:
	Vertice();
	~Vertice();

	// Devuelve la referencia a la lista de adyacencia
	Lista8<T>& getLista();

	string getPadre();
	string getElementoV();

	void setElemento(string el);
	void setPadre(string p);
};

template<class T>
inline Vertice<T>::Vertice()
{
	padre = ""; // Inicializamos padre como cadena vacía

	// Instanciamos la lista de adyacencia
	this->adyacencia = new Lista8<T>;
}

template<class T>
inline Vertice<T>::~Vertice()
{
	if (adyacencia != nullptr)
	{
		delete adyacencia; // Borramos la lista de memoria
		adyacencia = nullptr;
	}
}

// CORRECCIÓN AQUÍ: Agregado <T> después de Lista8
template<class T>
inline Lista8<T>& Vertice<T>::getLista()
{
	return *adyacencia;
}

template<class T>
inline string Vertice<T>::getPadre()
{
	return padre;
}

template<class T>
inline string Vertice<T>::getElementoV()
{
	return elemento;
}

template<class T>
inline void Vertice<T>::setPadre(string p)
{
	this->padre = p;
}

template<class T>
inline void Vertice<T>::setElemento(string el)
{
	this->elemento = el;
}