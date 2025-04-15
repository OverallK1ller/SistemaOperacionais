
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

Lde lista; // Lista duplamente encadeada usada como "banco de dados"
int id_global = 1;

// Função que processa cada requisição recebida em uma thread separada
void* processa_requisicao(void* arg) {
    string comando = *(string*)arg;
    delete (string*)arg;

    istringstream iss(comando);
    string tipo;
    getline(iss, tipo, ',');

    //Inserimento de um novo item
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
        salvarLdeEmArquivo(lista);

        pthread_mutex_unlock(&mutex_write);

        cout << "[INSERT] Nome inserido: " << nome << " com ID " << novo.id << endl;

    } //Exclusão de um item ja existe no 'banco de dados
    else if (tipo == "DELETE") {
        string id_str;
        getline(iss, id_str);
        int id = stoi(id_str);

        pthread_mutex_lock(&mutex_write);
        bool ok = retirarLde(lista, id);
        salvarLdeEmArquivo(lista);

        pthread_mutex_unlock(&mutex_write);

        cout << (ok ? "[DELETE] ID " + id_str + " removido." : "[DELETE] ID não encontrado.") << endl;

    } //Buscar por item no banco de dados
    else if (tipo == "SELECT") {
    } else if (tipo == "SELECTID") {
        string id_str;
        getline(iss, id_str);
        int id = stoi(id_str);

        No* res = buscarPorId(lista, id);
        if (res)
            cout << "[SELECT] ID: " << res->id << ", Nome: " << res->nome << endl;
        else
            cout << "[SELECT] ID " << id << " não encontrado." << endl;

    //Atualiza um item ja existente no banco de dados
     
    } else if (tipo == "SELECTNM") {
        string nome;
        getline(iss, nome);
        

        No* res = buscarPornm(lista, nome);
        if (res)
            cout << "[SELECT] Nome: " << res->nome << ", ID: " << res->id << endl;
        else
            cout << "[SELECT] Nome " << nome << " não encontrado." << endl;

    } else if (tipo == "UPDATE") {
        string id_str, nome;
        getline(iss, id_str, ',');
        getline(iss, nome);
        int id = stoi(id_str);

        pthread_mutex_lock(&mutex_write);
        No* res = buscarPorId(lista, id);
        if (res) {
            preencher_nome(*res, nome);
            salvarLdeEmArquivo(lista);

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
    //Cria o FIFO e abre para leitura
    mkfifo("requisicoes_fifo", 0666);
    // Abre o FIFO para leitura
    int fd = open("requisicoes_fifo", O_RDONLY);

    inicializarLde(lista);
    carregarLdeDeArquivo(lista);

    while (true) {
        char buffer[256] = {0};
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
