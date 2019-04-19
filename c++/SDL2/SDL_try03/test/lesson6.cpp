#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

using namespace std;

const int SCREEN_WIDTH=640;
const int SCREEN_HEIGHT=480;

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
    os<<msg<<" error: "<<SDL_GetError()<<std::endl;
}


void renderTexture(SDL_Texture *tex,SDL_Renderer *ren,SDL_Rect dst, SDL_Rect* clip=nullptr){
    SDL_RenderCopy(ren,tex,clip,&dst);
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
                   SDL_Rect *clip=nullptr){
    SDL_Rect dst;
    dst.x=x;
    dst.y=y;
    if(clip!=nullptr){
        dst.w=clip->w;
        dst.h=clip->h;
    }
    else{
        SDL_QueryTexture(tex,NULL,NULL,&dst.w,&dst.h);
    }

    renderTexture(tex,ren,dst,clip);
}

SDL_Texture* renderText(const std::string &message, const std::string &fontFile, SDL_Color color,
                        int fontSize, SDL_Renderer *renderer){
    TTF_Font *font=TTF_OpenFont(fontFile.c_str(),fontSize);
    if(font==nullptr){
        logSDLError(std::cout,"TTF_OpenFont");
        return nullptr;
    }
    SDL_Surface *surf=TTF_RenderText_Blended(font,message.c_str(),color);
    if(surf==nullptr){
        TTF_CloseFont(font);
        logSDLError(std::cout,"TTF_RenderText");
        return nullptr;
    }
    SDL_Texture *texture=SDL_CreateTextureFromSurface(renderer,surf);
    if(texture==nullptr){
        logSDLError(std::cout,"CreateTexture");
    }
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
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

int main(int argc, char *argv[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0){
        logSDLError(std::cout,"SDL_Init");
        return 1;
    }
    if(TTF_Init()!=0){
        logSDLError(std::cout,"TTF_Init");
        SDL_Quit();
        return 1;
    }
    SDL_Window *window=SDL_CreateWindow("Lesson 5",100,100,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    if(window==nullptr){
        logSDLError(std::cout,"CreateWindow");
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,
        SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(renderer==nullptr){
        logSDLError(std::cout,"CreateRenderer");
        cleanup(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    const std::string resPath=getResourcePath("Lesson6");
    SDL_Color color={255,255,255,255};
    SDL_Texture *image=renderText("Loading...", "/usr/share/fonts/truetype/ttf-khmeros-core/KhmerOSsys.ttf", color, 16, renderer);

    if(image==nullptr){
        cleanup(window,renderer,image);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    int iW, iH;
    SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
    //iW>>=1;iH>>=1;
    int x = SCREEN_WIDTH / 2 - iW / 2;
    int y = SCREEN_HEIGHT / 2 - iH / 2;

    SDL_Event e;
    bool quit=false;
    while(!quit){
        while(SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT){
                quit=true;
            }
            if(e.type==SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE){
                    quit=true;
            }
        }
        SDL_RenderClear(renderer);
        renderTexture(image,renderer,x,y);
        SDL_RenderPresent(renderer);
    }
   cleanup( window, renderer, image);
   TTF_Quit();
   SDL_Quit();
   return 0;
}
