#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include "file_string.h"
#include "parser.h"

int estado;
int original;
int auxiliar;

void handler(int signo){
	switch (signo){
			case SIGINT:
				if(estado == 0){
				 	kill(0,SIGQUIT);
				 	break;
				}
				if(estado == 2){
					remove_aux(auxiliar);
				}
				if(estado == 3){
					lseek(auxiliar, 0, SEEK_SET);
					copia_ficheiro(original,auxiliar);
					remove_aux(auxiliar);
				}
				if(estado == 4){
					remove_aux(auxiliar);
				}
				close(original);
				kill(0,SIGQUIT);
				break;
	}
}

int main(int argc, char *argv[]){
	
	signal(SIGINT, handler);

	// abrir ficheiro passado
	int fd = open(argv[1], O_RDONLY);

	original = fd;
	estado = 1;

	if(fd == -1){
		perror("File can't be opened!!");
		return -1;
	}

	// executar o programa
	int fdaux = read_all_lines(fd);

	auxiliar = fdaux;
	estado = 2;

	close(fd);

	fd = open(argv[1], O_TRUNC | O_WRONLY);
	if(fd == -1){
		perror("File can't be opened!!");
		return -1;
	}

	original = fd;
	auxiliar = fdaux;
	estado = 3;

	// posiciona o auxiliar no inicio
	lseek(fdaux, 0, SEEK_SET);

	// copia os ficheiros
	copia_ficheiro(fd,fdaux);

	original = fd;
	auxiliar = fdaux;
	estado = 4;

	// apaga o auxiliar
	remove_aux(fdaux);

	estado = 5;

	// fecha o original
	close(fd);

	return 0;
}