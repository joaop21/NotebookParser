#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "file_string.h"
#include "posicao.h"
#include "execution.h"

void executa_comando(int fdaux, char* line, POSICOES posi){
	
	escreve_em_ficheiro(fdaux,line);

	// separar o comando para ficar o essencial
	char** comando = separa_comando(line,0);

	// executa o comando e escreve em ficheiro
	resultado_inicio(fdaux);

	int inicio = lseek(fdaux,0,SEEK_CUR); // dá a posiçao do inicio do resultado
	if(inicio == -1){
		perror("lseek (inicio) retornou erro em executa_comando.");
		kill(0,SIGINT);
	}

	int status;
	if(fork() == 0){
		dup2(fdaux,1);
		close(fdaux);
		execvp(comando[0], comando);
		_exit(-1);
	}
	wait(&status);
	if(WIFEXITED(status) == 0){
		perror("nao executou o comando na executa_comando.");
		kill(0,SIGINT);
	}

	int fim = lseek(fdaux,0,SEEK_CUR); // dá a posiçao do fim do resultado
	if(fim == -1){
		perror("lseek (fim) retornou erro em executa_comando.");
		kill(0,SIGINT);
	}

	resultado_fim(fdaux);
	
	// colocar na estrutura a posição do resultado
	POSICAO posi_resultado = malloc(sizeof(struct posicao));
	posi_resultado->posicao_inicio = inicio;
	posi_resultado->num_bytes = fim-inicio;
	
	// adicionar a lista de posiçoes
	posi->pos[posi->pos_count] = posi_resultado;
	posi->pos_count++;
}


void execution_pipe(int fdaux, char* resultado, char** comando){

	int pd[2];

	pipe(pd);

	if(fork() == 0){
		close(pd[0]);
		dup2(pd[1],1);
		close(pd[1]);
		int nb = write(1,resultado,strlen(resultado));
		if(nb == -1){
			perror("write retornou erro em execution_pipe.");
			kill(0,SIGINT);
		}
		_exit(-1);
	}

	close(pd[1]);
	dup2(pd[0],0);
	dup2(fdaux,1);
	close(pd[0]);

	execvp(comando[0],comando);
	perror("Erro na execução do comando em execution_pipe");
	kill(0,SIGINT);

}

void executa_comando_pipe(int fdaux, char* line, POSICOES posi, int n_anterior){
	
	escreve_em_ficheiro(fdaux,line);

	resultado_inicio(fdaux);

	int n = 1;

	if(n_anterior == 1){
		char* carater = malloc(sizeof(char)*2);
		carater[0] = line[1];
		n = atoi(carater);
	}

	// separar o comando para ficar o essencial
	char** comando = separa_comando(line,n);

	POSICAO pos_anterior = posi->pos[(posi->pos_count)-n];
	char* resultado = le_resultado(fdaux, pos_anterior->posicao_inicio, pos_anterior->num_bytes);

	int inicio = lseek(fdaux,0,SEEK_CUR); // dá a posiçao do inicio do resultado
	if(inicio == -1){
		perror("lseek (inicio) retornou erro em executa_comando_pipe.");
		kill(0,SIGINT);
	}
	
	int status;
	if(fork()==0){
		execution_pipe(fdaux, resultado, comando);
		_exit(-1);
	}
	wait(&status);
	if(WIFEXITED(status) == 0){
		perror("nao executou o comando na executa_comando_pipe.");
		kill(0,SIGINT);
	}

	int fim = lseek(fdaux,0,SEEK_CUR); // dá a posiçao do fim do resultado
	if(fim == -1){
		perror("lseek (fim) retornou erro em executa_comando_pipe.");
		kill(0,SIGINT);
	}

  	resultado_fim(fdaux);

  	// colocar na estrutura a posição do resultado
	POSICAO posi_resultado = malloc(sizeof(struct posicao));
	posi_resultado->posicao_inicio = inicio;
	posi_resultado->num_bytes = fim-inicio;
	
	// adicionar a lista de posiçoes
	posi->pos[posi->pos_count] = posi_resultado;
	posi->pos_count++;
}

void executa_linha(int fdaux, char* line){
	// escreve a linha no auxiliar
	int n = write(fdaux, line, strlen(line));
	if(n == -1) {
		perror("Write retornou erro na executa_linha");
		kill(0,SIGINT);
	}
}

int execution(int fdaux, char* line, POSICOES posi, int resultado){
	// verifica o tipo da linha
	int tipo = line_nature(line, resultado);

	// executa ação perante o tipo
	if(tipo == 1) executa_comando(fdaux, line,posi);
	if(tipo == 2) executa_comando_pipe(fdaux, line, posi, 0);
	if(tipo == 7) executa_comando_pipe(fdaux, line, posi, 1);
	if(tipo == 3) executa_linha(fdaux, line);
	// ignora se for do tipo 4 ou 5 ou 6

	return tipo;
}