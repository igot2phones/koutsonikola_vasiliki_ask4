#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


sem_t storage_access;
sem_t storage_waiting;


void *contestant(void *id) {
    for (1;1;1){
    srand(time(NULL));
    int contestant_id = *((int *)id);

    printf("Contestant %d entered the kitchen and cooking.\n", contestant_id);
    // Ετοιμασία συνταγής για τυχαίο χρονικό διάστημα
    int cooking_time = 1 + rand() % 10 ;
    sleep(cooking_time);

    printf("Contestant %d finished cooking and wants to get in the food storage.\n", contestant_id);
    int v1,v2;
    sem_getvalue(&storage_access, &v1); //omg
    sem_getvalue(&storage_waiting, &v2);
    if (v2 != 0 || v1 != 0){
    sem_wait(&storage_waiting);

    printf("Contestant %d is waiting to enter the storage in the waiting room.\n", contestant_id);
    sem_wait(&storage_access);// an exei fugei o alos proxora sto storage room kai...
    printf("Contestant %d is leaving the waiting room.\n", contestant_id);
    sem_post(&storage_waiting);//...kanei post ton storage waiting gia na mpei o epomenos

    // Είσοδος στην αποθήκη
    printf("Contestant %d entered the storage.\n", contestant_id);

        // Παίρνει τα υλικά
        printf("Contestant %d is getting additional ingredients from the storage.\n", contestant_id);
        sleep(5); // Ο χρόνος παραμονής στην αποθήκη
        printf("Contestant %d is returning to the kitchen.\n", contestant_id);
        sem_post(&storage_access);
        } else {
            printf("Contestant %d is leaving and going back in the kitchen, no space.\n", contestant_id);
        }
    }
}

int main(int argc, char *argv[]) {
    int diagontizomenoi = atoi(argv[1]);
    pthread_t contestants[diagontizomenoi];
    int contestant_ids[diagontizomenoi];

    // Αρχικοποίηση σημαφόρων
    sem_init(&storage_access, 0, 1);
    sem_init(&storage_waiting, 0, 1);

    // Δημιουργία threads για τους διαγωνιζόμενους
    for (int i = 0; i < diagontizomenoi; i++) {
        contestant_ids[i] = i + 1;
        pthread_create(&contestants[i], NULL, contestant, (void *)&contestant_ids[i]);
    }

    // Αναμονή για την ολοκλήρωση των threads
    for (int i = 0; i < diagontizomenoi; i++) {
        pthread_join(contestants[i], NULL);
    }

    // Καταστροφή σημαφόρων
    sem_destroy(&storage_access);
    sem_destroy(&storage_waiting);

    return 0;
}
