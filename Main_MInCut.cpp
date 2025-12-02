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
    grafoOriginal.leerarchivo("texto.txt");
    grafoOriginal.mostrarGrafo();

    cout << "--- ESTADISTICAS DEL GRAFO ORIGINAL ---" << endl;
    cout << "Nodos: " << grafoOriginal.getNodosActivos() << endl;
    cout << "Aristas: " << grafoOriginal.getAristas().size() << endl;
    cout << "---------------------------------------" << endl;

    int corridas = 50000;
    int minCorte = INT_MAX;

    Grafo<string>* mejorGrafoResultante = nullptr;  // Puntero para guardar una COPIA del grafo que resulte ganador

    cout << "Iniciando " << corridas << " corridas..." << endl;

    // --- INICIAR CRONÓMETRO ---
    auto inicio = chrono::high_resolution_clock::now();

    for (int i = 0; i < corridas; i++)
    {
        
        Grafo<string> g(grafoOriginal); // Copia del grafo

        // Contracción hasta que queden 2 nodos
        while (g.getNodosActivos() > 2)
        {
            pair<string, string> arista = g.Azar();
            if (arista.first == "") 
                break;
            g.contraer_arista(arista.first, arista.second);
        }

        int corteActual = g.calcularCorte();  // Calcular corte

        cout << (i + 1) << ": " << corteActual; // Imprime: "Corrida: Corte"

        if (corteActual < minCorte) 
        {
            minCorte = corteActual;
            cout << " <--- ¡NUEVO MINIMO ENCONTRADO!";

            // Borra anterior mejor grafo guardado (si existe)
            if (mejorGrafoResultante != nullptr) 
            {
                delete mejorGrafoResultante;
            }
            mejorGrafoResultante = new Grafo<string>(g);  // Copia de grafo con menor corte
        }
        cout << endl;
    }

    // --- DETENER CRONÓMETRO ---
    auto fin = chrono::high_resolution_clock::now();
    chrono::duration<double> duracion = fin - inicio;

    cout << "\n================ RESULTADOS FINALES ================" << endl;
    cout << "Min Cut encontrado: " << minCorte << endl;
    cout << "Tiempo total: " << duracion.count() << " s" << endl;

    //  MOSTRAR CÓMO QUEDÓ EL GRAFO con MIN CUT
    if (mejorGrafoResultante != nullptr)
    {
        cout << "\n--- ASI QUEDO EL GRAFO ---" << endl;
        cout << "Nodos activos restantes: " << mejorGrafoResultante->getNodosActivos() << endl;
        mejorGrafoResultante->mostrarGrafo();
        delete mejorGrafoResultante;
    }
    cout << "====================================================" << endl;

    return 0;
}