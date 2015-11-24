#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "model.h"
#include "view.h"
#include "game.h"
#include "ground.h"
#include <QVector>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QPixmap>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Model *model = new Model();
    View *view = new View(8450,800,500,30);
    Game *game = new Game(model, view);

    view->setFlag(QGraphicsItem::ItemIsFocusable);
    view->setFocus();

    game -> send_elem_to_map();
    return a.exec();
}
