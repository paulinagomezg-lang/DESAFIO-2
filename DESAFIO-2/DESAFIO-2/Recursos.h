#ifndef RECURSOS_H
#define RECURSOS_H

class Recursos {
    static long iteraciones;
public:
    static void reiniciar() { iteraciones = 0; }
    static void sumar(long n = 1) { iteraciones += n; }
    static long obtenerIteraciones() { return iteraciones; }
};

#endif // RECURSOS_H
