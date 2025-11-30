#pragma once
#include "Nodo.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
using namespace std;

template <class T>
class HEAP
{
private:
	Nodo<int>* vec[60];
	int ultHoja;
public:
	HEAP();
	~HEAP();
	int getUltHoja();
	bool insertar(int elemento, string origen, string destino);
	bool recuperarAbajoArriba(int i);
	bool recuperarArribaAbajo(int i);
	Nodo<int>* eliminarMenor();
	void mostrar();
	bool leerArchivo();
	T* heapsort();
	void insertarALazar();

};

template<class T>
inline HEAP<T>::HEAP()
{
	ultHoja = 0;
	for (int i = 0; i < 60; ++i)
	{
		vec[i] = nullptr;
	}
}

template<class T>
inline HEAP<T>::~HEAP()
{
	for (int i = 1; i <= ultHoja; ++i)
	{
		delete vec[i];
		vec[i] = nullptr;
	}
}

template<class T>
inline int HEAP<T>::getUltHoja()
{
	return ultHoja;
}

template<class T>
inline bool HEAP<T>::insertar(int elemento, string origen, string destino)
{
	bool res = true;
	if (ultHoja == 0)
	{
		ultHoja = 1;
		vec[ultHoja] = new Nodo<int>(elemento, origen, destino);
	}
	else
	{
		ultHoja = ultHoja + 1;
		vec[ultHoja] = new Nodo<int>(elemento, origen, destino);
		res = recuperarAbajoArriba(ultHoja);
	}
	return res;
}

template<class T>
inline bool HEAP<T>::recuperarAbajoArriba(int i)
{
	bool res = true;
	int padre = i / 2;
	if (vec[padre] != NULL && vec[i] != NULL)
	{
		if (vec[i]->getElemento() < vec[padre]->getElemento() && padre >= 1)
		{
			swap(vec[i], vec[padre]);
			res = recuperarAbajoArriba(padre);
		}
	}
	return res;
}

template<class T>
inline bool HEAP<T>::recuperarArribaAbajo(int i)
{
	int izq = 2 * i;
	int der = 2 * i + 1;
	bool res = true;
	if (izq <= ultHoja && vec[izq] != NULL && vec[i] != NULL) //TIENE HIJO IZQ
	{
		if (der <= ultHoja && vec[der] != NULL && vec[der]->getElemento() < vec[izq]->getElemento()) // TIENE H DERECHO y es menor q el izq
		{
			swap(vec[der], vec[i]);
			res = recuperarArribaAbajo(der);
		}
		else
		{
			if (vec[i]->getElemento() > vec[izq]->getElemento()) //el izq es menor 
			{
				swap(vec[izq], vec[i]);
				res = recuperarArribaAbajo(izq);
			}
		}
	}
	return res;
}

template<class T>
inline Nodo<int>* HEAP<T>::eliminarMenor()
{
	if (ultHoja == 0)
	{
		return NULL;
	}
	Nodo<int>* menor = vec[1];
	vec[1] = vec[ultHoja];
	vec[ultHoja] = nullptr;
	ultHoja--;

	if (ultHoja > 0)
	{
		recuperarArribaAbajo(1);
	}
	return menor;
}

template<class T>
inline void HEAP<T>::mostrar()
{
	for (int i = 1; i <= ultHoja; ++i)
	{
		if (vec[i] != nullptr)
		{
			cout << vec[i]->getElemento() << " ";
		}
	}
	cout << endl;
}

template<class T>
inline bool HEAP<T>::leerArchivo()
{
	bool res = true;
	ifstream archivo("archivo");
	if (!archivo.is_open())
	{
		res = false;
	}
	else
	{
		string linea;
		while (getline(archivo, linea))
		{
			istringstream ss(linea);
			T palabra;
			while (ss >> palabra)
			{
				insertar(palabra);
			}
		}
	}
	return res;
}

template<class T>
inline T* HEAP<T>::heapsort()
{
	T* vec = new T[11000];
	int tam = ultHoja;
	int i = 1;
	while (ultHoja > 0 && i <= tam)
	{
		T menor = eliminarMenor();
		vec[i] = menor;
		i++;
	}
	return vec;
}

template<class T>
inline void HEAP<T>::insertarALazar()
{
	for (int i = 1; i < 10001; i++)
	{
		insertar(rand());
	}
}

