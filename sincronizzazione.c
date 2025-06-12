#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

typedef struct {
    char car_name[256];
    int status;
} Car;

Car *cars;
int car_count;

int count_cars() {
    FILE* file = fopen("catalog.txt", "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(1);
    }

    int count = 0;
    char car[256];
    while (fgets(car, sizeof(car), file)) {
        count++;
    }
    fclose(file);
    return count;
}

void catalogo() {
    FILE* file = fopen("catalog.txt", "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(1);
    }

    car_count = count_cars();
    cars = malloc(sizeof(Car) * car_count);
    if (cars == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    char car[256];
    for(int i = 0; i < car_count; i++) { 
        if (fgets(car, sizeof(car), file) != NULL) {
            car[strcspn(car, "\n")] = 0; 
            strncpy(cars[i].car_name, car, sizeof(cars[i].car_name) - 1);
            cars[i].car_name[sizeof(cars[i].car_name) - 1] = '\0';
            cars[i].status = 0;

            char sem_name[256];
            snprintf(sem_name, sizeof(sem_name), "/car_sem_%s", car);
            sem_t *semaphore = sem_open(sem_name, O_CREAT, 0644, 1);
            if (semaphore == SEM_FAILED) {
                perror("Failed to create semaphore");
                fclose(file);
                exit(1);
            }
            sem_close(semaphore);
        }
    }
    fclose(file);
}

void view() {
    for(int i = 0; i < car_count; i++) {
        printf("Car: %s, status: %s\n", 
               cars[i].car_name, 
               cars[i].status ? "busy" : "free");
    }
}

int find(const char *car_name) {
    for (int i = 0; i < car_count; i++) {
        if (strcmp(cars[i].car_name, car_name) == 0) {
            return i;
        }
    }
    return -1;
}

void lock_car(const char *car_name) {
    int idx = find(car_name);
    if (idx == -1) {
        printf("Cannot find car %s\n", car_name);
        return;
    }

    char sem_name[256];
    snprintf(sem_name, sizeof(sem_name), "/car_sem_%s", car_name);
    sem_t *semaphore = sem_open(sem_name, 0);
    if (semaphore == SEM_FAILED) {
        perror("sem_open failed");
        return;
    }

    if (sem_trywait(semaphore) == -1) {
        if (errno == EAGAIN) {
            printf("Error. Car %s already locked\n", car_name);
        } else {
            perror("sem_trywait failed");
        }
    } else {
        cars[idx].status = 1;
        printf("Car: %s is now locked\n", car_name);
    }
    sem_close(semaphore);
}

void release_car(const char *car_name) {
    int idx = find(car_name);
    if (idx == -1) {
        printf("Cannot find car %s\n", car_name);
        return;
    }

    char sem_name[256];
    snprintf(sem_name, sizeof(sem_name), "/car_sem_%s", car_name);
    sem_t *semaphore = sem_open(sem_name, 0);
    if (semaphore == SEM_FAILED) {
        perror("sem_open failed");
        return;
    }

    int val;
    sem_getvalue(semaphore, &val);
    if (val > 0) {
        printf("Error. Car %s already free\n", car_name);
    } else {
        sem_post(semaphore);
        cars[idx].status = 0;
        printf("Car: %s is now free\n", car_name);
    }
    sem_close(semaphore);
}

int main() {
    catalogo();

    char command[256];
    while (1) {
        printf("Command: ");
        if (fgets(command, sizeof(command), stdin) == NULL) break;
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "view") == 0) {
            view();
        } 
        else if (strncmp(command, "lock ", 5) == 0) {
            lock_car(command + 5);
        }
        else if (strncmp(command, "release ", 8) == 0) {
            release_car(command + 8);
        }
        else if (strcmp(command, "quit") == 0) {
            break;
        }
        else {
            printf("Unknown Command\n");
        }
    }

    return 0;
}