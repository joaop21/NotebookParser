#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include "file_string.h"

//////////////////// Ficheiros

int create_aux_file(){
	int fd = open("/tmp/auxiliar.nb", O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(fd == -1){
		perror("O ficheiro auxiliar nao conseguiu ser criado.");
		kill(0,SIGINT);
	}
	return fd;
}

void remove_aux(int fdaux){

	close(fdaux);

	if(fork()==0){
		execlp("rm","rm","/tmp/auxiliar.nb", NULL);
		perror("Erro ao apagar o ficheiro.");
		kill(0,SIGINT);
	}
	wait(NULL);
}

void resultado_inicio(int fdaux){
	char* inicio = malloc(sizeof(char)*5);
	inicio = ">>>\n\0";
	escreve_em_ficheiro(fdaux,inicio);
}

void resultado_fim(int fdaux){
	char* fim = malloc(sizeof(char)*5);
	fim = "<<<\n\0";
	escreve_em_ficheiro(fdaux,fim);
}

char* le_resultado(int fdaux, int inicio, int bytes){
    
    int in = lseek(fdaux,inicio,SEEK_SET);
    if(in == -1) {
    	perror("lseek (in) retornou erro em le_resultado.");
    	kill(0,SIGINT);
    }
    
    char* res = malloc(sizeof(char)*bytes);

    int nb = read(fdaux,res,bytes);
    if(nb == -1) {
    	perror("Read do le_resultado deu erro.");
    	kill(0,SIGINT);
    }

    int reposiciona = lseek(fdaux,0,SEEK_END);
    if(reposiciona == -1) {
    	perror("lseek (reposiciona) retornou erro em le_resultado.");
    	kill(0,SIGINT);
    }

    return res;
}

void copia_ficheiro(int fd, int fdaux){
	
	char buf[1];
	int n, i = 0;

	int tamanho = lseek(fdaux, 0, SEEK_END);
	if(tamanho == -1) {
		perror("lseek (tamanho) retornou erro em copia_ficheiro.");
		kill(0,SIGINT);
	}

	int cur = lseek(fdaux, 0, SEEK_SET);
	if(cur == -1) {
		perror("lseek (cur) retornou erro em le_resultado.");
		kill(0,SIGINT);
	}

	while(i < tamanho && (n = read(fdaux,buf,sizeof(buf))) > 0){
		int nb = write(fd,buf,sizeof(buf));
		if(nb == -1) {
			perror("write retornou erro em copia_ficheiro");
			kill(0,SIGINT);
		}
		i++;
	}
	if(n==-1) {
		perror("read retornou erro em copia_ficheiro");
		kill(0,SIGINT);
	}
}

void escreve_em_ficheiro(int fdaux, char* line){
	int tamanho = strlen(line);
	int i = 0;
	int buf[1];

	while(i<tamanho){
		buf[0] = line[i];
		int n = write(fdaux,buf,1);
		if(n == -1) {
			perror("Write do escreve_em_ficheiro");
			kill(0,SIGINT);
		}
		if(line[i]=='\n') break;
		i++;
	}
}

//////////////////// Strings

int line_nature(char line[], int resultado){
	if( line[0] == '$' && line[1] == ' ' ) return COMMAND;
	if( line[0] == '$' && line[1] == '|' ) return COMANDO_PIPE;
	if( line[0] == '$' && line[2] == '|' ) return COMANDO_PIPE_N;
	if( line[0] == '>' && line[1] == '>' && line[2] == '>' ) return RESULTADO_INICIO;
	if( line[0] == '<' && line[1] == '<' && line[2] == '<' ) return RESULTADO_FIM;
	if( resultado == 1 ) return RESULTADO;
	return LINHA;
}

char** separa_comando(char* line, int n){
	// para nao se perder a linha
	char* cp = strdup(line);

	char* cp_res;

	if(n == 0){ // comando normal
		cp_res = malloc(sizeof(char)*strlen(cp));
		int i = 0, j = 1;
		for(; j < strlen(cp) ; i++,j++){
			cp_res[i] = cp[j];
		}
		cp_res[i] = '\0';
	}
	if(n == 1){ // pipe com o resultado anterior
		cp_res = malloc(sizeof(char)*(strlen(cp)-1));
		int i = 0, j = 2;
		for(; j < strlen(cp) ; i++,j++){
			cp_res[i] = cp[j];
		}
		cp_res[i] = '\0';
	}
	if(n != 0 && n != 1){ // pipe com n comando anterior
		cp_res = malloc(sizeof(char)*(strlen(cp)-2));
		int i = 0, j = 3;
		for(; j < strlen(cp) ; i++,j++){
			cp_res[i] = cp[j];
		}
		cp_res[i] = '\0';
	}

	// primeiro separa-se a linha no que queremos
  	char *token;
  	const char delim[] = " \n"; //caracteres em que a string é dividida
  	char** res_aux = malloc(sizeof(char*)*1024);
  	int i = 0;
  	token = strtok (cp_res ,delim);
  	while (token != NULL){
  	    res_aux[i] = token;
  	    token = strtok (NULL, delim);
  	    i++;
  	}

  	// colocamos as divisões da linha em res para retornar
  	// res é um array de strings
  	char** res = malloc(sizeof(char*)*(i+1));
  	int j = 0;
  	for(; j < i ; j++){
  		res[j] = res_aux[j];
  	}
  	res[j] = NULL;
  	return res;
}