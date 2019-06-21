/******************************************************************************
Trabalho G2 - Sistemas Operacionais
Author: Lucas Linden - Cod. 1110139
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
int nProc, tempo = 0, count = 0, cron = 1;
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
    int alocado = 0; //0 = false, 1 = true
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

// cabecalho tabela legenda
void tituloLegenda() {
	cout << "\nPID\t\tSIMBOLO\t\tTAMANHO\t\tTEMP.EXEC.\tTEMP.REST.\n";
};

// tabela legenda com a descricao dos processos criados
void legenda(int nProc, Processo *processo) {
	for(int i = 0; i < nProc; i++) {
		cout << processo[i].pid << "\t\t" << processo[i].simbolo << "\t\t" << processo[i].tamanho 
		<< "\t\t" << processo[i].tempoExec << "\t\t" << processo[i].tempoRest << "\n";
	}
};

// constroi processos
Processo* construtorProc() {	
	Processo *processo = new Processo[nProc];
	// instancia n processos (quantidade definida em nProc)
	for(int i = 0; i < nProc; i++) {	
		 count = count + 1;
		 processo[i].pid = count;
		 processo[i].tamanho = rand() % 900 + 100;
		 processo[i].tempoExec = rand() % 300 + 1;
		 processo[i].simbolo = dicionario[i];
		 processo[i].tempoRest = processo[i].tempoExec;
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
void escreveProcesso(Processo processo, Bloco bloco){

	printf("escrevendo processo no bloco\n");
  cout << "dados bloco: \n";
	cout << "blocoInicio: " << bloco.inicio << "\n";
  cout << "blocofim: " << bloco.fim << "\n";
  cout << "blocotamanho:" << bloco.tamanho << "\n";  

  cout << "\n\n\ndados processo: \n";
	cout << "processotamanho: " << processo.tamanho << "\n";
	cout << "processosimbolo: " << processo.simbolo << "\n";

	
	for (int i = bloco.inicio; i < bloco.inicio + processo.tamanho; i++){
		vetor[i] = processo.simbolo;	
	}
  processo.alocado = 1;
};

// first-fit
void firstFit(Processo processo) {
  int encontrouBlocoParaAlocar = 0;
  Bloco blocoQueCabeOProcesso;
  
  Bloco *blocos = construtorBloco();
  
  for (int numeroBloco = 0; numeroBloco < 2500; numeroBloco++){
    Bloco blocoAtual = blocos[numeroBloco];
    if (blocoAtual.tamanho >= processo.tamanho){
      blocoQueCabeOProcesso = blocoAtual;
      encontrouBlocoParaAlocar = 1;
      break;
    }
  }
  
  if (encontrouBlocoParaAlocar == 1){
    escreveProcesso(processo, blocoQueCabeOProcesso);
  }
};	
			
	//system("clear");
	//exibeVetor();
	
	// // calcula tempo total
	// for (int i = 0; i < nProc ; i++){
	// 	if (processo[i].tempoExec > cron) {
	// 		cron = processo[i].tempoExec;
	// 	}
	// }			
			
	// // gerencia de tempo de execucao
	// while (cron > 0) {
	// 	for (int i = 0; i < nProc; i++) {
	// 		if (processo[i].tempoRest > 0) {
	// 			int tmp = processo[i].tempoRest - 1;
	// 				processo[i].tempoRest = tmp;
	// 		}						
	// 	}
	// 	// chama legenda para os n processos criados
	// 	exibeVetor();
	// 	tituloLegenda();
	// 	legenda(nProc, processo);
	// 	cron = cron - 1;
	// 	cout << "\nTEMPO TOTAL RESANTE: " << cron << "\n";
	// 	cout << flush;
	// 	sleep(1);
	// }


// best-fit
void bestFit(Processo processo) {

  //     int alocou = 0;

  //     Bloco blocoQueCabeOProcesso;
      
  //     Bloco *blocos = construtorBloco();

  //     // lista de blocos ???
  //     list<Bloco> listaDeBlocos;
      
  //     for (int numeroBloco = 0; numeroBloco < 2500; numeroBloco++){
  //       Bloco blocoAtual = blocos[numeroBloco];
  //       if (blocoAtual.tamanho >= processoAtual.tamanho){
  //         // lista guarda bloco TODO
  //         // Joe Macaroni
  //         listaDeBlocos.push_back(blocoAtual);
  //       }
  //     }
      
  //     listaDeBlocos.sort(tamanho) // NAO FUNCIONA HAHA!

  //     for (int numeroBloco = 0; numeroBloco < 2500; numeroBloco++){
  //       // chama e percorre a lista de blocos
  //       if (blocolistaDeBlocos[numeroBloco].tamanho >= processoAtual.tamanho){
  //         blocoQueCabeOProcesso = blocoAtual;
  //         alocou = 1;
  //         break;
  //       }
  //     }
      
  //     if (alocou == 1){
  //       escreveProcesso(processoAtual, blocoQueCabeOProcesso);
  //     }
	// }	
};

void alocarProcesso(){

  Processo proximoProcessoParaAlocar;
  int processoSelecionado = 0;

  //seleciona o próximo processo para alocar
	for (int indice = 0; indice < nProc ; indice++){ 
    if (listaProcessos[indice].alocado == 0 && listaProcessos[indice].tempoRest != 0){
      proximoProcessoParaAlocar = listaProcessos[indice];
      processoSelecionado = 1;
      break;
    }
  }

  if (processoSelecionado == 1){
    // seleciona algoritmo a ser executado
    switch (opcao) {			
      case Op1: // first-fit
        firstFit(proximoProcessoParaAlocar);
        break;
      case Op2: // circular-fit TODO
        break;
      case Op3: // best-fit
        bestFit(proximoProcessoParaAlocar);
        break;
      case Op4: // worst-fit TODO
        break;
      default:
        cout << "Comando nao encontrado!" "!\n";
        break;
      }						
  }
};

int deveAlocarProcesso(){
  int numeroGerado = rand() % 100 + 1;

  if (numeroGerado <= 20){
    return 1;
  } else {
    return 0;
  }
};

void descontarTempoDosProcessoEmAndamento(){
  for (int i = 0; i > nProc; i++){
    if (listaProcessos[i].alocado == 1){
      listaProcessos[i].tempoRest = listaProcessos[i].tempoRest - 1;
    }
  }
};

void removerProcessosFinalizadosDaMatriz(){
  for (int i = 0; i > nProc; i++){
    if (listaProcessos[i].alocado == 1 && listaProcessos[i].tempoRest == 0){ //deve remover      
      for (int j = 0; j < TAMANHO; j++) { //percorre matriz
        if (vetor[j] == listaProcessos[i].simbolo) {
          vetor[j] = '.'; //remove
        }
      }
    }
  }

}

void atualizarInformacoesParaRelatorio(){
  //TODO
}

void atualizaProcessos(){

  descontarTempoDosProcessoEmAndamento();
  removerProcessosFinalizadosDaMatriz();
  
  if (deveAlocarProcesso() == 1){
    alocarProcesso();
  }

  atualizarInformacoesParaRelatorio(); //TODO
};

void imprimeCiclo(){
  // chama legenda para os n processos criados
  tituloLegenda();
  legenda(nProc, processo);
  exibeVetor();
}

int existemProcessosPendentes(){
  for (int i = 0; i > nProc; i++){
    if (listaProcessos[i].alocado == 1 || listaProcessos[i].tempoRest != 0){
      return 1;
    }
  }
  return 0;
}

void espera(){
  sleep(tempo); //verifica se isso está correto
}

void imprimeRelatorioCompleto(){ //TODO

}


// inicio
int main(int argc, char *argv[ ]) {
	// reseta o seed de tempo
	srand(time(0));
	
	// trata argumentos passados via linha de comando
	string comando = argv[1];
	opcao = resolveOpresolveOp(comando);	
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

  imprimeRelatorioCompleto(); //TODO

  return 0;

};
