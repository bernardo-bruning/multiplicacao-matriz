//////////////////////////////////////////////////

typedef struct parallel_argument {
  int index;
  void *value;
  void *extra;
} parallel_argument;

parallel_argument *create_argument(int index, void *value, void* extra) {
  parallel_argument *argument = malloc(sizeof(parallel_argument));
  argument->index = index;
  argument->value = value;
  argument->extra = extra;
  return argument;
}

//////////////////////////////////////////////////
typedef struct matrix {
  int size_x;
  int size_y;
  int** values;
} matrix;

matrix *create_matrix(int size_x, int size_y, int values[size_x][size_y]) {
  matrix *self = malloc(sizeof(matrix));
  self->values = malloc(sizeof(int *)*size_x);
  for(int y=0;y<size_y;y++) {
    self->values[y] = malloc(sizeof(int));
  }

  for(int x=0;x<size_x;x++) {
    for(int y=0;y<size_y;y++) {
      self->values[x][y] = values[x][y];
    }
  }
  self->size_x = size_x;
  self->size_y = size_y;

  return self;
}

int matrix_get_cell(matrix* self, int x, int y) {
  return self->values[x][y];
}

int matrix_sum(matrix* self, matrix* matrix) {
  if(self->size_x != matrix->size_x || self->size_y != matrix->size_y)
    return 0;

  for(int x=0;x<self->size_x;x++){
    for(int y=0;y<self->size_y;y++){
      self->values[x][y] += matrix_get_cell(matrix, x, y);
    }
  }

  return 1;
}

int matrix_multiplicate(matrix *self, matrix *matrix) {
  return 0;
}

int matrix_print(matrix *self) {
  for(int x=0;x<self->size_x;x++){
    for(int y=0;y<self->size_y;y++){
      printf("matrix[%d][%d] = %d\n", x, y, self->values[x][y]);
    }
  }
}
//////////////////////////////////////////////////

typedef struct matrix_cell {
  int x;
  int y;
  matrix *matrix;
} matrix_cell;

matrix_cell *create_matrix_cell(int x, int y, matrix *matrix) {
  matrix_cell *cell = malloc(sizeof(matrix_cell));
  cell->x = x;
  cell->y = y;
  cell->matrix = matrix;
  return cell;
}

int matrix_cell_value(matrix_cell *cell) {
  return matrix_get_cell(cell->matrix, cell->x, cell->y);
}
//////////////////////////////////////////////////

void *print_value(int value) {
  sleep(1);
  printf("valor: %d\n", value);
}

void *print_value_parallel(void *arg) {
  parallel_argument *argument = (parallel_argument *) arg;
  print_value(argument->index);
  printf("extra: %d\n", *((int *)argument->extra));
}


void run_parallel(int size, int array[size], int size_threads, void* (*f)(void *), void* extra) {
  parallel_argument *arguments[size_threads];
  pthread_t threads[size_threads];
  int count = 0;
  while (count < size) {
    for(int i=0; i<size_threads; i++) {
      arguments[i] = create_argument(count, &array[i], extra);
      pthread_create(&threads[i], NULL, f, (void *) arguments[i]);
      count++;matriz

      if(count >= size)
        break;
    }

    for(int i=0; i<size_threads; i++) {
      pthread_join(threads[i], NULL);
      free(arguments[i]);
    }
  }
}

int main() {
  int values[2][3] = {
                      {1, 2, 3},
                      {4, 5, 6}
  };
  
  matrix *a = create_matrix(2, 3, values);
  matrix *b = create_matrix(2, 3, values);

  matrix_sum(a, b);
  matrix_multiplicate(a, b);
  matrix_
    print(a);
}
