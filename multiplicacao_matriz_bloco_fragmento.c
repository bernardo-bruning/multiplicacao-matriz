#include"multiplicacao_matriz_bloco_fragmento.h"

multiplicacao_matriz_bloco_fragmento *criar_multiplicacao_matriz_bloco_fragmento(int inicio_x, int inicio_y, int fim_x, int fim_y, multiplicacao_matriz_bloco *bloco) {
  #if DEBUG
  printf("DEBUG: criar_multiplicacao_matriz_bloco_fragmento(%d, %d, %d, %d, %p)\n", inicio_x, inicio_y, fim_x, fim_y, bloco);
  #endif
  multiplicacao_matriz_bloco_fragmento *self = malloc(sizeof(multiplicacao_matriz_bloco_fragmento));
  self->inicio_x = inicio_x;
  self->inicio_y = inicio_y;
  self->fim_x = fim_x;
  self->fim_y = fim_y;
  self->bloco = bloco;
  return self;
}

void multiplicacao_matriz_bloco_fragmento_processar(multiplicacao_matriz_bloco_fragmento *self){
  #if DEBUG
  printf("DEBUG: multiplicacao_matriz_fragmento_bloco_processar({ inicio_x: %d, inicio_y: %d, fim_x: %d, fim_y: %d })\n", self->inicio_x, self->inicio_y, self->fim_x, self->fim_y);
  #endif
  for(int x=self->inicio_x; x<self->fim_x; x++) {
    for(int y=self->inicio_y; y<self->fim_y; y++) {
      multiplicacao_matriz_bloco_celula_calcular(self->bloco, x, y);
    }
  }
}
