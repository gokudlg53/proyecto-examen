#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <limits>
#include <cstdlib>
//funcion para limmpiar la consola
void limpiarConsola() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
using namespace std;
const int MAX_PREGUNTAS = 100;
// Taxonomía de Bloom extendida a 6 niveles
enum NivelBloom { RECORDAR, COMPRENDER, APLICAR, ANALIZAR, EVALUAR, CREAR };
// Tipos de pregunta
enum TipoPregunta { VERDADERO_FALSO, SELECCION_MULTIPLE };
// Estructura para Pregunta
struct Pregunta {
    string enunciado;
    TipoPregunta tipo;
    NivelBloom nivel;
    int anioCreacion;
    int tiempoEstimadoSegundos;
    string opciones[5]; // máximo 5 opciones
    int cantidadOpciones;
    int indiceRespuestaCorrecta;
};
// Estructura para Prueba
struct Prueba {
    int tiempoTotal = 0;
    Pregunta preguntas[MAX_PREGUNTAS];
    int cantidadPreguntas = 0;
    string nombreArchivo;
    int anioExamen;
    string asignatura;
};
bool estaVencida(int anioCreacion) {
    return anioCreacion <= 2023;
}
//revisa si se uso un año anterior y evita que se use 2 años seguidos
bool seUsoEnAnioAnterior(const string& enunciado, int anioActual) {
    ifstream historial("historial_preguntas.txt");
    if (!historial.is_open()) return false;
    string linea;
    string preguntaEncontrada;
    int anioEncontrado = -1;
    while (getline(historial, linea)) {
        if (linea.find("Pregunta: ") == 0) {
            preguntaEncontrada = linea.substr(10); // Extrae el enunciado
        } else if (linea.find("Año: ") == 0) {
            anioEncontrado = stoi(linea.substr(5)); // Extrae el año
            if (preguntaEncontrada == enunciado && anioEncontrado == anioActual - 1) {
                return true; // Se usó el año anterior
            }
        }
    }
    return false;
}
string nivelComoTexto(NivelBloom nivel) {
    switch (nivel) {
        case RECORDAR: return "Recordar";
        case COMPRENDER: return "Comprender";
        case APLICAR: return "Aplicar";
        case ANALIZAR: return "Analizar";
        case EVALUAR: return "Evaluar";
        case CREAR: return "Crear";
    }
    return "";
}
string tipoComoTexto(TipoPregunta tipo) {
    return tipo == VERDADERO_FALSO ? "Verdadero/Falso" : "Seleccion Multiple";
}
//agrega una nueva pregunta y la guarda en archivo .txt
void agregarPregunta(Prueba& prueba) {
    if (prueba.cantidadPreguntas >= MAX_PREGUNTAS) {
    cout << "Limite de preguntas alcanzado.\n";
    return;
}
Pregunta nueva;
int tipoInt, nivelInt;
cout << "Tipo de pregunta (0 = Verdadero/Falso, 1 = Seleccion Multiple): ";
cin >> tipoInt;
nueva.tipo = (TipoPregunta)tipoInt;
cout << "Ingrese el enunciado de la pregunta:\n";
cin.ignore();
getline(cin, nueva.enunciado);
if (seUsoEnAnioAnterior(nueva.enunciado, prueba.anioExamen)) {
    cout << "Esta pregunta ya fue utilizada el anio pasado. No puede reutilizarse.\n";
    return;
}
cout << "Nivel taxonomico (0 = Recordar, 1 = Comprender, 2 = Aplicar, 3 = Analizar, 4 = Evaluar, 5 = Crear): ";
cin >> nivelInt;
nueva.nivel = (NivelBloom)nivelInt;
cout << "Anio de creacion: ";
cin >> nueva.anioCreacion;
cout << "Tiempo estimado para esta pregunta (en segundos): ";
cin >> nueva.tiempoEstimadoSegundos;
// Guardar en archivo principal
ofstream archivo(prueba.nombreArchivo, ios::app);
archivo << "Pregunta: " << nueva.enunciado << "\n";
archivo << "Tipo: " << (nueva.tipo == VERDADERO_FALSO ? "Verdadero/Falso" : "Seleccion Multiple") << "\n";
archivo << "Nivel: " << nivelInt << "\n";
archivo << "Anio de creacion: " << nueva.anioCreacion << "\n";
archivo << "Tiempo estimado: " << nueva.tiempoEstimadoSegundos << " segundos\n";
if (nueva.tipo == VERDADERO_FALSO) {
    nueva.opciones[0] = "Verdadero";
    nueva.opciones[1] = "Falso";
    nueva.cantidadOpciones = 2;
    cout << "¿Cual es la respuesta correcta? (0 = Verdadero, 1 = Falso): ";
    cin >> nueva.indiceRespuestaCorrecta;
    archivo << "Opciones: " << nueva.opciones[0] << ", " << nueva.opciones[1] << "\n";
    archivo << "Respuesta correcta: " << nueva.opciones[nueva.indiceRespuestaCorrecta] << "\n";
} else {
    cout << "¿Cuantas opciones tendra la pregunta?: ";
    cin >> nueva.cantidadOpciones;
    cin.ignore();

    for (int i = 0; i < nueva.cantidadOpciones; i++) {
        cout << "Opcion " << i << ": ";
        getline(cin, nueva.opciones[i]);
    }

    cout << "¿Cual es el numero de la opcion correcta?: ";
    cin >> nueva.indiceRespuestaCorrecta;
}

// Guardar en historial
std::ofstream historial("historial_preguntas.txt", std::ios::app);
if (historial.is_open()) {
    historial << "Pregunta: " << nueva.enunciado << "\n";
    historial << "Tipo: " << tipoComoTexto(nueva.tipo) << "\n";
    historial << "Nivel: " << nivelInt << "\n";
    historial << "Anio: " << nueva.anioCreacion << "\n";
    historial << "Tiempo: " << nueva.tiempoEstimadoSegundos << "s\n";
    for (int i = 0; i < nueva.cantidadOpciones; ++i) {
        historial << "  Opcion " << i << ": " << nueva.opciones[i] << "\n";
    }
    historial << "Respuesta correcta: Opcion " << nueva.indiceRespuestaCorrecta << "\n";
    historial << "-------------------------------\n";
    historial.close();
}
prueba.preguntas[prueba.cantidadPreguntas++] = nueva;
prueba.tiempoTotal += nueva.tiempoEstimadoSegundos;
cout << "Pregunta agregada exitosamente.\n\n";
}
//muestra la prueba actual que se esta ocupando
void mostrarPrueba(const Prueba& prueba) {
    if (prueba.nombreArchivo.empty()) {
        cout << "No se ha generado un examen aun.\n";
        return;
    }
    ifstream archivo(prueba.nombreArchivo);  // Abrir el archivo guardado
    if (!archivo) {
        cout << "Error al abrir el archivo del examen.\n";
        return;
    }
string linea;
while (getline(archivo, linea)) {
    cout << linea << endl;
}
archivo.close();
}
//muestra el promedio que debe tomar para realizar el examen
void mostrarTiempoTotal(const Prueba& prueba) {
    cout << "\nTiempo total estimado para completar el examen: "
    << prueba.tiempoTotal / 60 << " minutos.\n";
}
//edita una pregunta del examen en caso contrario dice que no hay preguntas para editar
void editarPregunta(Prueba& prueba) {
    if (prueba.cantidadPreguntas == 0) {
        cout << "No hay preguntas disponibles para editar.\n";
        return;
    }
    int numero;
    cout << "Ingrese el numero de la pregunta a editar (1 - " << prueba.cantidadPreguntas << "): ";
    cin >> numero;
    if (numero < 1 || numero > prueba.cantidadPreguntas) {
        cout << "Numero invalido.\n";
        return;
    }
    Pregunta& p = prueba.preguntas[numero - 1];
    cin.ignore();
    cout << "Nuevo enunciado: ";
    getline(cin, p.enunciado);
    cout << "Nuevo tipo de pregunta (0 = Verdadero/Falso, 1 = Seleccion Multiple): ";
    int tipoInt;
    cin >> tipoInt;
    p.tipo = (TipoPregunta)tipoInt;
    cout << "Nuevo nivel taxonómico (0 = Recordar, 1 = Comprender, 2 = Aplicar, 3 = Analizar, 4 = Evaluar, 5 = Crear): ";
    int nivelInt;
    cin >> nivelInt;
    p.nivel = (NivelBloom)nivelInt;
    cout << "Nuevo anio de creacion: ";
    cin >> p.anioCreacion;
    cout << "Nuevo tiempo estimado (en segundos): ";
    cin >> p.tiempoEstimadoSegundos;
    if (p.tipo == VERDADERO_FALSO) {
        p.opciones[0] = "Verdadero";
        p.opciones[1] = "Falso";
        p.cantidadOpciones = 2;
        cout << "Nueva respuesta correcta (0 = Verdadero, 1 = Falso): ";
        cin >> p.indiceRespuestaCorrecta;
    } else {
        cout << "Nueva cantidad de opciones: ";
        cin >> p.cantidadOpciones;
        cin.ignore();
        for (int i = 0; i < p.cantidadOpciones; ++i) {
            cout << "Opcion " << i << ": ";
            getline(cin, p.opciones[i]);
        }
        cout << "Nuevo indice de la respuesta correcta: ";
        cin >> p.indiceRespuestaCorrecta;
    }
    // Actualizar el archivo del examen
    ofstream archivo(prueba.nombreArchivo);
    if (!archivo) {
        cout << "Error al escribir el archivo del examen.\n";
        return;
    }
    archivo << "nombreExamen: " << prueba.nombreArchivo << "\n";
    archivo << "Asignatura: " << prueba.asignatura << "\n";
    archivo << "Anio del examen: " << prueba.anioExamen << "\n\n";
    for (int i = 0; i < prueba.cantidadPreguntas; ++i) {
        const Pregunta& q = prueba.preguntas[i];
        archivo << "Pregunta: " << q.enunciado << "\n";
        archivo << "Tipo: " << tipoComoTexto(q.tipo) << "\n";
        archivo << "Nivel: " << q.nivel << "\n";
        archivo << "Anio de creacion: " << q.anioCreacion << "\n";
        archivo << "Tiempo estimado: " << q.tiempoEstimadoSegundos << " segundos\n";
        archivo << "Opciones: ";
        for (int j = 0; j < q.cantidadOpciones; ++j) {
            archivo << q.opciones[j];
            if (j < q.cantidadOpciones - 1) archivo << ", ";
        }
        archivo << "\nRespuesta correcta: " << q.opciones[q.indiceRespuestaCorrecta] << "\n\n";
    }
    archivo.close();
    cout << "Pregunta actualizada exitosamente.\n";
}
//revisa las preguntas y muestra por nivel de taxonomia
void filtrarPorNivel(const Prueba& prueba, NivelBloom nivel) {
    bool encontrado = false;
    cout << " Preguntas con nivel " << nivelComoTexto(nivel) << ":\n";
    for (int i = 0; i < prueba.cantidadPreguntas; i++) {
        if (prueba.preguntas[i].nivel == nivel) {
            cout << "- " << prueba.preguntas[i].enunciado << endl;
            encontrado = true;
        }
    }
    if (!encontrado) {
        cout << "No se ha encontrado ninguna pregunta de este nivel taxonomico.\n";
        cout << "Presione Enter para volver al menu...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
}
//crea el archivo .txt del examen en donde se guarda el examen como tal con las preguntas que se utilizaran
void generarExamen(Prueba& prueba) {
    string nombreArchivo;
    char crearOtraPregunta;
    int anioExamen;
    string asignatura;
    // Solicitar nombre de archivo
    cout << "Ingrese nombre para el archivo de examen: ";
    cin >> nombreArchivo;
    nombreArchivo += ".txt";
    prueba.nombreArchivo = nombreArchivo;
    cout << "Ingrese la asignatura del examen: ";
    cin >> prueba.asignatura;
    cout << "Ingrese el anio del examen: ";
    cin >> prueba.anioExamen;
    ofstream archivo(nombreArchivo);
    cout << "Se le solicitara las preguntas: ";
    archivo << "nombreExamen: " << nombreArchivo << "\n";
    archivo << "Asignatura: " << prueba.asignatura << "\n";
    archivo << "Anio del examen: " << prueba.anioExamen << "\n\n";
    do {
        agregarPregunta(prueba);
        cout << "¿Desea agregar otra pregunta? (s/n): ";
        cin >> crearOtraPregunta;
    } while (crearOtraPregunta == 's' || crearOtraPregunta == 'S');
    if (!archivo) {
        cout << "Error al crear el archivo del examen.\n";
        return;
    }
    archivo.close();
    cout << "Examen generado exitosamente en el archivo '" << nombreArchivo << "'.\n";
    cout << "Presione Enter para volver al menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
//seleccionas una de las preguntas que contiene el examen para eliminar
void eliminarPregunta(Prueba& prueba) {
    if (prueba.cantidadPreguntas == 0) {
        cout << "No hay preguntas para eliminar.\n";
        return;
    }
    int numero;
    cout << "Ingrese el numero de la pregunta a eliminar (1 - " << prueba.cantidadPreguntas << "): ";
    cin >> numero;
    if (numero < 1 || numero > prueba.cantidadPreguntas) {
        cout << "Numero invalido.\n";
        return;
    }
    prueba.tiempoTotal -= prueba.preguntas[numero - 1].tiempoEstimadoSegundos;
    for (int i = numero - 1; i < prueba.cantidadPreguntas - 1; ++i) {
        prueba.preguntas[i] = prueba.preguntas[i + 1];
    }
    prueba.cantidadPreguntas--;
    ofstream archivo(prueba.nombreArchivo);
    if (!archivo) {
        cout << "Error al escribir el archivo del examen.\n";
        return;
    }
    archivo << "nombreExamen: " << prueba.nombreArchivo << "\n";
    archivo << "Asignatura: " << prueba.asignatura << "\n";
    archivo << "Anio del examen: " << prueba.anioExamen << "\n\n";
    for (int i = 0; i < prueba.cantidadPreguntas; ++i) {
        const Pregunta& p = prueba.preguntas[i];
        archivo << "Pregunta: " << p.enunciado << "\n";
        archivo << "Tipo: " << tipoComoTexto(p.tipo) << "\n";
        archivo << "Nivel: " << p.nivel << "\n";
        archivo << "Anio de creacion: " << p.anioCreacion << "\n";
        archivo << "Tiempo estimado: " << p.tiempoEstimadoSegundos << " segundos\n";
        archivo << "Opciones: ";
        for (int j = 0; j < p.cantidadOpciones; ++j) {
            archivo << p.opciones[j];
            if (j < p.cantidadOpciones - 1) archivo << ", ";
        }
        archivo << "\nRespuesta correcta: " << p.opciones[p.indiceRespuestaCorrecta] << "\n\n";
    }
    archivo.close();
    cout << "Pregunta eliminada exitosamente.\n";
}
//inicia el programa
int main() {
    Prueba miPrueba;
    int opcion;
    do {
        //antes de iniciar limpia la consola y despues muestra el menu
        limpiarConsola();
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
        limpiarConsola();
        //segun la opcion seleccionada actua el programa
        switch (opcion) {
            case 1:
                generarExamen(miPrueba);
                break;
            case 2:
                mostrarPrueba(miPrueba);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                break;
            case 3: {
                int nivel;
                cout << "Ingrese nivel (0=Recordar, 1=Comprender, 2=Aplicar, 3=Analizar, 4=Evaluar, 5=Crear): ";
                cin >> nivel;
                filtrarPorNivel(miPrueba, (NivelBloom)nivel);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                break;
            }
            case 4:
                agregarPregunta(miPrueba);
                break;
            case 5:
                editarPregunta(miPrueba);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                break;
            case 6:
                mostrarTiempoTotal(miPrueba);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                break;
            case 7:
            eliminarPregunta(miPrueba);
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
