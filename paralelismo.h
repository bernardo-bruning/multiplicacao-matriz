#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>


#ifndef _PARALELISMOH_
#define _PARALELISMOH_
typedef struct paralelismo {
  int capacidade;
  void* (*funcao)(void *);
  int quantidade;
  pthread_t *threads;
  int tamanho_elemento;
  void** elementos;
} paralelismo;

/**
 * Permite criar uma estrutura de paralelismo.
 **/
paralelismo *criar_paralelismo(int capacidade, void* (*funcao)(void *));
  
/**
 * Permite inserir um elemento no paralelismo.
 **/
void paralelismo_inserir(paralelismo *self, void* elemento);

/**
 * Permite aguardar o paralelismo ser concluído.
 **/
void paralelismo_aguardar(paralelismo *self);

/**
 * Permite processar sem gerenciar os elementos a serem inclusos
 **/
void paralelismo_processar(paralelismo *self, void *elemento);

/**
 * Permite deletar o paralelismo da memoria sem garantia de finalização.
 **/
void forca_deletar_paralelismo(paralelismo *self);

/**
 * Permite deletar o paralelismo da memoria garantindo que todos as threads foram finalizadas.
 **/
void deletar_paralelismo(paralelismo *self);
#endif
