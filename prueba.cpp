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
    int tiempoTotal = 0;
    Pregunta preguntas[MAX_PREGUNTAS];
    int cantidadPreguntas = 0;
    string nombreArchivo;
    int anioExamen;
    string asignatura;
public:
void limpiarConsola() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

}
