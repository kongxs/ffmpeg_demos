//#include <stdio.h>
//
//
//#include <libavcodec/avcodec.h>
//#include <libavformat/avformat.h>
//#include <libswscale/swscale.h>
//#include <libavutil/imgutils.h>
//
//#include <SDL2/SDL.h>
//
//
////Refresh Event
//#define SFM_REFRESH_EVENT   (SDL_USEREVENT + 1)
//#define SFM_BREAK_EVENT     (SDL_USEREVENT + 2)
//
//int thread_exit = 0;
//int thread_pause = 0;
//
//int sfp_refresh_thread(void *opaque)
//{
//    thread_exit = 0;
//    thread_pause = 0;
//    
//    while (!thread_exit) {
//        if (!thread_pause) {
//            SDL_Event event;
//            event.type = SFM_REFRESH_EVENT;
//            SDL_PushEvent(&event);
//        }
//        SDL_Delay(20);
//    }
//    thread_exit = 0;
//    thread_pause = 0;
//    //Break
//    SDL_Event event;
//    event.type = SFM_BREAK_EVENT;
//    SDL_PushEvent(&event);
//    
//    return 0;
//}
//
//int main(int argc, char *argv[])
//{
//    AVFormatContext *pFormatCtx;
//    int             i, videoindex;
//    AVCodecContext  *pCodecCtx;
//    AVCodec         *pCodec;
//    AVFrame *pFrame, *pFrameYUV;
//    uint8_t *out_buffer;
//    AVPacket *packet;
//    int ret, got_picture;
//    
//    //------------SDL----------------
//    int screen_w, screen_h;
//    SDL_Window *screen;
//    SDL_Renderer* sdlRenderer;
//    SDL_Texture* sdlTexture;
//    SDL_Rect sdlRect;
//    SDL_Thread *video_tid;
//    SDL_Event event;
//    
//    struct SwsContext *img_convert_ctx;
//    
//    char filepath[] = "/Users/kongxiangshu/Desktop/1111.mp4";
//    
//    av_register_all();
//    avformat_network_init();
//    pFormatCtx = avformat_alloc_context();
//    
//    if (avformat_open_input(&pFormatCtx, filepath, NULL, NULL) != 0) {
//        printf("Couldn't open input stream.\n");
//        return -1;
//    }
//    if (avformat_find_stream_info(pFormatCtx, NULL)<0) {
//        printf("Couldn't find stream information.\n");
//        return -1;
//    }
//    videoindex = -1;
//    for (i = 0; i<pFormatCtx->nb_streams; i++)
//        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
//            videoindex = i;
//            break;
//        }
//    if (videoindex == -1) {
//        printf("Didn't find a video stream.\n");
//        return -1;
//    }
//    //  pCodecCtx = pFormatCtx->streams[videoindex]->codecpar;
//    AVStream *stream = pFormatCtx->streams[videoindex];
//    pCodec = avcodec_find_decoder(stream->codecpar->codec_id);
//    pCodecCtx = avcodec_alloc_context3(pCodec);
//    avcodec_parameters_to_context(pCodecCtx, stream->codecpar);
//    if (pCodec == NULL) {
//        printf("Codec not found.\n");
//        return -1;
//    }
//    if (avcodec_open2(pCodecCtx, pCodec, NULL)<0) {
//        printf("Could not open codec.\n");
//        return -1;
//    }
//    pFrame = av_frame_alloc();
//    pFrameYUV = av_frame_alloc();
//    out_buffer = (uint8_t *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1));
//    av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1);
//    
//    //Output Info-----------------------------
//    printf("---------------- File Information ---------------\n");
//    av_dump_format(pFormatCtx, 0, filepath, 0);
//    printf("-------------------------------------------------\n");
//    
//    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
//                                     pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
//    
//    
//    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
//        printf("Could not initialize SDL - %s\n", SDL_GetError());
//        return -1;
//    }
//    //SDL 2.0 Support for multiple windows
//    screen_w = pCodecCtx->width;
//    screen_h = pCodecCtx->height;
//    screen = SDL_CreateWindow("Simplest ffmpeg player's Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
//                              screen_w, screen_h, SDL_WINDOW_OPENGL);
//    
//    if (!screen) {
//        printf("SDL: could not create window - exiting:%s\n", SDL_GetError());
//        return -1;
//    }
//    sdlRenderer = SDL_CreateRenderer(screen, -1, 0);
//    //IYUV: Y + U + V  (3 planes)
//    //YV12: Y + V + U  (3 planes)
//    sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, pCodecCtx->width, pCodecCtx->height);
//    
//    sdlRect.x = 0;
//    sdlRect.y = 0;
//    sdlRect.w = screen_w;
//    sdlRect.h = screen_h;
//    
//    packet = (AVPacket *)av_malloc(sizeof(AVPacket));
//    
//    video_tid = SDL_CreateThread(sfp_refresh_thread, NULL, NULL);
//    //------------SDL End------------
//    //Event Loop
//    
//    for (;;) {
//        //Wait
//        SDL_WaitEvent(&event);
//        if (event.type == SFM_REFRESH_EVENT) {
//            //------------------------------
//            if (av_read_frame(pFormatCtx, packet) >= 0) {
//                if (packet->stream_index == videoindex) {
//                    ret = avcodec_send_packet(pCodecCtx, packet);
//                    got_picture = avcodec_receive_frame(pCodecCtx, pFrame);
//                    if (ret < 0) {
//                        printf("Decode Error.\n");
//                        return -1;
//                    }
//                    if (got_picture == 0) {
//                        sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
//                        //SDL---------------------------
//                        SDL_UpdateTexture(sdlTexture, NULL, pFrameYUV->data[0], pFrameYUV->linesize[0]);
//                        SDL_RenderClear(sdlRenderer);
//                        //SDL_RenderCopy( sdlRenderer, sdlTexture, &sdlRect, &sdlRect );
//                        SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
//                        SDL_RenderPresent(sdlRenderer);
//                        //SDL End-----------------------
//                    }
//                }
//                av_packet_unref(packet);
//            }
//            else {
//                //Exit Thread
//                thread_exit = 1;
//            }
//        }
//        else if (event.type == SDL_KEYDOWN) {
//            //Pause
//            if (event.key.keysym.sym == SDLK_SPACE)
//                thread_pause = !thread_pause;
//        }
//        else if (event.type == SDL_QUIT) {
//            thread_exit = 1;
//        }
//        else if (event.type == SFM_BREAK_EVENT) {
//            break;
//        }
//        
//    }
//    
//    sws_freeContext(img_convert_ctx);
//    
//    SDL_Quit();
//    //--------------
//    av_frame_free(&pFrameYUV);
//    av_frame_free(&pFrame);
//    avcodec_close(pCodecCtx);
//    avformat_close_input(&pFormatCtx);
//    
//    return 0;
//}
////
