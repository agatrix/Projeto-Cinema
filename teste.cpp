#include <fstream>
#include <iostream>
#include<istream>>;
#include <list>
#include <sstream>
#include <string>
#include <vector>
#include <chrono> // Para medições de tempo
#include <set> //Utilizar o set, para unificar
//#include <map> //Utilização de map

using namespace std;
using namespace chrono; // contagem de tempo

//void int escolheOperacao(int valor)

void filtrarPorGenero() {
    vector<int> generosSelecionados;
    int genero, outroGenero, tipoFiltro;

    do {
        cin >> genero;

        // Simule o armazenamento do gênero selecionado
        // Na prática, você pode associar cada número a um gênero
        generosSelecionados.push_back((genero));

        cout << "Deseja filtrar por outro gênero? (1-Sim, 0-Não): ";
        cin >> outroGenero;

        if (outroGenero == 1) {
            cout << "Deseja aplicar filtro AND ou OR? (0-AND, 1-OR): ";
            cin >> tipoFiltro;

            // Aqui, você pode aplicar lógica de filtro com base na escolha do usuário
        }
    } while (outroGenero == 1);

    // Após a seleção, o programa volta ao menu de filtros
}
void filtrarPorTipo() {
    vector<int> tipoSelecionados;
    int tipo, outroTipo, tipoFiltro;

    do {
        cin >> tipo;

        // Simule o armazenamento do gênero selecionado
        // Na prática, você pode associar cada número a um gênero
        tipoSelecionados.push_back((tipo));

        cout << "Deseja filtrar por outro gênero? (1-Sim, 0-Não): ";
        cin >> outroTipo;

        if (outroTipo == 1) {
            cout << "Deseja aplicar filtro AND ou OR? (0-AND, 1-OR): ";
            cin >> tipoFiltro;

            // Aqui, você pode aplicar lógica de filtro com base na escolha do usuário
        }
    } while (outroTipo == 1);

    // Após a seleção, o programa volta ao menu de filtros
}
void filtrarPorDuracao(){
    int intervaloOrExpecifico,duracao,outraDuracao,tipoOperacao;

    cout << "filtrar por intervalo ou valor especifico?" << endl << "(0)Especidico (1)Intervalo" << endl;
    cin >> intervaloOrExpecifico;

    if(intervaloOrExpecifico == 0){
        cout << "Digite a duração"<< endl;
        cin >> duracao;
        //filtra pela duração
        cout << "Outra duracao? (0)Nao (1)Sim";
        cin >> outraDuracao;
        while(outraDuracao==1){
            
            cout << "tipo operação? (0)OR (1)E";
            cin >> tipoOperacao;
            if(tipoOperacao==0)
                //filtraDuracao OR
            else
                //filtraduracao E
        }
        
    }else if(intervaloOrExpecifico==1){
        int inicio,fim;
        cout << "digite o valor inicial"<<endl;
        cin >> inicio;
        cout << "digite o valor final"<<endl;
        cin >> fim;

        //filtra pelo intervalo
    }
}
void filtrarPorAno(){
    int intervaloOrExpecifico,Ano,outroAno,tipoOperacao;

    cout << "filtrar por intervalo ou valor especifico?" << endl << "(0)Especidico (1)Intervalo" << endl;
    cin >> intervaloOrExpecifico;

    if(intervaloOrExpecifico == 0){
        cout << "Digite o Ano"<< endl;
        cin >> Ano;
        //filtra pelo ano
        cout << "Outro Ano? (0)Nao (1)Sim";
        cin >> outroAno;
        while(outroAno==1){
            cout << "tipo operação? (0)OR (1)E";
            cin >> tipoOperacao;
            if(tipoOperacao==0)
                //filtraDuracao OR
            else
                //filtraduracao E

            cout << "Outro Ano? (0)Nao (1)Sim";
            cin >> outroAno; 
            
        }
        
        
    }else if(intervaloOrExpecifico==1){
        int inicio,fim;
        cout << "digite o valor inicial"<<endl;
        cin >> inicio;
        cout << "digite o valor final"<<endl;
        cin >> fim;

        //filtra pelo intervalo
    }


}
int main(){
    int escolha1;
    int escolhaFiltro,escolhaGenero,outroFIltro,juntaFiltros;

    do{
        cout << "(1)Filmes" << endl << "(2)Cinemas" << endl << "(0)Sair" << endl;
        cin >> escolha1;

        if(escolha1 == 1){
            cout << "(1)Genero" << endl << "(2)Tipo" << endl << "(3)Duracao" << endl;
            cin >> escolhaFiltro;

            switch (escolhaFiltro)
            {
            case 1:
                filtrarPorGenero();
                break;
            case 2:
                filtrarPorTipo();
                break;
            case 3:
                filtrarPorDuracao();
                break;
            case 4:
                filtrarPorAno();
                break;
            default:
                break;
            }

            cout << "Outro FIltro? (1)Sim (0)Não" <<endl;
            cin >> outroFIltro;
            if(outroFIltro == 1){
                cout << "filtrar usando (1)AND (2)OR";
                cin >> juntaFiltros;
            }
        }(outroFIltro != 0);

    }while(escolha1 != 0);//Cinemas ou filmes
}