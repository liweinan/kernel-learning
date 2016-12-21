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
    if ((pid = fork()) < 0) { /* create a process */
        err_sys("fork error");
    } else if (pid == 0) { /* child process */
        /* Child process直接退出 */
        exit (0); 
    } else {
        /* Parent process睡眠60秒，保证child process先退出*/
        fputs("parent process goes to sleep...", stderr);
        sleep (60); 
    }
    return 0;
}

// 错误处理函数
void err_sys(const char* fmt, ...) {
    /* va_list 可变长参数，使用 `man stdarg` 命令学习相关文档。*/
    va_list ap;
    fprintf(stderr, fmt, ap);
    exit(1);
}
