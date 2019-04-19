/*#include <stdio.h>
#include <iostream>

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "SDL2/SDL.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <SDL2/SDL.h>
#include <libavutil/imgutils.h>
#ifdef __cplusplus
};
#endif
#endif

//Output YUV420P data as a file
#define OUTPUT_YUV420P 0

double get_delay(PlayerState *ps) {
    double ret_delay = 0.0;
    double frame_delay = 0.0;
    double cur_audio_clock = 0.0;
    double compare = 0.0;
    double threshold = 0.0; //这里的delay是秒为单位， 化为毫秒：*1000
    //获取两帧之间的延时
    frame_delay = ps->cur_frame_pts - ps->pre_frame_pts;
    if (frame_delay <= 0 || frame_delay >= 1.0) {
        frame_delay = ps->pre_cur_frame_delay;
    }
    //两帧之间的延时存到统筹结构
    ps->pre_cur_frame_delay = frame_delay;
    ps->pre_frame_pts = ps->cur_frame_pts;
    cur_audio_clock = get_audio_clock(ps);
    //compare < 0 说明慢了， > 0说明快了
    compare = ps->cur_frame_pts - cur_audio_clock;
    //设置阀值, 是一个正数，最小阀值取它的负数。
    //这里设阀值为两帧之间的延迟，
    threshold = frame_delay;
                  //SYNC_THRESHOLD ? frame_delay : SYNC_THRESHOLD;
    if (compare <= -threshold) //慢， 加快速度，
    {
        ret_delay = frame_delay / 2; //这里用移位更好快
    } else if (compare >= threshold)  //快了，就在上一帧延时的基础上加长延时
    {
        ret_delay = frame_delay * 2; //这里用移位更好快
    }
    else {
        ret_delay = frame_delay;
    }
    return ret_delay;

}






int main(int argc, char* argv[])
{
    AVFormatContext	*pFormatCtx;
    int				i, videoindex;
    AVCodecContext	*pCodecCtx;
    AVCodec			*pCodec;
    AVFrame	*pFrame,*pFrameYUV;
    unsigned char *out_buffer;
    AVPacket *packet;
    int y_size;
    int ret, got_picture;
    int pts=0;
    struct SwsContext *img_convert_ctx;

    char filepath[]="rtsp://admin:gdutcs719@10.21.71.146:554/Streaming/Channels/101";
    //SDL---------------------------
    int screen_w=0,screen_h=0;
    SDL_Window *screen;
    SDL_Renderer* sdlRenderer;
    SDL_Texture* sdlTexture;
    SDL_Rect sdlRect;

    FILE *fp_yuv;

    av_register_all();
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();

    if(avformat_open_input(&pFormatCtx,filepath,NULL,NULL)!=0){
        printf("Couldn't open input stream.\n");
        return -1;
    }
    if(avformat_find_stream_info(pFormatCtx,NULL)<0){
        printf("Couldn't find stream information.\n");
        return -1;
    }
    videoindex=-1;
    for(i=0; i<pFormatCtx->nb_streams; i++)
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
            videoindex=i;
            break;
        }
    if(videoindex==-1){
        printf("Didn't find a video stream.\n");
        return -1;
    }

    pCodecCtx=pFormatCtx->streams[videoindex]->codec;
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL){
        printf("Codec not found.\n");
        return -1;
    }
    if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
        printf("Could not open codec.\n");
        return -1;
    }

    pFrame=av_frame_alloc();
    pFrameYUV=av_frame_alloc();


    out_buffer=(unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P,  pCodecCtx->width, pCodecCtx->height,1));
    av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize,out_buffer,
        AV_PIX_FMT_YUV420P,pCodecCtx->width, pCodecCtx->height,1);

    packet=(AVPacket *)av_malloc(sizeof(AVPacket));
    //Output Info-----------------------------
    printf("--------------- File Information ----------------\n");
    av_dump_format(pFormatCtx,0,filepath,0);
    printf("-------------------------------------------------\n");
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
        pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

#if OUTPUT_YUV420P
    fp_yuv=fopen("output.yuv","wb+");
#endif

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        printf( "Could not initialize SDL - %s\n", SDL_GetError());
        return -1;
    }

    screen_w = pCodecCtx->width;
    screen_h = pCodecCtx->height;
    //SDL 2.0 Support for multiple windows
    screen = SDL_CreateWindow("Simplest ffmpeg player's Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        screen_w, screen_h,
        SDL_WINDOW_OPENGL);

    if(!screen) {
        printf("SDL: could not create window - exiting:%s\n",SDL_GetError());
        return -1;
    }

    sdlRenderer = SDL_CreateRenderer(screen, -1, 0);
    //IYUV: Y + U + V  (3 planes)
    //YV12: Y + V + U  (3 planes)
    sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,pCodecCtx->width,pCodecCtx->height);

    sdlRect.x=0;
    sdlRect.y=0;
    sdlRect.w=screen_w;
    sdlRect.h=screen_h;

    //SDL End----------------------
    while(av_read_frame(pFormatCtx, packet)>=0){
        if(packet->stream_index==videoindex){
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
            if(ret < 0){
                printf("Decode Error.\n");
                return -1;
            }
            if(got_picture){

                SDL_UpdateYUVTexture(sdlTexture, &sdlRect,
                pFrame->data[0], pFrame->linesize[0],
                pFrame->data[1], pFrame->linesize[1],
                pFrame->data[2], pFrame->linesize[2]);

                SDL_RenderClear( sdlRenderer );
                SDL_RenderCopy( sdlRenderer, sdlTexture,  NULL, &sdlRect);

                if(pts!=0&&pts<pFrame->pts){
                    std::cout<<(pFrame->pts-pts)/1000<<std::endl;

                    SDL_Delay((pFrame->pts-pts)/1000);
                }
                pts=pFrame->pts;

                SDL_RenderPresent( sdlRenderer );

            }
        }
        av_free_packet(packet);
    }
    //flush decoder
    //FIX: Flush Frames remained in Codec
    while (1) {
        ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
        if (ret < 0)
            break;
        if (!got_picture)
            break;
        sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
            pFrameYUV->data, pFrameYUV->linesize);
#if OUTPUT_YUV420P
        int y_size=pCodecCtx->width*pCodecCtx->height;
        fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);    //Y
        fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);  //U
        fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);  //V
#endif
        //SDL---------------------------
        SDL_UpdateTexture( sdlTexture, &sdlRect, pFrameYUV->data[0], pFrameYUV->linesize[0] );
        SDL_RenderClear( sdlRenderer );
        SDL_RenderCopy( sdlRenderer, sdlTexture,  NULL, &sdlRect);
        SDL_RenderPresent( sdlRenderer );
        //SDL End-----------------------
        //Delay 40ms
        SDL_Delay(40);
    }

    sws_freeContext(img_convert_ctx);

#if OUTPUT_YUV420P
    fclose(fp_yuv);
#endif

    SDL_Quit();

    av_frame_free(&pFrameYUV);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);

    return 0;
}

*/
#include <stdio.h>
#include <iostream>
using namespace std;
#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "SDL2/SDL.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <SDL2/SDL.h>
#ifdef __cplusplus
};
#endif
#endif

