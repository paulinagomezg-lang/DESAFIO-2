#include "Usuario.h"
#include "Cadena.h"
#include "Recursos.h"
#include <cstdlib>

Usuario** Usuario::usuarios = nullptr;

int Usuario::cantidad = 0;

int Usuario::capacidad = 0;

MatrizAmistad Usuario::matriz;

Usuario::Usuario()

    : nombreUsuario(Cadena::copiar("")), contrasena(Cadena::copiar("")),
    correo(Cadena::copiar("")), ciudad(Cadena::copiar("")), pais(Cadena::copiar("")),

    indice(-1) {}

Usuario::Usuario(const char* nombreUsuario_, const char* contrasena_, const char* correo_,
                 const Fecha& fechaNacimiento_, const char* ciudad_, const char* pais_,
                 const Fecha& fechaInscripcion_)
    : nombreUsuario(Cadena::copiar(nombreUsuario_)), contrasena(Cadena::copiar(contrasena_)),
    correo(Cadena::copiar(correo_)), ciudad(Cadena::copiar(ciudad_)), pais(Cadena::copiar(pais_)),
    fechaNacimiento(fechaNacimiento_), fechaInscripcion(fechaInscripcion_), indice(-1) {}

void Usuario::copiarDatosDe(const Usuario& otro) {
    nombreUsuario = Cadena::copiar(otro.nombreUsuario);

    contrasena = Cadena::copiar(otro.contrasena);

    correo = Cadena::copiar(otro.correo);

    ciudad = Cadena::copiar(otro.ciudad);

    pais = Cadena::copiar(otro.pais);

    fechaNacimiento = otro.fechaNacimiento;

    fechaInscripcion = otro.fechaInscripcion;

    indice = otro.indice;

    solicitudesRecibidas = otro.solicitudesRecibidas;   // Lista ya hace copia profunda
}

void Usuario::liberar() {

    delete[] nombreUsuario;

    delete[] contrasena;

    delete[] correo;

    delete[] ciudad;

    delete[] pais;
}

Usuario::Usuario(const Usuario& otro) {

    copiarDatosDe(otro);
}

Usuario& Usuario::operator=(const Usuario& otro) {
    if (this != &otro) {       // evita destruirse a sí mismo en "a = a"
        liberar();
        copiarDatosDe(otro);
    }
    return *this;
}

Usuario::~Usuario() {
    liberar();
}

void Usuario::setCiudad(const char* nuevaCiudad) {
    char* copia = Cadena::copiar(nuevaCiudad);
    delete[] ciudad;
    ciudad = copia;
}

void Usuario::setPais(const char* nuevoPais) {
    char* copia = Cadena::copiar(nuevoPais);
    delete[] pais;
    pais = copia;
}

bool Usuario::verificarContrasena(const char* intento) const {
    return Cadena::iguales(contrasena, intento);
}

bool Usuario::operator<(const Usuario& otro) const {
    return Cadena::comparar(nombreUsuario, otro.nombreUsuario) < 0;
}

bool Usuario::operator==(const Usuario& otro) const {
    return Cadena::iguales(nombreUsuario, otro.nombreUsuario);
}

bool Usuario::operator==(const char* nombre) const {
    return Cadena::iguales(nombreUsuario, nombre);
}

std::ostream& operator<<(std::ostream& salida, const Usuario& u) {
    salida << u.nombreUsuario << " (" << u.ciudad << ", " << u.pais << ")"
           << " - nacimiento: " << u.fechaNacimiento
           << " - en Ude@Book desde: " << u.fechaInscripcion;
    return salida;
}

void Usuario::agregarUsuario(Usuario* nuevo) {
    if (cantidad == capacidad) {                       // arreglo lleno: hay que crecer
        int nuevaCapacidad = (capacidad == 0) ? 16 : capacidad * 2;
        Usuario** nuevoArreglo = new Usuario*[nuevaCapacidad];
        for (int i = 0; i < cantidad; i++) {           // se copian punteros, no usuarios
            nuevoArreglo[i] = usuarios[i];
            Recursos::sumar();
        }
        delete[] usuarios;                             // borra el arreglo viejo, NO los usuarios
        usuarios = nuevoArreglo;
        capacidad = nuevaCapacidad;
    }
    nuevo->indice = cantidad;
    usuarios[cantidad++] = nuevo;
}

