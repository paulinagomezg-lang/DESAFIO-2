#include <iostream>
using namespace std;
#include"FOTOS.h"

//Parte fija de la ruta. es la misma hasta que el docente la cambie

const char* const Foto::SUB_RUTA = "Ude@Book";

//Funciones auxiliares: cuenta los caracteres hasta el \0

int Foto::Longitud(const char* Cadena){
    if (Cadena == nullptr) return 0;
    int G = 0;
    while (Cadena[G] != '\0') {
        G++;
    }
    return G;
}

//Reserva un tamaño exacto y copia el texto

char* Foto::Copiar_Texto(const char* Origen){
    int G = Longitud(Origen);
    char* PUNTERO_BLOQUE_MEMORIA = new char [G+1];
    for (int i = 0; i < G; i++){
        PUNTERO_BLOQUE_MEMORIA[i] = Origen[i];
    }
    PUNTERO_BLOQUE_MEMORIA[G] ='\0';
    return PUNTERO_BLOQUE_MEMORIA;
}

//compara dos textos caracter a caracter y si son iguales lanza true

bool Foto::Comparacion_Caracter(const char* A, const char*B){
    if (A == nullptr || B == nullptr) return false;
    int i = 0;
    while (A[i] != '\0' && A[i] == B[i]) {
        i++;
    }
    return A[i] == B[i];
}

//Constructor que crea la foto (Se necesita para poder hacer newfoto[n]

Foto::Foto(){
    Nombre = Copiar_Texto(""); //Nombre vacio
    Tamanio = 0;
    Likes = 0;
}

//Constructor de datos que necesitamos

Foto::Foto(const char* Nuevo_Nombre, const Fecha& Nueva_Fecha, unsigned int Nuevo_Tamanio, unsigned int Nuevos_Likes){
    Nombre = Copiar_Texto(Nuevo_Nombre);
    Fecha_de_Captura = Nueva_Fecha;
    Tamanio = Nuevo_Tamanio;
    Likes = Nuevos_Likes;
}

//Constructor de copia para la foto nueva que recibe su propio bloque para el nombre

Foto::Foto(const Foto& Otra){
    Nombre = Copiar_Texto(Otra.Nombre);
    Fecha_de_Captura = Otra.Fecha_de_Captura;
    Tamanio = Otra.Tamanio;
    Likes = Otra.Likes;
}

//Destructor para liberar el bloque del nombre de la foto

Foto::~Foto(){
    delete[]Nombre;
}

//Sobrecarga de fotos

Foto& Foto::operator =(const Foto& Otra){
    if (this != &Otra){
        char* Nuevo_Nombre = Copiar_Texto(Otra.Nombre);
        delete[]Nombre;

        Nombre = Nuevo_Nombre;
        Fecha_de_Captura = Otra.Fecha_de_Captura;
        Tamanio = Otra.Tamanio;
        Likes = Otra.Likes;
    }
    return *this;
}

//si hay dos fotos con el mismo nombre son iguales

bool Foto::operator==(const Foto& Otra) const{
    return Comparacion_Caracter(Nombre, Otra.Nombre);
}

//setters por si el dato no es valido retornar false y no cambiar nada

bool Foto::setNombre(const char* Nuevo_Nombre){
    if (!Nombre_Valido(Nuevo_Nombre)) return false;
    char* Copia = Copiar_Texto(Nuevo_Nombre);
    delete[] Nombre;
    Nombre = Copia;
    return true;
}

bool Foto::setFecha_de_Captura(const Fecha& Nueva_Fecha){
    if (!Nueva_Fecha.esValida()) return false;
    Fecha_de_Captura = Nueva_Fecha;
    return true;
}

bool Foto::setTamanio(unsigned int Nuevo_Tamanio){
    if (Nuevo_Tamanio == 0) return false; //las fotos no pueden pesar 0bytes
    Tamanio = Nuevo_Tamanio;
    return true;
}

void Foto::setLikes(unsigned int Nuevos_Likes){
    Likes = Nuevos_Likes;
}

//parte de likes

void Foto::Dar_Like(){
    Likes++;
}

void Foto::Quitar_Like(){
    if(Likes > 0){ //Nunca va a ser 0
        Likes--;
    }
}

//Parte de Validacion de la parte de fotos

bool Foto::Nombre_Valido(const char* Revisar_Nombre){
    if (Revisar_Nombre == nullptr || Revisar_Nombre[0] == '\0') return false;
    for(int i = 0; Revisar_Nombre[i] != '\0'; i++){
        char C = Revisar_Nombre[i];
        if (C == '|' || C == '/' || C == ' '){
            return false; //aca rompe el archivo o la ruta
        }
    }
    return true;
}

bool Foto::Es_Valida() const{
    return Nombre_Valido(Nombre) && Fecha_de_Captura.esValida() && Tamanio > 0;
}

//impresion de las rutas como lo puso el profe

void Foto::Mostrar_Ruta(ostream& Salida, const char* Usuario, const char* Album) const{
    Salida << "/users/" << SUB_RUTA << "/" << Usuario << "/albums/" << Album << "/" << Nombre << ".png";
}

void Foto::Mostrar_Ruta_Perfil(ostream& Salida, const char* Usuario){
    Salida << "/users/" << SUB_RUTA << "/" << Usuario << "/albums/profile.png";
}

//consulta de cuantos bytes consume esta foto en la memoria

unsigned int Foto::Memoria() const{
    return sizeof(Foto) + (Longitud(Nombre)+1); //puntero, bloque, memria y likes y el bloqie de nombre
}

//Funcion amiga

ostream& operator << (ostream& Salida, const Foto& F){
    Salida << F.Nombre << "| Captura: " << F.Fecha_de_Captura << "|Tamanio: " << F.Tamanio << "bytes" << "| Likes: "<< F.Likes;
    return Salida;
}




