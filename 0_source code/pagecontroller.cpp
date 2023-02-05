#include "pagecontroller.h"
#include "ui_pagecontroller.h"
#include "pagemechanism.h"
#include <QHBoxLayout>

pageController::pageController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pageController)
{
    ui->setupUi(this);
    this->setWindowTitle("Go Bang Ver. 1.0");
    _uiPages = new QStackedWidget(this);
    _startPage = new Page_start(_uiPages);
    _charPage = new Page_char(_uiPages);
    _gamePage = new Page_game(_uiPages);
    _uiPages -> addWidget(_startPage);
    _uiPages -> addWidget(_charPage);
    _uiPages -> addWidget(_gamePage);
    QHBoxLayout *myLayout = new QHBoxLayout;
    myLayout->addWidget(_uiPages);
    setLayout(myLayout);
    _uiPages -> setCurrentWidget(_startPage);
    connect(_startPage,&PageMechanism::signal_changeToPageChar,
            this, &pageController::changeToPageChar);
    connect(_startPage,&Page_start::signal_setModeAsComputerMode,
            _charPage, &Page_char::setPageAsComputerMode);
    connect(_startPage,&Page_start::signal_setModeAsPlayersMode,
            _charPage, &Page_char::setPageAsPlayersMode);
    connect(_startPage,&PageMechanism::signal_exitGame,
            this, &pageController::exitGame);
    connect(_charPage,&PageMechanism::signal_changeToPageStart,
            this, &pageController::changeToPageStart);
    connect(_charPage,&PageMechanism::signal_changeToPageGame,
            this, &pageController::changeToPageGame);
    connect(_gamePage,&Page_game::signal_startOver,
            this, &pageController::startOver);
    connect(_gamePage,&Page_game::signal_backToStartPage,
            this, &pageController::changeToPageStart);

}

pageController::~pageController()
{
    delete ui;
}

void pageController::changeToPageStart()
{
    this->startOver();
    _uiPages -> setCurrentWidget(_startPage);
}

void pageController::changeToPageChar()
{
    _uiPages -> setCurrentWidget(_charPage);
}

void pageController::changeToPageGame()
{
    _uiPages -> setCurrentWidget(_gamePage);
    _gamePage->setIcon1(_charPage->getIconPlayer1());
    _gamePage->setIcon2(_charPage->getIconPlayer2());
    _gamePage->initializeHintOfCurrentPlayer();
    _gamePage->game->setGameMode(_startPage->passGameModetoGame());
}

void pageController::exitGame()
{
    this->close();
}

void pageController::startOver()
{
    this->_uiPages->removeWidget(_gamePage);
    delete this->_gamePage;
    this->_gamePage = new Page_game(_uiPages);
    this->_uiPages->addWidget(_gamePage);
    connect(_gamePage,&Page_game::signal_startOver,
            this, &pageController::startOver);
    connect(_gamePage,&PageMechanism::signal_changeToPageStart,
            this, &pageController::changeToPageStart);
    changeToPageGame();
}
