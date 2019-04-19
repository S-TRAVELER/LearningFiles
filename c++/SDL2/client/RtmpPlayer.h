#ifndef RTMPPLAYER_H
#define RTMPPLAYER_H

#include <iostream>
using namespace std;
#ifdef __cplusplus
extern "C" {
#endif
// 引入编码器
#include "libavformat/avformat.h"
// 引入时间
#include "libavutil/time.h"
#ifdef __cplusplus
}
#endif

class RtmpPlayer
{
public:
    RtmpPlayer();
};

#endif // RTMPPLAYER_H
