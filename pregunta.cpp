#ifndef PREGUNTA_H
#define PREGUNTA_H
#include <iostream>
#include <fstream>
#include <string>
#include <banco.h>
using namespace std;
enum NivelBloom { RECORDAR, COMPRENDER, APLICAR, ANALIZAR, EVALUAR, CREAR };
enum TipoPregunta { VERDADERO_FALSO, SELECCION_MULTIPLE };
class Pregunta{
public:
    string enunciado;
    TipoPregunta tipo;
    NivelBloom nivel;
    int anioCreacion;
    int tiempoEstimadoSegundos;
    string opciones[5]; // máximo 5 opciones
    int cantidadOpciones;
    int indiceRespuestaCorrecta;
void agregarPregunta(const string& nombreArchivo, int anioExamen) {
        int tipoInt, nivelInt;
        cout << "Tipo de pregunta (0 = Verdadero/Falso, 1 = Seleccion Multiple): ";
        cin >> tipoInt;
        tipo = (TipoPregunta)tipoInt;
        cout << "Ingrese el enunciado de la pregunta:\n";
        cin.ignore();
        getline(cin, enunciado);
        if (seUsoEnAnioAnterior(enunciado, anioExamen)) {
            cout << "Esta pregunta ya fue utilizada el anio pasado. No puede reutilizarse.\n";
            return;
        }
        cout << "Nivel taxonomico (0 = Recordar, 1 = Comprender, 2 = Aplicar, 3 = Analizar, 4 = Evaluar, 5 = Crear): ";
        cin >> nivelInt;
        nivel = (NivelBloom)nivelInt;
        cout << "Anio de creacion: ";
        cin >> anioCreacion;
        cout << "Tiempo estimado para esta pregunta (en segundos): ";
        cin >> tiempoEstimadoSegundos;
        ofstream archivo(nombreArchivo, ios::app);
        archivo << "Pregunta: " << enunciado << "\n";
        archivo << "Tipo: " << (tipo == VERDADERO_FALSO ? "Verdadero/Falso" : "Seleccion Multiple") << "\n";
        archivo << "Nivel: " << nivelInt << "\n";
        archivo << "Anio de creacion: " << anioCreacion << "\n";
        archivo << "Tiempo estimado: " << tiempoEstimadoSegundos << " segundos\n";
        if (tipo == VERDADERO_FALSO) {
            opciones[0] = "Verdadero";
            opciones[1] = "Falso";
            cantidadOpciones = 2;
            cout << "¿Cual es la respuesta correcta? (0 = Verdadero, 1 = Falso): ";
            cin >> indiceRespuestaCorrecta;
            archivo << "Opciones: " << opciones[0] << ", " << opciones[1] << "\n";
            archivo << "Respuesta correcta: " << opciones[indiceRespuestaCorrecta] << "\n";
        } else {
            cout << "¿Cuantas opciones tendra la pregunta?: ";
            cin >> cantidadOpciones;
            cin.ignore();
            for (int i = 0; i < cantidadOpciones; i++) {
                cout << "Opcion " << i << ": ";
                getline(cin, opciones[i]);
            }
            cout << "¿Cual es el numero de la opcion correcta?: ";
            cin >> indiceRespuestaCorrecta;
        }
        ofstream historial("historial_preguntas.txt", ios::app);
        if (historial.is_open()) {
            historial << "Pregunta: " << enunciado << "\n";
            historial << "Tipo: " << tipoComoTexto(tipo) << "\n";
            historial << "Nivel: " << nivelInt << "\n";
            historial << "Anio: " << anioCreacion << "\n";
            historial << "Tiempo: " << tiempoEstimadoSegundos << "s\n";
            for (int i = 0; i < cantidadOpciones; ++i) {
                historial << "  Opcion " << i << ": " << opciones[i] << "\n";
            }
            historial << "Respuesta correcta: Opcion " << indiceRespuestaCorrecta << "\n";
            historial << "-------------------------------\n";
            historial.close();
        }
        cout << "Pregunta agregada exitosamente.\n\n";
    }
};
#endif
