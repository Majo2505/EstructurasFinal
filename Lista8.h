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
	Tripla<T>* getPrimerTripla();
	Tripla<T>* getUltimaTripla();
	bool insertar_Principio(T el, int peso);
	bool insertar_Final(T el);
	bool eliminar_Principio();
	bool eliminar_Final();
	bool buscarR(T el, Tripla<T>* t);
	bool buscar(T el);
	Tripla<T>* buscarTripla(T el);
	void mostrar();
	void mostrarconPesosFinal();
	bool eliminar_el_dado(T el);
	void llenar_Lista8con1M(T n);
	Tripla<T>* buscarEnesimo(int pos);
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
	if (primer != NULL)
	{
		delete primer; //hara una eliminacion recursiva de todos las triplas q contenga, por el destructor de la tripla
	}
}

template<class T>
inline Tripla<T>* Lista8<T>::getPrimerTripla()
{
	return primer;
}

template<class T>
inline Tripla<T>* Lista8<T>::getUltimaTripla()
{
	return ultimo;
}

template<class T>
inline bool Lista8<T>::insertar_Principio(T el, int peso)
{
	bool resp = true;
	if (primer == NULL)
	{
		primer = new Tripla<T>(NULL, el, NULL, peso);
		ultimo = primer;
	}
	else
	{
		primer = new Tripla<T>(NULL, el, primer, peso);
		primer->getSiguiente()->setTriplaAnt(primer);
	}
	return resp;
}

template<class T>
inline bool Lista8<T>::insertar_Final(T el)
{
	bool resp = true;
	if (ultimo == NULL)
	{
		ultimo = new Tripla<T>(NULL, el, NULL);
		primer = ultimo;
	}
	else
	{
		ultimo = new Tripla<T>(ultimo, el, NULL);
		ultimo->getAnterior()->setTriplaSig(ultimo);
	}
	return resp;
}

template<class T>
inline bool Lista8<T>::eliminar_Principio()
{
	bool resp = true;
	if (primer == NULL) // lista vacia
	{
		resp = false;
	}
	else
	{
		if (primer == ultimo)  // solo un elemento en la lista
		{
			primer = NULL;
			ultimo = NULL;
			delete primer;
		}
		else
		{
			Tripla<T>* aux = primer;
			primer = primer->getSiguiente();
			primer->setTriplaAnt(NULL); //y ponemos q su anterior sea null para q deje de apuntar al antiguo primero
			aux->setTriplaSig(NULL);//ponemos q su sig del aux sea null para no tener problemas con el destructor de la tripla
			delete aux;
		}
	}
	return resp;
}

template<class T>
inline bool Lista8<T>::eliminar_Final()
{
	bool resp = true;
	if (ultimo == NULL) // si la lista esta vacia
	{
		resp = false;
	}
	else
	{
		if (primer == ultimo)  //  un elemento en la lista
		{
			primer = NULL;
			ultimo = NULL;
			delete ultimo;
		}
		else
		{
			Tripla<T>* aux = ultimo;
			ultimo = ultimo->getAnterior(); //movemos el ultimo al antepenultimo
			ultimo->setTriplaSig(NULL); // y ponemos q su sig sea null para q deje de apuntar al antiguo ultimo
			aux->setTriplaSig(NULL);//ponemos q su sig sea null para no tener problemas con el destructor de la tripla
			delete aux;
		}
	}

	return resp;
}

template<class T>
inline bool Lista8<T>::buscarR(T el, Tripla<T>* t)
{
	bool resp = true;
	if (t == NULL)
	{
		resp = false;
	}
	else
	{
		if (el == t->getElemento())
		{
			resp = true;
		}
		else
		{
			resp = buscarR(el, t->getSiguiente());
		}
	}
	return resp;
}

template<class T>
inline bool Lista8<T>::buscar(T el)
{
	bool resp = true;
	Tripla<T>* aux = primer;
	while (aux != NULL)
	{
		if (aux->getElemento() == el)
		{
			resp = true;
			break;
		}
		aux = aux->getSiguiente();
		resp = false;
	}
	return resp;
}

template<class T>
inline Tripla<T>* Lista8<T>::buscarTripla(T el)
{
	Tripla<T>* aux = primer;
	while (aux != NULL)
	{
		if (aux->getElemento() == el)
		{
			return aux;
		}
		aux = aux->getSiguiente();
	}
	return NULL;
}

template<class T>
inline void Lista8<T>::mostrar()
{
	Tripla<T>* aux = primer;
	while (aux != NULL)
	{
		cout << " " << aux->getElemento() << "(" << aux->getPeso() << ")";
		aux = aux->getSiguiente();
	}
	cout << endl;
}

template<class T>
inline void Lista8<T>::mostrarconPesosFinal()
{
	Tripla<T>* aux = primer;
	while (aux != NULL)
	{
		cout << " " << aux->getElemento();
		if (aux->getSiguiente() == NULL)
		{
			cout << "(" << aux->getPeso() << ")";
		}
		aux = aux->getSiguiente();

	}
	cout << endl;
}

template<class T>
inline bool Lista8<T>::eliminar_el_dado(T el)
{
	bool resp = true;
	if (primer == NULL) // si no hay ningun elemento en la lista
	{
		resp = false;
	}
	else
	{
		if (primer == ultimo) // si hay un elemento en la lista
		{
			if (primer->getElemento() == el)
			{
				delete primer;
				primer = NULL;
				ultimo = NULL;
			}
			else
			{
				resp = false;
			}
		}
		else // si hay varios elementos en la lista
		{
			Tripla<T>* aux = primer;
			while (aux != ultimo && aux->getElemento() != el)
			{
				aux = aux->getSiguiente();
			}
			if (aux->getElemento() == el) // si hay una tripla con el elemento
			{
				if (aux == primer)
				{
					eliminar_Principio();
				}
				else if (aux == ultimo)
				{
					eliminar_Final();
				}
				else
				{
					aux->getAnterior()->setTriplaSig(aux->getSiguiente());
					aux->getSiguiente()->setTriplaAnt(aux->getAnterior());
					aux->setTriplaSig(NULL); //ponemos q su sig sea null para no tener problemas con el destructor de la tripla
					delete aux;
				}
			}
			else // si no hay ninguna tripla con el elemento
			{
				resp = false;
			}
		}
	}
	return resp;
}

template<class T>
inline void Lista8<T>::llenar_Lista8con1M(T n)
{
	for (int i = 0; i < 10000; i++)
	{
		//insertar_Final(n);
	}
}

template<class T>
inline Tripla<T>* Lista8<T>::buscarEnesimo(int pos)
{
	if (pos < 0)
	{
		return NULL;
	}

	Tripla<T>* aux = primer;
	while (aux != NULL && pos - 1 > 0) //si tomamos como nuestro primer nodo como pos 0 entonces seria pos>0, pero si tomamos q nuestro primer nodo es 1, entonces pos-1>0
	{
		aux = aux->getSiguiente();
		pos--;
	}
	return aux; // Puede ser NULL si pos está fuera de rango
}

