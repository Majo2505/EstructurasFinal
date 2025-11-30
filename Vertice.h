#pragma once
#include "Lista8.h"

template<class T>
class Vertice
{

private:
	bool marca;
	string elemento;
	Lista8<T>* l8;
	string padre;
	int distancia;
public:
	Vertice();
	~Vertice();
	Lista8<T>& getLista();
	bool getMarca();
	string getPadre();
	string getElementoV();
	int getDistancia();
	void setDistancia(int dis);
	void setElemento(string el);
	void setMarca(bool m);
	void setPadre(string p);

};

template<class T>
inline Vertice<T>::Vertice()
{
	padre = -1;
	marca = false;
	this->l8 = new Lista8<T>;
	distancia = 999;
}

template<class T>
inline Vertice<T>::~Vertice()
{
	if (l8 != nullptr)
	{
		delete l8;
		l8 = nullptr;
	}
}

template<class T>
inline Lista8<T>& Vertice<T>::getLista()
{
	return *l8;
}

template<class T>
inline bool Vertice<T>::getMarca()
{
	return marca;
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
inline int Vertice<T>::getDistancia()
{
	return distancia;
}

template<class T>
inline void Vertice<T>::setDistancia(int dis)
{
	this->distancia = dis;
}

template<class T>
inline void Vertice<T>::setMarca(bool m)
{
	this->marca = m;
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

