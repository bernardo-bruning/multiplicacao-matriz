#include"multiplicacao_matriz_bloco.h"

typedef struct  {
  multiplicacao_matriz_bloco *bloco;
  int inicio_x;
  int inicio_y;
  int fim_x;
  int fim_y;
} multiplicacao_matriz_bloco_fragmento;

multiplicacao_matriz_bloco_fragmento *criar_multiplicacao_matriz_bloco_fragmento(int inicio_x, int inicio_y, int fim_x, int fim_y, multiplicacao_matriz_bloco *bloco);

void multiplicacao_matriz_bloco_fragmento_processar(multiplicacao_matriz_bloco_fragmento *self);
