#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_STOL 10

int n; 
int m; 
int *stolovi; 
int *ULAZ;
int *BROJ;

void uði_u_kritièni_odsjeèak(int i) {
    ULAZ[i] = 1;
    BROJ[i] = 1 + (BROJ[0] > BROJ[1] ? BROJ[0] : BROJ[1]);
    ULAZ[i] = 0;
    for (int j = 0; j < n; j++) {
        while (ULAZ[j] != 0) {}
        while (BROJ[j] != 0 && (BROJ[j] < BROJ[i] || (BROJ[j] == BROJ[i] && j < i))) {}
    }
}

void izaði_iz_kritiènog_odsjeèka(int i) {
    BROJ[i] = 0;
}

void *rezervacija(void *arg) {
    int *id = (int *)arg;
    int stol;
    while (1) {
        sleep(1);
        stol = rand() % m;
        printf("Dretva %d: odabirem stol %d\n", *id, stol);
        uði_u_kritièni_odsjeèak(*id);
        if (stolovi[stol] == 0) {
            stolovi[stol] = *id;
            printf("Dretva %d: rezerviram stol %d, stanje: ", *id, stol);
            for (int i = 0; i < m; i++) {
                if (stolovi[i] == 0) {
                    printf("-");
                } else {
                    printf("%d", stolovi[i]);
                }
            }
            printf("\n");
        } else {
            printf("Dretva %d: neuspjela rezervacija stola %d, stanje: ", *id, stol);
            for (int i = 0; i < m; i++) {
                if (stolovi[i] == 0) {
                    printf("-");
                } else {
                    printf("%d", stolovi[i]);
                }
            }
            printf("\n");
        }
        izaði_iz_kritiènog_odsjeèka(*id);
        
        int slobodni_stolovi = 0;
        for (int i = 0; i < m; i++) {
            if (stolovi[i] == 0) {
                slobodni_stolovi = 1;
                break;
            }
        }
        if (!slobodni_stolovi) {
            break;
        }
    }
    pthread_exit(NULL);
}

int main() {
    printf("Unesite broj dretvi: ");
    scanf("%d", &n);
    printf("Unesite broj stolova: ");
    scanf("%d", &m);

    if (n <= 0 || m <= 0 || m > MAX_STOL) {
        printf("Neispravni argumenti\n");
        return 1;
    }

    pthread_t threads[n];
    stolovi = (int *)calloc(m, sizeof(int));
    ULAZ = (int *)calloc(n, sizeof(int));
    BROJ = (int *)calloc(n, sizeof(int));

    srand(time(NULL));

    int thread_ids[n];
    for (int i = 0; i < n; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, rezervacija, &thread_ids[i]);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    free(stolovi);
    free(ULAZ);
    free(BROJ);

    return 0;
}

