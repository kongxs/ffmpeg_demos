//#include <stdio.h>
//
//
//#include <libavcodec/avcodec.h>
//#include <libavformat/avformat.h>
//#include <libswscale/swscale.h>
//#include <libavutil/imgutils.h>
//
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
////#include <SDL2/SDL_test_images.h>
//
//
//
//#define loged(format ,... ) av_log(NULL,AV_LOG_ERROR,format,##__VA_ARGS__)
//
//int main() {
//    
//    
//    SDL_Init(SDL_INIT_VIDEO);
//    
//    
//    SDL_Rect rect;
//    rect.x = 100;
//    rect.y = 100;
//    rect.w = 10;
//    rect.h = 10;
//    
//    SDL_Window *window =  SDL_CreateWindow("sdl windol", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 400, 400, SDL_WINDOW_SHOWN);
//    
//    char *name = "/Users/kongxiangshu/Desktop/sdl.png";
//    
//    SDL_Surface *surface = SDL_GetWindowSurface(window);
//    
//    SDL_Surface *image = IMG_Load(name);
//    image->w = 50;
//    image->h = 50;
//    
//    SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//    
//    SDL_Texture *texture =  SDL_CreateTextureFromSurface(render, image);
//    
//    
//    //将image添加到 surface上，并更新窗口
//    SDL_BlitSurface(image, NULL, surface, &rect);
//    SDL_UpdateWindowSurface(window);
//    
//    
//    //    SDL_RenderClear(render);
//    ////    SDL_UpdateWindowSurface(window);
//    //    SDL_RenderCopy(surface, texture,NULL , &rect);
//    //    SDL_RenderPresent(render);
//    
//    for(;;) {
//        
//        SDL_Event event;
//        
//        SDL_WaitEvent(&event);
//        switch (event.type) {
//            case SDL_QUIT:
//                
//                SDL_FreeSurface(surface);
//                SDL_DestroyWindow(window);
//                SDL_Quit();
//                
//                exit(0);
//                break;
//            case SDL_MOUSEMOTION :
//                //                loged("motion event : x = %d , y = %d \n " , event.motion.x , event.motion.y);
//                //                int x = event.motion.x;
//                //                int y = event.motion.y;
//                rect.x = event.motion.x - (image->w/2);
//                rect.y = event.motion.y - (image->h/2);;
//                
//                SDL_FillRect(surface, NULL, 0);
//                SDL_BlitSurface(image, NULL, surface, &rect);
//                SDL_UpdateWindowSurface(window);
//                
//                break;
//            case SDL_KEYDOWN:
//                switch( event.key.keysym.sym )
//            {
//                case SDLK_UP: loged("up"); break;
//                case SDLK_DOWN: ; break;
//                case SDLK_a: ; break;
//                case SDLK_RIGHT: ; break;
//            }
//                break;
//            default:
//                break;
//        }
//    }
//    
//    return 0;
//}
