//
// Created by lingrui on 2017/3/24.
//
#include <jni.h>
#include <stdio.h>
#include "AudioPlayer.h"
#include "AudioDecoder.h"

static AudioPlayer *player;
static AudioDecoder *decoder;

static int decoderBufferSize;

static volatile int g_loop_exit = 0;

JNIEXPORT void JNICALL Java_com_droidrui_ffmpegdemo_jni_AudioJNI_initDecode
        (JNIEnv *env, jobject thiz, jint bufferSize, jstring inputPath, jstring outputPath) {
    decoderBufferSize = bufferSize * 2 * sizeof(short) * 10;
    player = newAudioPlayer(44100, 2, bufferSize);
    const char *input = (*env)->GetStringUTFChars(env, inputPath, NULL);
    const char *output = (*env)->GetStringUTFChars(env, outputPath, NULL);
    decoder = newAudioDecoder(decoderBufferSize, input, output);
    (*env)->ReleaseStringUTFChars(env, inputPath, input);
    (*env)->ReleaseStringUTFChars(env, outputPath, output);
}

void *decode(void *context) {
    void *buffer;
    int size;
    g_loop_exit = 0;
    while (!g_loop_exit) {
        if (decoder->getFrame(decoder, &buffer, &size) == 0) {
            player->writeToPlayer(player, (short *) buffer, size / sizeof(2));
        } else {
            break;
        }
    }
    deleteAudioDecoder(decoder);
    decoder = NULL;
    deleteAudioPlayer(player);
    player = NULL;
    buffer = NULL;
}

JNIEXPORT void JNICALL Java_com_droidrui_ffmpegdemo_jni_AudioJNI_startDecode
        (JNIEnv *env, jobject thiz) {
    pthread_t threadInfo_;
    pthread_attr_t threadAttr_;

    pthread_attr_init(&threadAttr_);
    pthread_attr_setdetachstate(&threadAttr_, PTHREAD_CREATE_DETACHED);

    pthread_create(&threadInfo_, &threadAttr_, decode, NULL);
}


JNIEXPORT void JNICALL Java_com_droidrui_ffmpegdemo_jni_AudioJNI_stopDecode
        (JNIEnv *env, jobject thiz) {
    g_loop_exit = 1;
}


JNIEXPORT void JNICALL Java_com_droidrui_ffmpegdemo_jni_AudioJNI_initEncode
        (JNIEnv *env, jobject thiz, jint bufferSize, jstring inputPath, jstring outputPath) {

}


JNIEXPORT void JNICALL Java_com_droidrui_ffmpegdemo_jni_AudioJNI_startEncode
        (JNIEnv *env, jobject thiz) {

}


JNIEXPORT void JNICALL Java_com_droidrui_ffmpegdemo_jni_AudioJNI_stopEncode
        (JNIEnv *env, jobject thiz) {

}