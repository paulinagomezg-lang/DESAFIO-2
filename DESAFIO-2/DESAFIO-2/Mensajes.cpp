#include <iostream>
using namespace std;
#include "MENSAJES.h"

//Funciones auxiliares: Base sin utilizar string.

int Mensaje::Longitud(const char* Cadena){
    if (Cadena == nullptr) return 0;
    int G = 0;
    while (Cadena[G] != '\0') G++;
    return G;
}

char* Mensaje::Copiar_Texto(const char* Origen){ //Revisa un bloque de tamaño exacto
    int G = Longitud(Origen);
    char* PUNTERO_BLOQUE_MEMORIA = new char[G+1];
    for (int i = 0; i < G; i++) PUNTERO_BLOQUE_MEMORIA[i] = Origen[i]; //recorre y almacena
    PUNTERO_BLOQUE_MEMORIA[G] = '\0';
    return PUNTERO_BLOQUE_MEMORIA;
}

bool Mensaje::Comparacion_Caracter(const char* A, const char* B){
    //Te avisa si el dato ya existe para que tú evites pedirle más memoria RAM al sistema operativo de forma innecesaria.
    if(A == nullptr || B == nullptr) return false;
    int i = 0;
    while (A[i] != '\0' && A[i] == B[i]) i++;
    return A[i] == B[i];
}

bool Mensaje::Usuario_Valido(const char* Usuario){ //revisar si el usuario es valido
    if(Usuario == nullptr || Usuario[0] == '\0') return false;
    for(int i = 0; Usuario[i] != '\0'; i++){
        if(Usuario[i] == '|' || Usuario[i] == ' ') return false;
    }
    return true;
}

bool Mensaje::Texto_Valido(const char* Revisar_Texto){ //tres validaciones de seguridad para el mensaje sea vlido antes de guardarlo en la memoria
    if (Revisar_Texto == nullptr) return false;
    bool Tiene_Contenido = false; //revisa si hay contenido antes de tirar false
    int G = 0;
    while (Revisar_Texto[G] != '\0') {
        if(Revisar_Texto[G] != ' ' && Revisar_Texto[G] != '\t') Tiene_Contenido = true;
        G++;
        if(G > TEXTO_MAX) return false; //Se detine  cuando supera el limite
    }
    return Tiene_Contenido;
}

//Constructores y Destructor: crean y liberan la memoria

//Constructor por defecto: No recibe nada, crea un mensaje "en blanco"

Mensaje::Mensaje()
    : Emisor(Copiar_Texto("")), //Emisor como texto vacio
    Receptor(Copiar_Texto("")), //Receptor comienza vacio
    Fecha_de_Envio(), //Fecha sin valor
    Leidos(false), //Empieza como no leidos
    Texto(Copiar_Texto("")){} //texto comienza vacio

// Constructor con datos: recibe mensajes y guarda copia propia de cada texto

Mensaje::Mensaje(const char* Nuevo_Emisor, const char* Nuevo_Receptor, const Fecha& Nueva_Fecha, const char* Nuevo_Texto, bool Nuevo_Leidos ){
    Emisor = Copiar_Texto(Nuevo_Emisor);
    Receptor = Copiar_Texto(Nuevo_Receptor);
    Texto = Copiar_Texto(Nuevo_Texto);
    Fecha_de_Envio = Nueva_Fecha; //Copia de la fecha
    Leidos = Nuevo_Leidos; //False si no se indica
}

//Constructor de copia: M2 recibe sus propios bloques de memoria que no comparte con los de M1

Mensaje::Mensaje(const Mensaje& Otro){
    Emisor = Copiar_Texto(Otro.Emisor);
    Receptor = Copiar_Texto(Otro.Receptor);
    Texto = Copiar_Texto(Otro.Texto);
    Fecha_de_Envio = Otro.Fecha_de_Envio;
    Leidos = Otro.Leidos;
}

//Destructores: Libera los tres bloques de new y se ejecuta cuando deja de existir

Mensaje::~Mensaje(){ //~ es un destructor
    delete[] Emisor;
    delete[] Receptor;
    delete[] Texto;

}

Mensaje& Mensaje::operator=(const Mensaje& Otro){
    if(this != &Otro){
        char* Nuevo_Emisor = Copiar_Texto(Otro.Emisor);
        char* Nuevo_Receptor = Copiar_Texto(Otro.Receptor);
        char* Nuevo_Texto = Copiar_Texto(Otro.Texto);

        delete[] Emisor;
        delete[] Receptor;
        delete[] Texto;

        Emisor = Nuevo_Emisor;
        Receptor = Nuevo_Receptor;
        Texto = Nuevo_Texto;
        Fecha_de_Envio = Otro.Fecha_de_Envio;
        Leidos = Otro.Leidos;
    }
    return *this;
}

//Setters:Si el dato nuevo no es valido retornamos false y si es valido reservamos bloqeu de memoria y liberamos la anterior

bool Mensaje::setEmisor(const char* Nuevo_Emisor){
    if(!Usuario_Valido(Nuevo_Emisor)) return false;
    char* Copia = Copiar_Texto(Nuevo_Emisor);
    delete[] Emisor;
    Emisor = Copia;
    return true;
}

bool Mensaje::setReceptor(const char* Nuevo_Receptor){
    if(!Usuario_Valido(Nuevo_Receptor)) return false;
    char* Copia = Copiar_Texto(Nuevo_Receptor);
    delete[] Receptor;
    Receptor = Copia;
    return true;
}

bool Mensaje::setFecha_de_Envio(const Fecha& Nueva_Fecha){
    if(!Nueva_Fecha.esValida()) return false;
    Fecha_de_Envio = Nueva_Fecha;
    return true;
}

bool Mensaje::setTexto(const char* Nuevo_Texto){
    if (!Texto_Valido(Nuevo_Texto)) return false;
    char* Copia = Copiar_Texto(Nuevo_Texto);
    delete[] Texto;
    Texto = Copia;
    return true;
}

//Consultas: Compara nombres y fechas

bool Mensaje::Esta_en_Rango(const Fecha& Inicio, const Fecha& Fin) const{
    return Inicio <= Fecha_de_Envio && Fecha_de_Envio <= Fin;
}

unsigned int Mensaje::Memoria() const{
    return sizeof(Mensaje) //Cuanto vamos a ocupar en la memoria
           +(Longitud(Emisor)+ 1) //Se le suma esa longitud del emisor y el +1 es del '\0'
           +(Longitud(Receptor) + 1)
           +(Longitud(Texto) + 1);
}

ostream& operator<<(ostream& Salida, const Mensaje& S){
    Salida << "[" << S.Fecha_de_Envio <<" ] De: "<< S.Emisor << (S.Leidos ? "" : "(nuevo)") <<"\n  "<<S.Texto;
    return Salida;
}