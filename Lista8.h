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

	// --- MÉTODOS CLAVE PARA KARGER ---

	// 1. Fusión O(1): Pega una lista al final de otra sin recorrerla
	void absorberLista(Lista8<T>& otraLista);

	// 2. Requisito Obligatorio: Elimina TODAS las apariciones de un nodo
	// (Necesario para eliminar físicamente los autociclos de la lista)
	void eliminar_todos(T el);
};

template<class T>
inline Lista8<T>::Lista8()
{
	primer = NULL;
	ultimo = NULL;
}

// Destructor Iterativo (Seguro y Rápido)
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
	if (otraLista.primer == NULL) return; // Nada que absorber

	if (this->primer == NULL)
	{
		// Si yo estoy vacío, tomo la otra lista entera
		this->primer = otraLista.primer;
		this->ultimo = otraLista.ultimo;
	}
	else
	{
		// Conecto mi final con el principio de la otra
		this->ultimo->setTriplaSig(otraLista.primer);
		otraLista.primer->setTriplaAnt(this->ultimo);

		// Mi nuevo último es el último de la otra
		this->ultimo = otraLista.ultimo;
	}

	// Vaciamos la otra lista para evitar errores de memoria al destruirla
	otraLista.primer = NULL;
	otraLista.ultimo = NULL;
}

// --- ELIMINACIÓN TOTAL (Para borrar autociclos) ---
template<class T>
inline void Lista8<T>::eliminar_todos(T el)
{
	Tripla<T>* actual = primer;

	while (actual != NULL)
	{
		// Guardamos el siguiente antes de borrar, para no perder el hilo
		Tripla<T>* siguiente = actual->getSiguiente();

		if (actual->getElemento() == el)
		{
			// Lógica de desvinculación

			// 1. Es el primero
			if (actual == primer)
			{
				primer = primer->getSiguiente();
				if (primer != NULL) primer->setTriplaAnt(NULL);
				else ultimo = NULL; // Quedó vacía
			}
			// 2. Es el último
			else if (actual == ultimo)
			{
				ultimo = ultimo->getAnterior();
				ultimo->setTriplaSig(NULL);
			}
			// 3. Está en medio
			else
			{
				actual->getAnterior()->setTriplaSig(actual->getSiguiente());
				actual->getSiguiente()->setTriplaAnt(actual->getAnterior());
			}

			// Borramos nodo y prevenimos errores
			actual->setTriplaSig(NULL);
			delete actual;
		}

		// Avanzamos
		actual = siguiente;
	}
}

