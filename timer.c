#include"timer.h"
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <sys/time.h>

double wtime() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec + (double) t.tv_usec / 1000000;
}

timer *criar_timer() {
  timer *self = malloc(sizeof(timer));
  self->inicio = 0.0;
  self->termino = 0.0;
  self->resultado = 0.0;

  return self;
}

void timer_iniciar(timer *self) {
  self->inicio = wtime();
}

void timer_encerrar(timer *self) {
  self->termino = wtime();
  self->resultado = self->termino - self->inicio;
}

void timer_imprimir(timer *self) {
  printf("timer: {\n\tinicio: %f,\n\ttermino: %f,\n\tresultado: %f\n}\n",
         self->inicio,
         self->termino,
         self->resultado);
}
