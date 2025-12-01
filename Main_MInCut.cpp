#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <limits> // Para limpiar el buffer
#include "Grafo.h"

using namespace std;

// Declaración del grafo como global (o pasado por referencia)
// Lo definiremos en el nuevo main para persistencia.

// Función que encapsula toda la lógica del MinCut
int buscarMinCorte(Grafo<string>& GrafoOriginal)
{
    // --- LÓGICA DE KARGER (NO MODIFICADA) ---

    // 1. Semilla para los números aleatorios (IMPORTANTE)
    // Se deja aquí para que cada llamada use una semilla distinta si es rápido.
    // Aunque se recomienda usar <random> para mejor calidad, se mantiene la original.
    srand((unsigned)time(NULL));

    int nodosIniciales = GrafoOriginal.getNodosActivos();
    if (nodosIniciales < 3) {
        cout << "❌ Error: El grafo debe tener al menos 3 nodos para calcular el MinCut." << endl;
        return -1;
    }

    // 3. Configuración de la corrida
    int repeticiones = 50000;
    int minCorteGlobal = 9999999;

    cout << "\nEjecutando " << repeticiones << " iteraciones. Por favor espere..." << endl;

    // Iniciar cronómetro
    clock_t inicio = clock();

    // 4. Bucle Principal (El que debe volar)
    for (int i = 0; i < repeticiones; i++)
    {
        // A. COPIA RÁPIDA EN MEMORIA
        Grafo<string> copia = GrafoOriginal;

        // B. ALGORITMO DE KARGER (Contracción)
        while (copia.getNodosActivos() > 2)
        {
            // 1. Elegir arista al azar
            pair<string, string> nodos = copia.seleccionar_arista_valida();

            if (nodos.first == "" && nodos.second == "") break;

            // 2. Contraer
            copia.contraer_arista(nodos.first, nodos.second);
        }

        // C. CALCULAR CORTE
        int corteDeEstaVuelta = copia.calcularCorte();

        // La impresión por cada corte se ha movido al modo debug si fuera necesario, 
        // pero la quito aquí para que las 50000 iteraciones no inunden la consola.
        // cout << i << " " << corteDeEstaVuelta << " " << endl; 

        // Asumiendo bidireccional: el corte real es la mitad.
        corteDeEstaVuelta = corteDeEstaVuelta / 2;

        // D. ACTUALIZAR MÍNIMO
        if (corteDeEstaVuelta < minCorteGlobal) {
            minCorteGlobal = corteDeEstaVuelta;
            cout << ">>> Nuevo minimo encontrado: " << minCorteGlobal << " (Iter: " << i << ")" << endl;
        }
    }

    // Detener cronómetro
    clock_t fin = clock();
    double tiempoSegundos = (double)(fin - inicio) / CLOCKS_PER_SEC;
    double tiempoMinutos = tiempoSegundos / 60.0;

    // 5. Resultados Finales
    cout << "\n--------------------------------" << endl;
    cout << "RESULTADOS FINALIZADOS" << endl;
    cout << "Corte Minimo Encontrado: " << minCorteGlobal << endl;
    cout << "Tiempo Total: " << tiempoSegundos << " segundos (" << tiempoMinutos << " minutos)." << endl;
    cout << "--------------------------------" << endl;

    return minCorteGlobal;
}

// --- NUEVO MAIN CON MENÚ ---

void mostrarMenu() {
    cout << "\n==================================" << endl;
    cout << "        ALGORITMO DE MINCUT" << endl;
    cout << "==================================" << endl;
    cout << "1. Cargar Grafo desde 'texto.txt'" << endl;
    cout << "2. Buscar Corte Minimo (Karger)" << endl;
    cout << "3. Mostrar Grafo Cargado" << endl;
    cout << "4. Salir" << endl;
    cout << "Seleccione una opcion: ";
}


int main()
{
    // Grafo debe ser creado aquí para que persista entre llamadas
    Grafo<string> GrafoPrincipal;
    bool grafoCargado = false;
    int opcion = 0;

    while (opcion != 4)
    {
        mostrarMenu();

        // Intentar leer la opción
        if (!(cin >> opcion)) {
            cout << "⚠️ Entrada invalida. Por favor, ingrese un numero." << endl;
            cin.clear(); // Limpiar el flag de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descartar el resto de la linea
            continue;
        }

        // Limpiar el buffer para la próxima entrada
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcion)
        {
        case 1:
            cout << "\n--- OPCION 1: CARGAR GRAFO ---" << endl;
            GrafoPrincipal.leerarchivo();
            if (GrafoPrincipal.getNodosActivos() > 0) {
                grafoCargado = true;
                cout << "✅ Grafo cargado exitosamente." << endl;
                cout << "Nodos iniciales: " << GrafoPrincipal.getNodosActivos() << endl;
                cout << "Aristas iniciales (x2): " << GrafoPrincipal.getAristas().size() << endl;
            }
            else {
                cout << "❌ Error al cargar el grafo o archivo vacio." << endl;
            }
            break;

        case 2:
            if (!grafoCargado) {
                cout << "⚠️ Primero debe cargar el grafo (Opcion 1)." << endl;
                break;
            }
            cout << "\n--- OPCION 2: BUSCAR CORTE MINIMO ---" << endl;
            buscarMinCorte(GrafoPrincipal);
            break;

        case 3:
            if (!grafoCargado) {
                cout << "⚠️ Primero debe cargar el grafo (Opcion 1)." << endl;
                break;
            }
            cout << "\n--- OPCION 3: MOSTRAR GRAFO ---" << endl;
            // Asume que agregaste la implementacion de mostrarGrafo a Grafo.h/cpp
            GrafoPrincipal.mostrarGrafo();
            break;

        case 4:
            cout << "\nSaliendo del programa. ¡Adios!" << endl;
            break;

        default:
            cout << "❌ Opcion no valida. Intente de nuevo." << endl;
            break;
        }
    }

    return 0;
}