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

可以看到，我们这次所使用的代码其实和之前使用的差不多，而最大的区别就是，这次的代码是child process先退出，然后parent process要sleep 60秒才退出。

因此我们可以看看再这样的情况下，和child process先退出会有什么不同。首先我们编译这段代码：

```bash
[weli@fedora process]$ cc zombie.c -o zombie
```

然后我们运行编译后的代码：

```bash
[weli@fedora process]$ ./zombie 
parent process goes to sleep...
```

此时我们使用_ps_命令查看这个程序的两个process的运行状况：

```
[weli@fedora process]$ ps -ef | grep zombie
weli      9203  5502  0 19:53 pts/0    00:00:00 ./zombie
weli      9204  9203  0 19:53 pts/0    00:00:00 [zombie] <defunct>
```

此时通过pid和ppid的关系可以看到，parent process的id是9203，child process的id是9204。此时parent process在正常运行的状态，而child process的状态是_<defunct>_。

也就是说，child process虽然已经退出了，但是它并没有完全消失，而是还留下了一个_defunct_状态的"壳"，我们管这种状态的进程叫做zombie process，也就是僵尸进程。

为什么叫它zombie process？因为这个process的资源已经已经被释放干净了，只是个没有生命力的空壳而已。我们可以使用lsof命令来验证这点：

```bash
[weli@fedora process]$ lsof -p 9204
[weli@fedora process]$ 
```

如上所示，我们使用lsof命令来查看这个zombie process所使用的文件资源，因为只要是在运行的process，都会打开一些文件的，至少这个process自身所对应的程序文件是一定要打开的。

我们可以对比一下使用_lsof_命令查看正在运行的parent process的情况：

```bash
[weli@fedora process]$ lsof -p 9203
COMMAND   PID USER   FD   TYPE DEVICE SIZE/OFF   NODE NAME
zombie  9203 weli  cwd    DIR  253,2     4096 523330 /home/weli/projs/kernel-learning/process
zombie  9203 weli  rtd    DIR  253,0     4096      2 /
zombie  9203 weli  txt    REG  253,2     8912 523335 /home/weli/projs/kernel-learning/process/zombie
zombie  9203 weli  mem    REG  253,0  2093616 795514 /usr/lib64/libc-2.23.so
zombie  9203 weli  mem    REG  253,0   172080 795403 /usr/lib64/ld-2.23.so
zombie  9203 weli    0u   CHR  136,0      0t0      3 /dev/pts/0
zombie  9203 weli    1u   CHR  136,0      0t0      3 /dev/pts/0
zombie  9203 weli    2u   CHR  136,0      0t0      3 /dev/pts/0
```

从上面的输出可以看到，正在运行的parent process至少在使用着自己所对应的程序文件_/home/weli/projs/kernel-learning/process/zombie_，而实际上已经退出的child prorcess就是一个空壳，实实在在就是一个_zombie_！





