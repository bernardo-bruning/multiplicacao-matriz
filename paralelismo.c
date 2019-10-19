#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"paralelismo.h"

/**
 * Permite criar uma estrutura de paralelismo.
 **/
paralelismo *criar_paralelismo(int capacidade, void* (*funcao)(void *)) {
  paralelismo *self = malloc(sizeof(paralelismo));
  self->funcao = funcao;
  self->capacidade = capacidade;
  self->quantidade = 0;
  self->threads = malloc(sizeof(pthread_t)*capacidade);
  self->elementos = malloc(sizeof(void *)*capacidade);
  return self;
}

/**
 * Permite inserir um elemento no paralelismo.
 **/
void paralelismo_inserir(paralelismo *self, void* elemento) {
#if DEBUG
  printf("DEBUG: paralelismo_inserir\n");
#endif
  pthread_t *thread = &self->threads[self->quantidade];
  pthread_create(thread, NULL, self->funcao, elemento);
  self->quantidade++;
}

/**
 * Permite aguardar o paralelismo ser concluído.
 **/
void paralelismo_aguardar(paralelismo *self) {
#if DEBUG
  printf("DEBUG: paralelismo_aguardar\n");
#endif
  for(int i=0; i<self->quantidade; i++) {
    pthread_t thread = self->threads[i];
    pthread_join(thread, NULL);
  }

  self->quantidade = 0;
}

/**
 * Permite processar sem gerenciar os elementos a serem inclusos
 **/
void paralelismo_processar(paralelismo *self, void *elemento){
#if DEBUG
  printf("DEBUG: paralelismo_processar\n");
#endif
   paralelismo_inserir(self, elemento);
   if(self->quantidade==self->capacidade) {
     paralelismo_aguardar(self);
   }
}

/**
 * Permite deletar o paralelismo da memoria sem garantia de finalização.
 **/
void forca_deletar_paralelismo(paralelismo *self) {
  free(self->threads);
  free(self);
}

/**
 * Permite deletar o paralelismo da memoria garantindo que todos as threads foram finalizadas.
 **/
void deletar_paralelismo(paralelismo *self) {
  paralelismo_aguardar(self);
  forca_deletar_paralelismo(self);
}
