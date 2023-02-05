#ifndef COMPONENT_H
#define COMPONENT_H

#include <QObject>
#include <QWidget>
#include <definition.h>

class Chess: public QObject
{
private:
    Icon* icon;
public:
    explicit Chess(QObject *parent = nullptr);
    static QString addressOfIcon(Icon* icon);

};

class Player: public QObject
{
private:
    Chess* chess;
    QColor color;
public:
    explicit Player(QWidget *parent = nullptr);

};

class ChessBoard
{
private:
public:
    explicit ChessBoard();
    int const boardSize = 15;
    QPoint leftTopCornerCoor{240,10};

};





#endif // COMPONENT_H
