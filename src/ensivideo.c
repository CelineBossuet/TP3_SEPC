#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <pthread.h>

#include "stream_common.h"
#include "oggstream.h"
#include "synchro.h"

pthread_t affichage;


int main(int argc, char *argv[]) {
    int res;

    if (argc != 2) {
	fprintf(stderr, "Usage: %s FILE", argv[0]);
	exit(EXIT_FAILURE);
    }
    assert(argc == 2);

    //Initialisation des sémaphores
    init_sem();

    // Initialisation de la SDL
    res = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_EVENTS);
    atexit(SDL_Quit);
    assert(res == 0);
    
    // start the two stream readers
    printf("creation des threads \n");
    pthread_t video, son;
    int v = pthread_create(&video, NULL, theoraStreamReader, argv[1]);
    int s = pthread_create(&son, NULL, vorbisStreamReader, argv[1]);
    assert(v==0);
    assert(s==0);
    printf("creation des threads effectuée \n");

    
    // wait audio thread
    printf("attente de l'audio \n");
    pthread_join(son, NULL);

    // 1 seconde de garde pour le son,
    sleep(1);

    // tuer les deux threads videos si ils sont bloqués
    printf("on tue les deux threads video \n");
    pthread_cancel(video);
    pthread_cancel(affichage);


    // attendre les 2 threads videos
    printf("on attend les deux thread video \n");
    pthread_join(video, NULL);
    pthread_join(affichage, NULL);

    
    exit(EXIT_SUCCESS);    
}
