---
title: "阿男的Linux内核世界"
author: 阿男
date: Dec 22, 2016
mainfont: "FZBaoSong-Z04S"
CJKmainfont: "FZBaoSong-Z04S"
CJKoptions:
  - BoldFont=FZMeiHei-M07S
  - ItalicFont=STFangsong
  - Scale=1.0
---

# 开篇

按照和大家的约定，阿男要开始给大家开这个Linux内核专栏了。开这个专栏是非常开心的
事情，因为操作系统也是阿男两个最喜欢的领域之一（另一个是编译原理）。

这个专栏的方向无比清晰：让大家知道操作系统是怎么工作的。

阿男觉得学习操作系统和编译原理是最超值的，因为这种学习会彻底改变你在从事编程这件
事的时候，你眼前的世界的样子。

首先，很多表面上复杂的东西，其实理解了背后的工作原理，就会发现其实非常简单，正则
表达式就是最好的例子（参见阿男之前写的专栏：『阿男导读』＊正则表达式背后的有限状
态自动机＊）。

然后你能知道问题的边界在哪里，把手头的问题放入一个更为层次丰富，更为透明的世界里
，你会看得更清楚。所以把层次了解的丰富一些，更方便自己对手头的问题作出判断。

最后，深入的学习当然还有个好处就是，你手头的工具变得丰富了。当你解决一个问题的时
候，你可以更为合理地选择更多层面的工具。

阿男感受过这种体验，很棒。相信大家也会体验到。那么我们接下来就一起学习操作系统吧
！这个专栏可能会延续很久，没关系，我们有的是时间学习。

# 进程

进程（process）可能是Linux世界下最有生命力的存在了。所谓process就是活的程序，正
在运行中的代码。

计算机必须要有软件，人类才可能使用它。软件和硬件工程师们合作创造了BIOS（Basic 
Input/Output 
System），BIOS本身是软件，是程序，但是是固化在芯片里的代码（现代的BIOS也可以编程
，刷新，升级），随着计算机启动最先加载这些代码。

BIOS负责将计算机里面的各个硬件加载，运转起来。

科学家们给这一过程非常形象地起了一个名字：Bootstrap。

什么是Bootstrap？这个词初见于童话故事 
《吹牛大王历险记》：在书中，敏豪森男爵吹牛说，自己骑着马掉进了沼泽，但他天生神力
，两腿夹着马，双手抓着自己的小辫子，把自已和马一起从沼泽里拉到了半空！[^1]

[^1]: The Adventures of Baron 
Munchausen，《吹牛大王历险记》是18世纪德国著名的儿童文学作品。


计算机的启动过程就比较类似于这样，但是为什么后来这个启动过程叫做Bootstrap（自己
的鞋跟），而不是拉着自己的小辫子，这就说不清了。历史上一个说法是，这个德国民间故
事后来传到美国，就被再加工，变成了拉鞋跟了。

当BIOS完成Bootstrap后，会把控制权交接给操作系统。此时Linux操作系统的话，会接收控
制权，然后执行Bootloader，比如GRUB或者LILO，让你选择要运行的Linux系统[^2]。

[^2]: GRUB和LILO是两个比较常用的Boot 
Loader，用在同时安装了多个操作系统的机器上，供用户选择要启动哪个操作系统）

对于没有安装Boot 
Loader的Linux系统，则跳过选择这一步，直接开始Linux系统的加载过程。

Linux系统加载的过程是这样的：首先，一个编号为0的process产生了，它是所有后续proce
ss的parent。这个编号为0的process会产生一个编号为1的process，然后自我毁灭（准确来
讲，是变成swapper）。这个id为1的process就是init，它是所有后续process的parent。Li
nux下面的process，是parent与child这样的层级关系。

一个parent 
process可以克隆自己，拥有有多个children，而每个child只有一个parent。每个process
的编号叫做process id，简称PID。每个process的parent process的编号叫做parent 
process id，缩写是PPID。

我们在比较新的Linux操作系统里面，使用ps命令查看pid为1的process时，可能会和上面说
的的不太一样。比如阿男的Fedora 24系统：

