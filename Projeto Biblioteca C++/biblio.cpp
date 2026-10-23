#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

class Ficheiro {
protected:
    fstream ficheiro;
    string nome_Ficheiro;

public:
    Ficheiro(const string& nome) : nome_Ficheiro(nome) {}
    ~Ficheiro() { fechar_ficheiro(); }

    bool abrir_ficheiro(ios_base::openmode modo) {
        ficheiro.open(nome_Ficheiro, modo);
        if (ficheiro.is_open()) {
            cout << "Arquivo '" << nome_Ficheiro << "' aberto com sucesso.\n";
            return true;
        } else {
            cout << "Erro ao abrir o arquivo '" << nome_Ficheiro << "'.\n";
            return false;
        }
    }

    void fechar_ficheiro() {
        if (ficheiro.is_open()) {
            ficheiro.close();
            cout << "Arquivo '" << nome_Ficheiro << "' fechado.\n";
        }
    }

    void adicionar(const string& dados) {
        if (ficheiro.is_open()) {
            ficheiro << dados << endl;
        }
    }

    void excluir() {
        fechar_ficheiro();
        if (remove(nome_Ficheiro.c_str()) != 0) {
            cout << "Erro ao excluir o ficheiro" << endl;
        } else {
            cout << "Ficheiro excluído com sucesso" << endl;
        }
    }

    vector<string> obterLinhas() {
        vector<string> resultado;
        string linha;

        if (!ficheiro.is_open()) return resultado;

        ficheiro.clear();
        ficheiro.seekg(0);
        while (getline(ficheiro, linha)) {
            resultado.push_back(linha);
        }
        return resultado;
    }
};

class Livro {
private:
    string nome;
    string autor;
    string data;
    bool estado;

public:
    Livro(string nome, string autor, string data) : nome(nome), autor(autor), data(data), estado(true) {}

    Livro(string nome, string autor, string data, bool estado) : nome(nome), autor(autor), data(data), estado(estado) {}

    static Livro listringob(const string& linha) {//Metodo de tranformar todas as linhas do ficheiro em objetos
        istringstream ss(linha);
        string nome, autor, data, estado_str;
        bool estado;

        if (!getline(ss, nome, ';') || !getline(ss, autor, ';') || !getline(ss, data, ';') || !getline(ss, estado_str, ';')) {
            throw runtime_error("Erro ao converter string para Livro");
        }

        estado = stoi(estado_str);
        return Livro(nome, autor, data, estado);
    }

    string liobstring() const {//Metodo para transformar todos os objetos em strings, objetos presentes no vetor
        ostringstream ss;
        ss << nome << ";" << autor << ";" << data << ";" << (estado ? "1" : "0");
        return ss.str();
    }

    string getNome() const{
        return nome;
    }

    string getAutor() const{
        return autor;
    }

    string getData() const{
        return data;
    }

    bool getEstado() const{
        return estado;
    }

    void setEstadoDisponivel() {
        estado = true;
    }

    void setEstadoEmprestado() {
        estado = false;
    }
};

class Aluno {
private:
    int id;
    string nome;
    string turma;
    int idade;

public:
    Aluno(int id, string nome, string turma, int idade) : id(id), nome(nome), turma(turma), idade(idade) {}

    static Aluno alstringob(const string& linha) {//Metodo de tranformar todas as linhas do ficheiro em objetos
        istringstream ss(linha);
        string str_id, nome, turma, str_idade;
        int id, idade;

        if (!getline(ss, str_id, ';') || !getline(ss, nome, ';') || !getline(ss, turma, ';') || !getline(ss, str_idade, ';')) {
            throw runtime_error("Erro ao converter string para Livro");
        }

        id = stoi(str_id);
        idade = stoi(str_idade);
        return Aluno(id, nome, turma, idade);
    }

    string alobstring() const {//Metodo para transformar todos os objetos em strings, objetos presentes no vetor
        ostringstream ss;
        ss << id << ";" << nome << ";" << turma << ";" << idade;
        return ss.str();
    }

    int getId() const{
        return id;
    }

    string getNome() const{
        return nome;
    }

