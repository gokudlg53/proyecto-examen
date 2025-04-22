class BancoDePreguntas { //el Benja es putisimo 
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
        historial.close();
        return false;
    }
     void registrarPregunta(const Pregunta& p) {
        ofstream historial(archivoHistorial, ios::app);
        if (!historial) {
            cerr << "Error al abrir el archivo de historial" << endl;
            return;
        }

        historial << "Pregunta: " << p.enunciado << "s\n"
                  << "Tipo: " << tipoComoTexto(p.tipo) << "s\n"
                  << "Nivel: " << p.nivel << "s\n"
                  << "Año: " << p.anioCreacion << "s\n"
                  << "Tiempo: " << p.tiempoEstimadoSegundos << "s\n";

        for (int i = 0; i < p.cantidadOpciones; ++i) {
            historial << "  Opción " << i << ": " << p.opciones[i] << "s\n";
    
