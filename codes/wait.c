#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  pid_t pid = fork(); // create a child process

  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) { // I am child process
    printf("child process sleep for 3 seconds...\n");
    sleep(3); // sleep for 3 seconds so parent process needs to wait child process to exit
    printf("child process exit.\n");
    
  } else { // I am parent process
    printf("parent process waiting for child to exit...\n"); 
    wait(0); // Wait for child process to exit
    printf("parent process exit\n");
  }

  
}