    string getTurma() const{
        return turma;
    }

    int getIdade() const{
        return idade;
    }
};

class Emprestimo {
private:
    int idalu;
    string nomeli;

public:
    Emprestimo(int idalu, string& nomeli) : idalu(idalu), nomeli(nomeli) {}

    static Emprestimo emstringob(const string& linha) {//Metodo de tranformar todas as linhas do ficheiro em objetos
        istringstream ss(linha);
        string str_id, nomeli;
        int aluid;

        if (!getline(ss, str_id, ';') || !getline(ss, nomeli, ';')) {
            throw runtime_error("Erro ao converter string para Emprestimo");
        }

        aluid = stoi(str_id);
        return Emprestimo(aluid, nomeli);
    }

    string emobstring() const {//Metodo para transformar todos os objetos em strings, objetos presentes no vetor
        ostringstream ss;
        ss << idalu << ";" << nomeli;
        return ss.str();
    }

    int getIdaluno() const {//Metodo get para retornar o id do Aluno
        return idalu;
    }

    string getNomelivro() const {//Metodo get para retornar o nome do Livro
        return nomeli;
    }
};

class Biblioteca : public Ficheiro{
private:
    vector<Livro> gestli;
    vector<Aluno> gestal;
    vector<Emprestimo> gestem;

public:
    Biblioteca(const string& nomeFicheiro) : Ficheiro(nome_Ficheiro) {}

    void carregarli() {
        if (!abrir_ficheiro(ios::in)) {
            cout << "Erro ao abrir o ficheiro dos Livros." << endl;
            return;
        }

        vector<string> linhas = obterLinhas();
        gestli.clear();

        if (linhas.empty()) {
            cout << "Nenhum Livro encontrado no ficheiro." << endl;
            fechar_ficheiro();
            return;
        }

        for (const auto& linha : linhas) {
            try {
                Livro l = Livro::listringob(linha);
                gestli.push_back(l);
            } catch (const exception& e) {
                cout << "Erro ao processar a linha: " << linha << ". Erro: " << e.what() << endl;
            }
        }

        fechar_ficheiro();
        cout << "Livros carregados com sucesso." << endl;
    }

    void guardarli() {
        if (!abrir_ficheiro(ios::out | ios::trunc)) {
            cout << "Erro ao abrir ficheiro para guardar Livros." << endl;
            return;
        }

        for (const auto& livro : gestli) {
            ficheiro << livro.liobstring() << endl;
        }

        fechar_ficheiro();
    }

    void adicionarli() {
        string nome, autor, data;

        cin.ignore();
        cout << "Digite o Nome do Livro: ";
        getline(cin, nome);
        cout << "Digite o(a) Autor(a) do Livro: ";
        getline(cin, autor);
        cout << "Digite a Data de Lancamento do Livro: ";
        getline(cin, data);

        Livro novolivro(nome, autor, data);
        gestli.push_back(novolivro);
        guardarli();
        cout << "Livro adicionado com sucesso!" << endl;
    }

    void listarli() const {
        if (gestli.empty()) {
            cout << "Nenhum Livro encontrado." << endl;
            return;
        }

        cout << left << setw(3) << "Id" << setw(20) << "Nome" << setw(20) << "Autor" << setw(10) << "Data de Lancamento" << setw(10) << "Estado" << endl;
        for (const auto& livro : gestli) {
            cout << left << setw(20) << livro.getNome() << setw(20) << livro.getAutor() << setw(10) << livro.getData() << setw(10) << (livro.getEstado() ? "Disponível" : "Emprestado") << endl;
        }
    }

    void removerli() {
        listarli();

        int index;
        cout << "Digite o número do Livro que deseja remover (1 a " << gestli.size() << "): ";
        cin >> index;

        if (index < 1 || index > static_cast<int>(gestli.size())) {
            cout << "Indice invalido." << endl;
            return;
        }

        gestli.erase(gestli.begin() + (index - 1));
        guardarli();
        cout << "Livro removido com sucesso." << endl;
    }

