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
    unordered_map <string, Vertice<T>*>  G;
    vector<Arista<T>> aristas;
    int num_nodos_activos;

public:
    Grafo();
    Grafo(const Grafo& other); // Constructor de Copia
    ~Grafo();

    unordered_map<string, Vertice<T>*>& getMapa();
    bool insertarArista(string origen, string destino);
    void leerarchivo();

    void inicializar_nodos_activos();
    int getNodosActivos();

    // Getter para KargerSolver
    const vector<Arista<T>>& getAristas() const;

    string encontrar_representante(string x);
    pair<string, string> seleccionar_arista_valida();

    // MODIFICADO: Ahora elimina físicamente los autociclos
    void contraer_arista(string n1, string n2);
    int calcularCorte();
};

template<class T>
inline Grafo<T>::Grafo() { num_nodos_activos = 0; }

// Constructor de Copia (Versión Vector - La Robusta)
template<class T>
inline Grafo<T>::Grafo(const Grafo& other)
{
    this->num_nodos_activos = other.num_nodos_activos;
    this->aristas = other.aristas;

    for (const auto& par : other.G) {
        Vertice<T>* v = new Vertice<T>();
        v->setElemento(par.first);
        v->setPadre(par.first);
        this->G[par.first] = v;
    }

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
        if (par.second != NULL) { delete par.second; par.second = NULL; }
    }
    G.clear();
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
        G[origen]->setPadre(origen);
        G[origen]->setElemento(origen);
    }
    if (G.find(destino) == G.end()) {
        G[destino] = new Vertice<T>();
        G[destino]->setPadre(destino);
        G[destino]->setElemento(destino);
    }
    return G[origen]->getLista().insertar_Final(destino);
}

template<class T>
inline void Grafo<T>::leerarchivo()
{
    ifstream archivo("texto.txt");
    if (!archivo.is_open()) { cerr << "Error archivo" << endl; return; }

    string linea, origen, destino;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        if (!(ss >> origen)) continue;
        while (ss >> destino) {
            if (destino == "-1") break;
            insertarArista(origen, destino);
            aristas.push_back(Arista<T>(origen, destino));
        }
    }
    archivo.close();
    inicializar_nodos_activos();
}

template<class T>
inline void Grafo<T>::inicializar_nodos_activos() { num_nodos_activos = G.size(); }

template<class T>
inline int Grafo<T>::getNodosActivos() { return num_nodos_activos; }

// Path Compression
template<class T>
inline string Grafo<T>::encontrar_representante(string x)
{
    if (G[x]->getPadre() == x) return x;
    string padre_final = encontrar_representante(G[x]->getPadre());
    G[x]->setPadre(padre_final);
    return padre_final;
}

// Selección aleatoria
template<class T>
inline pair<string, string> Grafo<T>::seleccionar_arista_valida()
{
    if (aristas.empty()) return { "", "" };

    // Como ahora ELIMINAMOS los autociclos del vector en 'contraer',
    // cualquier arista que quede en el vector es válida (o casi todas).
    // Igual mantenemos el chequeo por seguridad.
    while (true)
    {
        int idx = rand() % aristas.size();
        const Arista<T>& a = aristas[idx];

        string p1 = encontrar_representante(a.getOrigen());
        string p2 = encontrar_representante(a.getDestino());

        if (p1 != p2) return { p1, p2 };

        // Si llegamos aqui, es un autociclo que sobrevivió. 
        // El while lo intentará de nuevo.
    }
}

// --- AQUI ESTA EL CAMBIO PARA CUMPLIR EL REQUISITO ---
template<class T>
inline void Grafo<T>::contraer_arista(string n1, string n2)
{
    // n1 absorbe a n2

    // 1. Fusionamos listas
    G[n1]->getLista().absorberLista(G[n2]->getLista());

    // 2. Establecemos el padre (Union)
    G[n2]->setPadre(n1);

    // 3. REQUISITO: ELIMINAR AUTOCICLOS DE LA LISTA DE ADYACENCIA
    // Eliminamos de la lista de n1 todas las referencias a sí mismo (n1)
    G[n1]->getLista().eliminar_todos(n1);
    // Nota: Como n2 ahora es parte de n1, si en la lista había referencias a 'n2',
    // técnicamente deberíamos buscarlas, pero en este diseño con 'padre', 
    // la lista fisica tiene strings originales.
    // Para cumplir estrictamente, también borramos referencias a n2 dentro de la lista de n1:
    G[n1]->getLista().eliminar_todos(n2);

    // 4. REQUISITO: ELIMINAR AUTOCICLOS DEL VECTOR (Optimizado "Swap & Pop")
    // Recorremos el vector y borramos las aristas que quedaron dentro del supernodo n1
    for (int i = 0; i < aristas.size(); )
    {
        string p1 = encontrar_representante(aristas[i].getOrigen());
        string p2 = encontrar_representante(aristas[i].getDestino());

        if (p1 == p2)
        {
            // Es un autociclo. Lo borramos rápido.
            // Sobrescribimos el actual con el último del vector
            aristas[i] = aristas.back();
            // Eliminamos el último (que ahora está duplicado en 'i')
            aristas.pop_back();
            // NO incrementamos 'i' porque ahora en 'i' hay un elemento nuevo que debemos revisar.
        }
        else
        {
            // Es válida, avanzamos
            i++;
        }
    }

    num_nodos_activos--;
}

template<class T>
inline int Grafo<T>::calcularCorte()
{
    // Como hemos eliminado los autociclos físicamente del vector en cada paso,
    // el tamaño del vector remanente ES el corte.
    // (Dividido entre 2 si es bidireccional, o directo si no).
    // Esto simplifica mucho el cálculo final.
    return aristas.size();
}


/*
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
    int num_nodos_activos;
public:
    Grafo();
    ~Grafo();
    unordered_map<string, Vertice<T>*>& getMapa();
    bool insertarArista(string origen, string destino);
    void leerarchivo();
    Arista<T> AlAzar();
    void mostrarGrafo();
    void inicializarPadres();
    string encontrar_representante(string x, Grafo<T>& g);

    //
     void inicializar_nodos_activos();	//Contar los vértices después de la lectura del archivo.
     pair<string, string> seleccionar_arista_valida();//	Bucle que usa AlAzar() y descarta self - loops(chequeo con encontrar_representante).
     void contraer_arista(string n1, string n2);

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
inline void Grafo<T>::inicializarPadres()
{
    for (auto& par : G)
    {
        par.second->setPadre(par.first);
    }
}


template<class T>
inline string Grafo<T>::encontrar_representante(string x, Grafo<T>& g)
{
    auto& mapa = g.getMapa();
    if (x == mapa[x]->getPadre())
    {
        return x;
    }
 en el camino de vuelta.
    mapa[x]->setPadre(encontrar_representante(mapa[x]->getPadre(), g));

    return mapa[x]->getPadre();
}

template<class T>
inline void Grafo<T>::inicializar_nodos_activos()
{
}

*/