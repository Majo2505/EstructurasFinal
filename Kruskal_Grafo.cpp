#include <iostream>
#include <string>
#include "Grafo.h" // Aquí está incluido Vertice.h, HEAP.h y la función Kruskal

using namespace std;

void menu() {
    cout << "\n==========================================" << endl;
    cout << "      ALGORITMO DE KRUSKAL (BOLIVIA)      " << endl;
    cout << "==========================================" << endl;
    cout << "1. Insertar Arista (Manual)" << endl;
    cout << "2. Leer de archivo (VALORADO BOLIVIA)" << endl;
    cout << "22. Leer de archivo2 sin ciclos" << endl;
    cout << "3. Mostrar Grafo Original" << endl;
    cout << "4. Ejecutar Algoritmo de Kruskal" << endl;
    cout << "5. Mostrar Arbol de Expansion (AE) y Costo" << endl;
    cout << "6. Buscar Ciclos en Grafos" << endl;
    cout << "7. Salir" << endl;
    cout << "==========================================" << endl;
    cout << "Ingrese una opcion: ";
}

int main() {
    // 1. Instancia del Grafo Principal
    Grafo<string> G;
    int total = 0;
    // Puntero para guardar el resultado (El Grafo AE)
    Grafo<string>* AE = nullptr;
    int opcion;
    string origen, destino;
    int peso;
    string nombreArchivo = "BOLIVIA.txt"; // Asegúrate de tener este archivo

    do {
        menu();
        cin >> opcion;

        switch (opcion) {
        case 1:
            cout << "\n--- Insertar Arista ---" << endl;
            cout << "Origen: "; cin >> origen;
            cout << "Destino: "; cin >> destino;
            cout << "Peso: "; cin >> peso;
            G.insertarArista(origen, destino);
            cout << "Arista insertada correctamente." << endl;
            break;

        case 2:
            cout << "\n--- Leyendo Archivo ---" << endl;
            G.leerarchivo();
            cout << "Datos cargados del archivo." << endl;
            break;
            
         
        case 3:
            cout << "\n--- Grafo Actual ---" << endl;
            G.mostrarGrafo();
            break;

        case 4:
            cout << "\n--- Ejecutando Kruskal ---" << endl;
            // Limpiamos memoria si ya se había ejecutado antes
            if (AE != nullptr) {
                delete AE;
                AE = nullptr;
            }
            // LLAMADA A LA FUNCIÓN INDEPENDIENTE
            AE = G.Kruskal(G, total);

            cout << "Algoritmo ejecutado. El AE ha sido generado." << endl;
            cout << "Seleccione la opcion 5 para ver los resultados." << endl;
            break;

        case 5:
            if (AE == nullptr) {
                cout << "\n[ERROR] Primero debe ejecutar Kruskal (Opcion 4)." << endl;
            }
            else {
                cout << "\n--- Arbol de Expansion Minima (Resultado) ---" << endl;
                AE->mostrarGrafo();
                cout << "\nCOSTO TOTAL MINIMO: " << total << endl;
            }
            break;

        case 6:

        {
            int opcion2;
            cout << "1 .Buscar Ciclos en el Grafo Origial" << endl;
            cout << "2 .Buscar Ciclos en el Grafo AE" << endl;
            cin >> opcion2;
            if (opcion2 == 1)
            {
                if (G.buscarCiclosenGrafo(G))
                {
                    cout << "existen ciclos en el grafo";
                }
                else
                {
                    cout << "NO existen ciclos en el grafo";
                }
            }
            else
            {
                if (opcion2 == 2)
                {
                    if (AE != NULL)
                    {
                        if (G.buscarCiclosenGrafo(*AE))
                        {
                            cout << "existen ciclos en el grafo";
                        }
                        else
                        {
                            cout << "NO existen ciclos en el grafo";
                        }
                    }
                    else
                    {
                        cout << "primero debe ejecutar el kruskal";
                    }
                }
                else
                {
                    cout << "seleccione un grafo valido";
                }
            }
            break;
        }
        case 7:
            cout << "Saliendo del programa..." << endl;
            break;

        default:
            cout << "Opcion no valida. Intente de nuevo." << endl;
        }

    } while (opcion != 7);

    // Limpieza final de memoria
    if (AE != nullptr) {
        delete AE;
    }

    return 0;
}