    void carregaral() {
        if (!abrir_ficheiro(ios::in)) {
            cout << "Erro ao abrir o ficheiro dos Alunos." << endl;
            return;
        }

        vector<string> linhas = obterLinhas();
        gestal.clear();

        if (linhas.empty()) {
            cout << "Nenhum Aluno encontrado no ficheiro." << endl;
            fechar_ficheiro();
            return;
        }

        for (const auto& linha : linhas) {
            try {
                Aluno a = Aluno::alstringob(linha);
                gestal.push_back(a);
            } catch (const exception& e) {
                cout << "Erro ao processar a linha: " << linha << ". Erro: " << e.what() << endl;
            }
        }

        fechar_ficheiro();
        cout << "Alunos carregados com sucesso." << endl;
    }

    void guardaral() {
        if (!abrir_ficheiro(ios::out | ios::trunc)) {
            cout << "Erro ao abrir ficheiro para guardar Alunos." << endl;
            return;
        }

        for (const auto& aluno : gestal) {
            ficheiro << aluno.alobstring() << endl;
        }

        fechar_ficheiro();
    }

    void adicionaral() {
        string nome, turma;
        int id, idade;

        cin.ignore();
        cout << "Digite o Id do Aluno: ";
        cin >> id;
        cout << "Digite o Nome do Aluno: ";
        getline(cin, nome);
        cout << "Digite a Turma do Aluno: ";
        getline(cin, turma);
        cout << "Digite a Idade do Aluno: ";
        cin >> idade;

        Aluno novoaluno(id, nome, turma, idade);
        gestal.push_back(novoaluno);
        guardaral();
        cout << "Aluno adicionado com sucesso!" << endl;
    }

    void listaral() const {
        if (gestal.empty()) {
            cout << "Nenhum Aluno encontrado." << endl;
            return;
        }

        cout << left << setw(3) << "Id" << setw(20) << "Nome" << setw(10) << "Turma" << setw(5) << "Idade" << endl;
        for (const auto& aluno : gestal) {
            cout << left << setw(3) << aluno.getId() << setw(20) << aluno.getNome() << setw(10) << aluno.getTurma() << setw(5) << aluno.getIdade() << endl;
        }
    }

    void carregarem() {
        if (!abrir_ficheiro(ios::in)) {
            cout << "Erro ao abrir o ficheiro dos Emprestimos." << endl;
            return;
        }

        vector<string> linhas = obterLinhas();
        gestem.clear();

        if (linhas.empty()) {
            cout << "Nenhum Emprestimo encontrado no ficheiro." << endl;
            fechar_ficheiro();
            return;
        }

        for (const auto& linha : linhas) {
            try {
                Emprestimo p = Emprestimo::emstringob(linha);
                gestem.push_back(p);
            } catch (const exception& e) {
                cout << "Erro ao processar a linha: " << linha << ". Erro: " << e.what() << endl;
            }
        }

        fechar_ficheiro();
        cout << "Emprestimos carregados com sucesso." << endl;
    }

    void guardarem() {
        if (!abrir_ficheiro(ios::out | ios::trunc)) {
            cout << "Erro ao abrir ficheiro para guardar Emprestimos." << endl;
            return;
        }

        for (const auto& emprestimo : gestem) {
            ficheiro << emprestimo.emobstring() << endl;
        }

        fechar_ficheiro();
    }

    void adicionarem() {
        int idalu;
        string nomeli;

        cout << "Lista de Alunos Disponiveis:" << endl;
        for (const auto& aluno : gestal) {
            cout << aluno.getId() << " - " << aluno.getNome() << endl;
        }

        cout << "Digite o ID do Aluno que deseja emprestar um livro: ";
        cin >> idalu;

        cout << "Lista de Livros Disponiveis:" << endl;
        for (const auto& livro : gestli) {
            if (livro.getEstado()) {
                cout << livro.getNome() << " - " << livro.getAutor() << endl;
            }
        }

        cin.ignore();
        cout << "Digite o Nome do Livro que deseja emprestar: ";
        getline(cin, nomeli);

        bool alunoExiste = false;
        for (const auto& aluno : gestal) {
            if (aluno.getId() == idalu) {
                alunoExiste = true;
                break;
            }
        }

        if (!alunoExiste) {
            cout << "Aluno nao encontrado!" << endl;
            return;
        }

        bool livroDisponivel = false;
        for (auto& livro : gestli) {
            if (livro.getNome() == nomeli && livro.getEstado()) {
                livroDisponivel = true;
                livro.setEstadoEmprestado();
                break;
            }
        }

        if (!livroDisponivel) {
            cout << "Livro nao encontrado ou ja emprestado!" << endl;
            return;
        }

        Emprestimo novoEmprestimo(idalu, nomeli);
        gestem.push_back(novoEmprestimo);
        guardarem();

        cout << "Emprestimo registado com sucesso!" << endl;
    }

