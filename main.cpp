#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono> // Para medições de tempo
#include <set> //Utilizar o set, para unificar

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

vector<Filme> geraVectorGenero(vector<Filme>& filmes, string genero){
  vector<Filme> vectorGenero;
  string palavra;

  for(int i=0;i<filmes.size();i++){
    stringstream ss(filmes[i].genero); 
    while (getline(ss, palavra, ',')) { //Pega as palavras do genero separados por virgula
      if(palavra == genero)
      vectorGenero.push_back((filmes[i]));
    }
    
  }

  return vectorGenero;
}

vector<Filme> geraVectorTipo(vector<Filme>& filmes, string tipo){
  vector<Filme> vectorTipo;
  string palavra;

  for(int i=0;i<filmes.size();i++){
    stringstream ss(filmes[i].tipoDoFilme); 
    while (getline(ss, palavra)) { //Pega as palavras do tipoDoFilme separados por virgula
      if(palavra == tipo)
      vectorTipo.push_back((filmes[i]));
    }
    
  }

  return vectorTipo;
} 

vector<Filme> geraVectorDuracao(vector<Filme>& filmes, int duracao){
  vector<Filme> vectorDuracao;
  string palavra;

  for(int i=0;i<filmes.size();i++){
    stringstream ss(to_string(filmes[i].duracao)); 
    while (getline(ss, palavra)) {
      if(palavra == to_string(duracao))
      vectorDuracao.push_back((filmes[i]));
    }
    
  }

  return vectorDuracao;
} 

vector<Filme> geraVectorAno(vector<Filme>& filmes, int ano){
  vector<Filme> vectorano;
  string palavra;

  for(int i=0;i<filmes.size();i++){
    stringstream ss(to_string(filmes[i].anoLancamento)); 
    while (getline(ss, palavra)) {
      if(palavra == to_string(ano))
      vectorano.push_back((filmes[i]));
    }
    
  }

  return vectorano;
} 

vector<Cinema> geraVectorPreco(vector<Cinema>& cinemas, int preco){
  vector<Cinema> vectorPreco;
  string palavra;

  for(int i=0;i<cinemas.size();i++){
    stringstream ss(to_string(int((cinemas[i].preco)*100))); 
    while (getline(ss, palavra)) {
      if(palavra == to_string(preco))
      vectorPreco.push_back((cinemas[i]));
    }
    
  }

  return vectorPreco;
} 

void separarTipos(const string& str, set<string>& set_) {
    stringstream ss(str);
    string item;
    while (getline(ss, item, ',')) {
        set_.insert(item);
    }
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

void merge(vector<Filme>& filmes, int esquerda,int meio,int direita){
  int valor1 = meio - esquerda + 1;//tamanho da primeira metade
  int valor2 = direita - meio;//tamanho da segunda metade

  vector<Filme> vectorEsquerda;
  vector<Filme> vectorDireita;

//copia os dados para vetores temporarios

  for(int i = 0;i<valor1;i++){
    vectorEsquerda.push_back(filmes[esquerda + i]);
  }
  for(int j = 0;j<valor2;j++){
    vectorDireita.push_back(filmes[meio + 1 +j]);
  }

//mesclar os vetores n original ordenando
  int i =0;int j= 0;int k = esquerda;
  while(i< valor1 && j < valor2){
    if(vectorEsquerda[i].idFilme < vectorDireita[j].idFilme){
      filmes[k] = vectorEsquerda[i];
      i++;
    }else{
      filmes[k] = vectorDireita[j];
      j++;
    }
    //cout << filmes[k].idFilme << endl;
    k++;
  }

  //copiar os elementos restantes de vectorEsquerda[] caso existam
  while(i< valor1){
    filmes[k] = vectorEsquerda[i];
    i++;
    k++;
  }
  //copiar os elementos restantes de vectorDireita[] caso existam
  while(j< valor2){
    filmes[k] = vectorDireita[j];
    j++;
    k++;
  }
 
}

//Função para printar os vectors de filmes
void printVector(vector<Filme*>& filmes){
  for(Filme* filme : filmes){
    cout << filme->tituloOriginal << endl;
  }
}

// Função para realizar a busca binária no vector de FILMES
int buscaBinariaFilmes(vector<Filme> &filmes, long valor_procurado) {
    int esquerda = 0;
    int direita = filmes.size() - 1;

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;

        // Verifica se o valor_procurado está no meio
        if (filmes[meio].idFilme == valor_procurado) {
            return meio; // Valor encontrado, retorna o índice
        }

        // Se o valor_procurado é maior, ignore a metade esquerda
        if (filmes[meio].idFilme < valor_procurado) {
            esquerda = meio + 1;
        } 
        // Se o valor_procurado é menor, ignore a metade direita
        else {
            direita = meio - 1;
        }
    }
    return -1;
}

