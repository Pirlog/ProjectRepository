#ifndef START_H
#define START_H

#include<QGraphicsScene>
#include <QMovie>

class StartMenu: public QGraphicsScene{
public:
    StartMenu(unsigned int s_w, unsigned int s_h);
    void start_movie();
    void stop_movie();
    ~StartMenu();
private:
    QMovie *movie;
};



#endif // START_H
