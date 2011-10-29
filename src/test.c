#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void assertEquals(char *test, int a, int b){
     if(a != b){
          printf("FALHA\n=> %s: Esperava %d, mas recebeu %d", test, a, b);
          printf("\n<ENTER>");
          getchar();
     }
     else{
          printf("%s OK!", test);
          printf("\n<ENTER>");
          getchar();
     }
}
