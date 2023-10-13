#ifndef J_LINKEDLIST_H
#define J_LINKEDLIST_H

// V 1.3 - Específico para o projeto aloca

#include<stdbool.h>
#include "./j_strings.h"
#include "./aloca.h"
#include "./j_transform.h"


// Estrutura do conteúdo de um node
struct Item
{
  char symbol;
};


// Estrutura node
struct Node
{
    struct Node *next;
    struct Node *prev;
    struct Item item;
} typedef * snode; 


// Estrura da lista ligada
typedef struct 
{
    snode ini;
    snode end;
    int len;
} list;


// -------------------------------------------------------------- //
// FUNÇÕES SUJEITAS A MUDANÇA, NECESSÁRIO ADAPTAR DE PROJETO PARA PROJETO

// Cria um novo item
// mudando os conteúdos de struct Item, se torna necessário
// também mudar esta função
struct Item item_new(char symbol)
{
  struct Item ret;
  ret.symbol = symbol;
  return ret;
}


// Função para comparar itens, 
// mudando os conteúdos de struct Item, se torna necessário
// também mudar esta função.
bool __item_Compare(struct Item i1, struct Item i2)
{
  if (i1.symbol == i2.symbol)
    return true;

  return false;
}


// Função que retorna uma cópia de item,
// necessário saber um pouco mais sobre C para usar isto corretamente
struct Item __item_Copy(struct Item value)
{
  // criação
  struct Item ret;

  // ret = value; só após C90 
  ret.symbol = value.symbol;

  return ret;
}


// Destrói o item se algo estiver alocado
// obviamente consome qualquer coisa alocada de dentro
int item_destroy(struct Item target) {
  // não existe nada alocado nessa lista.
  if (target.symbol+1 == target.symbol+1)
    return 1;
  return 0;
}




// ------------------------------------------------------------- //
/// FUNÇÕES NÃO SUJEITAS A MUDANÇA, NÃO TOQUE, SÓ EM CASO DE BUG

//Cria uma nova lista e retorna
list list_New()
{
  list ret;
  ret.len = 0;
  ret.ini = NULL;
  ret.end = NULL;
  return ret;
}


// Adiciona um elemento ao inicio da lista
// é necessário liberar a memória
void list_addFirst(list * target, struct Item value)
{
  snode new_n = (snode) aloca(sizeof( struct Node ));
  new_n->item = __item_Copy(value);
  new_n->prev = NULL;
  new_n->next = NULL;

  if ( target->len == 0 )
  {
    target->ini = new_n;
    target->end = new_n;
    target->len = 1;
    return;
  }

  target->len += 1;
  
  snode aux = target->ini;
  target->ini = new_n;

  new_n->next = aux;
  aux->prev = new_n;
}


// Adiciona um elemento ao final da lista
// é necessário liberar a memória
void list_addLast(list * target, struct Item value)
{
  snode new_n = (snode) aloca(sizeof( struct Node ));
  new_n->item = __item_Copy(value);
  new_n->prev = NULL;
  new_n->next = NULL;

  if ( target->len == 0 )
  {
    target->ini = new_n;
    target->end = new_n;
    target->len = 1;
    return;
  }

  target->len += 1;
  snode aux = target->end;
  target->end = new_n; 
  aux->next = new_n;
  new_n->prev = aux;
}


// Função para remover um nó com um nó como parâmetro
// O nó DEVE existir na lista, não é uma função de busca
// A cadeia de caracteres é consumida !!
void list_nodeRemove(list *target, snode node)
{
  item_destroy(node->item);
  snode parent = node->prev;
  snode child = node->next;
  
  // faz o antecessor se conectar ao sucessor
  if (parent != NULL)
    parent->next = child;

  else
    target->ini = child;

  
  // faz o sucessor se conectar ao antecessor
  if (child != NULL)
    child->prev = parent;

  else
    target->end = parent;

  target->len -= 1;
  libera(node);
}


// Função para remover um nó com uma estrutura como parâmetro
// Retorna true se foi removido com sucesso
bool list_itemRemove( list * target, struct Item value) 
{
  // checa o tamanho da lista
  if (target->len == 0) return false; 
  
  if (target->len == 1 ) 
  {
    if (__item_Compare( target->ini->item, value))
    {
      list_nodeRemove( target, target->ini );
      return true;
    }
    else return false;
  }

  // exitem mais de dois elementos:
  snode parent = (snode) target;
  while(parent->next != NULL)
  {
    if (__item_Compare( target->ini->item, value))
    {
      list_nodeRemove( target, parent->next);
      return true;
    }

    // movimenta
    parent = parent->next;
  } 
  
  return false;
}


// Retorna a quantidade de elementos da lista
int list_Count( list target )
{
  if (target.ini == NULL)
    return 0;
  
  snode current = target.ini;
  int count = 1;
  while ( current->next != NULL )
  {
    current = current->next;
    count += 1;
  }

  return count;
}


// Limpa a lista liberando toda sua memória
void list_Free( list *target)
{
  snode node = target->ini;
  snode aux;
  while ( node != NULL )
  {
    aux = node->next;
    list_nodeRemove(target, node);
    node = aux;
  }
}

#endif
