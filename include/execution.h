#ifndef __EXECUTION__
#define __EXECUTION__

/**
@file execution.h
Ficheiro com funções relacionadas com a execução do programa.
*/

#include "posicao.h"

/**
\brief Função que executa um comando simples equivalente a uma linha só com $.

@param fdaux Ficheiro que vai ser escrito
@param line Linha que vai ser executada
@param pos Estrutura que contêm os resultados das execuções anteriores
*/
void executa_comando(int fdaux, char* line, POSICOES pos);

/**
\brief Função que executa um comando com pipe equivalente a uma linha que começa por $|.

@param fdaux Ficheiro que vai ser escrito
@param line Linha que vai ser executada
@param pos Estrutura que contêm os resultados das execuções anteriores
*/
void executa_comando_pipe(int fdaux, char* line, POSICOES pos, int n_anterior);

/**
\brief Função que escreve uma linha, que não seja comandou ou resultado, no
ficheiro.

@param fdaux Ficheiro que vai ser escrito
@param line Linha que vai ser escrita
*/
void executa_linha(int fdaux, char* line);

/**
\brief Função que decide qual o tipo de execução a fazer consoante o
tipo da linha.

@param fdaux Ficheiro a ser alterado
@param line Linha que vai ser analisada
@param pos Estrutura que contêm os resultados das execuções anteriores
@param resultado Se estamos em situação de resultado ou não

@returns Natureza da linha.
*/
int execution(int fdaux, char* line, POSICOES pos, int resultado);

#endif