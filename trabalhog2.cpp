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

// definição da distribuição da matriz
#define NLIN 50
#define NCOL 100

using namespace std;

// variaveis globais
int nProc, tempo = 0;
char Matriz[NLIN][NCOL];
char dicionario[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M',
					'N','O','P','Q','R','S','T','U','V','X','Y','W','Z'};
	
// modelo do objeto Processo
class Processo {
	public:
		int pid;   
		int tamanho; 
		int tempoExec;
		char simbolo;	
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

// cria matriz principal
void criaMatriz(){
	int i, j;
	
	for(i=0; i < NLIN; i++) {     
		for(j=0; j < NCOL; j++) {    
			Matriz[i][j] = '.';
		}
	}						
};

// exibe matriz na tela
void exibeMatriz(){
	int i, j;
	
	system("clear");
		
	for(i=0; i < NLIN; i++) {     
		for(j=0; j < NCOL; j++) {   
			cout << Matriz[i][j];
		}
		cout<<endl;
	}		
};

// legenda com a descricao dos processos criados
void legenda(int pid, char simbolo, int tamanho, int tempoExec) {
	cout << "PID: " << pid << "\n";
	cout << "SIMBOLO: " << simbolo << "\n";
	cout << "TAMANHO: " << tamanho << "\n";
	cout << "TEMP. EXEC.: " << tempoExec << "\n";
	cout << "----------------------------\n";	
};

// inicio
int main(int argc, char *argv[ ]) {
	// variaveis locais
	int count = 0, pos;
	// reseta o seed pro rand
	srand(time(0));	
	
	// trata argumentos passados via linha de comando
	string comando = argv[1];
	resolveOp(comando);	
	nProc = atoi(argv[2]);
	// arumento tempo e opcional
	if (argv[3] != NULL) tempo = atoi(argv[3]);	
	
	// inicializa matriz
	criaMatriz();
	
	// constroi processos
	Processo *processo = new Processo[nProc];
	
	// instancia n processos (quantidade definida em nProc)
	for(int i = 0; i < nProc; i++) {	
		 count = count + 1;
		 processo[i].pid = count;
		 processo[i].tamanho = rand() % 900 + 100;
		 processo[i].tempoExec = rand() % 300 + 1;
		 processo[i].simbolo = dicionario[i];
	}
	
	// seleciona algoritmo a ser executado
	switch (resolveOp(comando)) {
			
		// first-fit
		case Op1:
			// imprime interface
			exibeMatriz();
			// chama legenda para os n processos criados
			for(int i = 0; i < nProc; i++) {
				legenda(processo[i].pid, processo[i].simbolo, processo[i].tamanho, processo[i].tempoExec);
			}
			sleep(2);
			

			// teste < AQUI A ALOCACAO PROPRIAMENTE DITA
			for (int numeroProcesso = 0; numeroProcesso < nProc ; numeroProcesso ++){ 
				Processo processoAtual = processo[numeroProcesso];
				pos = 0;
				for (int intTamanho = 0; intTamanho < processoAtual.tamanho; intTamanho++){
					for(int l = 0; l < NLIN; l++) {   
						for(int c = 0; c < NCOL; c++) {
							if (Matriz[l][c] == '.' && pos < processoAtual.tamanho){
								pos++;
								Matriz[l][c] = processoAtual.simbolo;
							}
						}          
					}
				}        
			}
			
			system("clear");
			exibeMatriz();			
			// chama legenda para os n processos criados
			for(int i = 0; i < nProc; i++) {
				legenda(processo[i].pid, processo[i].simbolo, processo[i].tamanho, processo[i].tempoExec);
			}
		
			break;
			
		// circular-fit
		case Op2:

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
