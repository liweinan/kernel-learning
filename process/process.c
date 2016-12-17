/* exit() */
#include <stdlib.h>

#include <sys/types.h>

/*
 * fork()
 */
#include <unistd.h>

/* 
 * stderr
 * stdout
 * fprintf
 */
#include <stdio.h>

void err_sys(const char* fmt, ...);

int main () {
	pid_t pid;
	if ((pid = fork()) < 0) { /* 创建child process */
		err_sys("fork error");
	} else if (pid == 0) { /* 这里是child process */
		puts("child process here."); 
		puts("child process will sleep for 10 seconds...");
		sleep(10);
	} else { /* 这里是parent process */
		puts("parent process here.");
		printf("parent get the child pid: %d\n", pid);
	}
	
	/* 父亲孩子共用的代码部分 */
	sleep(3);
	
	if (pid == 0) { /* child process */
		puts("child process exit.");
	} else { /* parent process */
		puts("parent process exit.");
	}
	
	return 0;
}

void err_sys(const char* fmt, ...) {
	va_list ap;
	fprintf(stderr, fmt, ap);
	exit(1);
}