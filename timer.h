typedef struct {
  double inicio;
  double termino;
  double resultado;
} timer;

timer *criar_timer();
void timer_iniciar(timer *self);
void timer_encerrar(timer *self);
void timer_imprimir(timer *self);
