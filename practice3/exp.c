#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

void main() {
  pid_t pid;
  if((pid = fork()) == 0){
    printf("1");
  }
  else {
    printf("2");
  }
  printf("3");
}
