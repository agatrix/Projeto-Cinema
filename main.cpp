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
  ifstream arquivo("f.txt");
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

vector<Filme*> geraVectorGenero(vector<Filme>& filmes, string genero){
  vector<Filme*> vectorGenero;
  string palavra;

  for(int i=0;i<filmes.size();i++){
    stringstream ss(filmes[i].genero); 
    while (getline(ss, palavra, ',')) { //Pega as palavras do genero separados por virgula
      if(palavra == genero)
      vectorGenero.push_back((&filmes[i]));
    }
    
  }

  return vectorGenero;
}

vector<Filme*> geraVectorTipo(vector<Filme>& filmes, string tipo){
  vector<Filme*> vectorTipo;
  string palavra;

  for(int i=0;i<filmes.size();i++){
    stringstream ss(filmes[i].tipoDoFilme); 
    while (getline(ss, palavra)) { //Pega as palavras do tipoDoFilme separados por virgula
      if(palavra == tipo)
      vectorTipo.push_back((&filmes[i]));
    }
    
  }

  return vectorTipo;
} 

vector<Filme*> geraVectorDuracao(vector<Filme>& filmes, int duracao){
  vector<Filme*> vectorDuracao;
  string palavra;

  for(int i=0;i<filmes.size();i++){
    stringstream ss(to_string(filmes[i].duracao)); 
    while (getline(ss, palavra)) {
      if(palavra == to_string(duracao))
      vectorDuracao.push_back((&filmes[i]));
    }
    
  }

  return vectorDuracao;
} 

void separarTipos(const string& str, set<string>& set_) {
    stringstream ss(str);
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

// Função para realizar a busca binária
int buscaBinaria(const vector<Filme*>& filmes, int valor_procurado) {
    int esquerda = 0;
    int direita = filmes.size() - 1;

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;

        // Verifica se o valor_procurado está no meio
        if (filmes[meio]->idFilme == valor_procurado) {
            return meio; // Valor encontrado, retorna o índice
        }

        // Se o valor_procurado é maior, ignore a metade esquerda
        if (filmes[meio]->idFilme < valor_procurado) {
            esquerda = meio + 1;
        } 
        // Se o valor_procurado é menor, ignore a metade direita
        else {
            direita = meio - 1;
        }
    }
    return -1;
}

vector<Filme*> filtrar(vector<Filme*>& filmes1, vector<Filme*> filmes2, vector<Filme*>solucao){
  

  return solucao;
}

// Função para trocar dois elementos
void swap(vector<int>& vetorD, int i, int j) {
    int temp = vetorD[i];
    vetorD[i] = vetorD[j];
    vetorD[j] = temp;
}

// Função para particionar o vetor
int particionar(vector<int>& vec, int baixo, int alto) {
    int pivo = vec[alto];  // Escolhe o último elemento como pivô
    int i = baixo - 1;     // Índice do menor elemento

    for (int j = baixo; j < alto; ++j) {
        if (vec[j] < pivo) {
            ++i;
            swap(vec, i, j);
        }
    }

    swap(vec, i + 1, alto);
    return i + 1; // Retorna o índice do pivô
}

// Função recursiva para aplicar QuickSort
void quickSort(vector<int>& vec, int baixo, int alto) {
    if (baixo < alto) {
        int pivoIndex = particionar(vec, baixo, alto);  // Particiona o vetor

        // Ordena as duas metades separadamente
        quickSort(vec, baixo, pivoIndex - 1);
        quickSort(vec, pivoIndex + 1, alto);
    }
}

// Função auxiliar para iniciar o QuickSort
void quickSort(vector<int>& vec) {
    quickSort(vec, 0, vec.size() - 1);
}

int main() {
  auto inicioTempo = high_resolution_clock::now(); //Inicio contagem de tempo inicializacao

  vector<Filme> filmes = lerArquivoFilme();
  vector<Cinema> cinemas = lerArquivoCinema(filmes);

  set<string> generos;    //O set permite a unificação dos elementos
  set<string> tiposFilme; 
  set<string> duracaosFilmes;

  vector<vector<Filme*>> matrizGenero; //Cria uma matriz onde cada linha possui filmes um genero
  vector<vector<Filme*>> matrizDuracao;

  vector<int> tamanhoGenero; //Pega o tamanho de cada linha, ou seja, a quantidade de filmes daquele genero
  vector<int> tamanhoDuracao;

  for(Filme filme : filmes){
    separarTipos(filme.genero, generos); //Separa os filmes Generos
  }

  for(Filme filme : filmes){
    separarTipos(filme.tipoDoFilme, tiposFilme); //Separa os filmes por Tipo
  }

  for(Filme filme : filmes){
    separarTipos(to_string(filme.duracao), duracaosFilmes); //Separa os filmes por Duracao
  }

  //Criacao do vector de duracao
  vector<int> duracoesFilme;
  for(auto x : duracaosFilmes){
    duracoesFilme.push_back(stoi(x));
  }
  quickSort(duracoesFilme);

  for(auto x : duracoesFilme)
    cout << x << endl;


  for(string genero : generos){ //Cria matriz de generos
    matrizGenero.push_back(geraVectorGenero(filmes,genero));
  }

  for(int duracoes : duracoesFilme){ //Cria matriz de duracao
    matrizDuracao.push_back(geraVectorDuracao(filmes,duracoes));
  }

  // for(int i=0 ; i<4; i++){
  //   cout << matrizDuracao[i][0]->duracao << endl;
  // }


  for(auto x : matrizGenero){ //Repeticao para pegar a quantiadade de filmes por generos 
    tamanhoGenero.push_back(x.size());
  }

  for(int i = 0; i < duracoesFilme.size(); i++){
    if(matrizDuracao[i][0]->duracao==12){
      for(auto x:matrizDuracao[i]){
        cout << x->tituloOriginal << " ";
      }
      cout << endl;
    }
  }

  vector<Filme*> solucao;

  // if(tamanhoGenero[0]<tamanhoGenero[2]){
  //   for(int i=0;i<tamanhoGenero[2];i++){
  //     solucao.push_back(matrizGenero[2][i]);
  //   }
    
  //   for(int j = 0; j<tamanhoGenero[0];j++){
  //     int indice = buscaBinaria(matrizGenero[2],matrizGenero[0][j]->idFilme);
  //     if(indice == -1)
  //       cout << matrizGenero[0][j]->tituloOriginal << endl;
  //   }

  // }


  auto fimTempo = high_resolution_clock::now(); //Fim da contagem de tempoi inicializacao
  duration<double> duracao = (fimTempo - inicioTempo);

  cout << "Tempo de Inicialização(segundos): " << duracao.count() << endl;
  return 0;
}