```c
$ ps -fp 1
UID        PID  PPID  C STIME TTY          TIME CMD
root         1     0  0 Dec25 ?        00:00:03 /usr/lib/systemd/systemd 
--switched-root --system --deserialize 23
```

pid为1的process是systemd，不是init，这是因为systemd现在会接管系统。关于systemd，
这篇文章不想展开，如果大家有兴趣，阿男以后可以看专栏。

总之我们知道了，有一个pid是1的process，它是所有后续process的parent，而它的parent
的id是0，启动后就自毁了。

对于很多初学者来讲，这个parent process克隆自己产生children 
processes的过程是很开脑洞的：首先parent 
process会复制自己，创建一个或多个新的children processes，这些children 
processes里面的所有内容都和parent process一摸一样。

像什么？有点类似于"单细胞分裂"。

生成的child里面对应的数据，代码，都是和parent一样的，也就是说在内存里面他们都是
一样的。

这个过程叫做fork，在Linux世界里就是fork()函数做这件事，具体的细节阿男会在下篇文
章中给大家讲。

那么问题来了，如果parent生成的children和自己一样，那岂不是所有的process都执行相
同的program了？

但实际上我们的系统是在执行很多不同的program啊？

这个问题，大家可以先带在身上，阿男后续给大家讲。

感觉这篇文章讲的东西够大家消化一下了，下篇阿男继续带大家学习process。

## 什么是进程

我们在上篇文章里，学习了计算机的启动过程，以及Linux是如何接管系统并且创建各个pro
cess的过程。

我们知道了process就是"活的"程序，这些processes就是实际在运行的代码。

然后我们学习了process是如何产生的：从一个初始的process，复制自身，产生了新的proc
esses。这个process把自己复制的过程叫做fork，我们这篇文章里就具体看一下fork()函数
的使用方法。

首先是fork()的定义：

```man
FORK(2)                                                         Linux 
Programmer's Manual                                                         
FORK(2)

NAME
       fork - create a child process

SYNOPSIS
       #include <unistd.h>

       pid_t fork(void);

DESCRIPTION
       fork()  creates  a  new  process by duplicating the calling process.  
The new process is referred to as the child process.  The calling process is
       referred to as the parent process.
```

文档写的很清楚，fork()函数的作用，就是把调用这个fork()函数的process自身，复制一
份，成为自己的child process，自己就是创建的这个child process的parent process。

这个函数比较有意思的一点是，它会分别针对parent process和child 
process进行返回。如果child创建成功的话，则：

- fork()会对parent process返回child process的id
- fork()会对child process返回0

这样，我们在process中就可以通过判断fork的返回，来确定自己是child还是parent了。

接下来写段代码来实际使用一下fork()函数：

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

这段代码的核心是这里：

```c
pid = fork()
```

从上面这行代码开始，这个process就会创建出一个child 
process来，和自己一模一样，并且会接着往下执行，因此后续的代码都要考虑自己是paren
t process还是child process了：

```c
if (pid == 0)
```

如果pid为0，那么自己就是child process；如果不为0，就是parent process。

所以说此时我们这段代码已经分别在parent和child 
process里面运行着，我们的大脑也要"分裂一下"。接下来继续看代码，这个程序针对child
 process和parent process有不同的代码：

```c
	} else if (pid == 0) { /* 这里是child process */
		puts("child process here."); 
		puts("child process will sleep for 10 seconds...");
		sleep(10);
	} else { /* 这里是parent process */
		puts("parent process here.");
		printf("parent get the child pid: %d\n", pid);
	}
```

如上代码所示，如果是child，则sleep 10秒钟。如果是parent，则打印出child 
process的id（从fork返回得来）。接下来是parent和child都会执行的代码：

```c
sleep(3);
```

sleep 3 秒，这样我们可以保证parent process和child 
process的执行顺序了：parent和child都要睡眠3秒，但是child 
process还要多睡10秒钟，所以肯定是parent先退出，child后退出。

我们在下篇文章中要用到这个执行顺序来讲解，所以要通过sleep来控制一下，到时候再详
细说。接下来我们编译上面的代码：

