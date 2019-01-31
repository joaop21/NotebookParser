#ifndef __POSICAO__
#define __POSICAO__

/**
@file posicao.h
Ficheiro com estrutura que contêm os resultados das execuções anteriores.
*/

/**
\brief Estrutura que contém a posição de início de um resultao e o número 
de bytes que o resultado ocupa.
*/
typedef struct posicao{
	int posicao_inicio;
	int num_bytes;
} *POSICAO;

/**
\brief Estrutura que contém um array de todas as posições dos resultados 
escritos escritos em ficheiro, bem como o contador de posições.
*/
typedef struct posicoes{
	POSICAO* pos;
	int pos_count;
} *POSICOES;

#endif