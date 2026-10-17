#include <iostream>
using namespace std;
#include"ALBUMES.h"

int Album::Longitud(const char* Cadena){ //cuenta los caracteres hasta \0
    if (Cadena == nullptr) return 0;
    int G = 0;
    while (Cadena[G] != '\0') {
        G++;
    }
    return G;
}

//Reservar un bloque de tamaño exacto y copia el texto

char* Album::Copiar_Texto(const char* Origen){
    int G = Longitud(Origen);
    char* PUNTERO_BLOQUE_MEMORIA = new char[G + 1];

    for (int i = 0; i < G; i++){
        PUNTERO_BLOQUE_MEMORIA[i] = Origen[i];
    }
    PUNTERO_BLOQUE_MEMORIA[G] = '\0';
    return PUNTERO_BLOQUE_MEMORIA;
}

//comparacion de dos textos para ver si son iguales

bool Album::Comparacion_Caracter(const char* A, const char* B){
    if (A == nullptr ||B == nullptr) return false;
    int i = 0;
    while (A[i] != '\0' && A[i] == B[i]) {
        i++;
    }
    return A[i] == B[i];
}

//constructor que crea el album

Album::Album(){
    Nombre = Copiar_Texto("");
    Privacidad = PRIVADO;  //por seguridad por si las quiere de una u otra forma
}

//constructor para los datos

Album::Album(const char* Nuevo_Nombre, int Nueva_Privacidad){
    Nombre = Copiar_Texto(Nuevo_Nombre);
    if (Privacidad_Valida(Nueva_Privacidad)){
        Privacidad = Nueva_Privacidad;
    }
    else{
        Privacidad = PRIVADO; //un valor invalido queda privado para no exponer las fotos
    }
}

//constructor  que hace una copia profunda del nombre y de las fotos

Album::Album(const Album& Otro){
    Nombre = Copiar_Texto(Otro.Nombre);
    Privacidad = Otro.Privacidad;
    Fotos = Otro.Fotos;
}

//destructor para liberar el nombre

Album::~Album(){
    delete[] Nombre;
}

//sobrecarga que crea una copia nueva y despues se libera la anterior

Album& Album::operator=(const Album& Otro){
    if (this != &Otro){
        char* Nuevo_Nombre = Copiar_Texto(Otro.Nombre);
        delete[] Nombre;

        Nombre = Nuevo_Nombre;
        Privacidad = Otro.Privacidad;
        Fotos = Otro.Fotos; //operator de lista
    }
    return *this;
}

//Por si dos albumes tienen el mismo nombre

bool Album::operator==(const Album& Otro) const{
    return Comparacion_Caracter(Nombre, Otro.Nombre);
}

//setters

bool Album::setNombre(const char* Nuevo_Nombre){
    if (!Nombre_Valido(Nuevo_Nombre)) return false;

    char* Copia = Copiar_Texto(Nuevo_Nombre);
    delete[] Nombre;
    Nombre = Copia;
    return true;
}

bool Album::setPrivacidad(int Nueva_Privacidad){
    if (!Privacidad_Valida(Nueva_Privacidad)) return false;
    Privacidad = Nueva_Privacidad;
    return true;
}

//Agrega fotos validas con su orden de nombre, fecha y tamaño correcto

bool Album::Agregar_Foto(const Foto& Nueva_Foto){
    if (!Nueva_Foto.Es_Valida()) return false;
    Fotos.agregar(Nueva_Foto);
    return true;
}

bool Album::Indice_Valido(int i) const{
    return i >= 0 && i < Fotos.tamano();
}

//permisos de visualizacion

bool Album::Puede_Ver(bool Es_Dueno, bool Es_Amigo) const{
    if (Es_Dueno) return true; //el dueño puede ver
    if (Privacidad == PUBLICO) return true; //publico puede ser cualquiera
    if (Privacidad == AMIGOS) return Es_Amigo; //solo si es amigo puede ver
    return false;
}

//CARRUSEL para poder ver una por una las fotos donde cumpleuna manera circular donde por el documento cumple la visualizacion de perfil

int Album::Siguiente(int Actual) const{
    int P = Fotos.tamano(); //P almacena cantidad de fotos
    if (P == 0) return -1;

    return (Actual + 1) % P; //de la ultima vuelve a la primera
}

int Album::Anterior(int Actual) const{
    int P = Fotos.tamano();
    if (P == 0) return -1;

    return (Actual -1 + P) % P; //de 0 pasa a la ultima
}

//validacion de nombre sin | ,ni / ni espacios porque rompe la ruta

bool Album::Nombre_Valido(const char* Revisar_Nombre){
    if (Revisar_Nombre == nullptr || Revisar_Nombre[0] == '\0') return false;
    for (int i = 0; Revisar_Nombre[i] != '\0'; i++){
        char C = Revisar_Nombre[i];
        if (C == '|' || C == '/' || C == ' '){
            return false;
        }
    }
    return true;
}

bool Album::Privacidad_Valida(int Revisar_Privacidad){
    return Revisar_Privacidad >= PUBLICO && Revisar_Privacidad <= AMIGOS;
}

//Rutas que se imprimen por partes sin reservar memoria

void Album::Mostrar_Ruta(ostream& Salida, const char* Usuario) const{
    Salida << "/users/" << Foto::SUB_RUTA << "/" << Usuario << "/albums/" << Nombre;
}

void Album::Mostrar_Ruta_Foto(ostream& Salida, int i, const char* Usuario) const{
    Fotos[i].Mostrar_Ruta(Salida, Usuario, Nombre);
}

//consultas

const char* Album::Texto_Privacidad(int Tipo){
    if (Tipo == PUBLICO) return "Publico";
    if (Tipo == AMIGOS) return "Amigos";
    return "Privado";
}

//memoria total del album obje+nom+arreg de lista+nom cada foto

unsigned int Album:: Memoria() const{
    unsigned int Total = sizeof(Album) + (Longitud(Nombre) + 1) + Fotos.memoria();

    //cada foto solo se le suma la parte dinamica que es el nombre ya que size ya esta en foto.memoria

    for (int i = 0; i < Fotos.tamano(); i++){
        Total += Fotos[i].Memoria() - sizeof(Foto);
    }
    return Total;
}

//Funcion amiga

ostream& operator<<(ostream& Salida, const Album& A){
    Salida <<A.Nombre << "("  << Album::Texto_Privacidad(A.Privacidad) << ", " << A.Fotos.tamano() << " fotos)";
    return Salida;
}
