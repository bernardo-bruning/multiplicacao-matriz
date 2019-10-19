#include<stdio.h>
#include"multiplicacao_matriz_paralela.h"

void *multiplicacao_matriz_fragmento_processar_paralelamente(void * arg) {
  #if DEBUG
  printf("DEBUG: multiplicacao_matriz_fragmento_processar_paralelamente \n");
  #endif
  multiplicacao_matriz_fragmento_processar(arg);
  return NULL;
}

multiplicacao_matriz_paralela *criar_multiplicacao_matriz_paralela(int capacidade, multiplicacao_matriz *matriz) {
#if DEBUG
  printf("DEBUG: criar_multiplicacao_matriz_paralela \n");
#endif
  multiplicacao_matriz_paralela *self = malloc(sizeof(multiplicacao_matriz_paralela));
  self->capacidade = capacidade;
  self->paralelismo = criar_paralelismo(capacidade, multiplicacao_matriz_fragmento_processar_paralelamente);
  self->matriz = matriz;
  return self;
}

void multiplicacao_matriz_paralela_processar(multiplicacao_matriz_paralela *self) {
  #if DEBUG
  printf("DEBUG: multiplicacao_matriz_paralela_processar\n");
  #endif
  multiplicacao_matriz_fragmento *fragmentos[self->capacidade];
  int tamanho = self->capacidade > self->matriz->largura ? 1 : self->capacidade / self->matriz->largura;
  for(int i=0; i<self->capacidade; i++) {
    int inicio = i * tamanho;
    int fim = i == self->capacidade - 1 ? self->matriz->largura : (i + 1) * tamanho;
    fragmentos[i] = criar_multiplicacao_matriz_fragmento(0, inicio, self->matriz->altura, fim, self->matriz);
    paralelismo_inserir(self->paralelismo, fragmentos[i]);
  }
  paralelismo_aguardar(self->paralelismo);
}

