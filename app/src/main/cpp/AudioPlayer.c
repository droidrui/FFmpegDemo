//
// Created by lingrui on 2017/3/24.
//

#include "AudioPlayer.h"

static ThreadLock *createThreadLock(void);

static void waitThreadLock(ThreadLock *lock);

static void notifyThreadLock(ThreadLock *lock);

static void destroyThreadLock(ThreadLock *lock);

static void playCallback(SLAndroidSimpleBufferQueueItf bq, void *context) {
    AudioPlayer *player = (AudioPlayer *) context;
    notifyThreadLock(player->playLock);
}

static void writeToPlayer(AudioPlayer *player, uint8_t *buffer, int size) {
    int index = player->currPlayWriteIndex;
    uint8_t *slBuffer = player->playBuffer[player->currPlayBufferIndex];
    for (int i = 0; i < size; i++) {
        slBuffer[index++] = buffer[i];
        if (index >= player->playBufferSize) {
            waitThreadLock(player->playLock);
            (*player->playerBufferQueueItf)->Enqueue(player->playerBufferQueueItf,
                                                     slBuffer,
                                                     player->playBufferSize * sizeof(uint8_t));
            player->currPlayBufferIndex = (player->currPlayBufferIndex ? 0 : 1);
            index = 0;
            slBuffer = player->playBuffer[player->currPlayBufferIndex];
        }
    }
    player->currPlayWriteIndex = index;
}

AudioPlayer *newAudioPlayer(int sampleRate, int numChannel, int bufferSize) {
    AudioPlayer *player = (AudioPlayer *) calloc(1, sizeof(AudioPlayer));
    slCreateEngine(&(player->engineObj), 0, NULL, 0, NULL, NULL);
    (*player->engineObj)->Realize(player->engineObj, SL_BOOLEAN_FALSE);
    (*player->engineObj)->GetInterface(player->engineObj, SL_IID_ENGINE, &(player->engineItf));

    player->playSampleRate = sampleRate;
    player->playChannel = numChannel;
    player->playBufferSize = bufferSize * numChannel * 2;

    player->playLock = createThreadLock();
    player->playBuffer[0] = (uint8_t *) calloc(player->playBufferSize, sizeof(uint8_t));
    player->playBuffer[1] = (uint8_t *) calloc(player->playBufferSize, sizeof(uint8_t));

    player->currPlayBufferIndex = 0;
    player->currPlayWriteIndex = 0;

    player->writeToPlayer = writeToPlayer;

    notifyThreadLock(player->playLock);

    SLuint32 sr = sampleRate;
    switch (sr) {
        case 8000:
            sr = SL_SAMPLINGRATE_8;
            break;
        case 11025:
            sr = SL_SAMPLINGRATE_11_025;
            break;
        case 16000:
            sr = SL_SAMPLINGRATE_16;
            break;
        case 22050:
            sr = SL_SAMPLINGRATE_22_05;
            break;
        case 24000:
            sr = SL_SAMPLINGRATE_24;
            break;
        case 32000:
            sr = SL_SAMPLINGRATE_32;
            break;
        case 44100:
            sr = SL_SAMPLINGRATE_44_1;
            break;
        case 48000:
            sr = SL_SAMPLINGRATE_48;
            break;
        case 64000:
            sr = SL_SAMPLINGRATE_64;
            break;
        case 88200:
            sr = SL_SAMPLINGRATE_88_2;
            break;
        case 96000:
            sr = SL_SAMPLINGRATE_96;
            break;
        case 192000:
            sr = SL_SAMPLINGRATE_192;
            break;
        default:
            sr = SL_SAMPLINGRATE_44_1;
            break;
    }
    SLDataLocator_AndroidSimpleBufferQueue srcLocat = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLuint32 channelMask;
    if (numChannel == 1) {
        channelMask = SL_SPEAKER_FRONT_CENTER;
    } else {
        channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
    }
    SLDataFormat_PCM srcFmt = {
            SL_DATAFORMAT_PCM,
            numChannel,
            sr,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            channelMask,
            SL_BYTEORDER_LITTLEENDIAN
    };
    SLDataSource dataSrc = {&srcLocat, &srcFmt};

    (*player->engineItf)->CreateOutputMix(player->engineItf, &(player->outputMixObj), 0, NULL,
                                          NULL);
    (*player->outputMixObj)->Realize(player->outputMixObj, SL_BOOLEAN_FALSE);
    SLDataLocator_OutputMix snkLocat = {SL_DATALOCATOR_OUTPUTMIX, player->outputMixObj};
    SLDataSink dataSnk = {&snkLocat, NULL};

    SLInterfaceID ids[] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE};
    SLboolean reqs[] = {SL_BOOLEAN_TRUE};
    (*player->engineItf)->CreateAudioPlayer(player->engineItf, &(player->playerObj), &dataSrc,
                                            &dataSnk, 1, ids, reqs);
    (*player->playerObj)->Realize(player->playerObj, SL_BOOLEAN_FALSE);
    (*player->playerObj)->GetInterface(player->playerObj, SL_IID_PLAY, &(player->playerItf));
    (*player->playerObj)->GetInterface(player->playerObj, SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
                                       &(player->playerBufferQueueItf));
    (*player->playerBufferQueueItf)->RegisterCallback(player->playerBufferQueueItf, playCallback,
                                                      player);
    (*player->playerItf)->SetPlayState(player->playerItf, SL_PLAYSTATE_PLAYING);

    return player;
}

void deleteAudioPlayer(AudioPlayer *player) {
    if (player->playerObj != NULL) {
        (*player->playerObj)->Destroy(player->playerObj);
        player->playerObj = NULL;
        player->playerItf = NULL;
        player->playerBufferQueueItf = NULL;
    }

    if (player->outputMixObj != NULL) {
        (*player->outputMixObj)->Destroy(player->outputMixObj);
        player->outputMixObj = NULL;
    }

    if (player->engineObj != NULL) {
        (*player->engineObj)->Destroy(player->engineObj);
        player->engineObj = NULL;
        player->engineItf = NULL;
    }

    if (player->playLock != NULL) {
        notifyThreadLock(player->playLock);
        destroyThreadLock(player->playLock);
        player->playLock = NULL;
    }

    if (player->playBuffer[0] != NULL) {
        free(player->playBuffer[0]);
        player->playBuffer[0] = NULL;
    }

    if (player->playBuffer[1] != NULL) {
        free(player->playBuffer[1]);
        player->playBuffer[1] = NULL;
    }

    free(player);
}

ThreadLock *createThreadLock(void) {
    ThreadLock *lock = (ThreadLock *) calloc(1, sizeof(ThreadLock));
    pthread_mutex_init(&(lock->m), NULL);
    pthread_cond_init(&(lock->c), NULL);
    lock->s = 1;
    return lock;
}

void waitThreadLock(ThreadLock *lock) {
    pthread_mutex_lock(&(lock->m));
    while (!lock->s) {
        pthread_cond_wait(&(lock->c), &(lock->m));
    }
    lock->s = 0;
    pthread_mutex_unlock(&(lock->m));
}

void notifyThreadLock(ThreadLock *lock) {
    pthread_mutex_lock(&(lock->m));
    lock->s = 1;
    pthread_cond_signal(&(lock->c));
    pthread_mutex_unlock(&(lock->m));
}

void destroyThreadLock(ThreadLock *lock) {
    if (lock == NULL) {
        return;
    }
    notifyThreadLock(lock);
    pthread_cond_destroy(&(lock->c));
    pthread_mutex_destroy(&(lock->m));
    free(lock);
}


