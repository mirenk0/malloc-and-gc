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

  printf("About to realloc\n");
  char *newstr = realloc(str, 20);
  if (!newstr) {
    printf("realloc failed\n");
    return 1;
  }
  printf("Realloc successful\n");

  strcat(newstr, " there");
  printf("str = %s\n", newstr);

  free(a);
  free(newstr);

  printf("Memory freed successfully\n");
  return 0;
}