int buscaBinariaInt(const vector<int>& valor, long valor_procurado) {
    int esquerda = 0;
    int direita = valor.size() - 1;

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;

        // Verifica se o valor_procurado está no meio
        if (valor[meio] == valor_procurado) {
            return meio; // Valor encontrado, retorna o índice
        }

        // Se o valor_procurado é maior, ignore a metade esquerda
        if (valor[meio] < valor_procurado) {
            esquerda = meio + 1;
        } 
        // Se o valor_procurado é menor, ignore a metade direita
        else {
            direita = meio - 1;
        }
    }
    return -1;
}

void filtrarOU(vector<Filme> &filme1,vector<Filme> &filme2,vector<Filme> &solucao){
  solucao.clear();
  for(auto filme : filme1){
    solucao.push_back(filme);
  }
  for(int j = 0; j<filme2.size();j++){
    int indice = buscaBinariaFilmes(filme1,filme2[j].idFilme);
    if(indice == -1){
        solucao.push_back(filme2[j]);
    }
  }
  merge(solucao,0,filme1.size()-1,solucao.size()-1);
}

void filtrarE(vector<Filme> &filmes1, vector<Filme> &filmes2, vector<Filme>& solucao){
  solucao.clear();
  for(int j = 0; j<filmes2.size();j++){
    if(buscaBinariaFilmes(filmes1,filmes2[j].idFilme) != -1){
      solucao.push_back(filmes2[j]);
    }
  }
  
}

void filtrarDuracao(vector<Filme>& filme,vector<Filme>& solucao,int limiteInf, int limiteSup){
  solucao.clear();
  for(int i = 0; i < filme.size(); i++){
    if(filme[i].duracao>limiteSup)
      return;

    if(filme[i].duracao>=limiteInf && filme[i].duracao<=limiteSup){
      solucao.push_back(filme[i]);
    }
  }
}

void filtrarAno(vector<Filme>& filme,vector<Filme>& solucao,int limiteInf, int limiteSup){
  solucao.clear();
  for(int i = 0; i < filme.size(); i++){
    if(filme[i].anoLancamento>limiteSup)
      return;

    if(filme[i].anoLancamento>=limiteInf && filme[i].anoLancamento<=limiteSup){
      solucao.push_back(filme[i]);
    }
  }
}

void filtrarFilme(vector<Filme> &filmes1, vector<Filme> &filmes2, vector<Filme>& solucao,int operador){
  if(operador==0){ //operador == 0 "OU"
    if(filmes1.size() >= filmes2.size()){ //Pegamos o maior vector pra trabalhar
      filtrarOU(filmes1,filmes2,solucao);
    }else{
      filtrarOU(filmes2,filmes1,solucao);
    }
  }else{            //operador == 1 "E"
    if(filmes1.size() >= filmes2.size()){ //Pegamos o menor vector pra trabalhar em cima
      filtrarE(filmes2,filmes1,solucao);
    }else{
      filtrarE(filmes1,filmes2,solucao);
    }
  }

}
void filtrarPrecoCinema(vector<vector<Cinema>> &cinemas, vector<Cinema> &solucao, int indice){
  solucao.clear();
  
  for(int i = 0; i <= indice; i++){
    for(auto x : cinemas[i]){
      solucao.push_back(x);
    }
  }
}


