#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_PRODUCTS 10 // 最大产品数量

int buffer[MAX_PRODUCTS] = {0}; // 共享缓冲区
int count = 0;                  // 产品数量
int in = 0;                     // 生产者插入位置
int out = 0;                    // 消费者取出位置

sem_t empty; // 定义同步信号量empty
sem_t full;  // 定义同步信号量full
sem_t mutex; // 定义互斥信号量mutex

void *producer() { // 生产者
  while (1) {
    sem_wait(&empty); // empty的P操作
    sem_wait(&mutex); // mutex的P操作

    buffer[in] = count;
    printf("producer: %d\n", in);
    in = (in + 1) % MAX_PRODUCTS;
    count++;

    sem_post(&mutex); // mutex的V操作
    sem_post(&full);  // full的V操作
  }
}

void *consumer() { // 消费者
  while (1) {
    sem_wait(&full);  // full的P操作
    sem_wait(&mutex); // mutex的P操作

    int item = buffer[out];
    printf("        consumer: %d\n", out);
    out = (out + 1) % MAX_PRODUCTS;
    count--;

    sem_post(&mutex); // mutex的V操作
    sem_post(&empty); // empty的V操作
  }
}

int main() {
  pthread_t id_producer;
  pthread_t id_consumer;
  int ret;
  sem_init(&empty, 0, MAX_PRODUCTS); // 设置empty到初值为10
  sem_init(&full, 0, 0);             // 设置full到初值为0
  sem_init(&mutex, 0, MAX_PRODUCTS); // 设置mutex到初值为10
  // pthread_mutex_init(&mutex, NULL); // 初始化互斥量
  ret = pthread_create(&id_producer, NULL, producer, NULL); // 创建生产者线程
  ret = pthread_create(&id_consumer, NULL, consumer, NULL); // 创建消费者线程
  pthread_join(id_producer, NULL); // 等待生产者线程结束
  pthread_join(id_consumer, NULL); // 等待消费者线程结束
  sem_destroy(&empty);             // 删除信号量
  sem_destroy(&full);
  sem_destroy(&mutex);
  return 0;
}