```c
cc process.c -o process
```

然后运行代码：

```c
$ ./a.out
parent process here.
parent get the child pid: 85797
child process here.
child process will sleep for 10 seconds...
parent process exit.
```

从上面的输出，我们看到程序的整个执行过程。

## Parent Process与Child Process

这篇文章我们接着学习Process。我们学习了fork()函数，并且写了sample 
code来使用fork()函数创建一个child process：

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

这篇文章里面，我们就仔细地分析一下这段代码的执行过程。首先把它编译成可执行文件：

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

然后马上用`ps`命令来查看process的运行情况：

```bash
[weli@fedora process]$ ps -ef | grep process | grep -v grep
weli     22417 18153  0 21:00 pts/2    00:00:00 ./process
weli     22418 22417  0 21:00 pts/2    00:00:00 ./process
```

如上所示，我们可以用ps看到parent和child两个process。并且从日志和ps的输出可以看出
来，编号为22418的是child 
process，它的parent是id为22417的process。因为我们写的代码是让parent 
process先退出，然后child要多等10秒再推出，因此我们可以看看parent 
process退出后，child process的状态是什么样子的：

```bash
[weli@fedora process]$ ps -ef | grep process | grep -v grep
weli     22418     1  0 21:00 pts/2    00:00:00 ./process
```

从上面的输出可以看到，如果我们等到parent process退出，而child 
process没有退出的时候，此时ps用查看，就可以正在运行的process只有一个了。而且我们
看此时这个在运行的child process，它的ppid变成了1。也就是说，因为parent 
process已经退出不存在了，所以1号进程称为了child 
process的parent，这是内核对process继承关系的管理办法。最后我们看一下，等到两个进
程都执行完后的状态：

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

### 小结

我们在这篇文章里看到了parent和child 
process的执行过程。并且我们知道了，如果parent 
process先退出，那么还在执行的child process的parent 
process就变成了1号process。下篇文章中，阿男给大家讲反过来的情况，就是child 
process先结束的情况。

## 僵尸进程

这篇文章里面，阿男仍然是接续上一篇的内容，带大家继续学习Process。上一篇文章里我
们使用了一段代码来展示了parent process和child process的关系：我们看到了parent 
process退出后，还在运行的child 
process的parent变成了pid为1的process。那如果是child 
process先退出，会是什么情况呢？我们这篇文章就讨论这种情况。这次我们要用到的代码
是这样的：

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

可以看到，我们这次所使用的代码其实和之前使用的差不多，而最大的区别就是，这次的代
码是child process先退出，然后parent process要sleep 
60秒才退出。因此我们可以看看再这样的情况下，和child 
process先退出会有什么不同。首先我们编译这段代码：

```bash
[weli@fedora process]$ cc zombie.c -o zombie
```

然后我们运行编译后的代码：

```bash
[weli@fedora process]$ ./zombie 
parent process goes to sleep...
```

此时我们使用`ps`命令查看这个程序的两个process的运行状况：

```
[weli@fedora process]$ ps -ef | grep zombie
weli      9203  5502  0 19:53 pts/0    00:00:00 ./zombie
weli      9204  9203  0 19:53 pts/0    00:00:00 [zombie] <defunct>
```

此时通过pid和ppid的关系可以看到，parent process的id是9203，child 
process的id是9204。此时parent process在正常运行的状态，而child 
process的状态是`<defunct>`。也就是说，child 
process虽然已经退出了，但是它并没有完全消失，而是还留下了一个defunct状态的"壳"，
我们管这种状态的进程叫做zombie process，也就是僵尸进程。

为什么叫它zombie 
process？因为这个process的资源已经已经被释放干净了，只是个没有生命力的空壳而已。
我们可以使用lsof命令来验证这点：

```bash
[weli@fedora process]$ lsof -p 9204
[weli@fedora process]$ 
```

如上所示，我们使用lsof命令来查看这个zombie 
process所使用的文件资源，因为只要是在运行的process，都会打开一些文件的，至少这个
process自身所对应的程序文件是一定要打开的。我们可以对比一下使用`lsof`命令查看正
在运行的parent process的情况：

