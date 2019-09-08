#include<stdio.h>

int ASSERT(int check, char* name_func) {
  if(check) {
    printf("OK - %s\n", name_func);
    return 1;
  } else {
    printf("NOK - %s\n", name_func);
    return 0;
  }
}
