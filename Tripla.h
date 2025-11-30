#pragma once
#include <iostream>
#include <string>
using namespace std;

template <class T>
class Tripla
{
private:
	Tripla<T>* ant;
	Tripla<T>* sig;
	T el;
	int peso;
public:
	Tripla(Tripla<T>* ant, T el, Tripla<T>* sig);
	~Tripla();
	T getElemento();
	Tripla<T>* getSiguiente();
	Tripla<T>* getAnterior();
	int getPeso();
	void setPeso(int peso);
	void setTriplaSig(Tripla<T>* t);
	void setTriplaAnt(Tripla<T>* t);
	void setElemento(T Elemento);

};

template<class T>
inline Tripla<T>::Tripla(Tripla<T>* ant, T el, Tripla<T>* sig)
{
	this->ant = ant;
	this->el = el;
	this->sig = sig;
}

template<class T>
inline Tripla<T>::~Tripla()
{
	if (sig != NULL)
	{
		delete sig;
	}
}

template<class T>
inline T Tripla<T>::getElemento()
{
	return el;
}

template<class T>
inline Tripla<T>* Tripla<T>::getSiguiente()
{
	return sig;
}

template<class T>
inline Tripla<T>* Tripla<T>::getAnterior()
{
	return ant;
}

template<class T>
inline int Tripla<T>::getPeso()
{
	return peso;
}

template<class T>
inline void Tripla<T>::setPeso(int peso)
{
	this->peso = peso;
}

template<class T>
inline void Tripla<T>::setTriplaSig(Tripla<T>* t)
{
	this->sig = t;
}

template<class T>
inline void Tripla<T>::setTriplaAnt(Tripla<T>* t)
{
	this->ant = t;
}

template<class T>
inline void Tripla<T>::setElemento(T Elemento)
{
	this->el = Elemento;
}
