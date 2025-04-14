#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
using namespace std;
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include "Servidor.h"

using namespace std;

void menu() {
    cout << "Escolha a operação:" << endl;
    cout << "1. Inserir produto (INSERT)" << endl;
    cout << "2. Deletar produto (DELETE)" << endl;
    cout << "3. Atualizar produto (UPDATE)" << endl;
    cout << "4. Consultar produto (SELECT)" << endl;
    cout << "5. Sair" << endl;
}

int main() {
    int fd;
    string comando;
    int opcao;

    // Cria o pipe (FIFO) para comunicação com o servidor
    fd = open("requisicoes_fifo", O_WRONLY);
    if (fd == -1) {
        perror("Erro ao abrir o pipe");
        return 1;
    }

    while (true) {
        menu();
        cout << "Escolha uma opção: ";
        cin >> opcao;

        switch (opcao) {
            case 1: {
                string nome;
                cout << "Digite o nome do produto: ";
                cin >> nome;
                comando = "INSERT," + nome;
                break;
            }
            case 2: {
                int id;
                cout << "Digite o id do produto a ser removido: ";
                cin >> id;
                comando = "DELETE," + to_string(id);
                break;
            }
            case 3: {
                int id;
                string nome;
                cout << "Digite o id do produto a ser atualizado: ";
                cin >> id;
                cout << "Digite o novo nome do produto: ";
                cin >> nome;
                comando = "UPDATE," + to_string(id) + "," + nome;
                break;
            }
            case 4: {
                int id;
                cout << "Digite o id do produto a ser consultado: ";
                cin >> id;
                comando = "SELECT," + to_string(id);
                break;
            }
            case 5:
                cout << "Saindo..." << endl;
                close(fd);
                return 0;
            default:
                cout << "Opção inválida. Tente novamente." << endl;
                continue;
        }

        // Envia o comando para o servidor via pipe
        write(fd, comando.c_str(), comando.size() + 1);
        cout << "Comando enviado: " << comando << endl;
    }

    return 0;
}
