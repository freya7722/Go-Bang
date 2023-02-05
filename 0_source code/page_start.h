#ifndef PAGE_START_H
#define PAGE_START_H

#include "pagemechanism.h"
#include <QWidget>
#include <definition.h>

namespace Ui {
class Page_start;
}

class Page_start : public PageMechanism
{
    Q_OBJECT
private:
    Ui::Page_start *ui;
    GameMode gameMode;
signals:
    void signal_setModeAsPlayersMode();
    void signal_setModeAsComputerMode();
public:
    explicit Page_start(QWidget *parent = nullptr);
    ~Page_start();

public slots:
    int passGameModetoGame();
};

#endif // PAGE_START_H
