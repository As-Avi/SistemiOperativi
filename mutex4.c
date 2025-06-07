#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
// Function prototypes
void A() { printf("A is executed\n"); }
void B() { printf("B is executed\n"); }
void C() { printf("C is executed\n"); }
void D() { printf("D is executed\n"); }
sem_t s1, s2;
void* t_A(void* arg){
A();
sem_post(&s1);
sem_post(&s1);
return NULL;
}
void* t_B(void* arg){
sem_wait(&s1);
B();
sem_post(&s2);
return NULL;
}
void* t_C(void* arg){
sem_wait(&s1);
C();
sem_post(&s2);
return NULL;
}
void* t_D(void* arg){
sem_wait(&s2);
sem_wait(&s2);
D();
return NULL;
}
int main() {
    pthread_t thread_A, thread_B, thread_C, thread_D;
    // Initialize semaphores
    sem_init(&s1, 0, 0);
    sem_init(&s2, 0, 0);
    // Create threads
    pthread_create(&thread_A, NULL, t_A, NULL);
    pthread_create(&thread_B, NULL, t_B, NULL);
    pthread_create(&thread_C, NULL, t_C, NULL);
    pthread_create(&thread_D, NULL, t_D, NULL);
    // Wait for threads to complete
    pthread_join(thread_A, NULL);
    pthread_join(thread_B, NULL);
    pthread_join(thread_C, NULL);
    pthread_join(thread_D, NULL);
    // Destroy semaphores
    sem_destroy(&s1);
    sem_destroy(&s2);
    return 0;
}