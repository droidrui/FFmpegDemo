//
// Created by lingrui on 2017/3/24.
//
#include <jni.h>
#include <stdio.h>
#include "opensl.h"
#include "ffmpeg.h"
#include "log.h"

static OpenSLEngine *engine;
static FILE *playFile;
static int playBufferSize;
static volatile int g_loop_exit = 0;

void *play(void *context) {
//    short buffer[playBufferSize];
//    g_loop_exit = 0;
//    while (!g_loop_exit && !feof(playFile)) {
//        fread(buffer, sizeof(short), playBufferSize, playFile);
//        writeToPlayer(engine, buffer, playBufferSize);
//    }
//    destroyEngine(engine);
//    fclose(playFile);
    int result = decode(NULL, NULL);
    LOGE("result = %d", result);
}

JNIEXPORT void JNICALL Java_com_droidrui_ffmpegdemo_jni_AudioJNI_initPlay
        (JNIEnv *env, jobject thiz, jint framesPerBuffer, jstring filePath) {
//    playBufferSize = framesPerBuffer * 1 * 4;
//    engine = createEngine();
//    createPlayer(engine, 44100, 1, framesPerBuffer);
//    const char *path = (*env)->GetStringUTFChars(env, filePath, NULL);
//    playFile = fopen(path, "rb");
//    (*env)->ReleaseStringUTFChars(env, filePath, path);
}

/*
 * Class:     com_droidrui_ffmpegdemo_jni_AudioJNI
 * Method:    startPlay
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_droidrui_ffmpegdemo_jni_AudioJNI_startPlay
        (JNIEnv *env, jobject thiz) {
    pthread_t threadInfo_;
    pthread_attr_t threadAttr_;

    pthread_attr_init(&threadAttr_);
    pthread_attr_setdetachstate(&threadAttr_, PTHREAD_CREATE_DETACHED);

    pthread_create(&threadInfo_, &threadAttr_, play, NULL);
}

/*
 * Class:     com_droidrui_ffmpegdemo_jni_AudioJNI
 * Method:    stopPlay
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_droidrui_ffmpegdemo_jni_AudioJNI_stopPlay
        (JNIEnv *env, jobject thiz) {
    g_loop_exit = 1;
}