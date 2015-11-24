#include "start.h"
#include <QLabel>


StartMenu::StartMenu(unsigned int s_w, unsigned int s_h){
    setSceneRect(0,0,s_w,s_h);
    QLabel *l= new QLabel();
    l->setGeometry(0,0,s_w ,s_h);
    l->setScaledContents(true);
    movie = new QMovie(":/peach/peach.gif");
    l->setMovie(movie);
    addWidget(l);
}

void StartMenu::start_movie(){
    movie->start();
}

void StartMenu::stop_movie(){
    movie->stop();
}

StartMenu::~StartMenu(){
    delete movie;
}

