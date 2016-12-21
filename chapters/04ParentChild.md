# 『阿男的Linux内核世界』＊04 Parent Process与Child Process＊

这篇文章我们接着学习Process。

上篇文章里我们学习了fork()函数，并且写了sample code来使用fork()函数创建一个child process：

```bash
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
```

这篇文章里面，我们就仔细地分析一下这段代码的执行过程。

首先把它编译成可执行文件：

```bash
[weli@fedora process]$ cc process.c -o process
```

编译完成后，我们把代码执行起来：

```bash
[weli@fedora process]$ ./process 
parent process here.
parent get the child pid: 22418
child process here.
child process will sleep for 10 seconds...
```

然后马上用_ps_命令来查看process的运行情况：

```bash
[weli@fedora process]$ ps -ef | grep process | grep -v grep
weli     22417 18153  0 21:00 pts/2    00:00:00 ./process
weli     22418 22417  0 21:00 pts/2    00:00:00 ./process
```

如上所示，我们可以用ps看到parent和child两个process。并且从日志和ps的输出可以看出来，22418 process，它的parent是id为22417的process。

因为我们写的代码是让parent process先退出，然后child要多等10秒再推出，因此我们可以看看parent process退出后，child process的状态是什么样子的：

```bash
[weli@fedora process]$ ps -ef | grep process | grep -v grep
weli     22418     1  0 21:00 pts/2    00:00:00 ./process
```

从上面的输出可以看到，如果我们等到parent process退出，而child process没有退出的时候，此时ps用查看，就可以正在运行的process只有一个了。

而且我们看此时这个在运行的child process，它的ppid变成了1。

也就是说因为，parent process已经退出不存在了，所以1号进程称为了child process的parent，这是内核对process继承关系的管理办法。

最后我们看一下，等到两个进程都执行完后的状态：

```bash
[weli@fedora process]$ ./process 
parent process here.
parent get the child pid: 22618
child process here.
child process will sleep for 10 seconds...
parent process exit.
[weli@fedora process]$ child process exit.
```

```bash
[weli@fedora process]$ ps -ef | grep process | grep -v grep
```

可以看到，此时ps已经看不到parent和child两个process了，因为它们都执行完成了。

## 小结

我们在这篇文章里看到了parent和child process的执行过程。并且我们知道了，如果parent process先退出，那么还在执行的child process的parent process就变成了1号process。

下篇文章中，阿男给大家讲反过来的情况，就是child process先结束的情况。

本篇就讲到这里！