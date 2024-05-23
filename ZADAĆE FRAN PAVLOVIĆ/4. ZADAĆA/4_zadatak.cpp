#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define N 5
#define M 15
#define SEMNAME_MJESTA "/mjesta"

void posjetitelj(int i) {
    sem_t* mjesta = sem_open(SEMNAME_MJESTA, 0);
    if (mjesta == SEM_FAILED) {
        perror("sem_open posjetitelj");
        exit(1);
    }

    while (1) {
        sem_wait(mjesta);
        printf("Proces %d je usao u vrtuljak\n", i);
        sleep(3);
    }

    sem_close(mjesta);
}

void vrtuljak() {
    int br = 0;
    sem_t* mjesta = sem_open(SEMNAME_MJESTA, 0);
    if (mjesta == SEM_FAILED) {
        perror("sem_open vrtuljak");
        exit(1);
    }

    int value;
    while (1) {
        do {
            sem_getvalue(mjesta, &value);
            printf("Broj slobodnih mjesta: %d\n", value);
            sleep(1);
        } while (value != 0);

        sleep(1);
        printf("Vrtuljak se vrti %d\n", br++);
        sleep(3);
        printf("Vrtuljak se zaustavio\n");

        for (int i = 0; i < N; i++) {
            sem_post(mjesta);
            printf("Sjedalo br %d na vrtuljku se ispraznilo!\n", i + 1);
        }
    }

    sem_close(mjesta);
}

int main() {
    sem_t* mjesta = sem_open(SEMNAME_MJESTA, O_CREAT, 0644, N);
    if (mjesta == SEM_FAILED) {
        perror("sem_open main");
        exit(1);
    }

    if (fork() == 0) {
        vrtuljak();
        exit(0);
    }

    sleep(1);

    for (int i = 0; i < M; i++) {
        if (fork() == 0) {
            posjetitelj(i);
            exit(0);
        }
    }

    for (int i = 0; i < M + 1; i++) {
        wait(NULL);
    }

    sem_close(mjesta);
    sem_unlink(SEMNAME_MJESTA);

    return 0;
}

