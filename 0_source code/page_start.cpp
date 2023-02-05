#include "page_start.h"
#include "ui_page_start.h"
#include "pagemechanism.h"

Page_start::Page_start(QWidget *parent) :
    PageMechanism(parent),
    ui(new Ui::Page_start)
{
    ui->setupUi(this);
    connect(ui->buttonStart_onePlayer,&QPushButton::clicked,
            this, &PageMechanism::signal_changeToPageChar);
    connect(ui->buttonStart_onePlayer,&QPushButton::clicked,[=]{
        this->gameMode = COMPUTER_MODE;
        emit signal_changeToPageChar();
        emit signal_setModeAsComputerMode();
    });
    connect(ui->buttonStart_twoPlayers,&QPushButton::clicked,[=]{
        this->gameMode = PLAYERS_MODE;
        emit signal_changeToPageChar();
        emit signal_setModeAsPlayersMode();
    });
    connect(ui->buttonExit,&QPushButton::clicked,
            this, &PageMechanism::signal_exitGame);
}

Page_start::~Page_start()
{
    delete ui;
}

int Page_start::passGameModetoGame()
{
    return int(gameMode);
}