    void listarem() const {
        if (gestem.empty()) {
            cout << "Nenhum Emprestimo ativo encontrado." << endl;
            return;
        }

        cout << left << setw(15) << "Aluno"
             << setw(20) << "Livro"
             << setw(20) << "Autor" << endl;

        for (const auto& emp : gestem) {
            string nomealu;
            string autorli;

            for (const auto& aluno : gestal) {
                if (aluno.getId() == emp.getIdaluno()) {
                    nomealu = aluno.getNome();
                    break;
                }
            }

            for (const auto& livro : gestli) {
                if (livro.getNome() == emp.getNomelivro()) {
                    autorli = livro.getAutor();
                    break;
                }
            }

            cout << left << setw(15) << nomealu
                 << setw(20) << emp.getNomelivro()
                 << setw(20) << autorli << endl;
        }
    }

    void devolverli() {//Metodo de devolver Livros
        string nomeli;
        cout << "Digite o Nome do Livro que deseja devolver: ";
        cin.ignore();
        getline(cin, nomeli);

        for (auto it = gestem.begin(); it != gestem.end(); ++it) {
            if (it->getNomelivro() == nomeli) {
                for (auto& livro : gestli) {
                    if (livro.getNome() == nomeli) {
                        livro.setEstadoDisponivel();
                        break;
                    }
                }

                gestem.erase(it);
                guardarem();
                cout << "Livro devolvido com sucesso!" << endl;
                return;
            }
        }

        cout << "Livro nao encontrado nos emprestimos ativos!" << endl;
    }
    //Este metodo pergunta qual livro quero devolver, depois percorre o vetor dos livros até encontrar o livro escrito e chama um outro metodo para mudar o
    //estado do livro para disponivel

};

void menu() { //Menu principal
    cout << "+-----------------------------------+" << endl;
    cout << "|            Biblioteca             |" << endl;
    cout << "+-----------------------------------+" << endl;
    cout << "| 1. Menu Livros                    |" << endl;
    cout << "| 2. Menu Alunos                    |" << endl;
    cout << "| 3. Menu Emprestimos               |" << endl;
    cout << "| 0. Sair                           |" << endl;
    cout << "+-----------------------------------+" << endl;
    cout << "Escolha uma opcao: ";
};

void menuli() {//Menu dos Livros
    cout << "+-----------------------------------+" << endl;
    cout << "|          Menu de Livros           |" << endl;
    cout << "+-----------------------------------+" << endl;
    cout << "| 1. Adicionar Livro                |" << endl;
    cout << "| 2. Listar Livros                  |" << endl;
    cout << "| 4. Remover Livro                  |" << endl;
    cout << "| 0. Voltar                         |" << endl;
    cout << "+-----------------------------------+" << endl;
    cout << "Escolha uma opcao: ";
};

void menualu() {//Menu dos Alunos
    cout << "+-----------------------------------+" << endl;
    cout << "|          Menu de Eventos          |" << endl;
    cout << "+-----------------------------------+" << endl;
    cout << "| 1. Adicionar Aluno                |" << endl;
    cout << "| 2. Listar Aluno                   |" << endl;
    cout << "| 0. Voltar                         |" << endl;
    cout << "+-----------------------------------+" << endl;
    cout << "Escolha uma opcao: ";
};

