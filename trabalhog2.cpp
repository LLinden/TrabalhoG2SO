/******************************************************************************
Trabalho G2 - Sistemas Operacionais
Author: Lucas Linden - Cod. 1110139
 ./main first-fit 5 0
*******************************************************************************/
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h> 
#include <stdlib.h> 
#include <unistd.h>

using namespace std;
// constantes
const int TAMANHO = 5000;
// variaveis globais
int nProc, tempo = 1, contadorCiclos = 0, memoriaOcupada = 0, ultimoIndiceEscrito = 0;
char vetor[TAMANHO];
char dicionario[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M',
					'N','O','P','Q','R','S','T','U','V','X','Y','W','Z'};
	
// modelo do objeto Processo
class Processo {
	public:
		int pid;   
		int tamanho; 
		int tempoExec;
		char simbolo;
		int tempoRest;
		int alocado;
		int tempoEmFila;
};

Processo *listaProcessos;

// modelo do objeto Bloco
class  Bloco {
	public:
		int id;
		int inicio;
		int fim;
		int tamanho;
		int usado;
};

// opcoes de comando
enum Opcoes {
	Op_Invalida,
	Op1,
	Op2,
	Op3,
	Op4,
};

Opcoes opcao;
	
// traduz valor do comando para uso dentro do switch
Opcoes resolveOp(string comando) {
	if(comando.find("first") != string::npos) return Op1;
	if(comando.find("circular") != string::npos) return Op2;
	if(comando.find("best") != string::npos) return Op3;
	if(comando.find("worst") != string::npos) return Op4;
	return Op_Invalida;
}

// legando dos processos
void imprimeProcesso(int indice){
  
  cout << "PID: " << listaProcessos[indice].pid << ", ";
  cout << "Simbolo: " << listaProcessos[indice].simbolo << ", ";
  cout << "Tamanho: " << listaProcessos[indice].tamanho << ", ";
  cout << "T. Execucao: " << listaProcessos[indice].tempoExec << ", ";
  cout << "T. Restante: " << listaProcessos[indice].tempoRest << ", ";
  cout << "Alocado: " << listaProcessos[indice].alocado << "\n";
};


// cria vetor principal
void criaVetor(){
	int i;
	
	for(i=0; i < TAMANHO; i++) {     
		vetor[i] = '.';
	}						
};

// exibe vetor na tela em forma de matriz
void exibeVetor(){
	int i;
	
	//system("clear");
		
	for(i=0; i < TAMANHO; i++) {  
		if( i % 100 == 0 && i != 0 ) printf("\n");   
			cout << vetor[i];
	}
		cout << endl;	
};

// tabela legenda com a descricao dos processos criados
void legenda() {

  cout << "\n\n\n\n";

	for(int i = 0; i < nProc; i++) {
    imprimeProcesso(i);
	}
};

// constroi processos
Processo* construtorProc() {	
	Processo *processo = new Processo[nProc];
	// instancia n processos (quantidade definida em nProc)
	for(int i = 0; i < nProc; i++) {	
		 processo[i].pid = i;
		 processo[i].tamanho = rand() % 900 + 100;
		 processo[i].tempoExec = rand() % 300 + 1;
		 processo[i].simbolo = dicionario[i];
		 processo[i].tempoRest = processo[i].tempoExec;
     processo[i].alocado = 0;
     processo[i].tempoEmFila = 0;
	}
	return processo;	
};

// constroi blocos
Bloco* construtorBloco() {
	int idBlocos = 0;
	
	// vetor de blocos possui a quantidade max. de blocos que pode haver
	Bloco *blocos = new Bloco[2500];
	
	for (int i = 0; i < TAMANHO; i++) {
		if (vetor[i] == '.') {
			idBlocos = idBlocos + 1;
			Bloco bloco;
			bloco.inicio = i;
			bloco.id = idBlocos;		
	
			for (int j = i; j <= TAMANHO; j++) {
				if(vetor[j] != '.' || j == TAMANHO) {
					// terminou a leitura e identifica ultima posicao
					bloco.fim = j - 1;
					// vai fazer o laço pular para o ponto de onde parou a leitura do bloco
					i = j; 

				break;
				}
			}
			
			bloco.tamanho = bloco.fim - bloco.inicio;
			blocos[idBlocos] = bloco;
		}

	}
	return blocos;
};

