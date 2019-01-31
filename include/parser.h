#ifndef __PARSER__
#define __PARSER__

/**
@file parser.h
Ficheiro com funções que realizam o parser do ficheiro.
*/

/**
\brief Função que lê uma linha de um ficheiro.

@param fd Ficheiro a ser lido

@returns Linha lida do ficheiro.
*/
char* read_line(int fd);

/**
\brief Função que lê todas as linhas de um ficheiro, separando-as.

@param fd Ficheiro a ser lido

@returns Ficheiro auxiliar.
*/
int read_all_lines(int fd);

#endif