//Refresh Event
#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)

#define SFM_BREAK_EVENT  (SDL_USEREVENT + 2)

int thread_exit=0;
int thread_pause=0;

int sfp_refresh_thread(void *opaque){
    thread_exit=0;
    thread_pause=0;

    while (!thread_exit) {
        if(!thread_pause){
            SDL_Event event;
            event.type = SFM_REFRESH_EVENT;
            SDL_PushEvent(&event);
        }
        SDL_Delay(2);
    }
    thread_exit=0;
    thread_pause=0;
    //Break
    SDL_Event event;
    event.type = SFM_BREAK_EVENT;
    SDL_PushEvent(&event);

    return 0;
}


int main(int argc, char* argv[])
{

    AVFormatContext	*pFormatCtx;
    int				i, videoindex;
    AVCodecContext	*pCodecCtx;
    AVCodec			*pCodec;
    AVFrame	*pFrame;
    AVPacket *packet;
    int ret, got_picture;

    //------------SDL----------------
    int screen_w,screen_h;
    SDL_Window *screen;
    SDL_Renderer* sdlRenderer;
    SDL_Texture* sdlTexture;
    SDL_Rect sdlRect;
    SDL_Thread *video_tid;
    SDL_Event event;

    //char filepath[]="bigbuckbunny_480x272.h265";
    char filepath[]=
//            "http://127.0.0.1/record/live/0/2019-01-23/11-00-53.mp4";
//            "rtmp://127.0.0.1/live/0";
            "rtsp://admin:gdutcs719@10.21.71.146:554/Streaming/Channels/101";
//    "rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov";

    av_register_all();
    avformat_network_init();
    pFormatCtx=avformat_alloc_context();

    av_register_all();
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();

    if(avformat_open_input(&pFormatCtx,filepath,NULL,NULL)!=0){
        printf("Couldn't open input stream.\n");
        return -1;
    }
    if(avformat_find_stream_info(pFormatCtx,NULL)<0){
        printf("Couldn't find stream information.\n");
        return -1;
    }
    videoindex=-1;
    for(i=0; i<pFormatCtx->nb_streams; i++)
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
            videoindex=i;
            break;
        }
    if(videoindex==-1){
        printf("Didn't find a video stream.\n");
        return -1;
    }
    cout<<pFormatCtx->duration<<endl;
    pCodecCtx=pFormatCtx->streams[videoindex]->codec;
    cout<<pCodecCtx->gop_size<<endl;
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL){
        printf("Codec not found.\n");
        return -1;
    }
    if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
        printf("Could not open codec.\n");
        return -1;
    }
    pFrame=av_frame_alloc();

    //Output Info-----------------------------
    printf("---------------- File Information ---------------\n");
    av_dump_format(pFormatCtx,0,filepath,0);

    printf("-------------------------------------------------\n");


    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        printf( "Could not initialize SDL - %s\n", SDL_GetError());
        return -1;
    }
    //SDL 2.0 Support for multiple windows
    screen_w = pCodecCtx->width;
    screen_h = pCodecCtx->height;
    screen = SDL_CreateWindow("Simplest ffmpeg player's Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        screen_w, screen_h,SDL_WINDOW_OPENGL);

    if(!screen) {
        printf("SDL: could not create window - exiting:%s\n",SDL_GetError());
        return -1;
    }
    sdlRenderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
    sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,pCodecCtx->width,pCodecCtx->height);

    sdlRect.x=0;
    sdlRect.y=0;
    sdlRect.w=screen_w;
    sdlRect.h=screen_h;

    packet=(AVPacket *)av_malloc(sizeof(AVPacket));

    video_tid = SDL_CreateThread(sfp_refresh_thread,NULL,NULL);
    //------------SDL End------------
    //Event Loop

    for (;;) {
        //Wait
        SDL_WaitEvent(&event);
        if(event.type==SFM_REFRESH_EVENT){
            while(1){
                if(av_read_frame(pFormatCtx, packet)<0)
                    thread_exit=1;

                if(packet->stream_index==videoindex)
                    break;
            }
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
            if(ret < 0){
                printf("Decode Error.\n");
                return -1;
            }
            if(got_picture){
                SDL_UpdateYUVTexture(sdlTexture, nullptr,
                                                 pFrame->data[0], pFrame->linesize[0],
                                                 pFrame->data[1], pFrame->linesize[1],
                                                 pFrame->data[2], pFrame->linesize[2]);

                SDL_RenderClear( sdlRenderer );
                SDL_RenderCopy( sdlRenderer, sdlTexture, NULL, NULL);
                SDL_RenderPresent( sdlRenderer );
                //SDL End-----------------------
            }
            av_free_packet(packet);
        }else if(event.type==SDL_KEYDOWN){
            //Pause
            if(event.key.keysym.sym==SDLK_SPACE)
                thread_pause=!thread_pause;
        }else if(event.type==SDL_QUIT){
            thread_exit=1;
        }else if(event.type==SFM_BREAK_EVENT){
            break;
        }

    }


    SDL_Quit();
    //--------------
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);

    return 0;
}

