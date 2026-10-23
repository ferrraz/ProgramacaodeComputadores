#include <iostream>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;


void parseXML(const string& ficheiro) {
    XMLDocument doc;
    doc.LoadFile(ficheiro.c_str());

    if(doc.Error()) {
        cout << "Erro ao abrir o ficheiro.." << endl;
        return;
    }

    const char* id;
    const char* nome;
    const char* localizacao;

    XMLElement* rootElement = doc.RootElement();
    if (!rootElement) {
        cout << "Erro: XML sem elemento root." << endl;
        return;
    }

    XMLElement* lojasElement = rootElement->FirstChildElement("lojas");
    if (!lojasElement) {
        cout << "Erro: Elemento <lojas> não encontrado." << endl;
        return;
    }

    XMLElement* loja = lojasElement->FirstChildElement("loja");

    while (loja) {

        cout << "+--------------------------------------------" << endl;
        cout << "|                                     " << endl;

        XMLElement* idElement = loja->FirstChildElement("Id");
        if (idElement && idElement->GetText()) {
            id = idElement->GetText();
            cout << "| Id da Loja: " << id << "                        " << endl;
        }

        XMLElement* name = loja->FirstChildElement("Nome");
        if (name && name->GetText()) {
            nome = name->GetText();
            cout << "| Nome da Loja: " << nome << "            " << endl;
        } else {
            cout << "| Nome da Loja: (nao encontrado)" << "              " << endl;
        }

        XMLElement* location = loja->FirstChildElement("Localizacao");
        if (location && location->GetText()) {
            localizacao = location->GetText();
            cout << "| Localizacao da Loja: " << localizacao << "                " << endl;
        } else {
            cout << "Localizacao da Loja: (nao encontrado)" << "                        " << endl;
        }

        cout << "|  " << endl;

        loja = loja->NextSiblingElement("loja");
    }

    cout << "+--------------------------------------------" << endl;
}

void insertxml(const string& ficheiro) {
    XMLDocument doc;
    doc.LoadFile(ficheiro.c_str());

    if (doc.Error()) {
        cout << "Erro ao abrir o ficheiro.." << endl;
        return;
    }

    char id[100];
    char name[100];
    char location[100];

    cout << "Qual o Id da Loja?: ";
    cin >> id;

    cout << "Qual o nome da Loja?: ";
    cin >> name;

    cout << "Qual a localizacao da Loja?: ";
    cin >> location;

    XMLElement* root = doc.RootElement();
    if (!root) {
        cout << "Erro: Elemento root nao encontrado." << endl;
        return;
    }

    XMLElement* lojasElement = root->FirstChildElement("lojas");
    if (!lojasElement) {
        cout << "Erro: Elemento <lojas> nao encontrado." << endl;
        return;
    }

    for (XMLElement* loja = lojasElement->FirstChildElement("loja"); loja != nullptr; loja = loja->NextSiblingElement("loja")) {
        XMLElement* idElement = loja->FirstChildElement("Id");
        if (idElement && idElement->GetText() && strcmp(idElement->GetText(), id) == 0) {
            cout << "Ja existe uma loja com o ID " << id << ". Insercao cancelada." << endl;
            return;
        }
    }

    XMLElement* lojaElement = doc.NewElement("loja");

    XMLElement* idElement = doc.NewElement("Id");
    idElement->SetText(id);
    lojaElement->InsertEndChild(idElement);

    XMLElement* nameElement = doc.NewElement("Nome");
    nameElement->SetText(name);
    lojaElement->InsertEndChild(nameElement);

    XMLElement* locationElement = doc.NewElement("Localizacao");
    locationElement->SetText(location);
    lojaElement->InsertEndChild(locationElement);

    lojasElement->InsertEndChild(lojaElement);

    doc.SaveFile(ficheiro.c_str());

    cout << "Loja adicionada com sucesso." << endl;
}


void deleteXML(const string& ficheiro) {
    XMLDocument doc;
    doc.LoadFile(ficheiro.c_str());

    if (doc.Error()) {
        cout << "Erro ao abrir o ficheiro.." << endl;
        return;
    }

    char id[100];

    cout << "Qual o Id da Loja que quer remover?: ";
    cin >> id;

    XMLElement* root = doc.RootElement();
    if (!root) {
        cout << "Erro: XML sem elemento root." << endl;
        return;
    }

    XMLElement* lojasElement = root->FirstChildElement("lojas");
    if (!lojasElement) {
        cout << "Erro: Elemento <lojas> nao encontrado." << endl;
        return;
    }

    bool apagado = false;

    for (XMLElement* loja = lojasElement->FirstChildElement("loja"); loja != nullptr; ) {
        XMLElement* next = loja->NextSiblingElement("loja");

        XMLElement* idElement = loja->FirstChildElement("Id");
        if (idElement && idElement->GetText() && strcmp(idElement->GetText(), id) == 0) {
            lojasElement->DeleteChild(loja);
            cout << "Loja com ID " << id << " foi removida com sucesso." << endl;
            apagado = true;
            break;
        }

        loja = next;
    }

    if (!apagado) {
        cout << "Nenhuma loja com o ID " << id << " foi encontrada." << endl;
    }

    doc.SaveFile(ficheiro.c_str());
}

void menu(){
    cout << "+--------------------------------+" << endl;
    cout << "|         Menu das Lojas         |" << endl;
    cout << "+--------------------------------+" << endl;
    cout << "|                                |" << endl;
    cout << "|     1. Adicionar uma Loja      |" << endl;
    cout << "|     2. Ver todas as Lojas      |" << endl;
    cout << "|     3. Deletar uma Loja        |" << endl;
    cout << "|                                |" << endl;
    cout << "|     0. Sair                    |" << endl;
    cout << "|                                |" << endl;
    cout << "+--------------------------------+" << endl;
    cout << "Escolha uma opcao: ";
}

int main(){

    int opcao;
    do{
        menu();
        cin >> opcao;
        cin.ignore();

        switch(opcao){
        case 1:
            insertxml("exemplo.xml");
            break;
        case 2:
            parseXML("exemplo.xml");
            break;
        case 3:
            parseXML("exemplo.xml");
            deleteXML("exemplo.xml");
            break;
        case 0:
            cout << "A fechar o programa..." << endl;
            break;
        default:
            cout << "Opcao desconhecida." << endl;
            break;
        }
        cout << endl;

    }while(opcao != 0);

    return 0;
}

