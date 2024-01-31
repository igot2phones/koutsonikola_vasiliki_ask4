#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define theseis 10
#define pelateia 15

sem_t spots_available;

void *car_wash(void *customer_id) {
    srand(time(NULL));
    int id = *((int *)customer_id);
    printf("Customer %d arrived at the car wash.\n", id);
    sem_wait(&spots_available); // blepei an yparxei diathesimi thesi sto plyntirio kai an oxi perimenei mexri na eleutherwthei mia thesi 
    printf("Customer %d is getting the car washed.\n", id);// an yparxei thesi to plynei
    int wash_time = 1 + rand() % 5 ; // xronos plyntiriasmou
    sleep(wash_time); // xronos plyntiriasmou
    printf("Customer %d finished washing the car in %d seconds.\n", id, wash_time);
    sem_post(&spots_available); // eleutherwnei thn thesi 
    pthread_exit(NULL); // termatismos thread
}

int main() {
    pthread_t customers[pelateia];
    int customer_ids[pelateia];
    sem_init(&spots_available, 0, theseis); // arxikopoihsh tou semaforou me timh theseis
    for (int i = 0; i < pelateia; i++) {
        customer_ids[i] = i + 1;
        pthread_create(&customers[i], NULL, car_wash, (void *)&customer_ids[i]);
    }
    // anamonh gia termatismo threads
    for (int i = 0; i < pelateia; i++) {
        pthread_join(customers[i], NULL);
    }

    // antio o simeoforos
    sem_destroy(&spots_available);

    return 0;
}