#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void error_handle(char *msg){
  printf("%s",msg);
  exit(1);
}
