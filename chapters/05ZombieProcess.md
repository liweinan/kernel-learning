# 『阿男的Linux内核世界』＊05 僵尸进程＊

这篇文章里面，阿男仍然是接续上一篇的内容，继续学习Process。

上一篇文章里我们使用了一段代码来展示了parent process和child process的关系：我们看到了parent process退出后，还在运行的child process的parent变成了pid为1的process。

那如果是child process先退出，会是什么情况呢？我们这篇文章就讨论这种情况。

这次我们要用到的代码是这样的：

```c
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

```
