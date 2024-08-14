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
  int idFilme;
  string tipoDoFilme;
  string tituloPrimario;
  string tituloOriginal;
  bool isAdult;
  int anoLancamento;
  int anoTermino;
  int duracao;
  string genero;

  //Contrutor Sobrecarregado
  Filme(int idFilme, const string& tipoDoFilme, const string& tituloPrimario, const string& tituloOriginal,
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

int removeLetraID(string id){
  string idint;
    for (char ch : id) {
        // Verifica se o caractere não é uma letra
        if (!isalpha(ch)) {
            idint += ch;
        }
    }
    return stol(idint);
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
        int idint = (!ids.empty()) ? removeLetraID(ids):0;
        Filme filme(idint, tipo, tituloPrimario, tituloOriginal, isAdult, anoLancamento, anoTermino, duracao, genero);
              
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
      vector<int> idFilmes;

      //>> ws serve para ignorar qualquer espaço em branco que venha antes
      getline(ss, ids, ',');
      getline(ss >> ws, nomeDoCinema, ',');
      getline(ss >> ws, cordXStr, ',');
      getline(ss >> ws, cordYStr, ',');
      getline(ss >> ws, precoStr, ',');
      while(getline(ss >> ws, idFilme, ',')){
        int id = removeLetraID(idFilme);
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
int buscaBinariaFilmes(vector<Filme> &filmes, int valor_procurado) {
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

int buscaBinariaInt(const vector<int>& valor, int valor_procurado) {
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

void filtrarOU(vector<Filme> filme1,vector<Filme> &filme2,vector<Filme> &solucao){
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

void filtrarE(vector<Filme> filmes1, vector<Filme> &filmes2, vector<Filme>& solucao){
  solucao.clear();
  for(int j = 0; j<filmes2.size();j++){
    if(buscaBinariaFilmes(filmes1,filmes2[j].idFilme) != -1){
      solucao.push_back(filmes2[j]);
    }
  }
}

void filtrarE(vector<Cinema> cinemas, vector<Filme> &filmes2, vector<Cinema>& solucao){
  solucao.clear();
  for(auto cinema : cinemas){
    for(int j = 0; j<filmes2.size();j++){
      if(buscaBinariaFilmes(cinema.filmes,filmes2[j].idFilme) != -1){
        solucao.push_back(cinema);
      }
    }
  }
}

void filtrarDuracao(vector<Filme> filme,vector<Filme>& solucao,int limiteInf, int limiteSup){
  if(filme[0].duracao>=limiteInf && filme[0].duracao<=limiteSup){
    for(int i = 0; i < filme.size(); i++){
      solucao.push_back(filme[i]);
    }
  }
}



void filtrarAno(vector<Filme>& filme,vector<Filme>& solucao,int limiteInf, int limiteSup){
  for(int i = 0; i < filme.size(); i++){
    if(filme[i].anoLancamento>limiteSup)
      return;

    if(filme[i].anoLancamento>=limiteInf && filme[i].anoLancamento<=limiteSup){
      solucao.push_back(filme[i]);
    }
  }
}


void listageneros(){
    cout << "(0)Action (1)Adult (2)Adventure (3)Animation" << endl;
    cout << "(4)Biography (5)Comedy (6)Crime (7)Documentary (8)Drama" << endl;
    cout << "(9)Family (10)Fantasy (11)Game-Show (12)History (13)Horror" << endl;
    cout << "(14)Music (15)Musical (16)Mystery (17)NULL (18)News" <<endl;
    cout << "(19)Reality-TV (20)Romance (21)Sci-Fi (22)Short (23)Sport" << endl;
    cout << "(24)Talk-Show (25)Thriller (26)War (27)Western" << endl;
}

void listaTipo(){
  cout << "(0)movie (1)short (2)tvEpisode (3)tvMiniSeries (4)tvMovie" << endl;
  cout << "(5)tvSeries (6)tvShort (7)tvSpecial (8)video (9)videoGame" << endl;
}

void filtrarPorGenero(vector<Filme>&solucao,vector<vector<Filme>>& matrizGenero,int juntar) {

  int genero, outroGenero, tipoFiltro,segundoGenero; 
  listageneros();
  cin >> genero;
  cout << "Deseja filtrar por outro gênero? (1-Sim, 0-Não): ";
  cin >> outroGenero;
  if(outroGenero==1){      
    cout << "Deseja aplicar filtro AND ou OR? (0-AND, 1-OR): ";
    cin >> tipoFiltro;
    listageneros();
    cin >> segundoGenero;

    auto inicioTempoFiltro = high_resolution_clock::now(); //Inicio contagem de tempo inicializacao
    if(tipoFiltro==1){
      filtrarOU(matrizGenero[genero],matrizGenero[segundoGenero],solucao);
    }else{
      filtrarE(matrizGenero[genero],matrizGenero[segundoGenero],solucao);
    }
    auto fimTempoFiltro = high_resolution_clock::now(); //Fim da contagem de tempoi inicializacao
    duration<double> duracaoFiltro = (fimTempoFiltro - inicioTempoFiltro);
    cout << ">>>>>>>Tempo de Filtragem(segundos): " << duracaoFiltro.count() << endl;

    cout << "Deseja filtrar por outro gênero? (1-Sim, 0-Não): ";
    cin >> outroGenero;
    while (outroGenero ==1){
      cout << "Deseja aplicar filtro AND ou OR? (0-AND, 1-OR): ";
      cin >> tipoFiltro;
      listageneros();
      cin >> segundoGenero;

      auto inicioTempoFiltro = high_resolution_clock::now(); //Inicio contagem de tempo inicializacao
      if(tipoFiltro==1){
        filtrarOU(solucao,matrizGenero[segundoGenero],solucao);
      }else{
        filtrarE(solucao,matrizGenero[segundoGenero],solucao);
      }
      auto fimTempoFiltro = high_resolution_clock::now(); //Fim da contagem de tempoi inicializacao
      duration<double> duracaoFiltro = (fimTempoFiltro - inicioTempoFiltro);
      cout << ">>>>>>>Tempo de Filtragem(segundos): " << duracaoFiltro.count() << endl;

      cout << "Deseja filtrar por outro gênero? (1-Sim, 0-Não): ";
      cin >> outroGenero; 
    }
  }else{
    auto inicioTempoFiltro = high_resolution_clock::now(); //Inicio contagem de tempo inicializacao
    if(juntar == 1)
      filtrarOU(solucao,matrizGenero[genero],solucao);
    else
      filtrarE(solucao,matrizGenero[genero],solucao);
    auto fimTempoFiltro = high_resolution_clock::now(); //Fim da contagem de tempoi inicializacao
    duration<double> duracaoFiltro = (fimTempoFiltro - inicioTempoFiltro);
    cout << ">>>>>>>Tempo de Filtragem(segundos): " << duracaoFiltro.count() << endl;
  }  
}

void filtrarPorTipo(vector<Filme>&solucao,vector<vector<Filme>>& matrizTipo,int juntar) {
  
  int tipo, outroTipo, tipoFiltro,segundoTipo; 
  listaTipo();
  cin >> tipo;
  cout << "Deseja filtrar por outro tipo? (1-Sim, 0-Não): ";
  cin >> outroTipo;
  if(outroTipo==1){      
    cout << "Deseja aplicar filtro AND ou OR? (0-AND, 1-OR): ";
    cin >> tipoFiltro;
    listaTipo();
    cin >> segundoTipo;
    auto inicioTempoFiltro = high_resolution_clock::now(); //Inicio contagem de tempo inicializacao
    if(tipoFiltro==1){
      filtrarOU(matrizTipo[tipo],matrizTipo[segundoTipo],solucao);
    }else{
      filtrarE(matrizTipo[tipo],matrizTipo[segundoTipo],solucao);
    }
    auto fimTempoFiltro = high_resolution_clock::now(); //Fim da contagem de tempoi inicializacao
    duration<double> duracaoFiltro = (fimTempoFiltro - inicioTempoFiltro);
    cout << ">>>>>>>Tempo de Filtragem(segundos): " << duracaoFiltro.count() << endl;

    cout << "Deseja filtrar por outro gênero? (1-Sim, 0-Não): ";
    cin >> outroTipo;
    while (outroTipo ==1){
      cout << "Deseja aplicar filtro AND ou OR? (0-AND, 1-OR): ";
      cin >> tipoFiltro;
      listaTipo();
      cin >> segundoTipo;

      auto inicioTempoFiltro = high_resolution_clock::now(); //Inicio contagem de tempo inicializacao
      if(tipoFiltro==1){
        filtrarOU(solucao,matrizTipo[segundoTipo],solucao);
      }else{
        filtrarE(solucao,matrizTipo[segundoTipo],solucao);
      }
      auto fimTempoFiltro = high_resolution_clock::now(); //Fim da contagem de tempoi inicializacao
      duration<double> duracaoFiltro = (fimTempoFiltro - inicioTempoFiltro);
      cout << ">>>>>>>Tempo de Filtragem(segundos): " << duracaoFiltro.count() << endl;

      cout << "Deseja filtrar por outro gênero? (1-Sim, 0-Não): ";
      cin >> outroTipo; 
    }
  }else{
    auto inicioTempoFiltro = high_resolution_clock::now(); //Inicio contagem de tempo inicializacao
    if(juntar == 1)
      filtrarOU(solucao,matrizTipo[tipo],solucao);
    else
      filtrarE(solucao,matrizTipo[tipo],solucao);
    
  }
  
}

void filtrarPorDuracao(vector<Filme>&solucao, vector<vector<Filme>>&filmes,int juntar){
  
  int limiteInferior,limiteSuperior;
  vector<Filme> temp; //vector temporario
  cout << "Digite o intervalo de tempo do filme(em minutos):\n" << endl;
  cin >> limiteInferior;
  cin >> limiteSuperior;
  
  if(juntar == 1){
    auto inicioTempoFiltro = high_resolution_clock::now(); //Inicio contagem de tempo inicializacao
    for(auto filme : filmes)
      filtrarDuracao(filme,temp,limiteInferior,limiteSuperior);
    filtrarOU(solucao,temp,solucao);
    temp.clear();
    auto fimTempoFiltro = high_resolution_clock::now(); //Fim da contagem de tempoi inicializacao
    duration<double> duracaoFiltro = (fimTempoFiltro - inicioTempoFiltro);
    cout << ">>>>>>>Tempo de Filtragem(segundos): " << duracaoFiltro.count() << endl;
  }else{
    auto inicioTempoFiltro = high_resolution_clock::now(); //Inicio contagem de tempo inicializacao
    for(auto filme : filmes)
      filtrarDuracao(filme,temp,limiteInferior,limiteSuperior);
    filtrarE(solucao,temp,solucao);
    auto fimTempoFiltro = high_resolution_clock::now(); //Fim da contagem de tempoi inicializacao
    duration<double> duracaoFiltro = (fimTempoFiltro - inicioTempoFiltro);
    cout << ">>>>>>>Tempo de Filtragem(segundos): " << duracaoFiltro.count() << endl;
  }
}


void filtrarPorAno(vector<Filme>&solucao, vector<vector<Filme>>&filmes,int juntar){
  auto inicioTempoFiltro = high_resolution_clock::now(); //Inicio contagem de tempo inicializacao
  int limiteInferior,limiteSuperior, escolha;
  vector<Filme> temp;
  cout << "(0)Buscar Ano Especifico\n(1)Buscar Intervalo" << endl;
  cin >> escolha;
  if(escolha == 1){
    cout << "Digite o intervalo de ano:" << endl;
    cin >> limiteInferior;
    cin >> limiteSuperior;
    
    if(juntar == 1){
      auto inicioTempoFiltro = high_resolution_clock::now(); //Inicio contagem de tempo inicializacao
      for(auto filme : filmes)
        filtrarAno(filme,temp,limiteInferior,limiteSuperior);
      filtrarOU(solucao,temp,solucao);
      temp.clear();
      auto fimTempoFiltro = high_resolution_clock::now(); //Fim da contagem de tempoi inicializacao
      duration<double> duracaoFiltro = (fimTempoFiltro - inicioTempoFiltro);
      cout << ">>>>>>>Tempo de Filtragem(segundos): " << duracaoFiltro.count() << endl;
    }else{ 
      for(auto filme : filmes)
        filtrarAno(filme,temp,limiteInferior,limiteSuperior);
      filtrarE(solucao,temp,solucao);
      temp.clear();
      auto inicioTempoFiltro = high_resolution_clock::now(); //Inicio contagem de tempo inicializacao
      auto fimTempoFiltro = high_resolution_clock::now(); //Fim da contagem de tempoi inicializacao
      duration<double> duracaoFiltro = (fimTempoFiltro - inicioTempoFiltro);
      cout << ">>>>>>>Tempo de Filtragem(segundos): " << duracaoFiltro.count() << endl;
    }
  }else{
    cout << "Digite o ano:" << endl;
    cin >> limiteInferior;
    if(juntar == 1){
      auto inicioTempoFiltro = high_resolution_clock::now(); //Inicio contagem de tempo inicializacao
      for(auto filme : filmes)
        filtrarAno(filme,temp,limiteInferior,limiteInferior);
      filtrarOU(solucao,temp,solucao);
      temp.clear();
      auto fimTempoFiltro = high_resolution_clock::now(); //Fim da contagem de tempoi inicializacao
      duration<double> duracaoFiltro = (fimTempoFiltro - inicioTempoFiltro);
      cout << ">>>>>>>Tempo de Filtragem(segundos): " << duracaoFiltro.count() << endl;
    }else{
      auto inicioTempoFiltro = high_resolution_clock::now(); //Inicio contagem de tempo inicializacao
      for(auto filme : filmes)
        filtrarAno(filme,temp,limiteInferior,limiteInferior);
      filtrarE(solucao,temp,solucao);
      temp.clear();
      auto fimTempoFiltro = high_resolution_clock::now(); //Fim da contagem de tempoi inicializacao
      duration<double> duracaoFiltro = (fimTempoFiltro - inicioTempoFiltro);
      cout << ">>>>>>>Tempo de Filtragem(segundos): " << duracaoFiltro.count() << endl;
    }
  }
}

// void filtrarFilme(vector<Filme> &filmes1, vector<Filme> &filmes2, vector<Filme>& solucao,int operador){
//   if(operador==0){ //operador == 0 "OU"
//     if(filmes1.size() >= filmes2.size()){ //Pegamos o maior vector pra trabalhar
//       filtrarOU(filmes1,filmes2,solucao);
//     }else{
//       filtrarOU(filmes2,filmes1,solucao);
//     }
//   }else{            //operador == 1 "E"
//     if(filmes1.size() >= filmes2.size()){ //Pegamos o menor vector pra trabalhar em cima
//       filtrarE(filmes2,filmes1,solucao);
//     }else{
//       filtrarE(filmes1,filmes2,solucao);
//     }
//   }

// }



//FILTRAGEM DE CINEMA
void filtrarPrecoCinema(vector<vector<Cinema>> &cinemas, vector<Cinema> &solucao, vector<int> precoCinema){ 
  int valor;
  cout<<"Preco maximo: ";
  cin>> valor;
  int indice = buscaBinariaInt(precoCinema,(valor)*100);
  if(indice == -1){
    for(int i = precoCinema.size()-1;i>=0;i--){
      if((valor*100)-precoCinema[i]<0){
        indice = i;
        break;
      }
    }
  }
  if(indice == -1 && (valor)*100-precoCinema[51] > 0){
   indice = 51; 
  }
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
  vector<Filme> solucao;
  vector<Cinema> solucaoCinema;
  vector<int> opcaoGenero;
  vector<int> opcaoTipo;

  // filtrarFilme(matrizGenero[0],matrizGenero[5],solucaoGenero,0);
  //filtrarAno(filmes,solucaoTipo,2010,2018);
  //filtrarPrecoCinema(matrizPreco,solucaoCinema,buscaBinariaInt(precoCinema,(12.5)*100));

  int escolha1;
  int escolhaFiltro,escolhaGenero,outroFIltro;
  int juntaFiltros=1;
 

  cout << "(1)Filmes" << endl << "(2)Cinemas" << endl << "(0)Sair" << endl;
  cin >> escolha1;
  do{
    if(escolha1 == 1){
      cout << "(1)Genero\n(2)Tipo\n(3)Duracao\n(4)Ano" << endl;
      cin >> escolhaFiltro;

      switch (escolhaFiltro){
        case 1:
          filtrarPorGenero(solucao,matrizGenero,juntaFiltros);
          break;
        case 2:
          filtrarPorTipo(solucao,matrizTipo,juntaFiltros);
          break;
        case 3:
          filtrarPorDuracao(solucao, matrizDuracao,juntaFiltros);
          break;
        case 4:
          filtrarPorAno(solucao, matrizAno,juntaFiltros);
          break;
        default:
          break;
      }
      cout << "Outro FIltro? (1)Sim (0)Não" <<endl;
      cin >> escolha1;
      if(escolha1 == 1){
        cout << "filtrar usando (1)OR (2)AND: ";
        cin >> juntaFiltros;
      }
    }
    if(escolha1 == 2){
      cout << "(1)Genero\n(2)Tipo\n(3)Duracao\n(4)Ano\n(5)Preco" << endl;
      cin >> escolhaFiltro;

      switch (escolhaFiltro){
        case 1:
          filtrarPorGenero(solucao,matrizGenero,juntaFiltros);
          break;
        case 2:
          filtrarPorTipo(solucao,matrizTipo,juntaFiltros);
          break;
        case 3:
          filtrarPorDuracao(solucao, matrizDuracao,juntaFiltros);
          break;
        case 4:
          filtrarPorAno(solucao, matrizAno,juntaFiltros);
          break;
        case 5:
          filtrarPrecoCinema(matrizPreco,solucaoCinema,precoCinema);
          break;
        default:
          break;
      }
      cout << "Outro FIltro? (1)Sim (0)Não" <<endl;
      cin >> escolha1;
      if(escolha1 == 1){
        cout << "filtrar usando (1)OR (2)AND: ";
        cin >> juntaFiltros;
      }
    }

  }while(escolha1 != 0);

  //filtrarE(cinemas,matrizGenero[0],solucaoCinema);

  for(auto x : solucaoCinema){
    cout << x.nomeDoCinema << "("<<x.preco<<")"<< endl;
  }

  for(auto x : solucao){
    cout << "("<<x.genero<<")"<< "("<<x.tipoDoFilme<<")"<< "("<<x.duracao<<")"<<x.idFilme <<endl; 
  }
  
  return 0;
}