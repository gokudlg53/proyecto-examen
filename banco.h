#ifndef BANCO_H
#define BANCO_H
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
class BancoDePreguntas { 
private:
    string archivoHistorial;
public:
    BancoDePreguntas(string nombreArchivo = "historial_preguntas.txt") : archivoHistorial(nombreArchivo) {}

    bool seUsoEnAnioAnterior(const string& enunciado, int anioActual) {
        ifstream historial(archivoHistorial);
        if (!historial.is_open()) return false;
        string linea, preguntaEncontrada;
        int anioEncontrado = -1;
         while(getline(historial, linea)) {
            if (linea.find("Pregunta: ") == 0) {
                preguntaEncontrada = linea.substr(10);
                } else if (linea.find("Año: ") == 0) {
                anioEncontrado = stoi(linea.substr(5));
                if (preguntaEncontrada == enunciado && anioEncontrado == anioActual - 1) {
                    historial.close();
                    return true;
                }
            }
        }
        return false;
    }
};
#endif
