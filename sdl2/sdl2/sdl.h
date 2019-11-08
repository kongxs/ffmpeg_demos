//
//  sdl.h
//  sdl2
//
//  Created by 孔祥书 on 2019/11/8.
//  Copyright © 2019年 kongxs. All rights reserved.
//

#ifndef sdl_h
#define sdl_h

#include <stdio.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>

#include <SDL2/SDL.h>

int getNumber();

int sfp_refresh_thread(void *opaque);


#define SFM_REFRESH_EVENT   (SDL_USEREVENT + 1)
#define SFM_BREAK_EVENT     (SDL_USEREVENT + 2)



#endif /* sdl_h */
