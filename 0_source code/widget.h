#ifndef WIDGET_H
#define WIDGET_H

#include <page_start.h>
#include <page_game.h>
#include <page_char.h>

#include <QStackedWidget>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QStackedWidget *_uiPages;
    Page_start *_startPage;
    Page_char *_charPage;
    Page_game *_gamePage;
    enum Icon{
        DUCK,
        APPLE,
        BEAR
    };
    Icon icon_p1 = DUCK;
    Icon icon_p2 = APPLE;
    void transferIconInfo(Icon icon1, Icon icon2);
private slots:
    void changePageStart();
    void changePageChar();
    void changePageGame();
    void exitGame();
    void plotChess(QPoint plotPos);

};
#endif // WIDGET_H
