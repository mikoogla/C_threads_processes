#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#define MAX_THREADS 100
#define MIN_THREADS 3

#define exit(q) {printf("Nieprawidlowe argumenty, kod: %d\n", q); return q;}

pthread_mutex_t mutex;
int id = 0, id_copy = 0, Num = 0;

enum order{increasing, decreasing};
enum order order;

void* thread(){
    int local_id; id_copy = id;
    pthread_mutex_lock(&mutex);
    printf("%d ", id);
    local_id = id;
    id++;
    pthread_mutex_unlock(&mutex);

    if(id >= Num){
        printf("\n");
        id--;
    }

    sleep(2);

    if(order == decreasing){
        up:
        if(local_id == id){
            goto exit;
        }else{
            goto up;
        }
        exit:
        printf("%d ", local_id);

        pthread_mutex_lock(&mutex);
        id--;
        pthread_mutex_unlock(&mutex);
    }

    else if(order == increasing){
        up_2:
        if(local_id == id-id_copy || id_copy<0){
            goto exit_2;
        }else{
            goto up_2;
        }
        exit_2:
        printf("%d ", local_id);

        pthread_mutex_lock(&mutex);
        id_copy--;
        pthread_mutex_unlock(&mutex);
    }
    else{
        printf("Order fail\n");
    }

    return NULL;
}
int Num_copy_validate(char *argv){
    int number=0;
    char N[4] = {'\0'};
    if(strlen(argv) > 3) return -1;
    strcpy(N, argv);
    for(int i=0; i< strlen(N); i++){
        if(isdigit(N[i]) == 0) return -1;
    }
    number = atoi(N);
    if(number < MIN_THREADS || number > MAX_THREADS) number = -1;
    return number;
}

int main(int argc, char *argv[])
{
    if(argv[1] == NULL) exit(1);
    if(argv[2] == NULL) exit(2);

    Num = Num_copy_validate(argv[1]);
    if(Num == -1) exit(3);

    if(strcmp(argv[2], "inc") == 0) order = increasing;
    else if(strcmp(argv[2], "dec") == 0) order = decreasing;
    else exit(4);

    if(order == 0) printf("order: increasing\n");
    if(order == 1) printf("order: decreasing\n");

    pthread_mutex_init(&mutex, NULL);
    pthread_t t[Num];
    for(int i=0; i<Num; i++){
        pthread_create(&(t[i]), NULL, thread, NULL);
    }
    for(int i=0; i<Num; i++){
        pthread_join(t[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    printf("\n");
    return 0;
}