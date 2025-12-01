#include <iostream>
#include <ctime>
#include <cstdlib>
#include "Grafo.h"

using namespace std;

int main()
{
    // 1. Semilla para los números aleatorios (IMPORTANTE)
    srand((unsigned)time(NULL));

    // 2. Cargar el grafo desde el archivo (UNA SOLA VEZ)
    cout << "--- INICIO DEL PROGRAMA ---" << endl;
    cout << "Cargando grafo desde 'texto.txt'..." << endl;

    Grafo<string> GrafoOriginal;
    GrafoOriginal.leerarchivo();

    int nodosIniciales = GrafoOriginal.getNodosActivos();
    cout << "Grafo cargado correctamente." << endl;
    cout << "Nodos iniciales: " << nodosIniciales << endl;
    cout << "Aristas iniciales (x2): " << GrafoOriginal.getAristas().size() << endl;

    // 3. Configuración de la corrida
    int repeticiones = 50000; // El número mágico para Karger
    int minCorteGlobal = 9999999;

    cout << "\nEjecutando " << repeticiones << " iteraciones. Por favor espere..." << endl;

    // Iniciar cronómetro
    clock_t inicio = clock();

    // 4. Bucle Principal (El que debe volar)
    for (int i = 0; i < repeticiones; i++)
    {
        // A. COPIA RÁPIDA EN MEMORIA
        // Usamos el constructor de copia optimizado que definiste en Grafo.h
        Grafo<string> copia = GrafoOriginal;

        // B. ALGORITMO DE KARGER (Contracción)
        // Mientras queden más de 2 supernodos...
        while (copia.getNodosActivos() > 2)
        {
            // 1. Elegir arista al azar
            pair<string, string> nodos = copia.seleccionar_arista_valida();

            // Si por alguna razón el grafo se desconectó antes, salimos
            if (nodos.first == "" && nodos.second == "") break;

            // 2. Contraer (Fusionar + Eliminar autociclos del vector y lista)
            copia.contraer_arista(nodos.first, nodos.second);
        }

        // C. CALCULAR CORTE
        // El tamaño del vector restante es el corte de esta corrida.
        int corteDeEstaVuelta = copia.calcularCorte();
		cout << corteDeEstaVuelta << endl;

        // OJO: Si tu archivo define aristas ida y vuelta (A->B y B->A), 
        // el corte real es la mitad. Si no, es el valor directo.
        // Asumiendo formato estándar de listas de adyacencia (bidireccional):
        corteDeEstaVuelta = corteDeEstaVuelta / 2;

        // D. ACTUALIZAR MÍNIMO
        if (corteDeEstaVuelta < minCorteGlobal) {
            minCorteGlobal = corteDeEstaVuelta;
            // Opcional: Ver progreso si encuentra uno mejor
            // cout << "Nuevo minimo encontrado: " << minCorteGlobal << " (Iter: " << i << ")" << endl;
        }

        // Al cerrar la llave '}', la 'copia' se destruye automáticamente.
    }

    // Detener cronómetro
    clock_t fin = clock();
    double tiempoSegundos = (double)(fin - inicio) / CLOCKS_PER_SEC;
    double tiempoMinutos = tiempoSegundos / 60.0;

    // 5. Resultados Finales
    cout << "\n--------------------------------" << endl;
    cout << "RESULTADOS FINALIZADOS" << endl;
    cout << "--------------------------------" << endl;
    cout << "Corte Minimo Encontrado: " << minCorteGlobal << endl;
    cout << "Tiempo Total: " << tiempoSegundos << " segundos (" << tiempoMinutos << " minutos)." << endl;
    cout << "--------------------------------" << endl;

    return 0;
}