// escreve processos dentro do bloco e no vetor
void escreveProcesso(int indexProcesso, Bloco bloco){
	for (int i = bloco.inicio; i < bloco.inicio + listaProcessos[indexProcesso].tamanho; i++){
		vetor[i] = listaProcessos[indexProcesso].simbolo;	
	}

  listaProcessos[indexProcesso].alocado = 1;

};

void firstFit(int indexProcesso) {
  int encontrouBlocoParaAlocar = 0;
  Bloco blocoQueCabeOProcesso;
  
  Bloco *blocos = construtorBloco();
  
  for (int numeroBloco = 0; numeroBloco < 2500; numeroBloco++){
    Bloco blocoAtual = blocos[numeroBloco];
    if (blocoAtual.tamanho >= listaProcessos[indexProcesso].tamanho){
      blocoQueCabeOProcesso = blocoAtual;
      encontrouBlocoParaAlocar = 1;
      break;
    }
  }
  
  if (encontrouBlocoParaAlocar == 1){
    escreveProcesso(indexProcesso, blocoQueCabeOProcesso);
  }
};	
			
// best-fit
void bestFit(int indexProcesso) {
  int encontrouBlocoParaAlocar = 0;
  Bloco blocoMenorTamanho;
  
  Bloco *blocos = construtorBloco();
  
  int menorTamanhoEncontrado = TAMANHO;

  for (int numeroBloco = 0; numeroBloco < 2500; numeroBloco++){
    Bloco blocoAtual = blocos[numeroBloco];
    if (blocoAtual.tamanho >= listaProcessos[indexProcesso].tamanho && blocoAtual.tamanho < menorTamanhoEncontrado ){
      blocoMenorTamanho = blocoAtual;
      encontrouBlocoParaAlocar = 1;
      break;
    }
  }
  
  if (encontrouBlocoParaAlocar == 1){
    escreveProcesso(indexProcesso, blocoMenorTamanho);
  }
};

// worst-fit
void worstFit(int indexProcesso) {
  int encontrouBlocoParaAlocar = 0;
  Bloco blocoMaiorTamanho;
  
  Bloco *blocos = construtorBloco();
  
  int maiorTamanhoEncontrado = 0;

  for (int numeroBloco = 0; numeroBloco < 2500; numeroBloco++){
    Bloco blocoAtual = blocos[numeroBloco];
    if (blocoAtual.tamanho >= listaProcessos[indexProcesso].tamanho && blocoAtual.tamanho > maiorTamanhoEncontrado ){
      blocoMaiorTamanho = blocoAtual;
      encontrouBlocoParaAlocar = 1;
      break;
    }
  }
  
  if (encontrouBlocoParaAlocar == 1){
    escreveProcesso(indexProcesso, blocoMaiorTamanho);
  }
};

// circular-fit
void circularFit(int indexProcesso) {
  int encontrouBlocoParaAlocar = 0;
  Bloco blocoTamanhoSuficiente;
  
  Bloco *blocos = construtorBloco();
  
  for (int numeroBloco = 0; numeroBloco < 2500; numeroBloco++){
    Bloco blocoAtual = blocos[numeroBloco];
    if (blocoAtual.tamanho >= listaProcessos[indexProcesso].tamanho && blocoAtual.inicio >= ultimoIndiceEscrito ){
      blocoTamanhoSuficiente = blocoAtual;
      encontrouBlocoParaAlocar = 1;
      break;
    }
  }

  if (encontrouBlocoParaAlocar == 0){
    for (int numeroBloco = 0; numeroBloco < 2500; numeroBloco++){
      Bloco blocoAtual = blocos[numeroBloco];
      if (blocoAtual.tamanho >= listaProcessos[indexProcesso].tamanho){
        blocoTamanhoSuficiente = blocoAtual;
        encontrouBlocoParaAlocar = 1;
        break;
      }
    }
  }
  
  if (encontrouBlocoParaAlocar == 1){
    ultimoIndiceEscrito = blocoTamanhoSuficiente.inicio + listaProcessos[indexProcesso].tamanho;
    escreveProcesso(indexProcesso, blocoTamanhoSuficiente);
  }
};

