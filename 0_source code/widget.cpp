#include "widget.h"
#include "./ui_widget.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QLabel>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
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
    ui->setupUi(this);

    connect(_startPage,&Page_start::changePageChar,
            this, &Widget::changePageChar);
    connect(_startPage,&Page_start::exitGame,
            this, &Widget::exitGame);
    connect(_charPage,&Page_char::changePageStart,
            this, &Widget::changePageStart);
    connect(_charPage,&Page_char::changePageGame,
            this, &Widget::changePageGame);
    connect(_gamePage,&Page_game::changePageStart,
            this, &Widget::changePageStart);
//    connect(_gamePage,&Page_game::mousePress,[this]{
//        plotChess(mapFromGlobal(QCursor::pos()));});
}
Widget::~Widget()
{
    delete ui;
}

void Widget::changePageChar()
{
    _uiPages -> setCurrentWidget(_charPage);
}

void Widget::changePageGame()
{
    _uiPages -> setCurrentWidget(_gamePage);
    _gamePage -> setIcon1(_charPage -> getIconPlayer1());
    _gamePage -> setIcon2(_charPage -> getIconPlayer2());

}

void Widget::changePageStart()
{
    _uiPages -> setCurrentWidget(_startPage);
}

void Widget::exitGame()
{
    this->close();
}

void Widget::plotChess(QPoint plotPos)
{
    QLabel *l = new QLabel(this); //使用label顯示剛剛在pixmap上的作圖
    QPixmap pm(":/char/Image/bad-pig-icon_s.png");//建立pixmap
    l->setGeometry(plotPos.x()-12,plotPos.y()-12,24,24);//label的大小要跟pixmap相當
    l->setPixmap(pm);
    l->show();//要設定顯示才會show
    qDebug() << "Press Location: UI::MapFromGlobal" << mapFromGlobal(QCursor::pos());
    qDebug() << "Press Location: UI::MapFromParent" << mapFromParent(QCursor::pos());
    qDebug() << "Press Location: UI::QCursor" << QCursor::pos();
}