```bash
[weli@fedora process]$ lsof -p 9203
COMMAND   PID USER   FD   TYPE DEVICE SIZE/OFF   NODE NAME
zombie  9203 weli  cwd    DIR  253,2     4096 523330 
/home/weli/projs/kernel-learning/process
zombie  9203 weli  rtd    DIR  253,0     4096      2 /
zombie  9203 weli  txt    REG  253,2     8912 523335 
/home/weli/projs/kernel-learning/process/zombie
zombie  9203 weli  mem    REG  253,0  2093616 795514 /usr/lib64/libc-2.23.so
zombie  9203 weli  mem    REG  253,0   172080 795403 /usr/lib64/ld-2.23.so
zombie  9203 weli    0u   CHR  136,0      0t0      3 /dev/pts/0
zombie  9203 weli    1u   CHR  136,0      0t0      3 /dev/pts/0
zombie  9203 weli    2u   CHR  136,0      0t0      3 /dev/pts/0
```

从上面的输出可以看到，正在运行的parent 
process至少在使用着自己所对应的程序文件`/home/weli/projs/kernel-learning/process
/zombie`，而实际上已经退出的child 
prorcess就是一个空壳，实实在在就是一个zombie！那么为什么child 
process执行完成后，内核还要保留这样一个zombie 
process呢？这个问题作为本篇文章的读后思考问题，阿男下篇文章为大家进行讲解。

## 为什么要有zombie process？

阿男在上篇文章里给大家留了一个问题：child process退出后，此时parent 
process还没退出，为什么child process此时要保留一个zombie process？

答案是：这是为了进程间同步而考虑。

Linux提供了一个函数叫做`waitpid`：

```man
WAIT(2)                                                         Linux 
Programmer's Manual                                                         
WAIT(2)

NAME
       wait, waitpid, waitid - wait for process to change state

SYNOPSIS
       #include <sys/types.h>
       #include <sys/wait.h>

       pid_t wait(int *status);
       pid_t waitpid(pid_t pid, int *status, int options);
```

这个函数可以让caller等待某一个进程（通过pid来识别）的状态改变，在没有检查到状态
转变之前，caller会一直hold在那里不往下执行。

我们可以思考一下，因为程序退出也是一种"状态的改变"，因此保留一个zombie 
process，这样如果有一个waitpid的caller process就可以捕获这个状态改变了。

对于退出的child process来讲，如果parent process想调用waipid来等待child 
process的退出，那么我们可以想一下，waitpid在设计实现上肯定需要child 
process在退出后保留一个zombie，而不是直接完全退出，这样waitpid才能检测这种退出状
态的发生。最后，waitpid是会负责回收zombie process的。

那么我们在之前，parent process先退出，child 
process后退出的情况下，为什么没有看到过zombie process？

其实很简单，因为parent process的parent是bash shell，parent process退出后，bash 
shell就处理掉相关的zombie process了。

另一方面，child process因为在parent 
process退出后仍然在运行，我们之前学习了，这种情况下，id为1的process会成为child 
process的parent。因此，child process退出后，id为1的process就也把child 
process的zombie释放了。所以这种情况下并不是没有zombie 
process，而是zombie被回收的过程是瞬间，所以我们在`ps`里面看不到。

只有当我们的parent process没退出，child 
process已经退出的情况下，才会在`ps`的输出里看到child 
process的zombie。因为内核预定由parent process负责回收child 
process的zombie，这样parent process的waitpid才能捕获child process的退出状态。

但如果我们的parent 
process一直不退出，也不回收zombie，就会在`ps`的输出里面看到child 
process的zombie。

这也是为什么服务器性质的代码往往会制造一堆zombie。比如http服务器，加入服务程序长
时间运行，生成了好多children 
process，等children运行完成后又不回收，就会看到进程列表里面好多zombie。这种情况
下只能kill掉主进程，这样所有的children的parent就变成了id为1的process，而id为1的p
rocess会把所有的zombie都释放掉。

在下一篇文章里，阿男给大家讲解waitpid的具体使用方法。
