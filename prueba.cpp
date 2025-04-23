#ifndef PRUEBA_H
#define PRUEBA_H 
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdlib>
#include "pregunta.h"
using namespace std;
class Prueba{
    static const int MAX_PREGUNTAS = 100;
private:
    Pregunta preguntas[MAX_PREGUNTAS];
    int cantidadPreguntas = 0;
    string asignatura;
public:
    int tiempoTotal = 0;
    string nombreArchivo;
    int anioExamen;

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
void limpiarConsola() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
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
    Pregunta p;
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
        prueba.cantidadPreguntas++;
        p.agregarPregunta(nombreArchivo, anioExamen);
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
};
#endif
