#ifndef MATRIZAMISTAD_H
#define MATRIZAMISTAD_H

#include "Lista.h"

// Matriz de adyacencia N x N completa, guardada bit a bit.
// La fila i ocupa 'bytesPorFila' bytes; el bit j de esa fila vale 1 si i y j son amigos.
class MatrizAmistad {
private:
    unsigned char* bits;   // un solo bloque de n * bytesPorFila bytes
    int n;
    int bytesPorFila;

    bool indiceValido(int i) const { return i >= 0 && i < n; }
    unsigned char* fila(int i) { return bits + (long)i * bytesPorFila; }
    const unsigned char* fila(int i) const { return bits + (long)i * bytesPorFila; }

public:
    MatrizAmistad();
    MatrizAmistad(const MatrizAmistad& otra);              // constructor de copia
    MatrizAmistad& operator=(const MatrizAmistad& otra);
    ~MatrizAmistad();

    void inicializar(int cantidadUsuarios);     // todos los bits en 0

    bool sonAmigos(int i, int j) const;
    bool agregarAmistad(int i, int j);          // false si i == j o los índices no existen
    int amigosEnComun(int i, int j) const;      // AND de las dos filas + conteo de bits
    int cantidadAmigos(int i) const;
    void amigosDe(int i, Lista<int>& destino) const;

    int tamano() const { return n; }
    long memoria() const;

    static int contarBits(unsigned char byte);
};

#endif