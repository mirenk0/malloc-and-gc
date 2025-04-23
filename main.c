#include "malloc.h"
#include <stdio.h>
#include <string.h>

int main() {
  printf("Custom malloc test\n");

  int *a = malloc(sizeof(int));
  if (!a)
    return 1;
  *a = 42;
  printf("a = %d\n", *a);

  char *str = calloc(10, sizeof(char));
  if (!str)
    return 1;
  strcpy(str, "hi");
  printf("str = %s\n", str);

  str = realloc(str, 20);
  if (!str)
    return 1;
  strcat(str, " there");
  printf("str = %s\n", str);

  free(a);
  free(str);

  printf("Memory freed successfully\n");
  return 0;
}
