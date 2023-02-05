#ifndef PAGE_CHAR_H
#define PAGE_CHAR_H

#include <QWidget>
#include <QPushButton>
#include <pagemechanism.h>
#include <definition.h>

namespace Ui {
class Page_char;
}

class Page_char : public PageMechanism
{
    Q_OBJECT

public:
    explicit Page_char(QWidget *parent = nullptr);
    ~Page_char();

public slots:
    void setPageAsComputerMode();
    void setPageAsPlayersMode();
private:
    Ui::Page_char *ui;
    Icon iconSelected = PIG;
    Icon icon_p1 = PIG;
    Icon icon_p2 = OWL;
    void changeIconSelected(QPushButton* iconButtonSelected);
    void changeCurrentPlayer(QPushButton* playerSelected);
    void setPlayerIcon();

public:
    int getIconPlayer1();
    int getIconPlayer2();

};

    //;

#endif // PAGE_CHAR_H