/*
//version 2
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
	bool eliminar_Principio();
	bool eliminar_Final();
	void mostrar();
	bool eliminar_el_dado(T el);
	inline Tripla<T>* getPrimerTripla();

	// NUEVA FUNCIÓN CLAVE PARA KARGER (Velocidad O(1))
	// Pasa todos los elementos de 'otraLista' al final de esta lista.
	
	void absorberLista(Lista8<T>& otraLista);
	inline void eliminar_todos(T el);
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
inline Tripla<T>* Lista8<T>::getPrimerTripla()
{
	return primer;
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
		// Creamos la nueva tripla apuntando atras al actual ultimo
		Tripla<T>* nuevo = new Tripla<T>(ultimo, el, NULL);
		ultimo->setTriplaSig(nuevo);
		ultimo = nuevo;
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
		Tripla<T>* aux = primer; // Guardamos referencia para borrar luego
		if (primer == ultimo)  // solo un elemento en la lista
		{
			primer = NULL;
			ultimo = NULL;
		}
		else
		{
			primer = primer->getSiguiente();
			primer->setTriplaAnt(NULL);
		}
		delete aux; // Borramos la memoria
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
		Tripla<T>* aux = ultimo; // Guardamos referencia
		if (primer == ultimo)  //  un elemento en la lista
		{
			primer = NULL;
			ultimo = NULL;
		}
		else
		{
			ultimo = ultimo->getAnterior();
			ultimo->setTriplaSig(NULL);
		}
		delete aux; // Borramos memoria
	}
	return resp;
}


template<class T>
inline void Lista8<T>::mostrar()
{
	Tripla<T>* aux = primer;
	while (aux != NULL)
	{
		cout << " " << aux->getElemento(); // Solo mostramos el elemento
		aux = aux->getSiguiente();
	}
	cout << endl;
}

template<class T>
inline bool Lista8<T>::eliminar_el_dado(T el)
{
	bool resp = true;
	if (primer == NULL)
	{
		resp = false;
	}
	else
	{
		// Caso especial: El elemento está en el principio
		if (primer->getElemento() == el)
		{
			eliminar_Principio();
		}
		// Caso especial: El elemento está en el final
		else if (ultimo->getElemento() == el)
		{
			eliminar_Final();
		}
		else
		{
			// Buscamos en el medio
			Tripla<T>* aux = primer->getSiguiente();
			while (aux != NULL && aux->getElemento() != el)
			{
				aux = aux->getSiguiente();
			}

			if (aux != NULL) // Lo encontramos en medio
			{
				aux->getAnterior()->setTriplaSig(aux->getSiguiente());
				aux->getSiguiente()->setTriplaAnt(aux->getAnterior());
				delete aux;
			}
			else // No encontrado
			{
				resp = false;
			}
		}
	}
	return resp;
}

// --- OPTIMIZACIÓN PARA KARGER ---
// Esta funcion permite fusionar listas en O(1) en lugar de O(N)
template<class T>
inline void Lista8<T>::absorberLista(Lista8<T>& otraLista)
{
	// Si la otra lista está vacía, no hay nada que hacer
	if (otraLista.primer == NULL) return;

	// Si mi lista está vacía, tomo todo de la otra
	if (this->primer == NULL)
	{
		this->primer = otraLista.primer;
		this->ultimo = otraLista.ultimo;
	}
	else
	{
		// Conectamos el final de mi lista con el principio de la otra
		this->ultimo->setTriplaSig(otraLista.primer);
		otraLista.primer->setTriplaAnt(this->ultimo);

		// Actualizamos mi nuevo ultimo
		this->ultimo = otraLista.ultimo;
	}

	// Vaciamos la otra lista (para que su destructor no borre nada)
	otraLista.primer = NULL;
	otraLista.ultimo = NULL;
}


// EN LISTA8.H - Agregar o reemplazar

// Esta función elimina TODAS las ocurrencias de 'el'.
// Es vital porque al fusionar nodos se crean múltiples bucles.
template<class T>
inline void Lista8<T>::eliminar_todos(T el)
{
	Tripla<T>* actual = primer;

	while (actual != NULL)
	{
		// Guardamos el siguiente antes de (posiblemente) borrar el actual
		Tripla<T>* siguiente = actual->getSiguiente();

		if (actual->getElemento() == el)
		{
			// --- Lógica de borrado (similar a eliminar_el_dado pero iterativa) ---

			// Caso 1: Es el primero
			if (actual == primer)
			{
				primer = primer->getSiguiente();
				if (primer != NULL) primer->setTriplaAnt(NULL);
				else ultimo = NULL; // Quedó vacía
			}
			// Caso 2: Es el último
			else if (actual == ultimo)
			{
				ultimo = ultimo->getAnterior();
				ultimo->setTriplaSig(NULL);
			}
			// Caso 3: Está en el medio
			else
			{
				actual->getAnterior()->setTriplaSig(actual->getSiguiente());
				actual->getSiguiente()->setTriplaAnt(actual->getAnterior());
			}

			// Importante: Borrar el nodo actual y anular sus punteros para que el destructor no falle
			actual->setTriplaSig(NULL);
			// (Nota: Tripla ya tiene destructor vacío, así que delete es seguro)
			delete actual;
		}

		// Avanzamos
		actual = siguiente;
	}
}*/

/*
Por qué absorberLista?
Cuando el nodo A se coma al nodo B:

En vez de hacer un ciclo while sacando elementos de B e insertándolos en A (lento).

Llamarás a listaDeA.absorberLista(listaDeB).

Esto simplemente cambia dos punteros. ¡Es instantáneo! Esto te ahorrará muchísimo tiempo en las 50,000 iteraciones.
*/