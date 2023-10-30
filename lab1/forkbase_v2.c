//
// Created by Haotian Li on 2023-09-17.
//
#include <stdio.h>
#include <unistd.h>

int main() {
  pid_t pid; // 存储进程ID号

  pid = fork(); // 创建子进程

  if (pid == 0) {
    // 这是子进程
    while (1) {
      printf("I am child, PID: %d\n", getpid());
      sleep(3); // 睡眠3秒
    }
  } else if (pid > 0) {
    // 这是父进程
    while (1) {
      printf("I am parent, PID: %d\n", getpid());
      sleep(3); // 睡眠3秒
    }
  } else {
    // fork失败
    fprintf(stderr, "Fork failed.\n");
    return 1;
  }

  return 0;
}
