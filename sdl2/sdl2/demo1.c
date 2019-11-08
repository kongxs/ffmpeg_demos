//#include <stdio.h>
//
//
//#include <libavcodec/avcodec.h>
//#include <libavformat/avformat.h>
//#include <libswscale/swscale.h>
//#include <libavutil/imgutils.h>
//
//#include <SDL2/SDL.h>
////#include <SDL2/SDL_test_images.h>
//
//
//
//int main() {
//    
//    
//    SDL_Init(SDL_INIT_VIDEO);
//    
//    
//    SDL_Window *window =  SDL_CreateWindow("sdl windol", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 400, 400, SDL_WINDOW_SHOWN);
//    
//    
//    SDL_Surface *surface = SDL_GetWindowSurface(window);
//    
//    SDL_Surface *image = SDL_LoadBMP("/Users/kongxiangshu/Desktop/sdl2.bmp");
//    
//    
//    //将image添加到 surface上，并更新窗口
//    SDL_UpperBlitScaled(image, NULL, surface, NULL);
//    SDL_UpdateWindowSurface(window);
//    
//    
//    
//    
//    
//    SDL_Event event;
//    
//    for(;;) {
//        SDL_WaitEvent(&event);
//        switch (event.type) {
//            case SDL_QUIT:
//                exit(1);
//                break;
//                
//            default:
//                break;
//        }
//        
//    }
//    
//    
//    SDL_DestroyWindow(window);
//    SDL_Quit();
//    
//    
//    
//    return 0;
//}