Usuario* Usuario::obtener(int i) {
    if (i < 0 || i >= cantidad) return nullptr;
    return usuarios[i];
}

void Usuario::liberarTodos() {
    for (int i = 0; i < cantidad; i++) delete usuarios[i];   // cada usuario
    delete[] usuarios;                                        // el arreglo de punteros
    usuarios = nullptr;
    cantidad = 0;
    capacidad = 0;
    matriz.inicializar(0);
}

// ---------------- Merge Sort ----------------

void Usuario::mezclar(Usuario** arreglo, Usuario** auxiliar, int izquierda, int medio, int derecha) {
    int i = izquierda;      // recorre la mitad izquierda  [izquierda .. medio]
    int j = medio + 1;      // recorre la mitad derecha    [medio+1 .. derecha]
    int k = izquierda;      // posición donde se escribe en el auxiliar

    while (i <= medio && j <= derecha) {
        Recursos::sumar();
        if (!(*arreglo[j] < *arreglo[i])) auxiliar[k++] = arreglo[i++];   // izquierdo <= derecho
        else auxiliar[k++] = arreglo[j++];
    }
    while (i <= medio)   { auxiliar[k++] = arreglo[i++]; Recursos::sumar(); }   // sobrantes izquierda
    while (j <= derecha) { auxiliar[k++] = arreglo[j++]; Recursos::sumar(); }   // sobrantes derecha

    for (k = izquierda; k <= derecha; k++) {        // se devuelve el tramo ya mezclado
        arreglo[k] = auxiliar[k];
        Recursos::sumar();
    }
}

void Usuario::mergeSort(Usuario** arreglo, Usuario** auxiliar, int izquierda, int derecha) {
    if (izquierda >= derecha) return;                       // 0 o 1 elemento: ya está ordenado
    int medio = izquierda + (derecha - izquierda) / 2;
    mergeSort(arreglo, auxiliar, izquierda, medio);         // ordena la mitad izquierda
    mergeSort(arreglo, auxiliar, medio + 1, derecha);       // ordena la mitad derecha
    mezclar(arreglo, auxiliar, izquierda, medio, derecha);  // une las dos mitades
}

void Usuario::ordenar() {
    if (cantidad > 1) {
        Usuario** auxiliar = new Usuario*[cantidad];        // se reserva una sola vez
        mergeSort(usuarios, auxiliar, 0, cantidad - 1);
        delete[] auxiliar;
    }
    for (int i = 0; i < cantidad; i++) {                    // cada uno queda con su nueva posición
        usuarios[i]->indice = i;
        Recursos::sumar();
    }
}

// ---------------- Duplicados y búsqueda binaria ----------------

int Usuario::eliminarDuplicados() {
    if (cantidad < 2) return 0;
    int escritura = 1;                                      // dónde va el siguiente usuario único
    for (int lectura = 1; lectura < cantidad; lectura++) {
        Recursos::sumar();
        if (*usuarios[lectura] == *usuarios[escritura - 1]) {
            delete usuarios[lectura];                       // repetido: se conserva el primero
        } else {
            usuarios[escritura] = usuarios[lectura];
            usuarios[escritura]->indice = escritura;
            escritura++;
        }
    }
    int eliminados = cantidad - escritura;
    cantidad = escritura;
    return eliminados;
}

int Usuario::buscar(const char* nombre) {
    int izquierda = 0, derecha = cantidad - 1;
    while (izquierda <= derecha) {
        Recursos::sumar();
        int medio = izquierda + (derecha - izquierda) / 2;
        int comparacion = Cadena::comparar(nombre, usuarios[medio]->nombreUsuario);
        if (comparacion == 0) return medio;                 // encontrado
        if (comparacion < 0) derecha = medio - 1;           // está en la mitad izquierda
        else izquierda = medio + 1;                         // está en la mitad derecha
    }
    return -1;                                              // no existe
}

//-------------------- Amistades y solis -------------------

void Usuario::prepararMatriz() {
    matriz.inicializar(cantidad);
}

bool Usuario::esAmigoDe(const Usuario& otro) const {
    return matriz.sonAmigos(indice, otro.indice);
}

int Usuario::cantidadAmigos() const {
    return matriz.cantidadAmigos(indice);
}

void Usuario::obtenerAmigos(Lista<int>& destino) const {
    matriz.amigosDe(indice, destino);
}

