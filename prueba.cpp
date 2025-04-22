#ifndef PRUEBA_H
#define PRUEBA_H
#include "pregunta.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdlib>
using namespace std;
class Prueba{
private:
    Pregunta preguntas[MAX_PREGUNTAS];
    int cantidadPreguntas = 0;
    string asignatura;
public:
    int tiempoTotal = 0;
    string nombreArchivo;
    int anioExamen;
void limpiarConsola() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

}
