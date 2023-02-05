#ifndef PAGE_GAME_H
#define PAGE_GAME_H
#include "pagemechanism.h"
#include <game.h>
#include <definition.h>
#include <component.h>
#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
using namespace std;



namespace Ui {
class Page_game;
}

class Page_game : public PageMechanism
{
    Q_OBJECT

public:
    explicit Page_game(QWidget *parent = nullptr);
    ~Page_game();
    enum Icon{
        PIG,
        OWL,
        RUBBER,
        SNOWMAN
    };
private:
    Ui::Page_game *ui;
    ChessBoard* chessBoard;
    Icon currIcon;
    Icon icon_p1;
    Icon icon_p2;
    void mouseMoveEvent(QMouseEvent *event);//重寫mouseMoveEvent函数
    void mousePressEvent(QMouseEvent *event); //重寫mousePressEvent函數
    void plotChessBoard(ChessBoard* chessBoard);
    void plotChess(QPoint plotPos, Icon icon);
    QPoint transfromToGridPos(QPoint clickPos,ChessBoard* chessBoard);
    struct Coor_row_column transformToRowAndCol(QPoint gridPos, QPoint boardLeftTopCorner);
    void afterMousePressProcedure(QPoint clickPos);
    void changeHintOfCurrentPlayer();
    void plotComputerChess(Coor_row_column chessGridCoor);
    void sleep(int msec);
private slots:
    void popUpBox(QString text);
signals:
    void mouseMove(QMouseEvent *event);//自定移動鼠標要發送的信號
    void mousePress(QMouseEvent *event);//自定一個按下鼠標要發送的信號
    void signal_startOver();
    void signal_backToStartPage();
public:
   Game* game;
    void setIcon1(int icon);
    void setIcon2(int icon);
    void initializeHintOfCurrentPlayer();
};

#endif // PAGE_GAME_H
