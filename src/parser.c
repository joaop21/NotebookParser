#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "execution.h"
#include "file_string.h"
#include "posicao.h"
#include "parser.h"

// função que lê uma linha dum ficheiro
char* read_line(int fd){
	char buffer[1024];
	char buf[1];
	int n,i;

	// lê carater a carater ate encontrar '\n' ou EOF
	for(i = 0 ; (n = read(fd,buf,sizeof(buf))) > 0 ; i++){
		// no caso de ser '\n'
		if(buf[0] == '\n'){
			break;
		}
		else buffer[i] = buf[0];
	}
	if(n == -1){
		perror("Read do read_line deu erro.");
		kill(0,SIGINT);
	}
	// quando sai do ciclo por ser ou '\n' ou EOF
	buffer[i] = '\n';
	buffer[i+1] = '\0';

	// coloca numa variavel com o tamanho correto e retorna
	char* res = malloc(sizeof(char)*(i+2));
	for(int j = 0 ; j<(i+2) ; j++) res[j] = buffer[j];

	return res;
}


int read_all_lines(int fd){

	int fdaux = create_aux_file();

	int resultado = 0; // usada para saber se a proxima linha é um resultado (1 quando o é)

	POSICOES posi = malloc(sizeof(struct posicoes));
	posi->pos = malloc(sizeof(struct posicao)*1024);
	posi->pos_count = 0;

	// lê aqui a primeira linha pq dava erro se implementasse depois do lseek
	char* s;
	s = read_line(fd);

	// retira o tamanho do ficheiro
	int tamanho = lseek(fd, 0, SEEK_END);
	if(tamanho == -1){
		perror("lseek (tamanho) não terminou direito em read_all_lines.");
		kill(0,SIGINT);
	}

	// coloca o offset no inicio do ficheiro
	int offset = 0;
	while(s != NULL){

		// processa a linha consoante o tipo dela
		int tipo = execution(fdaux, s, posi, resultado);
		if(tipo == 4) resultado = 1;
		if(tipo == 6) resultado = 0;

		// adiciona o tamanho de 's', para se posicionar corretamente no ficheiro
		offset += strlen(s);

		// em apontador está a posição no file medido em bytes 
		int apontador = lseek(fd, offset, SEEK_SET);
		if(apontador == -1) {
			perror("lseek (apontador) não terminou direito em read_all_lines.");
			kill(0,SIGINT);
		}

		// testa se o apontador é maior que o tamanho para saber se deve parar a execução
		if(apontador >= tamanho) break;

		// lê a linha seguinte
		s = read_line(fd);
	}

	return fdaux;

}