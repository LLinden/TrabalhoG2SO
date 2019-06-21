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
};

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
					cout << j;
					// vai fazer o laço pular para o ponto de onde parou a leitura do bloco
					i = j; 

				break;
				}
			}
			
			bloco.tamanho = bloco.fim - bloco.inicio;
			printf("\n\nid: ");
			cout << bloco.id << "\n";
			printf("tamanho: ");
			cout << bloco.tamanho << "\n";
			printf("inicio: ");
			cout << bloco.inicio << "\n";
			printf("fim: ");
			cout <<bloco.fim << "\n\n";
			cout << "-----------------------------------\n";
			blocos[idBlocos] = bloco;
		}

	}
	return blocos;
};

// escreve processos dentro do bloco e no vetor
void escreveProcesso(Processo processo, Bloco bloco){
	printf("escrevendo bloco no processo\nblocoInicio: ");
	cout << bloco.inicio;
	printf("\ntamanhoProcesso: ");
	cout << processo.tamanho;
	
	for (int i = bloco.inicio; i < processo.tamanho; i++){
		vetor[i] = processo.simbolo;	
		
	}
};

// first-fit
void firstFit(Processo *processo) {	
	// aqui a alocação propriamente dita
	/*for (int numeroProcesso = 0; numeroProcesso < nProc ; numeroProcesso++){ 
		Processo processoAtual = processo[numeroProcesso];
		pos = 0;
		for (int intTamanho = 0; intTamanho < processoAtual.tamanho; intTamanho++){
			for(int l = 0; l < TAMANHO; l++) {   
				// teste para verificar a possibilidade de alocacao
				if ((vetor[l] == '.' && pos < processoAtual.tamanho)) {
					pos++;
					vetor[l] = processoAtual.simbolo;
				}				         
			}
		}        
	}*/
	
	for (int numeroProcesso = 0; numeroProcesso < nProc ; numeroProcesso++){ 
		Processo processoAtual = processo[numeroProcesso];

		int alocou = 0;
		Bloco blocoQueCabeOProcesso;
		
		Bloco *blocos = construtorBloco();
		
		for (int numeroBloco = 0; numeroBloco < 2500; numeroBloco++){
			Bloco blocoAtual = blocos[numeroBloco];
			if (blocoAtual.tamanho >= processoAtual.tamanho){
				blocoQueCabeOProcesso = blocoAtual;
				alocou = 1;
				break;
			}
		}
		
		if (alocou == 1){
			printf("\n\nid: ");
			cout << blocoQueCabeOProcesso.id << "\n";
			printf("tamanho: ");
			cout << blocoQueCabeOProcesso.tamanho << "\n";
			printf("inicio: ");
			cout << blocoQueCabeOProcesso.inicio << "\n";
			printf("fim: ");
			cout << blocoQueCabeOProcesso.fim << "\n\n";
			cout << "-----------------------------------\n";
			printf("\n\nProcid: ");
			cout << processoAtual.pid << "\n";
			printf("Proctamanho: ");
			cout << processoAtual.tamanho << "\n";
			cout << "-----------------------------------\n";
			escreveProcesso(processoAtual, blocoQueCabeOProcesso);
		}
	}	
			
	//system("clear");
	exibeVetor();
	
	// calcula tempo total
	for (int i = 0; i < nProc ; i++){
		if (processo[i].tempoExec > cron) {
			cron = processo[i].tempoExec;
		}
	}			
			
	// gerencia de tempo de execucao
	while (cron > 0) {
		for (int i = 0; i < nProc; i++) {
			if (processo[i].tempoRest > 0) {
				int tmp = processo[i].tempoRest - 1;
					processo[i].tempoRest = tmp;
			}						
		}
		// chama legenda para os n processos criados
		exibeVetor();
		tituloLegenda();
		legenda(nProc, processo);
		cron = cron - 1;
		cout << "\nTEMPO TOTAL RESANTE: " << cron;
		cout << flush;
		sleep(1);
	}
};

// circular-fit
void circularFit() {
	//Bloco *blocos = construtorBloco();
	/*for (int i = 0; i < 2; i++) {
		cout << blocos[i].id << "\n";
		cout << blocos[i].tamanho << "\n";
		cout << blocos[i].inicio << "\n";
		cout <<blocos[i].fim << "\n\n";
		cout << "-----------------------------------\n";
	}
	cout << endl;*/
};

// inicio
int main(int argc, char *argv[ ]) {
	// reseta o seed de tempo
	srand(time(0));
	
	// trata argumentos passados via linha de comando
	string comando = argv[1];
	resolveOp(comando);	
	nProc = atoi(argv[2]);
	// arumento tempo opcional
	if (argv[3] != NULL) tempo = atoi(argv[3]);	
		
	// inicializa os processos
	Processo *processo = construtorProc();
	
	// inicializa vetor
	criaVetor();		
	
	// seleciona algoritmo a ser executado
	switch (resolveOp(comando)) {
			
		// first-fit
		case Op1:
			exibeVetor();
			// chama legenda para os n processos criados
			tituloLegenda();
			legenda(nProc, processo);

			sleep(2);
						
			firstFit(processo);
		
			break;
			
		// circular-fit
		case Op2:
			
			exibeVetor();
			circularFit();

			break;
			
		// best-fit
		case Op3:

			break;
				
		// worst-fit
		case Op4:

			break;		
				
		// comando não encontrado
		default:
			cout << "Argumentos inválidos!" "!\n";
		}						
	return 0;
};
