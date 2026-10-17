#include <iostream>
using namespace std;
#include "FECHA.h"

Fecha::Fecha() : Valor(0) {}

Fecha::Fecha(int Dia, int Mes, int Anio) : Valor(0){
    setFecha(Dia, Mes, Anio);
}

Fecha::Fecha(const char* Texto) : Valor(0){
    setFecha(Texto);
}

bool Fecha::Es_Bisisesto(int Anio){
    return( Anio % 4== 0 && Anio % 100 !=0) || (Anio % 400 ==0);
}

int Fecha::DiadelMes(int Mes, int Anio){
    static const int Dias[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (Mes == 2 && Es_Bisisesto(Anio)) return 29;
    return Dias[Mes-1];
}

void Fecha::setFecha(int Dia, int Mes, int Anio){
    if (Anio < 1||Anio > 9999||Mes < 1||Mes > 12 ||Dia < 1||Dia > DiadelMes(Mes,Anio)){
        Valor = 0;
        return;
    }
    Valor = Anio*1000 + Mes*100 + Dia;
}

//Setters individuales. Se arma la fecha candidata con el campo nuevo y los otros dos actuales; si no es válida no se modifica

bool Fecha::setDia(int Dia){
    Fecha Candidata(Dia, getMes(), getAnio()); //Vaiable local para reutilizar
    if (!Candidata.esValida()) return false;
    Valor = Candidata.Valor;
    return true;
}

bool Fecha::setMes(int Mes){
    Fecha Candidata(getDia(), Mes, getAnio());
    if (!Candidata.esValida()) return false;
    Valor = Candidata.Valor;
    return true;
}

bool Fecha::setAnio(int Anio){
    Fecha Candidata(getDia(), getMes(), Anio);
    if(!Candidata.esValida()) return false;
    Valor = Candidata.Valor;
    return true;
}

//Convierte "DD/MM/AAAA" a fecha. Recorre texto hasta el '\0', acumulando dígitos en día, mes y año según la cantidad de / encontrados. Cualquier otro carácter invalida la fecha.

void Fecha::setFecha(const char* Texto){
    Valor = 0;
    if(Texto ==nullptr) return;

    int Partes[3] = {0, 0, 0};
    int Parte = 0;
    int Digitos = 0;

    for (int i = 0; Texto[i] != '\0'; i++){
        char C = Texto[i];
        if (C >= '0' && C <= '9'){
            if (++Digitos > 4) return; //Evita desbordes de numeros largos
            Partes[Parte] = Partes[Parte] * 10 + (C - '0');
        }
        else if (C == '/' && Parte < 2 && Digitos > 0){
            Parte ++;
            Digitos = 0;
        }
        else {
            return ;
        }
    }
    if ( Parte == 2 && Digitos > 0) setFecha(Partes[0], Partes[1], Partes[2]);
}

void Fecha::A_Texto(char* PUNTERO_BLOQUE_MEMORIA) const{
    int D = getDia(), M = getMes(), A = getAnio();
    PUNTERO_BLOQUE_MEMORIA[0] = '0' + D / 10;
    PUNTERO_BLOQUE_MEMORIA[1] = '0' +D % 10;
    PUNTERO_BLOQUE_MEMORIA[2] = '/';
    PUNTERO_BLOQUE_MEMORIA[3] = '0' + M / 10;
    PUNTERO_BLOQUE_MEMORIA[4] = '0' + M % 10;
    PUNTERO_BLOQUE_MEMORIA[5] = '/';
    PUNTERO_BLOQUE_MEMORIA[6] = '0' + (A / 1000) % 10;
    PUNTERO_BLOQUE_MEMORIA[7] = '0' + (A/100) % 10;
    PUNTERO_BLOQUE_MEMORIA[8] = '0' + (A /10) % 10;
    PUNTERO_BLOQUE_MEMORIA[9] = '0' + A % 10;
    PUNTERO_BLOQUE_MEMORIA[10] = '\0';
}

bool Fecha::Esta_en_Rango(const Fecha& Inicio, const Fecha& Fin)const{
    return Inicio <= *this && *this <= Fin;
}

ostream& operator<<(ostream& Salida, const Fecha& F){
    char PUNTERO_BLOQUE_MEMORIA[Fecha::TAM_TEXTO];
    F.A_Texto(PUNTERO_BLOQUE_MEMORIA);
    Salida <<PUNTERO_BLOQUE_MEMORIA;
    return Salida;
}