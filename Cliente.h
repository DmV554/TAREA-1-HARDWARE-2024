#ifndef CLIENTE_H
#define CLIENTE_H

class Cliente {
private:
    int tiempoEntrada;
    int tiempoEspera;
    int tiempoCorte;
    int sillaUtilizada;
    int sillaBUtilizada;
    int barbero;

public:
    Cliente(int entrada, int espera, int corte); // Constructor
    int obtenerTiempoEntrada();
    int obtenerTiempoEspera();
    int obtenerTiempoCorte();
    int obtenerSillaUtilizada();
    int obtenerSillaBUtilizada();
    int obtenerBarbero();
    void setearSillaUtilizada(int);
    void setearSillaBUtilizada(int);
    void setearBarbero(int);
};

#endif