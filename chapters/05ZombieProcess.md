# 『阿男的Linux内核世界』＊05 僵尸进程＊

这篇文章里面，阿男仍然是接续上一篇的内容，继续学习Process。

上一篇文章里我们使用了一段代码来展示了parent process和child process的关系：我们看到了parent process退出后，还在运行的child process的parent变成了pid为1的process。

那如果是child process先退出，会是什么情况呢？我们这篇文章就讨论这种情况。

这次我们要用到的代码是这样的：

