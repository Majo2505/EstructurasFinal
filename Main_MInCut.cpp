#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <chrono> 
#include "Grafo.h"

using namespace std;

int main()
{
    srand(time(0));

    Grafo<string> grafoOriginal;
    grafoOriginal.leerarchivo("texto2.txt");

    cout << "--- ESTADISTICAS DEL GRAFO ORIGINAL ---" << endl;
    cout << "Nodos: " << grafoOriginal.getNodosActivos() << endl;
    cout << "Aristas: " << grafoOriginal.getAristas().size() << endl;
    cout << "---------------------------------------" << endl;

    int corridas = 50000;
    int minCorte = INT_MAX;

    // Puntero para guardar una COPIA del grafo que resulte ganador
    Grafo<string>* mejorGrafoResultante = nullptr;

    cout << "Iniciando " << corridas << " corridas..." << endl;

    // --- INICIAR CRONÓMETRO ---
    auto inicio = chrono::high_resolution_clock::now();

    for (int i = 0; i < corridas; i++)
    {
        // 1. Copia fresca del grafo
        Grafo<string> g(grafoOriginal);

        // 2. Contracción hasta que queden 2 nodos
        while (g.getNodosActivos() > 2)
        {
            pair<string, string> arista = g.seleccionar_arista_valida();
            if (arista.first == "") break;
            g.contraer_arista(arista.first, arista.second);
        }

        // 3. Calcular corte
        int corteActual = g.calcularCorte();

        // 4. IMPRESIÓN EN VIVO (Lo que pediste)
        // Imprime: "Corrida: Corte"
        cout << (i + 1) << ": " << corteActual;

        // 5. Verificar si es el mejor hasta ahora
        if (corteActual < minCorte) {
            minCorte = corteActual;
            cout << " <--- ¡NUEVO MINIMO ENCONTRADO!";

            // --- GUARDAR EL GRAFO GANADOR ---
            // Borramos el anterior mejor grafo guardado (si existe)
            if (mejorGrafoResultante != nullptr) {
                delete mejorGrafoResultante;
            }
            // Creamos una copia exacta de cómo quedó 'g' en este momento
            mejorGrafoResultante = new Grafo<string>(g);
        }
        cout << endl;
    }

    // --- DETENER CRONÓMETRO ---
    auto fin = chrono::high_resolution_clock::now();
    chrono::duration<double> duracion = fin - inicio;

    cout << "\n================ RESULTADOS FINALES ================" << endl;
    cout << "Min Cut encontrado: " << minCorte << endl;
    cout << "Tiempo total: " << duracion.count() << " s" << endl;
    cout << "Promedio por corrida: " << (duracion.count() / corridas) * 1000 << " ms" << endl;

    // 6. MOSTRAR CÓMO QUEDÓ EL GRAFO GANADOR
    if (mejorGrafoResultante != nullptr) {
        cout << "\n--- ASI QUEDO EL GRAFO (Deben ser 2 nodos) ---" << endl;
        cout << "Nodos activos restantes: " << mejorGrafoResultante->getNodosActivos() << endl;

        // Recorremos el mapa para mostrar los nodos sobrevivientes
        // Nota: Como es un mapa, iteramos sobre pares (clave, valor)
        int contador = 1;
        for (auto& par : mejorGrafoResultante->getMapa()) {
            cout << "\nSuper-Nodo " << contador++ << " (ID: " << par.first << "):" << endl;
            cout << "Conectado a: ";
            par.second->getLista().mostrar(); // Muestra la lista de adyacencia
        }

        // Limpiamos memoria
        delete mejorGrafoResultante;
    }
    cout << "====================================================" << endl;

    return 0;
}