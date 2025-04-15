
#include <iostream> //asd
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <cstring>
#include "Servidor.h"

using namespace std;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex global
Lde lista;

void* processa_requisicao(void* arg) {
    string comando = *(string*)arg;
    delete (string*)arg;

    pthread_mutex_lock(&mutex);

    if (comando.find("INSERT") == 0) {
        string nome = comando.substr(7); // extrai nome após "INSERT,"
        No novo;
        novo.id = (lista.fim ? lista.fim->id + 1 : 1);
        strncpy(novo.nome, nome.c_str(), 50);
        inserirFinalLde(lista, novo);
        cout << "[INSERT] Nome inserido: " << nome << endl;

    } else if (comando.find("DELETE") == 0) {
        int id = stoi(comando.substr(7));
        if (retirarLde(lista, id)) {
            cout << "[DELETE] Registro ID " << id << " removido." << endl;
        } else {
            cout << "[DELETE] ID " << id << " não encontrado." << endl;
        }

    } else if (comando.find("SELECT") == 0) {
        int id = stoi(comando.substr(7));
        No* aux = lista.comeco;
        while (aux != nullptr) {
            if (aux->id == id) {
                cout << "[SELECT] ID: " << aux->id << ", Nome: " << aux->nome << endl;
                break;
            }
            aux = aux->eloP;
        }
        if (aux == nullptr)
            cout << "[SELECT] ID " << id << " não encontrado." << endl;

    } else if (comando.find("UPDATE") == 0) {
        size_t pos1 = comando.find(",");
        size_t pos2 = comando.find(",", pos1 + 1);
        if (pos1 != string::npos && pos2 != string::npos) {
            int id = stoi(comando.substr(pos1 + 1, pos2 - pos1 - 1));
            string novo_nome = comando.substr(pos2 + 1);
            No* aux = lista.comeco;
            while (aux != nullptr) {
                if (aux->id == id) {
                    strncpy(aux->nome, novo_nome.c_str(), 50);
                    cout << "[UPDATE] ID " << id << " alterado para: " << novo_nome << endl;
                    break;
                }
                aux = aux->eloP;
            }
            if (aux == nullptr)
                cout << "[UPDATE] ID " << id << " não encontrado." << endl;
        } else {
            cout << "[UPDATE] Comando mal formatado." << endl;
        }

    } else {
        cout << "[ERRO] Comando não reconhecido: " << comando << endl;
    }

    pthread_mutex_unlock(&mutex);
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
            pthread_detach(thread); // liberação automática após fim
        }
    }

    close(fd);
    unlink("requisicoes_fifo");
    return 0;
}