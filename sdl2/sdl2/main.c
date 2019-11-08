#include <stdio.h>


#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
    
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_test_images.h>



#define loged(format ,... ) av_log(NULL,AV_LOG_ERROR,format,##__VA_ARGS__)

// 使用 render 渲染
int main() {
    
    char *name = "/Users/kongxiangshu/Desktop/sdl.png";
    
    SDL_Init(SDL_INIT_VIDEO);
    
    
    SDL_Rect rect;
    rect.x = 100;
    rect.y = 100;
    rect.w = 120;
    rect.h = 120;
    
    SDL_Window *window =  SDL_CreateWindow("sdl windol", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 400, 400, SDL_WINDOW_SHOWN);
    SDL_Renderer *render =  SDL_CreateRenderer(window, -1, 0);
    SDL_Surface *image = IMG_Load(name);
    
    

    
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    
    SDL_Texture *texture =  SDL_CreateTextureFromSurface(render, image);
    
    
    //将image添加到 surface上，并更新窗口
//    SDL_BlitSurface(image, NULL, surface, &rect);
//    SDL_UpdateWindowSurface(window);
    SDL_Event event;
    while (1)
    {//主消息循环
        SDL_WaitEvent(&event);
        
        switch (event.type)
        {   //用户从菜单要求退出程序
            case SDL_QUIT:
                SDL_FreeSurface(surface);
                SDL_DestroyWindow(window);
                SDL_Quit();
                exit(0);
            break;
            case SDL_MOUSEMOTION :
//                loged("motion event : x = %d , y = %d \n " , event.motion.x , event.motion.y);
                                //                int x = event.motion.x;
                                //                int y = event.motion.y;
                rect.x = event.motion.x - (rect.w/2);
                rect.y = event.motion.y - (rect.h/2);
                
            break;
            case SDL_KEYUP:
//                loged("key up %p  " ,event.key.keysym.sym );
                
                if (event.key.keysym.sym == SDLK_KP_ENTER) {
                    SDL_FreeSurface(surface);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    exit(0);
                }
                break;
        }
        
        //如果指定显示位置使用下面注释起来的两句
        SDL_RenderClear(render);
        //把贴图材质复制到渲染器
        SDL_RenderCopy(render, texture, NULL, &rect);
        //显示出来
        SDL_RenderPresent(render);
    }
    
    return 0;
}
