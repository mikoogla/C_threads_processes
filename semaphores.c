#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#define SEM_NAME "/sem"

int main(int argc, char *argv[]) {

    const char *name = getenv("SO2");
    if(name == NULL){name = "";}

    if(strcmp(name, "NEW") != 0){
        sem_t *sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, S_IRWXU, 0);
        if (errno == 0) {
            getchar();
            sem_close(sem);
            sem_unlink(SEM_NAME);
        } else {
            puts("proces jest już uruchomiony");
            return 0;
        }
    }else{
        //puts("SO2=NEW:");
        sem_t *sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, S_IRWXU, 0);

        if(errno != 0){
            //puts("Semaphore already exists");
            sem = sem_open(SEM_NAME, O_CREAT, S_IRWXU, 1);
            sem_post(sem);
            sleep(1);
            sem_wait(sem);
        }else{
            //puts("New semaphore created");
            sem_wait(sem);
        }
    }
    return 0;
}