int main() {
  auto inicioTempo = high_resolution_clock::now(); //Inicio contagem de tempo inicializacao

  vector<Filme> filmes = lerArquivoFilme();
  vector<Cinema> cinemas = lerArquivoCinema(filmes);

  set<string> generos;    //O set permite a unificação dos elementos
  set<string> tiposFilme; 
  set<string> duracaoFilmeSet;
  set<string> anoFilmeSet;
  set<string> precoCinemaSet;

  vector<vector<Filme>> matrizGenero; //Cria uma matriz onde cada linha possui filmes um genero
  vector<vector<Filme>> matrizDuracao;
  vector<vector<Filme>> matrizTipo;
  vector<vector<Filme>> matrizAno;
  vector<vector<Cinema>> matrizPreco;

  //SEPARACAO DE INFORMACOES DAS COLUNAS
  for(Filme filme : filmes){
    separarTipos(filme.genero, generos); //Separa os filmes Generos
  }

  for(Filme filme : filmes){
    separarTipos(filme.tipoDoFilme, tiposFilme); //Separa os filmes por Tipo
  }

  for(Filme filme : filmes){
    separarTipos(to_string(filme.duracao), duracaoFilmeSet); //Separa os filmes por Duracao
  }

  for(Filme filme : filmes){
    separarTipos(to_string(filme.anoLancamento), anoFilmeSet); //Separa os filmes por Ano
  }

  for(Cinema cinema : cinemas){
    separarTipos(to_string(cinema.preco*100), precoCinemaSet); //Separa os cinemas por Preco
  }

  //Criacao do vector de duracao
  vector<int> duracaoFilme;
  for(auto x : duracaoFilmeSet){
    if(stoi(x) != 0)
      duracaoFilme.push_back(stoi(x));
  }
  duracaoFilmeSet.clear(); //Limpamos o set para limpar memoria
  quickSort(duracaoFilme);

  //Criacao do vector de ano
  vector<int> anoFilme;
  for(auto x : anoFilmeSet){
    if(stoi(x) != 0)
      anoFilme.push_back(stoi(x));
  }
  anoFilmeSet.clear(); //Limpamos o set para limpar memoria
  quickSort(anoFilme);

  //Criacao do vector de ano
  vector<int> precoCinema;
  for(auto x : precoCinemaSet){
    if(stoi(x) != 0)
      precoCinema.push_back(stoi(x));
  }
  precoCinemaSet.clear(); //Limpamos o set para limpar memoria
  quickSort(precoCinema);

  //CRIAÇAO DE MATRIZES
  for(string genero : generos){ //Cria matriz de generos
    matrizGenero.push_back(geraVectorGenero(filmes,genero));
  }

  for(string tipo : tiposFilme){ //Cria matriz de tipos de filme
    matrizTipo.push_back(geraVectorTipo(filmes,tipo));
  }

  for(int duracoes : duracaoFilme){ //Cria matriz de duracao
    matrizDuracao.push_back(geraVectorDuracao(filmes,duracoes));
  }

  for(int ano : anoFilme){ //Cria matriz de duracao
    matrizAno.push_back(geraVectorAno(filmes,ano));
  }

  for(int preco : precoCinema){ //Cria matriz de duracao
    matrizPreco.push_back(geraVectorPreco(cinemas,preco));
  }

  auto fimTempo = high_resolution_clock::now(); //Fim da contagem de tempoi inicializacao
  duration<double> duracaoInit = (fimTempo - inicioTempo);

  cout << "Tempo de Inicialização(segundos): " << duracaoInit.count() << endl;
  //FIM DA INICIALIZACAO

  //INICIO FILTRAGEM
  auto inicioTempoFiltro = high_resolution_clock::now(); //Inicio contagem de tempo inicializacao

  vector<Filme> solucaoTipo;
  vector<Filme> solucaoGenero;
  vector<Cinema> solucaoCinema;
  vector<int> opcaoGenero;
  vector<int> opcaoTipo;

  // filtrarFilme(matrizGenero[0],matrizGenero[5],solucaoGenero,0);
  // filtrarAno(solucaoGenero,solucaoTipo,2010,2018);

  filtrarPrecoCinema(matrizPreco,solucaoCinema,buscaBinariaInt(precoCinema,(12.5)*100));

  for(auto x : solucaoCinema){
    cout << x.id << " " << x.nomeDoCinema << endl;
  }


  
  auto fimTempoFiltro = high_resolution_clock::now(); //Fim da contagem de tempoi inicializacao
  duration<double> duracaoFiltro = (fimTempoFiltro - inicioTempoFiltro);

  cout << "Tempo de Filtragem(segundos): " << duracaoFiltro.count() << endl;
  
  return 0;
}