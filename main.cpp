#include "pregunta.h"
#include "prueba.cpp"
#include <iostream>
#include <string>
//inicia el programa
int main() {
    Pregunta p;
    Prueba miPrueba;
    int opcion;
    string examen;
    int fecha;
    do {
        //antes de iniciar limpia la consola y despues muestra el menu
        miPrueba.limpiarConsola();
        cout << "\n=== MENU PRINCIPAL ===\n";
        cout << "1. Crear examen\n";
        cout << "2. Mostrar examen\n";
        cout << "3. Filtrar preguntas por nivel de Bloom\n";
        cout << "4. Agregar pregunta a examen\n";
        cout << "5. Editar pregunta existente\n";
        cout << "6. Mostrar tiempo total estimado del examen\n";
        cout << "7. Eliminar pregunta\n";
        cout << "8. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        miPrueba.limpiarConsola();
        //segun la opcion seleccionada actua el programa
        switch (opcion) {
            case 1: 
                miPrueba.generarExamen(miPrueba);
                break;
            case 2:
                miPrueba.mostrarPrueba(miPrueba);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                break;
            case 3: {
                int nivel;
                cout << "Ingrese nivel (0=Recordar, 1=Comprender, 2=Aplicar, 3=Analizar, 4=Evaluar, 5=Crear): ";
                cin >> nivel;
                miPrueba.filtrarPorNivel(miPrueba,(NivelBloom)nivel);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                break;
            }
            case 4:
                cout<<"Que examen quiere agregar pregunta?";
                cin >> examen;
                examen += ".txt";
                cout<<"anio?";
                cin >> fecha;
                p.agregarPregunta(examen,fecha);
                break;
            case 5:
                miPrueba.editarPregunta(miPrueba);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                break;
            case 6:
                miPrueba.mostrarTiempoTotal(miPrueba);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                break;
            case 7:
                miPrueba.eliminarPregunta(miPrueba);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                break;
            case 8:
                cout << "¡Hasta luego!\n";
                break;
            default:
                cout << "Opcion invalida.\n";
        }
    }
    while (opcion != 8);
    return 0;
}
