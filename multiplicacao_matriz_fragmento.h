#include<stdio.h>
#include"multiplicacao_matriz.h"

typedef struct  {
  multiplicacao_matriz *multiplicacao_matriz;
  int inicio_x;
  int inicio_y;
  int fim_x;
  int fim_y;
} multiplicacao_matriz_fragmento;

multiplicacao_matriz_fragmento *criar_multiplicacao_matriz_fragmento(int inicio_x, int inicio_y, int fim_x, int fim_y, multiplicacao_matriz *multiplicacao_matriz);

void multiplicacao_matriz_fragmento_processar(multiplicacao_matriz_fragmento *self);
