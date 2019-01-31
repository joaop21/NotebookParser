#ifndef __FILE_STRING__
#define __FILE_STRING__

/**
@file file_string.h
Funções relacionadas com os ficheiros usados no programa e as diferentes formas de tratar as suas linhas.
*/

#define COMMAND 1
#define COMANDO_PIPE 2
#define LINHA 3
#define RESULTADO_INICIO 4
#define RESULTADO 5
#define RESULTADO_FIM 6
#define COMANDO_PIPE_N 7

/**
\brief Função que cria um ficheiro auxiliar com o intuito de nele ficar todo a informação 
que vai constar no ficheiro final.

@returns File descriptor do ficheiro criado.
*/
int create_aux_file();

/**
\brief Função que apaga o ficheiro auxiliar utilizado no processo.

@param fdaux Ficheiro a ser apagado
*/
void remove_aux(int fdaux);

/**
\brief Função que coloca os caracteres que indicam o início do resultado de um comando.

@param fdaux Ficheiro a ser alterado
*/
void resultado_inicio(int fdaux);

/**
\brief Função que coloca os caracteres que indicam o fim do resultado de um comando.

@param fdaux Ficheiro a ser alterado
*/
void resultado_fim(int fdaux);

/**
\brief Função que lê dum resultado anterior um determinado número de bytes a partir 
de um apontador denominado início, que é o início do tal resultado anterior.

@param fdaux Ficheiro a ser alterado
@param inicio Início da leitura
@param bytes Número de bytes a ler

@returns Resultado que foi lido do ficheiro auxiliar.
*/
char* le_resultado(int fdaux, int inicio, int bytes);

/**
\brief Função que copia toda a informação dum ficheiro auxiliar para o ficheiro resultado.

@param fd Ficheiro a ser alterado
@param fdaux Ficheiro a ser copiado
*/
void copia_ficheiro(int fd, int fdaux);

/**
\brief Função que tem o propósito de escrever num ficheiro a string passada.

@param fdaux Ficheiro a ser alterado
@param line Linha que será escrita no ficheiro
*/
void escreve_em_ficheiro(int fdaux, char* line);

/**
\brief Função que diz qual a natureza de uma linha lida consoante os primeiros carateres.

@param line Linha a ser analisada
@param resultado Se estamos em situação de resultado ou não

@returns Natureza da linha.
*/
int line_nature(char line[], int resultado);

/**
\brief Função que recebe uma linha, e usando o strtok separa esta mesma nos 
caracteres ' ' e '\ n', colocando o resultado num array de strings, que são 
o comando e seus argumentos.

@param line Linha a ser separada
@param n Número passado no caso de ser para executar o n-ésimo comando anterior

@retunrs Array de strings que vai ser usado na execução.
*/
char** separa_comando(char* line, int n);

#endif