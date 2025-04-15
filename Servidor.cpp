
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sstream>
#include "Servidor.h"

using namespace std;

pthread_mutex_t mutex_write = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_id = PTHREAD_MUTEX_INITIALIZER;
Lde lista;
int id_global = 1;

void* processa_requisicao(void* arg) {
    string comando = *(string*)arg;
    delete (string*)arg;

    istringstream iss(comando);
    string tipo;
    getline(iss, tipo, ',');

    if (tipo == "INSERT") {
        string nome;
        getline(iss, nome);

        No novo;
        pthread_mutex_lock(&mutex_id);
        novo.id = id_global++;
        pthread_mutex_unlock(&mutex_id);

        preencher_nome(novo, nome);

        pthread_mutex_lock(&mutex_write);
        inserirFinalLde(lista, novo);
        pthread_mutex_unlock(&mutex_write);

        cout << "[INSERT] Nome inserido: " << nome << " com ID " << novo.id << endl;

    } else if (tipo == "DELETE") {
        string id_str;
        getline(iss, id_str);
        int id = stoi(id_str);

        pthread_mutex_lock(&mutex_write);
        bool ok = retirarLde(lista, id);
        pthread_mutex_unlock(&mutex_write);

        cout << (ok ? "[DELETE] ID " + id_str + " removido." : "[DELETE] ID não encontrado.") << endl;

    } else if (tipo == "SELECT") {
        string id_str;
        getline(iss, id_str);
        int id = stoi(id_str);

        No* res = buscarPorId(lista, id);
        if (res)
            cout << "[SELECT] ID: " << res->id << ", Nome: " << res->nome << endl;
        else
            cout << "[SELECT] ID " << id << " não encontrado." << endl;

    } else if (tipo == "UPDATE") {
        string id_str, nome;
        getline(iss, id_str, ',');
        getline(iss, nome);
        int id = stoi(id_str);

        pthread_mutex_lock(&mutex_write);
        No* res = buscarPorId(lista, id);
        if (res) {
            preencher_nome(*res, nome);
            cout << "[UPDATE] ID " << id << " alterado para: " << nome << endl;
        } else {
            cout << "[UPDATE] ID " << id << " não encontrado." << endl;
        }
        pthread_mutex_unlock(&mutex_write);

    } else {
        cout << "[ERRO] Comando não reconhecido: " << comando << endl;
    }

    return nullptr;
}

int main() {
    mkfifo("requisicoes_fifo", 0666);
    int fd = open("requisicoes_fifo", O_RDONLY);

    inicializarLde(lista);

    while (true) {
        char buffer[128] = {0};
        int n = read(fd, buffer, sizeof(buffer));
        if (n > 0) {
            string* comando = new string(buffer);
            pthread_t thread;
            pthread_create(&thread, nullptr, processa_requisicao, comando);
            pthread_detach(thread);
        }
    }

    close(fd);
    unlink("requisicoes_fifo");
    return 0;
}
