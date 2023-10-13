#ifndef HTURING
#define HTURING

#define  _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include "./lib/j_strings.h"
#include "./lib/j_linkedList.h"

/// máquina de turing ordinária
  struct transition {
    char* state0; // APONTA PARA a mesma string que char **states
    char* state1; // mantém relação de igualdade de ponteiros
    char movement;// 'L'-> esquerda   'R'-> direita
    char write; 
    char read;
  };


  typedef struct {
    // 7-upla 
    struct transition *transitions;
    char **states; // <-| 
    char  *state0; //   | mantém relação de igualdade de ponteiros
    char **accept; // ->| aponta para as mesmas strings que states**
    char *symbols_alphabet;
    char *symbols_tape;
    char  symbol_empty;
    

    // dados
    int  transitions_n;
    list tape;
  } turing;


/// Checa se é uma máquina de turing válida
/// Se for válida retorna 1
int turing_Check(turing target) {
  if ((target.states == 0)
    ||(target.state0 == 0)
    ||(target.accept == 0)
    ||(target.symbols_alphabet == 0)
    ||(target.symbols_tape == 0)
    ||(target.transitions == 0)) {
    return 0;
  }
  return 1;
}


/// Sincroniza os ponteiros da transição e checa por erros com a máquina target (como símbolo fora do alfabeto)
struct transition turing_syncTransition(turing target, char *state0, char *state1, char mov, char write, char read) {
  struct transition ret;
  ret.movement = mov;

  int exists_write = 0;
  int exists_read  = 0;
  int exists_state0= 0;
  int exists_state1= 0;

  // checa símbolo de escrita
  if (write != target.symbol_empty) {
    for (int i = 0; target.symbols_tape[i] != 0; i++) {
      if (write == target.symbols_tape[i]) {
        ret.write = write;
        exists_write = 1;
        break;
      }
    }
    if (exists_write == 0) {
      // erro
      printf("Transição inválida com símbolo %c\n", write);
    }
  } else {
    ret.write = target.symbol_empty;
    exists_write = 1;
  }
  
  
  // checa símbolo de leitura
  if (read != target.symbol_empty) {
    for (int i = 0; target.symbols_tape[i] != 0; i++) {
      if (read == target.symbols_tape[i]) {
        ret.read = read;
        exists_read = 1;
        break;
      }
    }
    if (exists_read == 0) {
      // erro
      printf("Transição inválida com símbolo %c\n", read);
    }
  } else {
    ret.read = target.symbol_empty;
    exists_read = 1;
  }

  // checa se existe o estado
  for(int i = 0; target.states[i] != 0; i++) {
    if(str_match(state0, target.states[i])) {
      exists_state0 = 1;
      ret.state0 = target.states[i]; 
    }

    if (str_match(state1, target.states[i])) {
      exists_state1 = 1;
      ret.state1 = target.states[i];
    }

    if ((exists_state0 * exists_state1) != 0)
      break;
  }
  if ((exists_state0 * exists_state1) == 0) {
    printf("Estados inválidos na transição");
  }

  return ret;
}


/// Submete a palavra sobre a máquina turing alvo
/// returna 1 se aceitar ou 0 se rejeitar.
int turing_Run(turing target, char *word) {
  if (turing_Check(target) == 0) {
    printf("Máquina de turing inválida\n");
    return 0;
  }
  
    // preenche a fita
  if (target.tape.ini != 0) {
    list_Free(&target.tape);
  }

  target.tape = list_New();
  for (size_t i = 0; word[i] != 0; i++) {
    list_addLast( &target.tape, item_new(word[i]));
  }

  // prepara cabeçalho, estado inicial e parada.
  
  struct Node *header = target.tape.ini;
  char *state = target.state0;
  char run = 1;

  // roda a máquina
  while(run) {
    char symbol = header->item.symbol;
    run = 0; // define condição de parada
    
    // pesquisa transições
    for(int i = 0; i < target.transitions_n; i++) {
      if ((target.transitions[i].read == symbol) && (target.transitions[i].state0 == state)) {
        // transicao valida
        header->item.symbol = target.transitions[i].write;
        state = target.transitions[i].state1;
        if (target.transitions[i].movement == 'E') {
          // para esquerda
          header = header->prev;
          if (header == NULL) {
            list_addFirst(&target.tape, item_new(target.symbol_empty));
            header = target.tape.ini;
          }

        } else if (target.transitions[i].movement == 'D'){
          // para direita
          header = header->next;
          if (header == NULL) {
            list_addLast(&target.tape, item_new(target.symbol_empty));
            header = target.tape.end;
          }
        }
        // continua o algoritmo
        run = 1;
        continue;
      }
    }
  }
  // condição de parada
  // checa se está em um estado de aceitação
  for (int i = 0; target.accept[i] != 0; i++) {
    if (state == target.accept[i]) {
      return 1;
    }
  }


  return 0;
}
#endif

