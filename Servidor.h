
#include <iostream>
#include <locale.h>
#include <fstream>
#include <sstream>

using namespace std;

struct No {
    int id=0;
    char nome[50];
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
  
  //função para mostrar o nome
  void mostrar_dados(No dado){
    cout<< dado.id<<": ";
    int i=0;
    char exit[1];
    while(dado.nome[i]!= exit[0]){
    cout<<dado.nome[i];
    }
  }
/*
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
  */
  
// Função para buscar um produto na lista duplamente encadeada
bool buscarLde(Lde &lista) {
    No *aux = lista.comeco;
    int id_busca;
    cout << "digite um id para buscar: ";
    cin >> id_busca;
    cout << endl;
    while (aux != nullptr) {
      if (aux->id == id_busca) {
        mostrar_dados(*aux);
        return true;
      }
      aux = aux->eloP;
    }
    cout << "\n\tproduto não encontrado\n";
    return false;
  }
  
  
  void preencher_no_string(No novidade, string nome){
      for(int i=0;i<50;i++){
        novidade.nome[i]=nome[i];
      }

  }

  void preencher_no_no(No novidade, No novo){
    novo.id=novidade.id;
      for (int i=0; i<50; i++){
        novo.nome[i] = novidade.nome[i];
      }
    
  }
  
  // Função para inserir um nó no final da lista duplamente encadeada
  bool inserirFinalLde(Lde &lista, No novidade) {
    No *novo = new No;
    No *aux = lista.comeco;
  
    if (novo == nullptr)
      return false;
  
    preencher_no_no(novidade,*novo);
    
    
  
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
  bool adicionarproduto(Lde &lista) {
    No *novidade = new No;
    No *aux = lista.comeco;
    string nome;
    novidade->id =lista.fim->id+1;
  while(1){
    cout << "\n\nDigite o nome do cliente: ";
    cin >> nome;
    cout << endl;
    if (nome.length()>50) break;
  }
  preencher_no_string(*novidade,nome);

   
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
      string id, nome;
      
  
      // Extrai os valores da linha separados por vírgula
      if (getline(iss, id, ',') && getline(iss, nome, ',')) {
        // Converte as strings para floats
  
        novidade->id = stoi(id);
        preencher_no_string(*novidade,nome);
  
       
      
        inserirFinalLde(lista, *novidade);
  
      } else {
        cout << "Erro ao ler os valores da linha do arquivo." << endl;
      }
    }
  
    return 0;
  }
  
string ctos(No aux, string nome){
for (int i=0;i<50;i++){
  nome[i]=aux.nome[i];
}
return nome;
}

  // Função para escrever os dados da lista em um arquivo
  bool fim(Lde &lista) {
  
    No *aux = lista.comeco;
    string filename = "dados.txt";
  
    // Abrindo o arquivo em modo de escrita, o que vai limpar o conteúdo do
    // arquivo
    ofstream file(filename, ofstream::out | ofstream::trunc);
  string nome;
    if (file.is_open()) {
      file.close();
  
      // Abrindo o arquivo novamente para escrever informações
      file.open(filename,
                ofstream::out |
                    ofstream::app); // Modo de escrita no final do arquivo
  
      if (file.is_open()) {
        while (aux != nullptr) {
          // Escrevendo informações no arquivo
          file << aux->id << " , " << ctos(*aux, nome)  << endl;
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
  
      cout << "\tid: " << aux->id << endl;
      cout << "\tnome: " << aux->nome << endl;
      cout << "\n";
      aux = aux->eloP;
    }
  }
  
  // Função para retirar um produto da lista
  bool retirarLde(Lde &lista, int id) {
    if (lista.comeco == nullptr)
      return false;
  
    No *aux = lista.comeco;
    while (aux != nullptr && aux->id != id) {
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
    int id;
    cout << "Digite o id do produto que deseja retirar: ";
    cin >> id;
    cout << endl;
    retirarLde(lista, id);
    return true;
  }
  
bool verifica_id(Lde lista,int mudanca_id){
  if (lista.comeco == nullptr)
  return true;

No *aux = lista.comeco;
while (aux != nullptr && aux->id != mudanca_id) {
  aux = aux->eloP;
}
if (aux == nullptr)
  return true; 
if(aux->id == mudanca_id)
return false;

return false;
}

  // Função para mudar o preço, a quantidade ou o desconto de um produto
  bool mudarproduto(Lde lista) {
    No *aux = lista.comeco;
    int menus = 0;
    int esc = 0;
    int id;
    string nome;
    int mudanca_id;
    string mudanca_nome;
  
    while (menus != 4) {
      mostrarLdeC(lista);
  
      cout << "\tDigite o id do produto: ";
      cin >> id;
      cout << endl;
      cout << "\tDigite o que quer mudar: " << endl;
      cout << "\t(1)-Id" << endl;
      cout << "\t(2)-Nome" << endl;
      cout << "\t(3)-Sair" << endl;
      cout << "\tEscolha: ";
      cin >> menus;
      cout << endl;
      cout << endl;
      switch (menus) {
      case 1:
        while (aux != nullptr) {
          if (aux->id == id) {
            cout << "\tMude o id: ";
            cin >> mudanca_id;
            cout << endl;
            if (verifica_id(lista,mudanca_id)== true){
            aux->id = mudanca_id;
            }
            else{
              cout<<"impossivel mudar o id porque esse id já existe em outro produto";
            }
  
            break;
          }
          aux = aux->eloP;
        }
        break;
      case 2:
        while (aux != nullptr) {
          if (aux->id == id) {
            cout << "\tMude o nome: ";
            cin >> mudanca_nome;
            cout << endl;
            preencher_no_string(*aux,nome);
            
  
            break;
          }
          aux = aux->eloP;
        }
        break;
  
      
      case 3:
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
  
  
// Função para procurar por nome
bool procurarnome(Lde &lista) {
    string nome;
    No *aux = lista.comeco;
    cout << "\tQual nome voce gostaria de procurar?\n";
    cin >> nome;
    while (aux != nullptr) {
      if (aux->nome ==nome) {
        cout << endl;
        cout << "\tId:" << aux->id << endl;
        cout << "\tNome: " << aux->nome << endl;
        cout << endl;
        return true;
      }
      aux = aux->eloP;
    }
    return false;
  }
  
  