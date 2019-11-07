//
//  main.c
//  toPics
//
//  Created by kongxs. on 2019/11/7.
//  Copyright © 2019年 kongxs. All rights reserved.
//

#include <stdio.h>

#include <libavformat/avformat.h>


#define LOGE(FORMAT, ...) av_log(NULL, AV_LOG_ERROR, FORMAT, #__VA_ARGS__)


void ffmpegInit() {
    av_register_all();
    avcodec_register_all();
}


void savePics(AVFrame *frame,char *path) ;

/**
 * 从视频中抽取图片
 
    准备工作：  1. 打开视频文件，并获取到视频流信息 ， 将视频流解码
              2. 解码后的frame 进行编码成图片格式
 */
int main(int argc, const char * argv[]) {
    
    
    char *file = "/Users/kongxiangshu/Desktop/1111.mp4";
    char *outDir = "/Users/kongxiangshu/Desktop/pics";
    
    AVFormatContext *in_context ;
    
    in_context = avformat_alloc_context();
    
    if (avformat_open_input(&in_context, file, NULL, NULL) < 0 ) {
        LOGE("can not open file %s \n" , file);
        exit(1);
    }
    
    if( avformat_find_stream_info(in_context, NULL) < 0 ) {
        LOGE("can not find media info \n");
        exit(1);
    }
    
//    av_dump_format(in_context, -1, file, 0);
    
    int best_stream_index = -1;
    
    if(best_stream_index =
       av_find_best_stream(in_context, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0) < 0 ){
        LOGE("can not find video stream \n ");
        exit(1);
    }
    
    LOGE("best stream index : %d , \n" , best_stream_index);
    
    
    
    for(int i = 0; i< in_context->nb_streams;i++) {
        
        AVStream *stream = in_context->streams[i];
        
        if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            best_stream_index = i;
        }
    }
    
    LOGE("best stream index : %d , \n" , best_stream_index);
    
    
    // codec
    
    AVStream *beststream = in_context->streams[best_stream_index];
    
    AVCodec *codec = avcodec_find_decoder(beststream->codecpar->codec_id);
    
    
    if( codec  == NULL) {
        
        LOGE("can not find codec \n");
        exit(1);
    }
    
    AVCodecContext *codec_context = avcodec_alloc_context3(codec);
    
    if (codec_context == NULL) {
        LOGE("can not alloc codec context \n ");
        exit(1);
    }
    
    // param
    if(avcodec_parameters_to_context(codec_context, beststream->codecpar) < 0){
        LOGE("copy codec param error ");
        exit(1);
    }
    
    if ( avcodec_open2(codec_context, codec, NULL) < 0 ) {
        LOGE("can not open codec  \n");
        exit(1);
    }
    
    
    // get frames
    
    AVPacket pkt ;
    
    av_init_packet(&pkt);
    pkt.size = 0;
    pkt.data = NULL;
    
//    pkt = *av_packet_alloc();
//
//    pkt.data = NULL;
//    pkt.size = 0;
    
    
    AVFrame *frame = av_frame_alloc();
    
    int frame_count = 0;
    
    char buf[1024] = "";
    
    while (av_read_frame(in_context, &pkt) >= 0) {
        
        if (pkt.stream_index == best_stream_index) {
            
            AVStream *stream =  in_context->streams[pkt.stream_index] ;
            
            if (stream == NULL) {
                LOGE("error to get  stream  in read freme \n ");
                exit(1);
            }
            
            // decode frame
            if (avcodec_send_packet(codec_context, &pkt) < 0) {
                LOGE("packet decode failed \n") ;
                exit(1);
            }
            
            while(avcodec_receive_frame(codec_context, frame) ==0) {
                
                // ready to save pics
                snprintf(buf, sizeof(buf), "%s/Demo-%d.jpg", outDir, frame_count);
                
//                sprintf(name,"asdffasd");
                if (frame_count % 25 == 0) {
                    savePics(frame,buf);
                }
            }
            
            frame_count ++ ;
            
            av_packet_unref(&pkt);
        }
    }
    
    return 0;
}

void savePics(AVFrame *frame, char *name) {
    
    //对frame进行编码 ，并保存到文件中
    
    printf("%s \n" , name);
    
    AVFormatContext *out_context ;
    
    out_context = avformat_alloc_context();
    
    
    if ( avformat_alloc_output_context2(&out_context, NULL, NULL, name)) {
        LOGE("输出环境初始化失败 \n ");
        exit(1);
    }
    
    
    AVStream *stream =  avformat_new_stream(out_context, 0) ;
    
    if (stream == NULL) {
        LOGE("scan not get new stream  \n");
    }
    
    
    AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_MJPEG);
    
    if (codec == NULL) {
        LOGE("can not find jpeg codec");
        exit(1);
    }
    
    AVCodecContext *avcodec_context = avcodec_alloc_context3(codec);
    
    if (avcodec_context == NULL) {
        LOGE("codec context failed ");
        exit(1);
    }
    
    avcodec_context->width = frame->width;
    avcodec_context->height = frame->height;
    avcodec_context->time_base.num = 100;
    avcodec_context->time_base.den = 1000;
    avcodec_context->pix_fmt = AV_PIX_FMT_YUVJ420P;
    avcodec_context->codec_id = codec->id;
    avcodec_context->codec_type = AVMEDIA_TYPE_VIDEO;
    
    
    avcodec_open2(avcodec_context, codec, NULL);
    
    
    
    //writer heaer
    
    avformat_write_header(out_context, NULL);
    
    
    
    //编码帧
    int ret = avcodec_send_frame(avcodec_context, frame);
    if (ret < 0) {
        LOGE("encode error  \n");
        exit(1);
    }
    
    int y_size = frame->width * frame->height;
    
    //Encode
    // 给AVPacket分配足够大的空间
    AVPacket pkt;
    
    av_new_packet(&pkt, y_size * 3);
    
    ret = avcodec_receive_packet(avcodec_context, &pkt);
    
    if (ret < 0) {
        LOGE("receive frame error \n ");
        exit(1);
    }
    
    
    av_write_frame(out_context, &pkt);
    
    
    av_write_trailer(out_context);
    
    
    av_packet_unref(&pkt);
    
    avformat_free_context(out_context);
    
}
