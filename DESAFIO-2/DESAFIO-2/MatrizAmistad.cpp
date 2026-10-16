#include "MatrizAmistad.h"
#include "Recursos.h"

MatrizAmistad::MatrizAmistad() : bits(nullptr), n(0), bytesPorFila(0) {}

MatrizAmistad::MatrizAmistad(const MatrizAmistad& otra)

    : bits(nullptr), n(otra.n), bytesPorFila(otra.bytesPorFila) {

    long total = (long)n * bytesPorFila;

    if (total > 0) {

        bits = new unsigned char[total];

        for (long k = 0; k < total; k++) {

            bits[k] = otra.bits[k];

            Recursos::sumar();
        }
    }
}

MatrizAmistad& MatrizAmistad::operator=(const MatrizAmistad& otra) {

    if (this != &otra) {

        delete[] bits;

        n = otra.n;

        bytesPorFila = otra.bytesPorFila;

        long total = (long)n * bytesPorFila;

        bits = (total > 0) ? new unsigned char[total] : nullptr;

        for (long k = 0; k < total; k++) {

            bits[k] = otra.bits[k];

            Recursos::sumar();
        }
    }
    return *this;
}

MatrizAmistad::~MatrizAmistad() {

    delete[] bits;
}

void MatrizAmistad::inicializar(int cantidadUsuarios) {

    delete[] bits;

    n = (cantidadUsuarios > 0) ? cantidadUsuarios : 0;

    bytesPorFila = (n + 7) / 8;                 // redondeo hacia arriba

    long total = (long)n * bytesPorFila;

    bits = (total > 0) ? new unsigned char[total]() : nullptr;   // () deja todo en 0
}

bool MatrizAmistad::sonAmigos(int i, int j) const {

    if (!indiceValido(i) || !indiceValido(j)) return false;

    return (fila(i)[j >> 3] & (1 << (j & 7))) != 0;
}

bool MatrizAmistad::agregarAmistad(int i, int j) {

    if (!indiceValido(i) || !indiceValido(j) || i == j) return false;

    fila(i)[j >> 3] |= (unsigned char)(1 << (j & 7));

    fila(j)[i >> 3] |= (unsigned char)(1 << (i & 7));

    return true;
}

int MatrizAmistad::contarBits(unsigned char byte) {

    int cuenta = 0;

    while (byte != 0) {

        byte &= (unsigned char)(byte - 1);      // apaga el 1 más bajo

        cuenta++;

        Recursos::sumar();
    }
    return cuenta;
}

int MatrizAmistad::amigosEnComun(int i, int j) const {

    if (!indiceValido(i) || !indiceValido(j)) return 0;

    const unsigned char* a = fila(i);

    const unsigned char* b = fila(j);

    int total = 0;

    for (int k = 0; k < bytesPorFila; k++) {

        Recursos::sumar();

        unsigned char comunes = a[k] & b[k];     // 8 usuarios comparados en una operación

        if (comunes != 0) total += contarBits(comunes);
    }
    return total;
}

int MatrizAmistad::cantidadAmigos(int i) const {

    if (!indiceValido(i)) return 0;

    const unsigned char* f = fila(i);

    int total = 0;

    for (int k = 0; k < bytesPorFila; k++) {

        Recursos::sumar();

        if (f[k] != 0) total += contarBits(f[k]);
    }
    return total;
}

void MatrizAmistad::amigosDe(int i, Lista<int>& destino) const {

    if (!indiceValido(i)) return;

    const unsigned char* f = fila(i);

    for (int k = 0; k < bytesPorFila; k++) {

        Recursos::sumar();

        if (f[k] == 0) continue;                // se saltan 8 usuarios de una vez

        for (int b = 0; b < 8; b++) {

            Recursos::sumar();

            if (f[k] & (1 << b)) destino.agregar(k * 8 + b);
        }
    }
}

long MatrizAmistad::memoria() const {

    return (long)sizeof(*this) + (long)n * bytesPorFila;
}