//
//  main.c
//  ffmpeg
//
//  Created by 孔祥书 on 2019/11/5.
//  Copyright © 2019年 孔祥书. All rights reserved.
//

#include <stdio.h>


#include "ffmpegs.c"


int main(int argc, const char * argv[]) {
    
    

    char *file = "/Users/kongxiangshu/Desktop/1111.mp4";
    char *outfile = "/Users/kongxiangshu/Desktop/1111_out.mp4";
    
    AVFormatContext *inputContext;
    
    if (avformat_open_input(&inputContext, file, NULL, NULL) < 0) {
        LOGE("scan not open file %s ", file);
    }
    
    //解封装，找到视频和音频流信息
    
    if(avformat_find_stream_info(inputContext, NULL) < 0) {
        LOGE("没找到音视频信息");
    }
    
    av_dump_format(inputContext, 0, file, 0);
    
    AVDictionaryEntry *m = NULL;
    
//    for(int i = 0 ; i < inputContext->nb_streams;i++) {
//        AVStream *stream =  inputContext->streams[i];
//        if (!stream) {
//            LOGE("视频stream error ");
//        }
//
//        if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
//            LOGE("获取到视频信息 ... 、\n ") ;
//
//            while(m=av_dict_get(inputContext->metadata,"",m,AV_DICT_IGNORE_SUFFIX)){
//
//                LOGE("meta 信息 key : %s , value : %s  \n" , m->key , m->value);
//            }
//
//        } else if (stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
//            LOGE("获取到音频信息 ... \n ") ;
//
//            int tns, thh, tmm, tss;
//            tns  = (inputContext->duration) / 1000000;
//            thh  = tns / 3600;
//            tmm  = (tns % 3600) / 60;
//            tss  = (tns % 60);
//            LOGE(" 视频时长为：  %02d:%02d:%02d",thh,tmm,tss);
//        }
//    }
    
    
    int best_video_stream_index = av_find_best_stream(inputContext,AVMEDIA_TYPE_VIDEO , -1, -1, NULL, 0);
    
    if (best_video_stream_index >= 0) {
        LOGE("find best stream index :     %d \n" , best_video_stream_index);
    }
    
    
    //抽取音频
    /**
        解封装 ，然后将音频数据取出来，封装后写到文件中
     **/
    
    
    //输出的上下文
    AVFormatContext *outContext = NULL;
    
    if( avformat_alloc_output_context2(&outContext, NULL, NULL, outfile) < 0) {
        LOGE("输出文件上下文失败 ") ;
    }
    
    //新建输出流
    AVStream *outStream = avformat_new_stream(outContext, NULL) ;
    
    if (!outStream) {
        LOGE("can not create out stream ") ;
    }
    
    //输出流 编码
    if(avcodec_parameters_copy(outStream->codecpar, inputContext->streams[best_video_stream_index]->codecpar) < 0) {
        LOGE("输出环境编码失败. .. ") ;
    }
    
    
    if(avio_open(&outContext->pb, outfile, AVIO_FLAG_WRITE) < 0) {
        LOGE("输出文件创建失败 .... ");
    }
    
    AVPacket pkt ;
    
    av_init_packet(&pkt);
    
    pkt.data = NULL;
    pkt.size = 0;
    
    AVStream *in_stream = inputContext->streams[best_video_stream_index];
    
    avformat_write_header(outContext, NULL);
    
    while (av_read_frame(inputContext,&pkt) >= 0) {
        
        if (pkt.stream_index == best_video_stream_index) {
            
            pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, outStream->time_base,
                                       AV_ROUND_NEAR_INF);
            pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, outStream->time_base,
                                       AV_ROUND_NEAR_INF );
            
            pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, outStream->time_base);
            pkt.pos = -1;
            pkt.stream_index = 0;
            //将包写到输出媒体文件
            av_interleaved_write_frame(outContext, &pkt);
            //减少引用计数，避免内存泄漏
            av_packet_unref(&pkt);
        }
    }
    
    av_write_trailer(outContext);
    
    LOGE("handle finish ： successed  ：  %s \n  " , outfile) ;
    
    
    return 0;
}
