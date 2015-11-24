#-------------------------------------------------
#
# Project created by QtCreator 2015-11-07T18:46:49
#
#-------------------------------------------------

QT       += core gui \
            multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mario
TEMPLATE = app


SOURCES += main.cpp \
    bad_flower.cpp \
    block.cpp \
    block_n.cpp \
    block_m.cpp \
    element.cpp \
    champi.cpp \
    character.cpp \
    coin.cpp \
    flower.cpp \
    game.cpp \
    ground.cpp \
    hero.cpp \
    koopa.cpp \
    loot.cpp \
    model.cpp \
    monster.cpp \
    peach_graphics_item.cpp \
    pipe.cpp \
    player_info.cpp \
    start.cpp \
    turnip.cpp \
    view.cpp \
    goomba.cpp \
    block_b.cpp \
    tile_factory.cpp \
    separator.cpp

HEADERS  += \
    bad_flower.h \
    block.h \
    block_n.h \
    block_m.h \
    element.h \
    champi.h \
    character.h \
    coin.h \
    flower.h \
    game.h \
    ground.h \
    hero.h \
    koopa.h \
    loot.h \
    model.h \
    monster.h \
    peach_graphics_item.h \
    pipe.h \
    player_info.h \
    start.h \
    turnip.h \
    view.h \
    goomba.h \
    block_b.h \
    tile_factory.h \
    separator.h

RESOURCES += \
    font/font.qrc \
    img/img.qrc \
    json/json.qrc \
    sounds/sounds.qrc
