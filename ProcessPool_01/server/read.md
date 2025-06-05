# `workerArr[i].sockfd` 使用解析

你提出了一个很好的问题。让我来解释一下：

1. `workerArr[i]` 确实是保存子进程信息的结构体，但 `sockfd` 字段保存的是**父进程**用来与对应子进程通信的文件描述符。

2. 通信机制如下：
```mermaid
graph LR
    A[父进程] --"通过fds[0]发送"--> B[socketpair通道]
    B --"通过fds[1]接收"--> C[子进程]
```

3. 代码中的分配：
- 父进程保留 `fds[0]`
- 子进程保留 `fds[1]`
- `workerArr[i].sockfd = fds[0]` 记录父进程与第i个子进程通信的描述符

4. 完整的文件描述符分配：

```c
if (pid == 0) {  // 子进程
    // 子进程使用fds[1]
    recvfd(fds[1], &netfd);
    // ...
} else {  // 父进程
    close(fds[1]);  // 关闭父进程不需要的一端
    workerArr[i].sockfd = fds[0];  // 保存与子进程通信的fd
}
```

这样设计的原因：
1. 父进程需要知道如何与每个子进程通信
2. `workerArr` 数组在父进程中维护
3. 每个子进程只需要知道自己的通信fd(`fds[1]`)
4. 父进程通过 `workerArr[i].sockfd` 可以找到对应的子进程进行通信

所以这个设计是合理的：`workerArr[i].sockfd` 保存的是父进程这一端的文件描述符，用于与对应的子进程通信。