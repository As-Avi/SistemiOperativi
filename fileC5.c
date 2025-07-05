#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct {
    char car_name[256];
    int status;
} Car;

Car *cars;

int count_cars(){
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

    int car_count = count_cars();
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

void view(){
    int lines = count_cars();
    for(int i = 0; i < lines; i++){
        printf("Car: %s, status: %s\n", cars[i].car_name, cars[i].status ? "busy" : "free");
    }
}

int find(const char *car_name) {
    for (int i = 0; i < count_cars(); i++) {
        if (strcmp(cars[i].car_name, car_name) == 0) {
            return i;
        }
    }
    return -1;
}

void *noleggio_thread(void *arg) {
    char *car_name = (char *)arg;
    int index = find(car_name);
    if (index == -1) {
        printf("Cannot find car %s\n", car_name);
        return NULL;
    }

    sem_t *semaphore;
    char sem_name[256];
    snprintf(sem_name, sizeof(sem_name), "%s_semaphore", cars[index].car_name);
    semaphore = sem_open(sem_name, 0);
    if (semaphore == SEM_FAILED) {
        perror("Failed to open semaphore");
        exit(1);
    }
    sem_wait(semaphore);
    if (cars[index].status == 0) {
        cars[index].status = 1;
        printf("Car %s locked successfully.\n", cars[index].car_name);
    } else {
        printf("Car %s is already locked.\n", cars[index].car_name);
    }
    sem_post(semaphore);
    sem_close(semaphore);
    exit(0);
}

void *release_thread(void *arg) {
    char *car_name = (char *)arg;
    int index = find(car_name);
    if (index == -1) {
        printf("Car not found: %s\n", car_name);
        return NULL;
    }

    sem_t *semaphore;
    char sem_name[256];
    snprintf(sem_name, sizeof(sem_name), "%s_semaphore", cars[index].car_name);
    semaphore = sem_open(sem_name, 0);
    if (semaphore == SEM_FAILED) {
        perror("Failed to open semaphore");
        exit(1);
    }
    sem_wait(semaphore);
    if (cars[index].status == 1) {
        cars[index].status = 0;
        printf("Car %s released successfully.\n", cars[index].car_name);
    } else {
        printf("Car %s is not locked.\n", cars[index].car_name);
    }
    sem_post(semaphore);
    sem_close(semaphore);
    exit(0);
}


int main(int argc, char *argv[]){
    char command[100];
    char car_name[256];

    int num_cars = count_cars();
    cars = malloc(sizeof(Car) * num_cars);

    if (cars == NULL) {
        perror("Failed to allocate memory");
        return EXIT_FAILURE;
    }
    catalogo();

    while(1){
        printf("Command: ");
        scanf("%s", command);

        if(strcmp(command, "view") == 0){
            view();
        } else if (strncmp(command, "lock", 4) == 0) {
            strncpy(car_name, command + 4, sizeof(car_name) - 1);
            car_name[sizeof(car_name) - 1] = '\0';

            pthread_t thread;
            if (pthread_create(&thread, NULL, noleggio_thread, (void*)car_name) != 0) {
                perror("Failed to create thread");
                free(cars);
                return EXIT_FAILURE;
            }
            pthread_join(thread, NULL);
        } else if (strncmp(command, "release", 7) == 0) {
            strncpy(car_name, command + 7, sizeof(car_name) - 1);
            car_name[sizeof(car_name) - 1] = '\0';

            pthread_t thread;
            if (pthread_create(&thread, NULL, release_thread, (void*)car_name) != 0) {
                perror("Failed to create thread");
                free(cars);
                return EXIT_FAILURE;
            }
            pthread_join(thread, NULL);
        }
        else if (strcmp(command, "quit") == 0) {
            free(cars);
            exit(0);
        }
        else {
            printf("Unknown Command\n");
        }
    }
    free(cars);
    return 0;
}