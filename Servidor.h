
#include <iostream>
#include <locale.h>
#include <fstream>
#include <sstream>

using namespace std;

struct No {
    string produto;
    string categoria;
    float valor;
    int quantidade;
    float desconto;
    No *eloA, *eloP;
  };
  
  struct Lde {
    No *comeco;
    No *fim;
  };
  
  void inicializarLde(Lde &lista) {
    lista.comeco = nullptr;
    lista.fim = nullptr;
  }
  
  // Função para converter uma string em float
float stringToFloat(const string &str) {
    istringstream iss(str);
    float result;
    iss >> result;
    return result;
  }
  // Função para remover espaços em branco de uma string
string trim(const string &str) {
    size_t first = str.find_first_not_of(
        " \t\n\r"); // Encontra o primeiro caractere não em branco
    size_t last = str.find_last_not_of(
        " \t\n\r"); // Encontra o último caractere não em branco
  
    if (first == string::npos) // Caso a string esteja vazia ou contenha apenas
                               // espaços em branco
      return "";
  
    return str.substr(
        first, last - first + 1); // Retorna a substring sem os espaços em branco
  }
  
  
// Função para buscar um produto na lista duplamente encadeada
bool buscarLde(Lde &lista) {
    No *aux = lista.comeco;
    string produto;
    cout << "digite um produto para buscar: ";
    cin >> produto;
    cout << endl;
    while (aux != nullptr) {
      if (aux->produto == produto) {
        cout << aux->categoria << " ";
        cout << aux->produto << " ";
        cout << aux->valor << " ";
        cout << aux->quantidade << " ";
        cout << aux->desconto << "\n";
        return true;
      }
      aux = aux->eloP;
    }
    cout << "\n\tproduto não encontrado\n";
    return false;
  }
  
  // Função para inserir um nó no final da lista duplamente encadeada
  bool inserirFinalLde(Lde &lista, No novidade) {
    No *novo = new No;
    No *aux = lista.comeco;
  
    if (novo == nullptr)
      return false;
  
    novo->categoria = novidade.categoria;
    novo->produto = novidade.produto;
    novo->valor = novidade.valor;
    novo->quantidade = novidade.quantidade;
    novo->desconto = novidade.desconto;
  
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
  
  // Função para adicionar um novo produto à lista
  bool INSERT(Lde &lista) {
    No *novidade = new No;
    No *aux = lista.comeco;
  
    cout << "\n\nDigite a categoria do produto: ";
    cin >> novidade->categoria;
    cout << endl;
    cout << "Digite o nome do produto: ";
    cin >> novidade->produto;
    cout << endl;
    while (aux != nullptr) {
      if (aux->produto == novidade->produto) {
        cout << "\tproduto já cadastrado\n";
        return false;
      }
      aux = aux->eloP;
    }
    cout << "\tDigite o preço do produto: ";
    cin >> novidade->valor;
    cout << endl;
    cout << "\tDigite a quantidade do produto:";
    cin >> novidade->quantidade;
    cout << endl;
    cout << "\tDigite o desconto do produto: ";
    cin >> novidade->desconto;
    cout << endl;
    inserirFinalLde(lista, *novidade);
    return true;
  }
  
  
// Função para ler dados de um arquivo e inserir na lista
int lerarq(Lde &lista) {
    No *novidade = new No;
  
    ifstream arquivo("dados.txt"); // Abre o arquivo "dados.txt"
  
    if (!arquivo) {
      cerr << "Erro ao abrir o arquivo." << endl;
      return 1;
    }
  
    string linha;
    while (getline(arquivo, linha)) {
      istringstream iss(linha);
      string categoria, produto;
      string quantidade_str, valor_str, desconto_str;
  
      // Extrai os valores da linha separados por vírgula
      if (getline(iss, categoria, ',') && getline(iss, produto, ',') &&
          getline(iss, valor_str, ',') && getline(iss, quantidade_str, ',') &&
          getline(iss, desconto_str)) {
        // Converte as strings para floats
  
        novidade->categoria = trim(categoria);
        novidade->produto = trim(produto);
        trim(valor_str);
        trim(quantidade_str);
        trim(desconto_str);
        trim(desconto_str);
  
        novidade->quantidade = stoi(quantidade_str);
        novidade->valor = stringToFloat(valor_str);
        novidade->desconto = stringToFloat(desconto_str);
        novidade->categoria = trim(categoria);
        novidade->produto = trim(produto);
  
        inserirFinalLde(lista, *novidade);
  
      } else {
        cout << "Erro ao ler os valores da linha do arquivo." << endl;
      }
    }
  
    return 0;
  }
  
  // Função para escrever os dados da lista em um arquivo
  bool fim(Lde &lista) {
  
    No *aux = lista.comeco;
    string filename = "dados.txt";
  
    // Abrindo o arquivo em modo de escrita, o que vai limpar o conteúdo do
    // arquivo
    ofstream file(filename, ofstream::out | ofstream::trunc);
  
    if (file.is_open()) {
      file.close();
  
      // Abrindo o arquivo novamente para escrever informações
      file.open(filename,
                ofstream::out |
                    ofstream::app); // Modo de escrita no final do arquivo
  
      if (file.is_open()) {
        while (aux != nullptr) {
          // Escrevendo informações no arquivo
          file << aux->categoria << " , " << aux->produto << " , " << aux->valor
               << " , " << aux->quantidade << " , " << aux->desconto << endl;
          aux = aux->eloP;
        }
        file.close();
  
      } else {
        cerr << "Erro ao abrir o arquivo para escrever informações." << endl;
      }
    } else {
      cerr << "Erro ao abrir o arquivo." << endl;
    }
    return true;
  }
  
  // Função para mostrar os dados da lista na tela
  void mostrarLdeC(Lde &lista) {
    No *aux = lista.comeco;
  
    while (aux != nullptr) {
  
      cout << "\tCategoria: " << aux->categoria << endl;
      cout << "\tproduto: " << aux->produto << endl;
      cout << "\tValor: " << aux->valor << endl;
      cout << "\tQuantidade: " << aux->quantidade << endl;
      cout << "\tDesconto: " << aux->desconto << endl;
      cout << "\n";
      aux = aux->eloP;
    }
  }
  
  // Função para retirar um produto da lista
  bool DELETE(Lde &lista, string produto) {
    if (lista.comeco == nullptr)
      return false;
  
    No *aux = lista.comeco;
    while (aux != nullptr && aux->produto != produto) {
      aux = aux->eloP;
    }
    if (aux == nullptr)
      return false; // Valor nãoo esta na lista
  
    No *anterior = aux->eloA;
    No *proximo = aux->eloP;
  
    if (aux == lista.comeco && aux == lista.fim) { // Caso 1
      lista.comeco = nullptr;
      lista.fim = nullptr;
      delete aux;
      return true;
    }
  
    if (aux == lista.comeco) { // Caso 2
      lista.comeco = proximo;
      proximo->eloA = nullptr;
      delete aux;
      return true;
    }
  
    if (aux == lista.fim) { // Caso 3
      lista.fim = anterior;
      anterior->eloP = nullptr;
      delete aux;
      return true;
    }
  
    // Caso 4
    anterior->eloP = proximo;
    proximo->eloA = anterior;
    delete aux;
    return true;
  }
  
  // Função para retirar um produto da lista com base na entrada do usuário
  bool retirar(Lde &lista) {
    string produto;
    cout << "Digite o produto que deseja retirar: ";
    cin >> produto;
    cout << endl;
    DELETE(lista, produto);
    return true;
  }
  
  // Função para mudar o preço, a quantidade ou o desconto de um produto
  bool mudarproduto(Lde lista) {
    No *aux = lista.comeco;
    int menus = 0;
    int esc = 0;
    float mudanca;
    int estoque;
    string produto;
  
    while (menus != 4) {
      mostrarLdeC(lista);
  
      cout << "\tDigite o nome do produto: ";
      cin >> produto;
      cout << endl;
      cout << "\tDigite o que quer mudar: " << endl;
      cout << "\t(1)-Preco" << endl;
      cout << "\t(2)-Quantidade" << endl;
      cout << "\t(3)-Desconto" << endl;
      cout << "\t(4)-Sair" << endl;
      cout << "\tEscolha: ";
      cin >> menus;
      cout << endl;
      cout << endl;
      switch (menus) {
      case 1:
        while (aux != nullptr) {
          if (aux->produto == produto) {
            cout << "\tMude o valor: ";
            cin >> mudanca;
            cout << endl;
            aux->valor = mudanca;
  
            break;
          }
          aux = aux->eloP;
        }
        break;
      case 2:
        while (aux != nullptr) {
          if (aux->produto == produto) {
            cout << "\tMude o estoque: ";
            cin >> estoque;
            cout << endl;
            aux->quantidade = estoque;
  
            break;
          }
          aux = aux->eloP;
        }
        break;
  
      case 3:
        while (aux != nullptr) {
          if (aux->produto == produto) {
            cout << "\tMude o desconto: ";
            cin >> mudanca;
            cout << endl;
  
            aux->desconto = mudanca;
  
            break;
          }
          aux = aux->eloP;
        }
        break;
  
      case 4:
        cout << "saindo";
        break;
      }
      cout << "\tGostaria de Mudar outro produto?" << endl;
      cout << "\t(1)continuar" << endl;
      cout << "\t(2)sair" << endl;
      cout << "\tEscolha: ";
      cin >> esc;
      cout << endl;
  
      if (esc != 1) {
        break;
      }
    }
    return false;
  }
  
  // Função para liberar a memória alocada para os nós da lista
  void liberarLde(Lde &lista) {
    No *aux = lista.comeco;
    No *aux2;
  
    while (aux != nullptr) {
      aux2 = aux->eloP;
      delete aux;
      aux = aux2;
    }
  }
  
  
// Função para procurar produtos de uma determinada categoria na lista
bool procurarcategoria(Lde &lista) {
    string categoria;
    No *aux = lista.comeco;
    cout << "\tQual categoria voce gostaria de procurar?\n";
    cin >> categoria;
    while (aux != nullptr) {
      if (aux->categoria == categoria && aux->quantidade != 0) {
        cout << endl;
        cout << "\tProduto:" << aux->produto << endl;
        cout << "\tValor: " << aux->valor << endl;
        cout << "\tQuantidade: " << aux->quantidade << endl;
        cout << "\tDesconto: " << aux->desconto << endl;
        cout << endl;
      }
      aux = aux->eloP;
    }
  }
  
  