/*//
// Created by Kenny Lau on 2019-01-18.
// TODO: 拉rtsp流，然后以rtmp流推送服务器；
//
#include <iostream>
#include "SDL2/SDL.h"
using namespace std;
#ifdef __cplusplus
extern "C" {
#endif
// 引入编码器
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
// 引入时间
#include "libavutil/time.h"
#include "libswscale/swscale.h"
#include "libavcodec/avcodec.h"
#ifdef __cplusplus
}
#endif

const int bpp=12;

int screen_w=500,screen_h=500;
const int pixel_w=320,pixel_h=180;
SDL_Texture *_texture;
SDL_Renderer *_render;
 SDL_Window *_win;

int init(){
    if(SDL_Init(SDL_INIT_VIDEO)){
        cout<<"Initialize SDL failed: "<<SDL_GetError()<<endl;
    }

    //SDL 2.0 Support for multiple windows
    _win = SDL_CreateWindow("Simplest Video Play SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        screen_w, screen_h,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    if(!_win) {
        printf("SDL: could not create window - exiting:%s\n",SDL_GetError());
        return -1;
    }


    _render = SDL_CreateRenderer(_win, -1, SDL_RENDERER_ACCELERATED);
}


void destory(){
    if (_texture) {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }
    if (_render) {
        SDL_DestroyRenderer(_render);
        _render = nullptr;
    }
    if (_win) {
        SDL_DestroyWindow(_win);
        _win = nullptr;
    }
}


int main(){
    AVFormatContext *pFormatCtx;
    int videoIndex;
    AVCodecContext *pCodeCtx;
    AVCodec *pCodec;
    AVFrame *pFrame,*pFrameYUV;
    uint8_t *out_buffer;
    AVPacket *packet;
    int y_size;
    int ret,got_picture;
    struct SwsContext *img_convert_ctx;

    string str="rtsp://admin:gdutcs719@10.21.71.146:554/Streaming/Channels/101";

    av_register_all();
    avformat_network_init();
    pFormatCtx=avformat_alloc_context();

    ret=avformat_open_input(&pFormatCtx,str.data(),NULL,NULL);
    if(ret!=0){
        cout<<"play video failed:";
        return AVERROR(ret);
    }

    ret=avformat_find_stream_info(pFormatCtx,NULL);
    if(ret!=0){
        cout<<"play video failed:";
        return AVERROR(ret);
    }

    videoIndex=-1;
    for(int i=0;i<pFormatCtx->nb_streams;++i){
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
            videoIndex=i;
            break;
        }
    }
    cout<<videoIndex<<endl;
    if(videoIndex==-1){
        cout<<"Didn't find a video stream"<<endl;
        return -1;
    }

    pCodeCtx=pFormatCtx->streams[videoIndex]->codec;
    pCodec=avcodec_find_decoder(pCodeCtx->codec_id);
    if(pCodec==NULL){
        cout<<"Codec not found"<<endl;
        return -1;
    }

    if(avcodec_open2(pCodeCtx,pCodec,NULL)<0){
        cout<<"Could not open codec"<<endl;
        return -1;
    }

    pFrame=av_frame_alloc();
    pFrameYUV=av_frame_alloc();
    out_buffer=(uint8_t*)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodeCtx->width, pCodeCtx->height,1));
    av_image_fill_arrays(pFrameYUV->data,pFrameYUV->linesize,out_buffer,AV_PIX_FMT_YUV420P,
                         pCodeCtx->width,pCodeCtx->height,1);
    packet=(AVPacket*)av_malloc((sizeof (AVPacket)));
    img_convert_ctx=sws_getContext(pCodeCtx->width,pCodeCtx->height,pCodeCtx->pix_fmt,
                                   pCodeCtx->width,pCodeCtx->height,AV_PIX_FMT_YUV420P,SWS_BICUBIC,NULL,NULL,NULL);

    init();

    if (_render && !_texture) {
        _texture = SDL_CreateTexture(_render, SDL_PIXELFORMAT_IYUV,
                                     SDL_TEXTUREACCESS_STREAMING,
                                     pCodeCtx->width,
                                     pCodeCtx->height);
    }

    bool nQuit=true;
    while(nQuit){

       if(av_read_frame(pFormatCtx,packet)<0){
           cout<<"..."<<endl;
           continue;
       }

        ret=avcodec_decode_video2(pCodeCtx,pFrame,&got_picture,packet);
        if(ret<0){
            continue;
        }
        if(!got_picture){
            sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodeCtx->height,
                        pFrameYUV->data, pFrameYUV->linesize);

            SDL_UpdateYUVTexture(_texture, nullptr,
                                 pFrame->data[0], pFrame->linesize[0],
                                 pFrame->data[1], pFrame->linesize[1],
                                 pFrame->data[2], pFrame->linesize[2]);

            SDL_RenderClear(_render);
            SDL_RenderCopy(_render, _texture, nullptr, nullptr);
            SDL_RenderPresent(_render);
        }

        av_free_packet(packet);
    }
    destory();
    sws_freeContext(img_convert_ctx);
    av_frame_free(&pFrameYUV);
    av_frame_free(&pFrame);
    avcodec_close(pCodeCtx);
    avformat_close_input(&pFormatCtx);

    return 0;

}*/


