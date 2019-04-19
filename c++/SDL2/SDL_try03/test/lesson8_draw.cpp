#include <SDL2/SDL.h>
#include <cstdio>
// 窗口
SDL_Window *Window = NULL;
// 渲染器
SDL_Renderer *Render = NULL;
// 渲染器


//初始化函数
int Init()
{
    SDL_Init(SDL_INIT_VIDEO);
    // 创建窗口
    Window = SDL_CreateWindow("SDL_Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 240, SDL_WINDOW_SHOWN);

    if (Window == NULL)
    {
        return -1;
    }

    // 创建渲染器
    Render = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);

    if ( Render == NULL)
    {
        return -2;
    }

    //设置渲染颜色 此处为白色
    SDL_SetRenderDrawColor(Render, 0xff, 0xff, 0xff, 0xff);
    return 0;
}


void FillCircle(int x0, int y0,int r=5){
    int x,y;
    int d;
    d=1-r;x=0;y=r;
    while(x<=y)
    {
        SDL_RenderDrawLine(Render,x0,y0,x+x0,y+y0);
        SDL_RenderDrawLine(Render,x0,y0,-x+x0,y+y0);
        SDL_RenderDrawLine(Render,x0,y0,x+x0,-y+y0);
        SDL_RenderDrawLine(Render,x0,y0,-x+x0,-y+y0);
        SDL_RenderDrawLine(Render,x0,y0,y+x0,x+y0);
        SDL_RenderDrawLine(Render,x0,y0,y+x0,-x+y0);
        SDL_RenderDrawLine(Render,x0,y0,-y+x0,x+y0);
        SDL_RenderDrawLine(Render,x0,y0,-y+x0,-x+y0);
        if(d<0)
        {
            d=d+2*x+3;
        }
        else
        {
            d=d+2*(x-y)+5;y=y-1;
        }
           x=x+1;
    }
}

void DrawCircle(int x0,int y0,int r)
{
    int x,y;
    int d;
    d=1-r;x=0;y=r;
    while(x<=y)
    {
        SDL_RenderDrawPoint(Render,x+x0,y+y0);
        SDL_RenderDrawPoint(Render,-x+x0,y+y0);
        SDL_RenderDrawPoint(Render,x+x0,-y+y0);
        SDL_RenderDrawPoint(Render,-x+x0,-y+y0);
        SDL_RenderDrawPoint(Render,y+x0,x+y0);
        SDL_RenderDrawPoint(Render,y+x0,-x+y0);
        SDL_RenderDrawPoint(Render,-y+x0,x+y0);
        SDL_RenderDrawPoint(Render,-y+x0,-x+y0);
    if(d<0)
    {
        d=d+2*x+3;
    }
    else
    {
        d=d+2*(x-y)+5;y=y-1;
    }
           x=x+1;
    }
}



//清理函数
void Destroy()
{
    SDL_DestroyRenderer(Render);
    SDL_DestroyWindow(Window);
    SDL_Quit();
}


int main(int args, char *argv[])
{
    if (Init() != 0)
        return -1;

    //清理渲染器
    SDL_RenderClear(Render);
    SDL_SetRenderDrawColor(Render, 0xff, 0, 0, 0xff);
    //画点
    SDL_RenderDrawPoint(Render, 50, 150);
    //画线
    SDL_RenderDrawLine(Render, 0, 0, 400, 240);
    // 将要被绘制的矩形方块
    SDL_Rect Rect1 = {100, 60, 200, 120};
    SDL_Rect Rect2 = {100, 60, 100, 60};
    SDL_Rect Rect3 = {200, 120, 100, 60};

    SDL_Point point={50,50};
    //设置渲染颜色 绿色
    SDL_SetRenderDrawColor(Render, 0, 0xff, 0, 0xff);
    //绘制矩形方框
    SDL_RenderDrawRect(Render, &Rect1);

    SDL_RenderDrawPoint(Render,10,100);

   SDL_SetRenderDrawColor(Render, 255, 0, 0, 0);
    DrawCircle(10,10,5);
    FillCircle(30,30,8);
    //设置渲染颜色 蓝色(带透明色，但是被忽略）
    SDL_SetRenderDrawColor(Render, 0, 0, 0xff, 0xFF / 2);
    //填充矩形
    SDL_RenderFillRect(Render, &Rect2);
    //设置支持 Alpha
    SDL_SetRenderDrawBlendMode(Render, SDL_BLENDMODE_BLEND);
    //设置渲染颜色 蓝色(带透明色）
    SDL_SetRenderDrawColor(Render, 0, 0, 0xff, 0xFF / 2);
    //填充矩形
    SDL_RenderFillRect(Render, &Rect3);
    //刷新窗口
    SDL_RenderPresent(Render);
    SDL_Delay(2000);
    Destroy();
    return 0;
}
