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