// aloca processos
void alocarProcesso(){

  int indiceProcessoSelecionado = -1;

  //seleciona o próximo processo para alocar
	for (int indice = 0; indice < nProc ; indice++){ 
    if (listaProcessos[indice].alocado == 0 && listaProcessos[indice].tempoRest != 0){
      indiceProcessoSelecionado = indice;
      break;
    }
  }

  if (indiceProcessoSelecionado > -1){
    // seleciona algoritmo a ser executado
    switch (opcao) {			
      case Op1: // first-fit
        firstFit(indiceProcessoSelecionado);
        break;
      case Op2: // circular-fit 
        circularFit(indiceProcessoSelecionado);
        break;
      case Op3: // best-fit
        bestFit(indiceProcessoSelecionado);
        break;
      case Op4: // worst-fit 
        worstFit(indiceProcessoSelecionado);
        break;
      default:
        cout << "Comando nao encontrado!" "!\n";
        break;
      }						
  }
};

// chance de gerar processo
int deveAlocarProcesso(){
  int numeroGerado = rand() % 100 + 1;

  if (numeroGerado <= 20){
    return 1;
  } else {
    return 0;
  }
};

// cronometro
void descontarTempoDosProcessoEmAndamento(){
  for (int i = 0; i < nProc; i++){
    if (listaProcessos[i].alocado == 1){
      listaProcessos[i].tempoRest = listaProcessos[i].tempoRest - 1;
    }
  }
};

// remove os processos concluidos
void removerProcessosFinalizadosDaMatriz(){
  for (int i = 0; i < nProc; i++){
    if (listaProcessos[i].alocado == 1 && listaProcessos[i].tempoRest == 0){ //deve remover      
      for (int j = 0; j < TAMANHO; j++) { //percorre matriz
        if (vetor[j] == listaProcessos[i].simbolo) {
          vetor[j] = '.'; //remove
        }
      }
      listaProcessos[i].alocado = 0;
    }
  }

}

// atuaaliza informacoes
void atualizarInformacoesParaRelatorio(){
	for (int i = 0; i < TAMANHO; i++) {
		if (vetor[i] != '.') {
      memoriaOcupada++;
		}
	}

  for (int i = 0; i < nProc; i++){
    if (listaProcessos[i].alocado == 0 && listaProcessos[i].tempoRest != 0){
      listaProcessos[i].tempoEmFila++;
    }
  }
}

// atualiza processo
void atualizaProcessos(){
  contadorCiclos++;

  descontarTempoDosProcessoEmAndamento();
  removerProcessosFinalizadosDaMatriz();
  
  if (deveAlocarProcesso() == 1){
    alocarProcesso();
  }

  atualizarInformacoesParaRelatorio();
};

// exibe processos
void imprimeCiclo(){
  // chama legenda para os n processos criados
  legenda();
  exibeVetor();
}

// verifica  processos pendentes
int existemProcessosPendentes(){
  for (int i = 0; i < nProc; i++){
    if (listaProcessos[i].alocado == 1 || listaProcessos[i].tempoRest != 0){
      return 1;
    }
  }
  return 0;
}

// tempo de espera definido pelo usuario
void espera(){
  if (tempo > 0){
    sleep(tempo);
  }
}

// impressao relatorio
void imprimeRelatorioCompleto(){ 
  cout << "\n\nRelatorio completo:\n";

  int mediaOcupada = memoriaOcupada * 100 /  (TAMANHO * contadorCiclos);
  int mediaLivre = 100 - mediaOcupada;

  int totalCiclosAguardando = 0;
  
  for (int i = 0; i < nProc; i++){
    totalCiclosAguardando += listaProcessos[i].tempoEmFila;
  }

  int mediaCiclosAguardados = totalCiclosAguardando / contadorCiclos;

  cout << "media de memoria ocupada por ciclo: " << mediaOcupada << "%;\n";
  cout << "media de memoria livre por ciclo: " << mediaLivre << "%;\n";
  cout << "processos criados: " << nProc << ";\n";
  cout << "media de ciclos aguardados ate alocar: " << mediaCiclosAguardados << ".\n";

}


// inicio
int main(int argc, char *argv[ ]) {
	// reseta o seed de tempo
	srand(time(0));
	
	// trata argumentos passados via linha de comando
	string comando = argv[1];
	opcao = resolveOp(comando);	
	nProc = atoi(argv[2]);
	// arumento tempo opcional
	if (argv[3] != NULL) tempo = atoi(argv[3]);	
		
	// inicializa os processos
	listaProcessos = construtorProc();
	
	// inicializa vetor
	criaVetor();		

  while(existemProcessosPendentes() == 1){
    atualizaProcessos();
    imprimeCiclo();
    espera();
  }

  imprimeRelatorioCompleto();

  return 0;

};
