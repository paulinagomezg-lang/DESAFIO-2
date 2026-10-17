#ifndef FECHA_H
#define FECHA_H
#include <iostream>
using namespace std;

//Guarda el dia, mes y año en un entero con el formato AAAMMDD.
//Ocupa 4 bytes y compara dos fechas en un solo entero, agiliza el ordenamiento de mensajes y la busqueda en rango.
//El formato texto archivo en la consola es DD/MM/AAAA.

class Fecha{
private:
    int Valor; //AAAAMMDD Y EL 0 ES FECHA INVALIDA
    static bool Es_Bisisesto(int Anio);
    static int DiadelMes(int Mes, int Anio);

public:
    static const int TAM_TEXTO = 11;
    Fecha(); //fecha invalida
    Fecha(int Dia, int Mes, int Anio);
    explicit Fecha(const char* Texto);

    //Getters: se extraen del entero con division y modulo
    int getDia() const{
        return Valor % 100;
    }
    int getMes() const{
        return (Valor/100) % 100;
    }
    int getAnio() const{
        return Valor / 10000;
    }
    int getValor() const {
        return Valor;
    }

    //Setters: Si la fehca no es valida se vuelve en 0

    void setFecha(int Dia, int Mes, int Anio);
    void setFecha(const char* Texto);

    //Setter individuales: Solo cambian en el campo si el resultado sigue siendo una fecha valida true.

    bool setDia(int Dia);
    bool setMes(int Mes);
    bool setAnio(int Anio);
    bool esValida() const{
        return Valor != 0;
    }

    //Escribe en el PUNTERO_BLOQUE_MEMORIA la fecha DD/MM/AAAA recibiendo (min TAM_TEXTO char)

    void A_Texto(char* PUNTERO_BLOQUE_MEMORIA) const;

    //Operadores de comparacion sobrecarga para comparar dos clases

    bool operator == (const Fecha& otra) const{
        return Valor == otra.Valor;
    }
    bool operator != (const Fecha& otra) const{
        return Valor != otra.Valor;
    }
    bool operator < (const Fecha& otra) const{
        return Valor < otra.Valor;
    }
    bool operator > (const Fecha& otra) const{
        return Valor > otra.Valor;
    }
    bool operator <= (const Fecha& otra) const{
        return Valor <= otra.Valor;
    }
    bool operator >= (const Fecha& otra) const{
        return Valor >= otra.Valor;
    }

    //Si la fecha esya en el inicio y fin
    bool Esta_en_Rango(const Fecha& Inicio, const Fecha& Fin)const;

    //Bytes ocupados po el objeto SIN UTILIZAR memoria dinamica
    unsigned int MEMORIA() const{
        return sizeof(Fecha);
    }

    //Funcion amiga para poder imprimir
    friend ostream& operator<<(ostream& Salida, const Fecha& F);

};

#endif // FECHA_H
