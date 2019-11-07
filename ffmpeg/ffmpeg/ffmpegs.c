//
//  ffmpegs.c
//  ffmpeg
//
//  Created by 孔祥书 on 2019/11/6.
//  Copyright © 2019年 孔祥书. All rights reserved.
//



#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include "libavformat/avformat.h"
#include "libavutil/log.h"
#include "libavutil/avutil.h"
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libavutil/mem.h>

#include <stdio.h>

#define LOGE(FORMAT,...) av_log(NULL, AV_LOG_ERROR, FORMAT, ##__VA_ARGS__);


