#include <iostream>
#include "sdl2widget.h"

using namespace std;

const int SCREEN_WIDTH=640;
const int SCREEN_HEIGHT=480;
const int TILE_SIZE=40;
std::string getResourcePath(const std::string &sudDir=""){
#ifdef _WIN32
    const char PATH_SEP='\\';
#else
    const char PATH_SEP='/';
#endif
    static string baseRes;
    if(baseRes.empty()){
        char* basePath=SDL_GetBasePath();
        if(basePath){
            baseRes=basePath;
            SDL_free(basePath);
        }
        else{
            std::cerr<<"Error getting resource path: "<<SDL_GetError()<<std::endl;
            return "";
        }
        size_t pos=baseRes.rfind("bin");
        baseRes=baseRes.substr(0,pos)+"res"+PATH_SEP;
    }
    return sudDir.empty()?baseRes:baseRes+sudDir+PATH_SEP;
}




void logSDLError(std::ostream &os,const std::string &msg){
    os<<msg<<" error: "<<IMG_GetError()<<std::endl;
}


SDL_Texture* loadTexture(const std::string &file,SDL_Renderer *ren){
    /*SDL_Texture *texture=nullptr;
    SDL_Surface *loadedImage=SDL_LoadBMP(file.c_str());
    if(loadedImage!=nullptr){
        texture=SDL_CreateTextureFromSurface(ren,loadedImage);
        SDL_FreeSurface(loadedImage);
        if(texture==nullptr){
            logSDLError(std::cout,"CreateTextureFromSurface");
        }
    }
    else{
        logSDLError(std::cout,"LoadBMP");
    }*/
    SDL_Texture *texture=IMG_LoadTexture(ren,file.c_str());
    if(texture==nullptr){
        logSDLError(std::cout,"LoadTexture");
    }
    return texture;
}

void renderTexture(SDL_Texture *tex,SDL_Renderer *ren,int x,int y, int w=0, int h=0){

    if(w==0||h==0){
        SDL_QueryTexture(tex, NULL, NULL, &w, &h);
        renderTexture(tex,ren,x,y,w,h);
    }
    else{
        SDL_Rect dst;
        dst.x=x;
        dst.y=y;
        dst.w=w;
        dst.h=h;
        SDL_RenderCopy(ren,tex,NULL,&dst);
    }
}

void cleanup(SDL_Window *win=nullptr, SDL_Renderer *ren=nullptr, SDL_Texture *image=nullptr,SDL_Texture *background=nullptr){
    if(background!=nullptr){
        SDL_DestroyTexture(background);
    }
    if(image!=nullptr){
        SDL_DestroyTexture(image);
    }
    if(ren!=nullptr){
        SDL_DestroyRenderer(ren);
    }
    if(win!=nullptr){
        SDL_DestroyWindow(win);
    }

}

SDL2Widget::SDL2Widget(QWidget* parent):QWidget(parent){
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0){
        logSDLError(std::cout,"SDL_Init");
        return;
    }

    if((IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG)!=IMG_INIT_PNG){
        logSDLError(std::cout,"IMG_Init");
        SDL_Quit();
        return;
    }

}

SDL2Widget::~SDL2Widget(){
    cleanup(window);
    IMG_Quit();
    SDL_Quit();
}


void SDL2Widget::paintEvent(QPaintEvent* event)
{

    std::cout<<"paint..."<<std::endl;
    window=SDL_CreateWindowFrom((void*)this->winId());
    if(window==nullptr){
        logSDLError(std::cout,"CreateWindow");
        return;
    }
    SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,
        SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(renderer==nullptr){

        return;
    }
    const std::string resPath=getResourcePath("Lesson4");
    SDL_Texture *image=loadTexture(resPath+"image.png",renderer);

    if(image==nullptr){

        return;
    }
    int iW, iH;
    SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
    int x = SCREEN_WIDTH / 2 - iW / 2;
    int y = SCREEN_HEIGHT / 2 - iH / 2;

    SDL_RenderClear(renderer);
    renderTexture(image,renderer,x,y);
    SDL_RenderPresent(renderer);
    SDL_Delay(100);
    cleanup( nullptr, renderer, image);
}
