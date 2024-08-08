#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>
#include <chrono> // Para medições de tempo
#include <set> //Utilizar o set, para unificar
#include <map> //Utilização de map

using namespace std;
using namespace chrono; // contagem de tempo

class Filme {
public:
  long idFilme;
  string tipoDoFilme;
  string tituloPrimario;
  string tituloOriginal;
  bool isAdult;
  int anoLancamento;
  int anoTermino;
  int duracao;
  string genero;

  //Contrutor Sobrecarregado
  Filme(long idFilme, const string& tipoDoFilme, const string& tituloPrimario, const string& tituloOriginal,
  bool isAdult, int anoLancamento, int anoTermino, int duracao, const string& genero)
: idFilme(idFilme), tipoDoFilme(tipoDoFilme), tituloPrimario(tituloPrimario), tituloOriginal(tituloOriginal),
  isAdult(isAdult), anoLancamento(anoLancamento), anoTermino(anoTermino), duracao(duracao), genero(genero) {}
};

class Cinema {
public:
  string id;
  string nomeDoCinema;
  double cordX;
  double cordY;
  float preco;
  vector<Filme> filmes;

  Cinema(string id, string nomeDoCinema, double cordX, double  cordY, float preco, vector<Filme> filmes)
  : id(id), nomeDoCinema(nomeDoCinema), cordX(cordX), cordY(cordY), preco(preco), filmes(filmes) {}
};

long removeLetraID(string id){
  string idLong;
    for (char ch : id) {
        // Verifica se o caractere não é uma letra
        if (!isalpha(ch)) {
            idLong += ch;
        }
    }
    return stol(idLong);
}

vector<Filme> lerArquivoFilme(){
  ifstream arquivo("filmesCrop.txt");
  vector<Filme> filmes;
  string linha;
  if (arquivo.is_open()) {
    getline(arquivo, linha);

    while (getline(arquivo, linha)) {
      istringstream ss(linha);
      string ids, tipo, tituloPrimario, tituloOriginal, isAdultStr, anoLancamentoStr,anoTerminoStr,duracaoStr,genero;

      getline(ss, ids, '\t');
      getline(ss, tipo, '\t');
      getline(ss, tituloPrimario, '\t');
      getline(ss, tituloOriginal, '\t');
      getline(ss, isAdultStr, '\t');
      getline(ss, anoLancamentoStr, '\t');
      getline(ss, anoTerminoStr, '\t');
      getline(ss, duracaoStr, '\t');
      getline(ss, genero, '\t');
      
      try {
        bool isAdult = (isAdultStr != "\\N" && isAdultStr == "1");
        int anoLancamento = (anoLancamentoStr != "\\N" && !anoLancamentoStr.empty()) ? stoi(anoLancamentoStr) : 0;
        int anoTermino = (anoTerminoStr != "\\N" && !anoTerminoStr.empty()) ? stoi(anoTerminoStr) : 0;
        int duracao = (duracaoStr != "\\N" && !duracaoStr.empty()) ? stoi(duracaoStr) : 0;
        if(genero =="\\N"){
          genero = "NULL";
        }
        long idLong = (!ids.empty()) ? removeLetraID(ids):0;
        Filme filme(idLong, tipo, tituloPrimario, tituloOriginal, isAdult, anoLancamento, anoTermino, duracao, genero);
              
        filmes.push_back(filme);
      } catch (const invalid_argument& e) {
          cerr << "Erro de conversão: argumento inválido para ano de lançamento, ano de término ou duração" << endl;
      } catch (const out_of_range& e) {
          cerr << "Erro de conversão: valor fora do intervalo para ano de lançamento, ano de término ou duração" << endl;
      }
    }
    arquivo.close();
  }else {
      cerr << "Não foi possível abrir o arquivo." << endl;
      return filmes;
  }

  //Repetição para Printar os valores dentro do vector FILMES
  // for (const auto& filme : filmes) {
  //   cout << "ID do Filme: " << filme.idFilme
  //        << ", Tipo do Filme: " << filme.tipoDoFilme
  //        << ", Título Primário: " << filme.tituloPrimario
  //        << ", Título Original: " << filme.tituloOriginal
  //        << ", Adulto: " << (filme.isAdult ? "Sim" : "Não")
  //        << ", Ano de Lançamento: " << filme.anoLancamento
  //        << ", Ano de Término: " << filme.anoTermino
  //        << ", Duração: " << filme.duracao
  //        << ", Gênero: " << filme.genero << endl;
  // }

  return filmes;
}

