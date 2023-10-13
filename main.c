// importa alocação junto
#include <stdio.h>
#include <stdlib.h>


/// AVISO
/// O código foi pensado não voltado ao arquivo exemplo mas sim à seção de objetivo comentada e à definição formal
/// da máquina de turing, portanto o código é MUITO mais verboso que seria se estivéssemos só considerando o arquivo
/// de exemplo. ISSO e eu estava sem acesso ao arquivo eu tinha feito uma "lib" de máquina de turing com suporte à strings,
/// por tanto a aplicação passa da proposta do projeto e vira um matar uma formiga com uma bazuca igual o adilson fala.
///
/// Para resolver descrições como no arquivo ou com as considerações ( n[alfabeto] < 30, estados < 50 ...)
/// uma implementação básica já basta.
///
/// !! OS OUTROS arquivos no projeto são todos meus (sem plágio), quase todo projeto eu carrego as 
/// coisas que eu já tenho feito, então desnecessariamente eu tenho uma alocação manual de memória no aloca.h,
/// isso porque quase todos os outros arquivos acabam usando ela, é tipo javascript a gnt só usa pq todo mundo faz.

/// Uma lib que generaliza a máquina turing
#include "./turing.h"


/// main, linka o objetivo do projeto com a lib que generaliza a mt
int main(int argc, char **argv) 
{
  if (argc != 2) 
  {
    printf("Insira um caminho para um arquivo com a descrição da máquina como argumento");
    return -1;
  }


  // prepara a máquina de turing conforme o projeto especifica
  // parecido com uma máquina de estados (lendo 1 -> lendo 2).
  turing maquina;
  maquina.symbol_empty = '-';

  // flags para a "máquina de estados"
  int index_linha = 0;
  int index_transicoes = 0; // aonde terminam as transicoes
  int quant_palavras = 0;
  int palavra_atual = 1;

  // abre o arquivo e lê linha a linha
  char *str_linha = 0;
  size_t useless_var_nao_sei_que = 0;
  int len_linha = 0;
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) return -1;
  
  while((len_linha = getline(&str_linha, &useless_var_nao_sei_que, fp)) != -1 )
  {
    // remove quebra de linha
    str_linha[len_linha-1] = 0;
   
    // linha 1 - seta o alfabeto
    if (index_linha == 0) 
    {
      maquina.symbols_alphabet = str_copy(str_linha);
      maquina.symbols_tape = maquina.symbols_alphabet;
      printf("%s\n", maquina.symbols_alphabet);
    } 

    // linha 2 - seta os estados
    else if (index_linha == 1) 
    {
      int num = ret_int(str_linha);
      maquina.states = malloc(sizeof(char*)*num);

      // gera o nome dos estados automaticamente
      // (para a lib conseguir resolver)
      // o projeto em si não dá os nomes dos estados
      for (int i = 0; i < num; i++) 
      {
        char *nome = ret_str(i+1); // já aloca o nome
        maquina.states[i] = nome;
      }

      // conforme projeto, define estado inicial e de acceitação
      // um estado de aceitação só, para uma variável que aceita vários
      maquina.accept =  (char**) aloca(sizeof(char*)*2);
      maquina.accept[1] = NULL;
      maquina.accept[0] = maquina.states[num-1]; // último estado lido do arquivo
      maquina.state0 =    maquina.states[0];     // primeiro estado lido do arquivo

    }

    // linha 3 - quant de transições
    else if (index_linha == 2) 
    {
      int num = ret_int(str_linha);
      maquina.transitions_n = num;
      maquina.transitions = (struct transition *) aloca(sizeof(struct transition) * num);

      //seta o index de transicoes
      index_transicoes = index_linha + num;
    }

    // linha 3+ - se esiver lendo transições
    else if ((index_transicoes != 0) && (index_linha <= index_transicoes)) 
    {
      //printf("Lendo transição :%s\n", str_linha);
      char ** caracteres = str_split(str_linha, ' '); // split
      // para não precisar criar outro index, iteramos inversamente                                                      
      maquina.transitions[index_transicoes - index_linha] = turing_syncTransition(
          maquina, caracteres[0], caracteres[4], caracteres[3][0], caracteres[2][0], caracteres[1][0]  
          ); // [x][0] quando queremos pegar só um char, é desordenado pois como eu falei, a lib turing veio antes do projeto
    } 

    // linha transicoes+ - lendo as palavras e classificando (final)
    else if (index_linha > index_transicoes && quant_palavras == 0) 
    {
      quant_palavras = ret_int(str_linha);
    } 
    
    else if (quant_palavras > 0)
    {
      if (turing_Run(maquina, str_linha) == 1) 
      {
        printf("%d: %s OK\n",palavra_atual, str_linha); 
      }

      else 
      {
        printf("%d: %s not OK\n",palavra_atual, str_linha); 

      }
      palavra_atual += 1;
    }

    index_linha += 1;
    free(str_linha);
    str_linha = 0;
  };

  return 0;
}