void menuemp() {//Menu dos Emprestimos
    cout << "+-----------------------------------+" << endl;
    cout << "|        Menu de Emprestimos        |" << endl;
    cout << "+-----------------------------------+" << endl;
    cout << "| 1. Adicionar Emprestimo           |" << endl;
    cout << "| 2. Listar Emprestimo              |" << endl;
    cout << "| 3. Devolver Livro                 |" << endl;
    cout << "| 0. Voltar                         |" << endl;
    cout << "+-----------------------------------+" << endl;
    cout << "Escolha uma opcao: ";
};

int main() {//Criação de um obejto para cada classe, utilizando o nome do ficheiro da informação que vai ser guardada.
    Biblioteca bibliolivro("livros.txt");
    Biblioteca biblioaluno("alunos.txt");
    Biblioteca biblioemprestimo("emprestimos.txt");

    int opcao;//Opção para o menu principal

    bibliolivro.carregarli();
    biblioaluno.carregaral();
    biblioemprestimo.carregarem();
    //È chamada as funções de carregar as informações para saber se existe ou não algum registo nos ficheiros.

    do {//Ciclo do...while para fazer sempre o menu até que a variavel opção seja 0.
        menu();//Chama o menu principal
        cin >> opcao;
        cin.ignore();

            switch (opcao) {//Um switch pois sabemos quais vão ser as opções, 1 - Menu de Livros, 2 - Menu de Alunos,
                                                                            //3 - Menu de Emprestimos
            case 1:
                int opcaoli;//Inicializamos uma outra variavel para saber qual opção do menu Livros que o utilizador escolheu
                do{
                    menuli();
                    cin >> opcaoli;

                    switch(opcaoli){
                    case 1://Opção de Adicionar Livros
                        bibliolivro.adicionarli();
                        break;
                    case 2://Opção de Listar Livros
                        bibliolivro.listarli();
                        break;
                    case 3://Opção de Remover Livros
                        bibliolivro.removerli();
                        break;
                    case 0:
                        cout << "A Voltar..." << endl;//Caso o utilizador escreva 0 o progrma retrocede ao menu principal
                        break;
                    default:
                        cout << "Opção inválida." << endl;
                        break;
                    }

                    cout << endl;
                } while (opcaoli != 0);

                break;
            case 2://Inicializamos uma outra variavel para saber qual opção do menu Alunos que o utilizador escolheu
                int opcaoalu;
                do{
                    menualu();
                    cin >> opcaoalu;

                    switch(opcaoalu){
                    case 1://Opção de Adicionar Alunos
                        biblioaluno.adicionaral();
                        break;
                    case 2://Opção de Listar Alunos
                        biblioaluno.listaral();
                        break;
                    case 0:
                        cout << "A Voltar..." << endl;//Caso o utilizador escreva 0 o progrma retrocede ao menu principal
                        break;
                    default:
                        cout << "Opção inválida." << endl;
                        break;
                    }

                    cout << endl;
                } while (opcaoalu != 0);

                break;
            case 3://Inicializamos uma outra variavel para saber qual opção do menu Emprestimos que o utilizador escolheu
                int opcaoemp;
                do{
                    menuemp();
                    cin >> opcaoemp;

                    switch(opcaoemp){
                    case 1://Opção de Adicionar Emprestimos
                        biblioemprestimo.adicionarem();
                        break;
                    case 2://Opção de Listar Emprestimos
                        biblioemprestimo.listarem();
                        break;
                    case 3://Opção de Devolver Livro
                        biblioemprestimo.listarem();
                        biblioemprestimo.devolverli();
                        break;
                    case 0:
                        cout << "A Voltar..." << endl;//Caso o utilizador escreva 0 o progrma retrocede ao menu principal
                        break;
                    default:
                        cout << "Opção inválida." << endl;
                        break;
                    }

                    cout << endl;
                } while (opcaoemp != 0);
                break;
            case 0:
                cout << "A Fechar o Sistema..." << endl;//Caso o utilizador escolha 0 o programa fecha.
                break;
            default:
                cout << "Opcao invalida." << endl;//Caso o utilizador escreva uma outra opção não presente, mostra uma mensagem de opção inválida
                break;
            }

        cout << endl;
    } while (opcao != 0);

    return 0;
}

