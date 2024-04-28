#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <mutex>
#include <list>
#include "Cliente.h"
#include <condition_variable>

using namespace std;

mutex mtx;
mutex mtx2;
mutex mtx3;
mutex mtx4;
mutex mtx5;
mutex mtxc;
std::condition_variable cv;
mutex mtx_cv;

int verificar(int arreglo[], int tam){
    for(int i = 0; i < tam; i++) {
        if(arreglo[i] == 0){
            arreglo[i] = 1;
            return i;
        }
    }
    return -1;
}

void liberar(int pos, int arreglo[]){
    arreglo[pos] = 0;
}

void gestionar(Cliente* cliente, int id, int sillas[], int cantSillas, int sillasB[], int cantSillasB, int barberos[], int cantBarberos){
    int flag = 0;

    std::this_thread::sleep_for(std::chrono::seconds(cliente->obtenerTiempoEntrada()));
    
    //LLEGADA A LA BARBERÍA
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Cliente " << id << " llega a la barberia." << std::endl;
    }

    //REVISIÓN DE SILLA
    {
        cliente->setearSillaUtilizada(verificar(sillas, cantSillas));
        if (cliente->obtenerSillaUtilizada() == -1) {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Cliente " << id << " se va porque no hay sillas de espera." << std::endl;
            return;
        }

        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Cliente " << id << " se sienta en la silla " << cliente->obtenerSillaUtilizada() << std::endl;
        }
    }

    auto now = std::chrono::steady_clock::now;
    auto tiempo = std::chrono::seconds{cliente->obtenerTiempoEspera()};
    auto stop_time = now() + tiempo;
    while (now() < stop_time)
    {
        cliente->setearSillaBUtilizada(verificar(sillasB, cantSillasB));
        if(cliente->obtenerSillaBUtilizada() != -1){
            flag = 1;
            {
                std::lock_guard<std::mutex> lock(mtx);
                std::cout << "Cliente " << id << " se sienta en la silla de barbero " << cliente->obtenerSillaBUtilizada() << std::endl;
            }
            liberar(cliente->obtenerSillaUtilizada(), sillas);
            break;
        }
    }
    if(flag == 0){
        std::cout << "Cliente " << id << " se va (no hay sillas disponibles) " << std::endl;
        return;
    }

    //REVISIÓN DE BARBERO DISPONIBLE
    while(1){
        cliente->setearBarbero(verificar(barberos, cantBarberos));
        if(cliente->obtenerBarbero() != -1){
            {
                std::lock_guard<std::mutex> lock(mtx);
                std::cout << "Cliente " << id << " es atendido por barbero " << cliente->obtenerBarbero() << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::seconds(cliente->obtenerTiempoCorte()));
            
            {
                std::unique_lock<std::mutex> lock(mtx);
                std::cout << "Cliente " << id << " sale (es atendido por completo)" << std::endl;
            }
            liberar(cliente->obtenerSillaBUtilizada(), sillasB);
            liberar(cliente->obtenerBarbero(), barberos);
            return;
        }
    }
}


int main() {
    ifstream archivoBarbero("file0.data");
    if (!archivoBarbero.is_open()) {
        cout << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    list<Cliente*> listaClientes;
    Cliente* clienteBarberia;
    int sillasEspera, cantBarberos, sillasBarb;

    archivoBarbero >> sillasEspera >> cantBarberos >> sillasBarb;

    cout << "Sillas de espera: " << sillasEspera << endl;
    cout << "Cantidad de barberos: " << cantBarberos << endl;
    cout << "Sillas de barberos: " << sillasBarb << endl;

    int sillas[sillasEspera];
    for(int i = 0; i < sillasEspera; i++) {
        sillas[i] = 0;
        }
    
    int barberos[cantBarberos];
    for(int i = 0; i < cantBarberos; i++) barberos[i] = 0;

    int sillasB[sillasBarb];
    for(int i = 0; i < sillasBarb; i++) sillasB[i] = 0;

    int acum = 0;
    while (!archivoBarbero.eof()) {
        int tiempoEntrada, tiempoEspera, tiempoCorte;

        char c1 = archivoBarbero.get();
        char c2 = archivoBarbero.peek();
        
        
        if (c2 != '\n')
        {
            archivoBarbero >> tiempoEntrada >> tiempoEspera >> tiempoCorte;
            acum = acum + tiempoEntrada;
            clienteBarberia = new Cliente(acum, tiempoEspera, tiempoCorte);
            listaClientes.push_back(clienteBarberia);
        }
    }



    //Se muestran los clientes por tiempo (acumulado)
    /*int index = 0;
    auto it = listaClientes.begin();
    while (it != listaClientes.end()) {
        cout << "Cliente " << index << " tl: "<<(*it)->obtenerTiempoEntrada() << " te: " <<
        (*it)->obtenerTiempoEspera() << " tc: " <<
        (*it)->obtenerTiempoCorte() << endl;
        ++it;
        ++index;
    }*/

    std::vector<std::thread> hilos;

    int indice = 0;
    auto it2 = listaClientes.begin();
    while (it2 != listaClientes.end()) {
        Cliente* aux = (*it2);
        std::thread t([aux, indice, &sillas, sillasEspera, &sillasB, sillasBarb, &barberos, cantBarberos]() { gestionar(aux, indice, sillas, sillasEspera, sillasB, sillasBarb, barberos, cantBarberos); });
        hilos.push_back(std::move(t));
        ++indice;
        ++it2;
    }
    
    for (auto& hilo : hilos) {
    hilo.join();
    }

    return 0;
}

