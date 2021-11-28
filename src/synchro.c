#include "synchro.h"
#include "ensitheora.h"
#include <semaphore.h>
#include <pthread.h>


extern bool fini;

/* les variables pour la synchro, ici */

pthread_mutex_t mut_hashmap =PTHREAD_MUTEX_INITIALIZER;
sem_t taille;
sem_t texture;
int compteur = 0;
pthread_mutex_t matmut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conso = PTHREAD_COND_INITIALIZER;
pthread_cond_t depo = PTHREAD_COND_INITIALIZER;

/* l'implantation des fonctions de synchro ici */
void envoiTailleFenetre(th_ycbcr_buffer buffer) {
    windowsx = buffer[0].width;
    windowsy = buffer[0].height;
    sem_post(&taille);
}

void attendreTailleFenetre() {
    sem_wait(&taille);
}

void signalerFenetreEtTexturePrete() {
    sem_post(&texture);
}

void attendreFenetreTexture() {
    sem_wait(&texture);
}

void debutConsommerTexture() {
    pthread_mutex_lock(&matmut);
    while(compteur == 0){
        pthread_cond_wait(&conso, &matmut);
    }
    compteur--;
}

void finConsommerTexture() {
    pthread_mutex_unlock(&matmut);
    pthread_cond_broadcast(&conso); //libération de tous les consos possibles
}


void debutDeposerTexture() {
    pthread_mutex_lock(&matmut);
    while (compteur==NBTEX)
    {
        pthread_cond_wait(&depo, &matmut);
    }
    compteur++;   
}

void finDeposerTexture() {
    pthread_mutex_unlock(&matmut);
    pthread_cond_broadcast(&depo);
}

void init_sem(){
    sem_init(&taille, PTHREAD_PROCESS_SHARED, 0);
    sem_init(&texture, PTHREAD_PROCESS_SHARED, 0);
}