//
// Created by lingrui on 2017/3/24.
//
#include <jni.h>
#include <stdio.h>
#include "opensl.h"
#include "AudioDecoder.h"
#include "log.h"

//static OpenSLEngine *engine;
//static FILE *playFile;
//static int playBufferSize;
//static volatile int g_loop_exit = 0;
static AudioDecoder *decoder;

JNIEXPORT void JNICALL Java_com_droidrui_ffmpegdemo_jni_AudioJNI_initDecode
        (JNIEnv *env, jobject thiz, jint bufferSize, jstring inputPath, jstring outputPath) {
//    playBufferSize = bufferSize * 1 * 4;
//    engine = createEngine();
//    playFile = fopen(input, "rb");
//    createPlayer(engine, 44100, 1, bufferSize);
    const char *input = (*env)->GetStringUTFChars(env, inputPath, NULL);
    const char *output = (*env)->GetStringUTFChars(env, outputPath, NULL);
    decoder = newAudioDecoder(input, output);
    (*env)->ReleaseStringUTFChars(env, inputPath, input);
    (*env)->ReleaseStringUTFChars(env, outputPath, output);
}

void *decode(void *context) {
//    short buffer[playBufferSize];
//    g_loop_exit = 0;
//    while (!g_loop_exit && !feof(playFile)) {
//        fread(buffer, sizeof(short), playBufferSize, playFile);
//        writeToPlayer(engine, buffer, playBufferSize);
//    }
//    destroyEngine(engine);
//    fclose(playFile);
    int result = decoder->getFrame(decoder, NULL, 0);
    LOGE("result = %d", result);
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
    //g_loop_exit = 1;
    deleteAudioDecoder(decoder);
    decoder = NULL;
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