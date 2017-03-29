//
// Created by lingrui on 2017/3/24.
//

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct ThreadLock {

    pthread_mutex_t m;
    pthread_cond_t c;
    unsigned char s;

} ThreadLock;

typedef struct AudioPlayer {

    SLObjectItf engineObj;
    SLEngineItf engineItf;

    SLObjectItf outputMixObj;

    SLObjectItf playerObj;
    SLPlayItf playerItf;
    SLAndroidSimpleBufferQueueItf playerBufferQueueItf;

    int playBufferSize;
    int playChannel;
    int playSampleRate;
    short *playBuffer[2];
    int currPlayBufferIndex;
    int currPlayWriteIndex;
    ThreadLock *playLock;

    void (*writeToPlayer)(struct AudioPlayer *player, short *buffer, int size);

} AudioPlayer;

AudioPlayer *newAudioPlayer(int sampleRate, int numChannel, int bufferSize);

void deleteAudioPlayer(AudioPlayer *player);