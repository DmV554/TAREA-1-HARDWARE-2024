#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex> 

using namespace std;

int main() {
    ifstream archivoBarbero("file0.data");
    if (!archivoBarbero.is_open()) {
        cout << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    int sillasEspera, cantBarberos, sillasBarb;

    archivoBarbero >> sillasEspera >> cantBarberos >> sillasBarb;

    cout << "Sillas de espera: " << sillasEspera << endl;
    cout << "Cantidad de barberos: " << cantBarberos << endl;
    cout << "Sillas de barberos: " << sillasBarb << endl;

    while (!archivoBarbero.eof()) {
        int tiempoEntrada, tiempoEspera, tiempoCorte;
        archivoBarbero >> tiempoEntrada >> tiempoEspera >> tiempoCorte;

        cout << "Tiempo de entrada: " << tiempoEntrada << " "
         << "Tiempo de espera: " << tiempoEspera << " "
        << "Tiempo de corte: " << tiempoCorte << endl;
    }

    return 0;
}