vector<Cinema> lerArquivoCinema(vector<Filme> filmes){
  ifstream arquivo("cinemas.txt");
  vector<Cinema> cinemas;
  vector<Filme> filmesCinema;
  string linha;
  int auxFilmes; //auxiliar para atribuir os filmes do cinema

  if (arquivo.is_open()) {
    getline(arquivo, linha);

    while (getline(arquivo, linha)) {
      istringstream ss(linha);
      string ids, nomeDoCinema, cordXStr, cordYStr, precoStr,idFilme;
      vector<long> idFilmes;

      //>> ws serve para ignorar qualquer espaço em branco que venha antes
      getline(ss, ids, ',');
      getline(ss >> ws, nomeDoCinema, ',');
      getline(ss >> ws, cordXStr, ',');
      getline(ss >> ws, cordYStr, ',');
      getline(ss >> ws, precoStr, ',');
      while(getline(ss >> ws, idFilme, ',')){
        long id = removeLetraID(idFilme);
        idFilmes.push_back(id);
      }
    
      for (const auto& idFilme : idFilmes){
        auxFilmes = 0;
        for(const auto& filme : filmes){
          if(idFilme-filme.idFilme <= 0){
            filmesCinema.push_back(filme);
            break;
          }
        }
      }
     
      double cordX = stod(cordXStr);
      double cordY = stod(cordYStr);
      float preco = stof(precoStr);
      
      Cinema cinema(ids, nomeDoCinema,cordX,cordY,preco,filmesCinema);
      
      cinemas.push_back(cinema);
      filmesCinema.clear();
    }
    arquivo.close();
  }else {
      cerr << "Não foi possível abrir o arquivo." << endl;
      return cinemas;
  }
  
  return cinemas;
}

map<string,vector<Filme*>> gerarMatrizGenero(vector<Filme>& filmes, string genero,map<string,vector<Filme*>>& matrizGenero){
 
  string palavra;
  for(int i=0;i<filmes.size();i++){
    stringstream ss(filmes[i].genero); 
    while (getline(ss, palavra, ',')) { //Pega as palavras do genero separados por virgula
      if(palavra == genero)
      matrizGenero[genero].emplace_back(&filmes[i]);
    }
    
  }
  
  return matrizGenero;
} 

map<string,vector<Filme*>> gerarMatrizTipo(vector<Filme>& filmes, string tipoFilme,map<string,vector<Filme*>>& matrizTipo){
 
  string palavra;
  for(int i=0;i<filmes.size();i++){
    stringstream ss(filmes[i].tipoDoFilme); 
    while (getline(ss, palavra, ',')) { //Pega as palavras do tipoFilme separados por virgula
      if(palavra == tipoFilme)
      matrizTipo[tipoFilme].emplace_back(&filmes[i]);
    }
    
  }
  
  return matrizTipo;
} 

void separarTipos(const string& genero, set<string>& set_) {
    stringstream ss(genero);
    string item;
    while (getline(ss, item, ',')) {
        set_.insert(item);
    }
}

//Função para printar os vectors de filmes
void printVector(vector<Filme*>& filmes){
  for(Filme* filme : filmes){
    cout << filme->tituloOriginal << endl;
  }
}

int main() {
  auto inicioTempo = high_resolution_clock::now(); //Inicio contagem de tempo inicializacao

  vector<Filme> filmes = lerArquivoFilme();
  vector<Cinema> cinemas = lerArquivoCinema(filmes);

  auto fimTempo = high_resolution_clock::now(); //Fim da contagem de tempoi inicializacao
  duration<double> duracao = (fimTempo - inicioTempo);

  set<string> generos;    //O set permite a unificação dos elementos
  set<string> tiposFilme; //

  for(Filme filme : filmes){
    separarTipos(filme.genero, generos); //Separa os filmes Generos
  }
  for(Filme filme : filmes){
    separarTipos(filme.tipoDoFilme, tiposFilme); //Separa os filmes por Tipo
  }

  //vector<Filme*> filmesSport = gerarMatrizGenero(filmes, "Sport");
  map<string,vector<Filme*>> matrizGeneros;
  map<string,vector<Filme*>> matrizTipos;

  for (string elem : generos) {
    matrizGeneros = (gerarMatrizGenero(filmes,elem,matrizGeneros));
  }

  for (string elem : tiposFilme) {
    matrizTipos = (gerarMatrizTipo(filmes,elem,matrizTipos));
  }
  


  //TENTAR PEGAR APENAS UM GENERO
  auto i = matrizTipos.find("video");
  for (const auto& [tiposFilme, filmes] : matrizTipos) {
      if(i != matrizTipos.end()){
        cout << tiposFilme << ":\n";
        for (const auto& filme : filmes) {
            cout << "  " << filme->tituloOriginal << " (" << filme->tipoDoFilme << ")\n";
        }
      }
  }


  //printVector(filmesSport);
  // for(Cinema cinema : cinemas){
  //   cout << "Nome cinema: " << cinema.nomeDoCinema << endl;
  //   cout << "filmes: ";
  //   for(Filme filme : cinema.filmes){
  //     cout << filme.tituloOriginal << endl;
  //   }
  // }
  cout << "Tempo de Inicialização(segundos): " << duracao.count() << endl;
  return 0;
}