#ifndef CADENA_H
#define CADENA_H

class Cadena {
public:
    static int longitud(const char* texto);

    static char* copiar(const char* texto);

    static int comparar(const char* a, const char* b);

    static bool iguales(const char* a, const char* b);

    static int dividir(char* linea, char separador, char** campos, int maxCampos);

    static void quitarFinDeLinea(char* texto);

    static void anexar(char* destino, int tamano, const char* texto);
};

#endif // CADENA_H