bool Usuario::tieneSolicitudDe(int indiceEmisor) const {
    return solicitudesRecibidas.buscar(indiceEmisor) != -1;
}

void Usuario::agregarSolicitudRecibida(int indiceEmisor) {
    if (indiceEmisor == indice) return;                     // de sí mismo: no
    if (matriz.sonAmigos(indice, indiceEmisor)) return;     // ya son amigos: no
    if (tieneSolicitudDe(indiceEmisor)) return;             // repetida: no
    solicitudesRecibidas.agregar(indiceEmisor);
}

Usuario::ResultadoSolicitud Usuario::enviarSolicitud(Usuario& receptor) {
    if (receptor.indice == indice) return ES_EL_MISMO_USUARIO;
    if (esAmigoDe(receptor)) return YA_SON_AMIGOS;
    if (receptor.tieneSolicitudDe(indice)) return SOLICITUD_YA_ENVIADA;
    if (tieneSolicitudDe(receptor.indice)) return EL_OTRO_YA_TE_ENVIO;
    receptor.solicitudesRecibidas.agregar(indice);          // queda en la lista del RECEPTOR
    return SOLICITUD_ENVIADA;
}

bool Usuario::aceptarSolicitud(int posicion) {
    if (posicion < 0 || posicion >= solicitudesRecibidas.tamano()) return false;
    int emisor = solicitudesRecibidas[posicion];
    solicitudesRecibidas.eliminarEn(posicion);              // sale de pendientes
    return matriz.agregarAmistad(indice, emisor);           // enciende los 2 bits
}

// ---------------- Fotos de perfil ponderadas ----------------

// Sub-ruta fija del enunciado: /users/(sub-ruta-X-fija)/usuario/...   AJUSTAR al valor del PDF
static const char* RUTA_BASE = "/users/sub-ruta-X";

// SUPUESTO: el peso depende de cuántos amigos en común hay con ese amigo.
// Si el enunciado define otra regla, solo se cambia pesoAmistad().
static const float PESO_BAJO  = 1.0f;    // 0 amigos en común
static const float PESO_MEDIO = 2.5f;    // 1 a 5 amigos en común
static const float PESO_ALTO  = 3.5f;    // 6 o más amigos en común
static const int LIMITE_MEDIO = 5;

void Usuario::rutaFotoPerfil(char* destino, int tamano) const {
    destino[0] = '\0';
    Cadena::anexar(destino, tamano, RUTA_BASE);
    Cadena::anexar(destino, tamano, "/");
    Cadena::anexar(destino, tamano, nombreUsuario);
    Cadena::anexar(destino, tamano, "/profile.png");
}

float Usuario::pesoAmistad(int usuario, int amigo) {
    int comunes = matriz.amigosEnComun(usuario, amigo);
    if (comunes == 0) return PESO_BAJO;
    if (comunes <= LIMITE_MEDIO) return PESO_MEDIO;
    return PESO_ALTO;
}

int Usuario::elegirAmigosPonderados(int* elegidos, int k) const {
    Lista<int> amigos;
    obtenerAmigos(amigos);
    int activos = amigos.tamano();
    if (activos == 0 || k <= 0) return 0;

    // 1. Peso de cada amigo y suma total
    float* pesos = new float[activos];
    float total = 0.0f;
    for (int i = 0; i < activos; i++) {
        pesos[i] = pesoAmistad(indice, amigos[i]);
        total += pesos[i];
        Recursos::sumar();
    }

    int cuantos = 0;
    while (cuantos < k && activos > 0) {
        // 2. Ruleta: número al azar en [0, total) y se recorren los pesos acumulados
        float azar = (float)rand() / ((float)RAND_MAX + 1.0f) * total;
        float acumulado = 0.0f;
        int elegido = activos - 1;            // por si el redondeo deja el azar al final
        for (int i = 0; i < activos; i++) {
            Recursos::sumar();
            acumulado += pesos[i];
            if (azar < acumulado) {
                elegido = i;
                break;
            }
        }
        elegidos[cuantos++] = amigos[elegido];

        // 3. Se saca del sorteo: el último activo ocupa su lugar (O(1), sin desplazar)
        total -= pesos[elegido];
        pesos[elegido] = pesos[activos - 1];
        amigos[elegido] = amigos[activos - 1];
        activos--;
    }

    delete[] pesos;
    return cuantos;
}
