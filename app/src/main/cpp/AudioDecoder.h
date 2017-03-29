//
// Created by Administrator on 2017/3/27.
//

#include <stdio.h>
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"

#define MAX_AUDIO_FRAME_SIZE 192000

typedef struct AudioDecoder {

    AVFormatContext *pFormatCtx;
    int audioStream;
    AVCodecContext *pCodecCtx;
    AVCodec *pCodec;
    AVPacket *packet;
    uint8_t *out_buffer;
    AVFrame *pFrame;
    int ret;
    int got_picture;
    int index;
    int64_t in_channel_layout;
    SwrContext *au_convert_ctx;

    int out_buffer_size;

    FILE *pFile;

    int (*getFrame)(struct AudioDecoder *decoder, void *buffer, int size);

} AudioDecoder;

AudioDecoder *newAudioDecoder(const char *inputPath, const char *outputPath);

void deleteAudioDecoder(AudioDecoder *decoder);


