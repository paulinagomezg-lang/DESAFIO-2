#include "Cadena.h"
#include "Recursos.h"

int Cadena::longitud(const char* texto) {
    if (texto == nullptr) return 0;
    int n = 0;
    while (texto[n] != '\0') {
        n++;
        Recursos::sumar();
    }
    return n;
}

char* Cadena::copiar(const char* texto) {
    int n = longitud(texto);
    char* copia = new char[n + 1];       // +1 para el '\0'
    for (int i = 0; i < n; i++) {
        copia[i] = texto[i];
        Recursos::sumar();
    }
    copia[n] = '\0';
    return copia;
}

int Cadena::comparar(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && a[i] == b[i]) {
        i++;
        Recursos::sumar();
    }
    return (unsigned char)a[i] - (unsigned char)b[i];
}

bool Cadena::iguales(const char* a, const char* b) {
    return comparar(a, b) == 0;
}

int Cadena::dividir(char* linea, char separador, char** campos, int maxCampos) {
    if (maxCampos <= 0) return 0;
    int cuantos = 1;
    campos[0] = linea;                       // el primer campo empieza al inicio
    for (int i = 0; linea[i] != '\0' && cuantos < maxCampos; i++) {
        Recursos::sumar();
        if (linea[i] == separador) {
            linea[i] = '\0';                 // corta el campo anterior
            campos[cuantos++] = &linea[i + 1];   // el siguiente empieza justo después
        }
    }
    return cuantos;
}

void Cadena::quitarFinDeLinea(char* texto) {
    int n = longitud(texto);
    while (n > 0 && (texto[n - 1] == '\r' || texto[n - 1] == '\n')) {
        texto[--n] = '\0';
    }
}

void Cadena::anexar(char* destino, int tamano, const char* texto) {
    int i = longitud(destino);               // dónde termina lo que ya hay
    int j = 0;
    while (texto[j] != '\0' && i < tamano - 1) {   // tamano - 1 deja espacio al '\0'
        destino[i++] = texto[j++];
        Recursos::sumar();
    }
    destino[i] = '\0';
}