#include"multiplicacao_matriz_fragmento.h"


multiplicacao_matriz_fragmento *criar_multiplicacao_matriz_fragmento(int inicio_x, int inicio_y, int fim_x, int fim_y, multiplicacao_matriz *multiplicacao_matriz) {
  #if DEBUG
  printf("DEBUG: criar_multiplicacao_matriz_fragmento\n");
  #endif
  multiplicacao_matriz_fragmento *self = malloc(sizeof(multiplicacao_matriz_fragmento));
  self->inicio_x = inicio_x;
  self->inicio_y = inicio_y;
  self->fim_x = fim_x;
  self->fim_y = fim_y;
  self->multiplicacao_matriz = multiplicacao_matriz;
  return self;
}

void multiplicacao_matriz_fragmento_processar(multiplicacao_matriz_fragmento *self){
  #if DEBUG
  printf("DEBUG: multiplicacao_matriz_fragmento_processar({ inicio_x: %d, inicio_y: %d, fim_x: %d, fim_y: %d })\n", self->inicio_x, self->inicio_y, self->fim_x, self->fim_y);
  #endif
  for(int x=self->inicio_x; x<self->fim_x; x++) {
    for(int y=self->inicio_y; y<self->fim_y; y++) {
      multiplicacao_matriz_celula_calcular(self->multiplicacao_matriz, x, y);
    }
  }
}
