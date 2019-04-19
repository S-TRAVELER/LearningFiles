#ifndef SDL2WIDGET_H
#define SDL2WIDGET_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <QWidget>
#include <QPaintEvent>

class SDL2Widget : public QWidget
{
    Q_OBJECT
public:
    SDL2Widget(QWidget* parent=0);
    ~SDL2Widget();

protected:
    void paintEvent(QPaintEvent* event);
    //svoid initialize();
private:
     //SDL_Surface *surface=nullptr;
    SDL_Window *window;
};


#endif
