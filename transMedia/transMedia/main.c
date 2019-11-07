//
//  main.c
//  transMedia
//
//  Created by kong on 2019/11/6.
//  Copyright © 2019年 kong. All rights reserved.
//

#include <stdio.h>

#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include "libavformat/avformat.h"
#include "libavutil/log.h"
#include "libavutil/avutil.h"
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libavutil/mem.h>

#define LOGE(FORM , ... ) av_log(NULL, AV_LOG_ERROR, FORM,##__VA_ARGS__)


int ffmpegInit() {
    av_register_all();
    avcodec_register_all();
    
    return 0;
}

/**
 *  转换视频格式
 *
 */
int main(int argc, const char * argv[]) {
    
    // insert code here...
    
    
    char *file  = "/Users/kongxiangshu/Desktop/1111.mp4";
    char *outfile  = "/Users/kongxiangshu/Desktop/1111_out.avi";
    
    
//    ffmpegInit();
    
    AVFormatContext *inputContext  = NULL;
    
    if(avformat_open_input(&inputContext, file, NULL, NULL) < 0 ) {
        LOGE("can not open input file : %s \n" , file);
        exit(1);
    }
    
//    av_bsf_get_by_name(), av_bsf_alloc(), and av_bsf_init()
    
//    AVBitStreamFilter filter = av_bsf_get_by_name("h264_mp4toannexb");
    
    AVBitStreamFilterContext* aacbsfc = av_bitstream_filter_init("aac_adtstoasc");
    
    AVBitStreamFilterContext *filter = av_bitstream_filter_init("h264_mp4toannexb");
    if (NULL == filter)
    {
        LOGE(" fileter is failed ");
        exit(1);
    }

    av_register_bitstream_filter(filter->filter);
    av_register_bitstream_filter(aacbsfc->filter);
    
    if(avformat_find_stream_info(inputContext, NULL) < 0 ) {
        LOGE("can not find stream info ");
        exit(1);
    }
    
    //准备输出文件环境
    
    AVFormatContext *outContext = NULL;
    
    if (avformat_alloc_output_context2(&outContext, NULL, NULL, outfile) != 0) {
        LOGE("output context is failed ");
        exit(1);
    }
    
    if (!outContext) {
        LOGE("out context failed");
        exit(1);
    }
    
    AVOutputFormat *ofmt = NULL;
    
    ofmt = outContext->oformat;
    
    //创建输出流 , 拷贝 编解码器
    
    for(int i = 0;i < inputContext->nb_streams;i++) {
        
        AVStream *instream = inputContext->streams[i];
        
        
        
        AVCodec *codec = avcodec_find_decoder(instream->codecpar->codec_id);
        
//        if (instream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
//            codec = avcodec_find_decoder_by_name("mp4a");
//        }
        
        AVStream *outstream = avformat_new_stream(outContext, codec);
        
        if (!outstream) {
            LOGE("can not create out stream ... ");
            exit(1);
        }
        
//        AVCodecContext *pCodecCtx = avcodec_alloc_context3(codec);
//
//        if( avcodec_parameters_to_context(pCodecCtx, instream->codecpar) < 0 ) {
//            LOGE("copy param to context failed ");
//            exit(1);
//        }
        
        
//        pCodecCtx->codec_tag = 0;
        
//        if (outContext->oformat->flags & AVFMT_GLOBALHEADER) {
//            pCodecCtx->flags |= CODEC_FLAG_GLOBAL_HEADER;
//        }
        
        if (avcodec_parameters_copy(outstream->codecpar, instream->codecpar) < 0) {
            LOGE("copy param from context failed");
            exit(0);
        }
        
        outstream->codecpar->codec_tag = 0;
    }
    
    av_dump_format(inputContext, 0, file, 0);
    
    av_dump_format(outContext, 0, outfile, 1);
    
    
    if (!(ofmt->flags & AVFMT_NOFILE)) {
        if( avio_open(&outContext->pb, outfile, AVIO_FLAG_WRITE) < 0 ) {
            LOGE("can not open out file : %s " , outfile);
            exit(1);
        }
    }
    
    // end output context
    
    AVPacket pkt ;
    
//    av_init_packet(&pkt);| AV_ROUND_PASS_MINMAX
//
//    pkt.data = NULL;
//    pkt.size = 0;
    
    if( avformat_write_header(outContext, NULL) < 0) {
        LOGE("header write failed \n  ");
        exit(1);
    }
    
    
    while (av_read_frame(inputContext,&pkt) >= 0) {
        
        AVStream *inputstreams = inputContext->streams[pkt.stream_index];
        AVStream *outputstreams = outContext->streams[pkt.stream_index];
        
        
        if (pkt.stream_index == 0) {
            AVPacket fpkt = pkt;
            int a = av_bitstream_filter_filter(filter,
                                               outputstreams->codec, NULL, &fpkt.data, &fpkt.size,
                                               pkt.data, pkt.size, pkt.flags & AV_PKT_FLAG_KEY);
            pkt.data = fpkt.data;
            pkt.size = fpkt.size;
        }
//
        
        pkt.dts = av_rescale_q_rnd(pkt.dts, inputstreams->time_base, outputstreams->time_base, AV_ROUND_NEAR_INF);

        pkt.pts = av_rescale_q_rnd(pkt.pts, inputstreams->time_base, outputstreams->time_base, AV_ROUND_NEAR_INF);

        pkt.duration = av_rescale_q(pkt.duration,  inputstreams->time_base, outputstreams->time_base);
//
//
//        pkt.pos = -1;
        
        if( av_interleaved_write_frame(outContext, &pkt) < 0 ) {
            LOGE("write failed ... ");
            exit(1);
        }
        
        av_packet_unref(&pkt);
    }
    
    
    
    av_write_trailer(outContext);
    
    LOGE("transformat is successed \n ");
    
    
    return 0;
}

