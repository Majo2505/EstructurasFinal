#pragma once
#include <iostream>
#include <string>
using namespace std;

template <class T>
class Nodo
{
private:
	T peso;
	string origen;
	string destino;
public:
	Nodo(T peso, string origen, string destino);
	Nodo();
	~Nodo();
	T getElemento();
	string getOrigen();
	string getDestino();
	void setOrigen(string o);
	void setDestino(string d);
	void setElemento(T Elemento);
};

template<class T>
inline Nodo<T>::Nodo(T peso, string origen, string destino)
{
	this->peso = peso;
	this->origen = origen;
	this->destino = destino;
}

template<class T>
inline Nodo<T>::Nodo()
{
}

template<class T>
inline Nodo<T>::~Nodo()
{

}

template<class T>
inline T Nodo<T>::getElemento()
{
	return peso;
}

template<class T>
inline string Nodo<T>::getOrigen()
{
	return origen;
}

template<class T>
inline string Nodo<T>::getDestino()
{
	return destino;
}

template<class T>
inline void Nodo<T>::setOrigen(string o)
{
	this->origen = o;
}

template<class T>
inline void Nodo<T>::setDestino(string d)
{
	this->destino = d;
}

template<class T>
inline void Nodo<T>::setElemento(T Elemento)
{
	this->peso = Elemento;
}
