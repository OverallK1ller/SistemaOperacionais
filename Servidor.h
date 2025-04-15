
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
using namespace std;

struct No {
    int id = 0;
    char nome[50];
    No* eloA;
    No* eloP;
};

struct Lde {
    No* comeco;
    No* fim;
};

void inicializarLde(Lde& lista) {
    lista.comeco = nullptr;
    lista.fim = nullptr;
}

void preencher_nome(No& n, const string& nome_str) {
    strncpy(n.nome, nome_str.c_str(), sizeof(n.nome) - 1);
    n.nome[sizeof(n.nome) - 1] = '\0';
}

bool inserirFinalLde(Lde& lista, const No& novidade) {
    No* novo = new No;
    novo->id = novidade.id;
    strncpy(novo->nome, novidade.nome, sizeof(novo->nome));
    novo->nome[sizeof(novo->nome) - 1] = '\0';
    novo->eloA = nullptr;
    novo->eloP = nullptr;

    if (lista.comeco == nullptr) {
        lista.comeco = novo;
        lista.fim = novo;
    } else {
        lista.fim->eloP = novo;
        novo->eloA = lista.fim;
        lista.fim = novo;
    }
    return true;
}

bool retirarLde(Lde& lista, int id) {
    No* aux = lista.comeco;
    while (aux != nullptr && aux->id != id) {
        aux = aux->eloP;
    }
    if (!aux) return false;
    if (aux == lista.comeco) lista.comeco = aux->eloP;
    if (aux == lista.fim) lista.fim = aux->eloA;
    if (aux->eloA) aux->eloA->eloP = aux->eloP;
    if (aux->eloP) aux->eloP->eloA = aux->eloA;
    delete aux;
    return true;
}


bool retirarLdenm(Lde& lista, string nome) {
    No* aux = lista.comeco;
    while (aux != nullptr && aux->nome != nome) {
        aux = aux->eloP;
    }
    if (!aux) return false;
    if (aux == lista.comeco) lista.comeco = aux->eloP;
    if (aux == lista.fim) lista.fim = aux->eloA;
    if (aux->eloA) aux->eloA->eloP = aux->eloP;
    if (aux->eloP) aux->eloP->eloA = aux->eloA;
    delete aux;
    return true;
}

void mostrarLdeC(Lde& lista) {
    No* aux = lista.comeco;
    while (aux) {
        cout << "\tid: " << aux->id << endl;
        cout << "\tnome: " << aux->nome << endl;
        aux = aux->eloP;
    }
}

No* buscarPorId(Lde& lista, int id) {
    No* aux = lista.comeco;
    while (aux) {
        if (aux->id == id) return aux;
        aux = aux->eloP;
    }
    return nullptr;
}
No* buscarPornm(Lde& lista, string nome) {
    No* aux = lista.comeco;
    while (aux) {
        if (aux->nome == nome) return aux;
        aux = aux->eloP;
    }
    return nullptr;
}
void salvarLdeEmArquivo(Lde& lista) {
    ofstream arq("dados.txt");
    No* aux = lista.comeco;
    while (aux) {
        arq << aux->id << "," << aux->nome << endl;
        aux = aux->eloP;
    }
    arq.close();
    cout << "[ARQUIVO] dados.txt atualizado.\n";
}

void carregarLdeDeArquivo(Lde& lista) {
    ifstream arq("dados.txt");
    if (!arq.is_open()) return;

    string linha;
    while (getline(arq, linha)) {
        istringstream ss(linha);
        string id_str, nome;
        getline(ss, id_str, ',');
        getline(ss, nome);

        No novo;
        novo.id = stoi(id_str);
        preencher_nome(novo, nome);
        inserirFinalLde(lista, novo);
    }
    arq.close();
}

