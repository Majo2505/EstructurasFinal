#pragma once
#include "Vertice.h"
#include "Arista.h"
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <cstdlib> 
#include <ctime>
#include <iostream>

using namespace std;

template<class T>
class Grafo
{
private:
    unordered_map<string, Vertice<T>*> G; // mapa de nodos
    vector<Arista<T>> aristas;           // vector de aristas
    int num_nodos_activos;

public:
    Grafo();
    Grafo(const Grafo& other); // copia rápida para cada corrida
    ~Grafo();

    unordered_map<string, Vertice<T>*>& getMapa();
    const vector<Arista<T>>& getAristas() const;

    bool insertarArista(string origen, string destino);
    void leerarchivo(const string& nombreArchivo);

    void inicializar_nodos_activos();
    int getNodosActivos();

    pair<string, string> Azar();
    void contraer_arista(string n1, string n2);
    int calcularCorte();
    void mostrarGrafo();
};

template<class T>
inline Grafo<T>::Grafo() { num_nodos_activos = 0; }

template<class T>
inline Grafo<T>::Grafo(const Grafo& other)
{
    this->num_nodos_activos = other.num_nodos_activos;
    this->aristas = other.aristas;

    // Copiamos nodos
    for (const auto& par : other.G) {
        Vertice<T>* v = new Vertice<T>();
        v->setElemento(par.first);
        this->G[par.first] = v;
    }

    // Copiamos listas de adyacencia
    for (const auto& arista : this->aristas) {
        if (this->G.find(arista.getOrigen()) != this->G.end()) {
            this->G[arista.getOrigen()]->getLista().insertar_Final(arista.getDestino());
        }
    }
}

template<class T>
inline Grafo<T>::~Grafo()
{
    for (auto& par : G) {
        delete par.second;
        par.second = nullptr;
    }
    G.clear();
    aristas.clear();
}

template<class T>
inline unordered_map<string, Vertice<T>*>& Grafo<T>::getMapa() { return G; }

template<class T>
inline const vector<Arista<T>>& Grafo<T>::getAristas() const { return aristas; }

template<class T>
inline bool Grafo<T>::insertarArista(string origen, string destino)
{
    if (G.find(origen) == G.end()) {
        G[origen] = new Vertice<T>();
        G[origen]->setElemento(origen);
    }
    if (G.find(destino) == G.end()) {
        G[destino] = new Vertice<T>();
        G[destino]->setElemento(destino);
    }
    return G[origen]->getLista().insertar_Final(destino);
}



template<class T>
inline void Grafo<T>::leerarchivo(const string& nombreArchivo)
{
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) { cerr << "Error archivo" << endl; return; }

    string linea, origen, destino;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        if (!(ss >> origen)) continue;

        // Quitamos los dos puntos ':' si vienen pegados al nombre (ej: "LP:")
        if (origen.back() == ':') origen.pop_back();

        while (ss >> destino) 
        {
            if (destino == "-1") 
                break;

            insertarArista(origen, destino);
            if (origen < destino) {
                aristas.push_back(Arista<T>(origen, destino));
            }
        }
    }
    archivo.close();
    inicializar_nodos_activos();
}



template<class T>
inline void Grafo<T>::inicializar_nodos_activos() { num_nodos_activos = G.size(); }

template<class T>
inline int Grafo<T>::getNodosActivos() { return num_nodos_activos; }

// Selección aleatoria de arista válida
template<class T>
inline pair<string, string> Grafo<T>::Azar()
{
    if (aristas.empty()) return { "", "" };

    while (true)
    {
        int idx = rand() % aristas.size();
        const Arista<T>& a = aristas[idx];

        string p1 = a.getOrigen();
        string p2 = a.getDestino();

        if (p1 != p2)
            return { p1, p2 }; // si es autociclo, reintenta
    }
}


// Contraer arista n1 (sobrevive) <- n2 (desaparece)
template<class T>
inline void Grafo<T>::contraer_arista(string n1, string n2)
{
   
    // Recorremos TODAS las aristas. Si alguna apunta a n2, ahora debe apuntar a n1.
    for (int i = 0; i < aristas.size(); i++) {
        if (aristas[i].getOrigen() == n2) {
            aristas[i].setOrigen(n1);
        }
        if (aristas[i].getDestino() == n2) {
            aristas[i].setDestino(n1);
        }
    }

    // 2. FASE DE LIMPIEZA DE AUTO-CICLOS
    // Ahora que todo apunta a n1, se eliminab aristas que son n1 -> n1
    // (Originalmente eran n1->n2, n2->n1 o n1->n1)
    for (int i = 0; i < aristas.size(); ) {
        string o = aristas[i].getOrigen();
        string d = aristas[i].getDestino();

        if (o == d) {
            aristas[i] = aristas.back();
            aristas.pop_back();
        }
        else {
            i++;
        }
    }

    if (G.find(n2) != G.end()) {
        G[n1]->getLista().absorberLista(G[n2]->getLista());
        delete G[n2]; // Liberar memoria
        G.erase(n2);  // Sacarlo del mapa
    }

    // Reducimos nodos activos
    num_nodos_activos--;
}

// Calcula corte mínimo (tamaño vector de aristas restante)
template<class T>
inline int Grafo<T>::calcularCorte()
{
    return aristas.size();
}


template<class T>
void Grafo<T>::mostrarGrafo()
{
    cout << "========================================" << endl;
    cout << "Total Nodos Activos: " << getNodosActivos() << endl;
    cout << "Total Aristas Vivas: " << aristas.size() << endl;
    cout << "----------------------------------------" << endl;

    int contador = 1;
    typename unordered_map<string, Vertice<T>*>::iterator it;

    for (it = G.begin(); it != G.end(); it++) {
        // it->first  es la CLAVE (el nombre del nodo, string)
        // it->second es el VALOR (el puntero al Vertice)
        cout << contador++ << ". [" << it->first << "] -> ";
        it->second->getLista().mostrar();

        cout << endl;
    }
    cout << "========================================\n" << endl;
}