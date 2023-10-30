#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 5        // 缓冲区大小
#define MAX_PRODUCTS 10      // 最大产品数量
#define MAX_STRING_LENGTH 20 // 生成的字符串最大长度

char *buffer[BUFFER_SIZE]; // 共享缓冲区
int count = 0;             // 缓冲区中的产品数量
int in = 0;                // 生产者插入位置
int out = 0;               // 消费者取出位置

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
  for (int i = 0; i < MAX_PRODUCTS; i++) {
    pthread_mutex_lock(&mutex);

    while (count == BUFFER_SIZE) {
      pthread_cond_wait(&empty, &mutex);
    }

    char *new_string = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
    snprintf(new_string, MAX_STRING_LENGTH, "Product %d", i + 1);

    buffer[in] = new_string;
    in = (in + 1) % BUFFER_SIZE;
    count++;

    printf("Produced: %s\n", new_string);

    pthread_cond_signal(&full);
    pthread_mutex_unlock(&mutex);
  }

  return NULL;
}

void *consumer(void *arg) {
  for (int i = 0; i < MAX_PRODUCTS; i++) {
    pthread_mutex_lock(&mutex);

    while (count == 0) {
      pthread_cond_wait(&full, &mutex);
    }

    char *item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    count--;

    printf("Consumed: %s\n", item);

    free(item); // 释放内存
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutex);
  }

  return NULL;
}

int main() {
  pthread_t producer_thread, consumer_thread;

  pthread_create(&producer_thread, NULL, producer, NULL);
  pthread_create(&consumer_thread, NULL, consumer, NULL);

  pthread_join(producer_thread, NULL);
  pthread_join(consumer_thread, NULL);

  return 0;
}
