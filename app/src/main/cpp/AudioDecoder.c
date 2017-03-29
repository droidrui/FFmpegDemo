//
// Created by Administrator on 2017/3/27.
//

#include "AudioDecoder.h"


static int getFrame(struct AudioDecoder *decoder, void *buffer, int size) {
    while (av_read_frame(decoder->pFormatCtx, decoder->packet) >= 0) {
        if (decoder->packet->stream_index == decoder->audioStream) {

            decoder->ret = avcodec_decode_audio4(decoder->pCodecCtx, decoder->pFrame,
                                                 &(decoder->got_picture), decoder->packet);
            if (decoder->ret < 0) {
                printf("Error in decoding audio frame.\n");
                return -1;
            }
            if (decoder->got_picture > 0) {
                swr_convert(decoder->au_convert_ctx, &(decoder->out_buffer), MAX_AUDIO_FRAME_SIZE,
                            (const uint8_t **) decoder->pFrame->data, decoder->pFrame->nb_samples);

                printf("index:%5d\t pts:%lld\t packet size:%d\n", decoder->index,
                       decoder->packet->pts,
                       decoder->packet->size);
                //Write PCM
                fwrite(decoder->out_buffer, 1, decoder->out_buffer_size, decoder->pFile);
                decoder->index++;
            }
        }
        av_packet_unref(decoder->packet);
    }
    return 0;
}

AudioDecoder *newAudioDecoder(const char *inputPath, const char *outputPath) {
    AudioDecoder *decoder = (AudioDecoder *) calloc(1, sizeof(AudioDecoder));
    decoder->getFrame = getFrame;
    decoder->pFile = fopen(outputPath, "wb");
    decoder->index = 0;

    av_register_all();
    avformat_network_init();
    decoder->pFormatCtx = avformat_alloc_context();
    //Open
    if (avformat_open_input(&(decoder->pFormatCtx), inputPath, NULL, NULL) != 0) {
        printf("Couldn't open input stream.\n");
        return NULL;
    }
    // Retrieve stream information
    if (avformat_find_stream_info(decoder->pFormatCtx, NULL) < 0) {
        printf("Couldn't find stream information.\n");
        return NULL;
    }
    // Dump valid information onto standard error
    av_dump_format(decoder->pFormatCtx, 0, inputPath, 0);

    // Find the first audio stream
    decoder->audioStream = -1;
    for (int i = 0; i < decoder->pFormatCtx->nb_streams; i++)
        if (decoder->pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            decoder->audioStream = i;
            break;
        }

    if (decoder->audioStream == -1) {
        printf("Didn't find a audio stream.\n");
        return NULL;
    }

    // Get a pointer to the codec context for the audio stream
    decoder->pCodecCtx = decoder->pFormatCtx->streams[decoder->audioStream]->codec;

    // Find the decoder for the audio stream
    decoder->pCodec = avcodec_find_decoder(decoder->pCodecCtx->codec_id);
    if (decoder->pCodec == NULL) {
        printf("Codec not found.\n");
        return NULL;
    }

    // Open codec
    if (avcodec_open2(decoder->pCodecCtx, decoder->pCodec, NULL) < 0) {
        printf("Could not open codec.\n");
        return NULL;
    }

    decoder->packet = (AVPacket *) av_malloc(sizeof(AVPacket));
    av_init_packet(decoder->packet);

    //Out Audio Param
    uint64_t out_channel_layout = AV_CH_LAYOUT_MONO;
    //nb_samples: AAC-1024 MP3-1152
    int out_nb_samples = decoder->pCodecCtx->frame_size;
    enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
    int out_sample_rate = 44100;
    int out_channels = av_get_channel_layout_nb_channels(out_channel_layout);
    //Out Buffer Size
    decoder->out_buffer_size = av_samples_get_buffer_size(NULL, out_channels, out_nb_samples,
                                                          out_sample_fmt, 1);

    decoder->out_buffer = (uint8_t *) av_malloc(MAX_AUDIO_FRAME_SIZE * 2);
    decoder->pFrame = av_frame_alloc();

    //FIX:Some Codec's Context Information is missing
    decoder->in_channel_layout = av_get_default_channel_layout(decoder->pCodecCtx->channels);
    //Swr
    decoder->au_convert_ctx = swr_alloc();
    decoder->au_convert_ctx = swr_alloc_set_opts(decoder->au_convert_ctx, out_channel_layout,
                                                 out_sample_fmt,
                                                 out_sample_rate,
                                                 decoder->in_channel_layout,
                                                 decoder->pCodecCtx->sample_fmt,
                                                 decoder->pCodecCtx->sample_rate, 0, NULL);
    swr_init(decoder->au_convert_ctx);

    return decoder;
}

void deleteAudioDecoder(AudioDecoder *decoder) {
    swr_free(&(decoder->au_convert_ctx));

    fclose(decoder->pFile);

    av_free(decoder->out_buffer);
    // Close the codec
    avcodec_close(decoder->pCodecCtx);
    // Close the video file
    avformat_close_input(&(decoder->pFormatCtx));
    free(decoder);
}
