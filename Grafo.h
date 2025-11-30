#pragma once
#include "Vertice.h"
#include "HEAP.h"
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "Arista.h"
#include <vector>
#include <cstdlib> // Para std::rand, std::srand, RAND_MAX
#include <ctime>
using namespace std;

template<class T>
class Grafo
{
private:
	unordered_map <string, Vertice<T>*>  G;
	vector<Arista<T>> aristas; // Para almacenar las aristas del grafo si es necesario
public:
	Grafo();
	~Grafo();
	unordered_map<string, Vertice<T>*>& getMapa();
	bool insertarArista(string origen, string destino);
	void leerarchivo();
    //void leerarchivo2();
    Arista<T> AlAzar();
    void mostrarGrafo();
    void llenar(HEAP<int>& colaP);
    void inicializarPadres();
    Grafo<T>* Kruskal(Grafo<T>& g, int& total);
    string BuscarCiclo(string x, Grafo<T>& g);
    bool buscarCiclosenGrafo(Grafo<T>& g);

};

template<class T>
inline Grafo<T>::Grafo()
{
}

template<class T>
inline Grafo<T>::~Grafo()
{
    for (auto& par : G)
    {
        // par.first  -> Es el nombre (string)
        // par.second -> Es el puntero al Vertice (Vertice<T>*)
        if (par.second != NULL) {
            delete par.second;
            par.second = NULL;
        }
    }
    G.clear();
}

template<class T>
inline unordered_map<string, Vertice<T>*>& Grafo<T>::getMapa()
{
    return G;
}

template<class T>
inline bool Grafo<T>::insertarArista(string origen, string destino)
{
    bool res = true;
    if (G.find(origen) == G.end()) 
    {
        G[origen] = new Vertice<T>();
    }
    if (G.find(destino) == G.end()) 
    {
        G[destino] = new Vertice<T>();
    }
    G[origen]->setPadre(origen);
    G[destino]->setPadre(destino);
     res = G[origen]->getLista().insertar_Final(destino);
     return res;
}

template<class T>
inline void Grafo<T>::leerarchivo()
{
    ifstream archivo("texto.txt");
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        return;
    }
    string linea;
    int p;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string origen;
        string destino;

        if (!(ss >> origen)) {
            continue; 
        }
        while (ss >> destino) {

            if (destino == "-1") {
                break;
            }
            insertarArista(origen, destino);
            aristas.push_back(Arista<T>( origen, destino ));
        }
    }
	archivo.close();
}
/*
template<class T>
inline void Grafo<T>::leerarchivo2()
{
    ifstream archivo("texto2.txt");
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        return;
    }
    string linea, u, v;
    int p;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        ss >> u >> v >> p;
        insertarArista(u, v, p);
    }
}*/

template<class T>
inline Arista<T> Grafo<T>::AlAzar()
{
    if (aristas.empty()) {
        cout << "El vector de aristas está vacío. No se puede elegir al azar." << endl;
    }

    int indice_aleatorio = std::rand() % aristas.size();

    return aristas[indice_aleatorio];
}

template<class T>
inline void Grafo<T>::mostrarGrafo()
{
    cout << "--- Grafo ---" << endl;
    for (auto& par : G) 
    {
        cout << "[" << par.first << "] -> ";
        par.second->getLista().mostrar();
    }
}

template<class T>
inline void Grafo<T>::llenar(HEAP<int>& colaP)
{
    for (auto& par : G) {
        string origen = par.first;
        Tripla<T>* aux = par.second->getLista().getPrimerTripla();
        while (aux != nullptr) {
            colaP.insertar(aux->getPeso(), origen, aux->getElemento());
            aux = aux->getSiguiente();
        }
    }
    colaP.mostrar();
}

template<class T>
inline void Grafo<T>::inicializarPadres()
{
    for (auto& par : G) 
    {
        par.second->setPadre(par.first);
    }
}

template<class T>
inline Grafo<T>* Grafo<T>::Kruskal(Grafo<T>& g, int& total)
{
    g.inicializarPadres();
    HEAP<int> ColaP;
    g.llenar(ColaP);
    Grafo<T>* AE = new Grafo<T>();
    int cont = 0;

    auto& mapaG = g.getMapa();
    int tam = ColaP.getUltHoja();
    while (tam> cont)
    {
        Nodo<int>* Trio = ColaP.eliminarMenor();
        if (Trio == nullptr)
        {
            total = ColaP.getUltHoja();
        }
        else
        {
            string origen = Trio->getOrigen();
            string destino = Trio->getDestino();
            int peso = Trio->getElemento();

            string Vo = BuscarCiclo(origen, g);
            string Vd = BuscarCiclo(destino, g);
            if (Vo != Vd)
            {
                AE->insertarArista(origen, destino);
                total = total + peso;
                mapaG[Vo]->setPadre(mapaG[Vd]->getPadre());
            }
            cont++;
        }
    }
    return AE;
}

template<class T>
inline string Grafo<T>::BuscarCiclo(string x, Grafo<T>& g)
{
    auto& mapa = g.getMapa();
    while (x != mapa[x]->getPadre())
    {
        x = mapa[x]->getPadre();
    }
    return x;
}

template<class T>
inline bool Grafo<T>::buscarCiclosenGrafo(Grafo<T>& g)
{
    bool res = false;
    g.inicializarPadres();
    HEAP<int> ColaP;
    g.llenar(ColaP);
    while (ColaP.getUltHoja() > 0 && !res)
    {
        Nodo<int>* Trio = ColaP.eliminarMenor();
        if(Trio != NULL)
        {
            string origen = Trio->getOrigen();
            string destino = Trio->getDestino();

            string Vo = BuscarCiclo(origen, g);
            string Vd = BuscarCiclo(destino, g);
            if (Vo == Vd)
            {
                res = true;
            }
            else
            {
                g.getMapa()[Vo]->setPadre(g.getMapa()[Vd]->getPadre());
            }
            delete Trio;
        }
    }
    return res;
}
