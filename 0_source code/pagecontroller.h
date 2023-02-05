#ifndef PAGECONTROLLER_H
#define PAGECONTROLLER_H
#include <page_start.h>
#include <page_game.h>
#include <page_char.h>

#include <QStackedWidget>
#include <QWidget>


namespace Ui {
class pageController;
}

class pageController : public QWidget
{
    Q_OBJECT

public:
    explicit pageController(QWidget *parent = nullptr);
    ~pageController();

private:
    Ui::pageController *ui;
    QStackedWidget *_uiPages;
    Page_start *_startPage;
    Page_char *_charPage;
    Page_game *_gamePage;

private slots:
    void changeToPageStart();
    void changeToPageChar();
    void changeToPageGame();
    void exitGame();
    void startOver();

};

#endif // PAGECONTROLLER_H
