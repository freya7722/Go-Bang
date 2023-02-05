#include "page_char.h"
#include "ui_page_char.h"
#include "pagemechanism.h"
#include "iconadress.h"
#include <QDebug>

Page_char::Page_char(QWidget *parent) :
    PageMechanism(parent),
    ui(new Ui::Page_char)
{
    ui->setupUi(this);
//    this->centralWidget()->setStyleSheet("background-image:url(\"bkg.jpg\"); background-position: center; ");
//    this->setStyleSheet( "background-image: url(qrc:/picture/Image/chessBoard.PNG);");
    ui->label_Player1Pic->setStyleSheet("QLabel{background-color: white}");
    ui->label_Player2Pic->setStyleSheet("QLabel{background-color: black}");
    connect(ui->buttonBack,&QPushButton::clicked,
            this, &PageMechanism::signal_changeToPageStart);
    connect(ui->buttonGameStart,&QPushButton::clicked,
           this, &PageMechanism::signal_changeToPageGame);

//    connect(ui->buttonSelect,&QPushButton::clicked,
//            this, &Page_char::setPlayerIcon);
    connect(ui->iconButton_pig,&QPushButton::clicked,[=]{
        changeIconSelected(ui->iconButton_pig);});
    connect(ui->iconButton_owl,&QPushButton::clicked,[=]{
        changeIconSelected(ui->iconButton_owl);});
    connect(ui->iconButton_rubber,&QPushButton::clicked,[=]{
        changeIconSelected(ui->iconButton_rubber);});
    connect(ui->iconButton_snowman,&QPushButton::clicked,[=]{
        changeIconSelected(ui->iconButton_snowman);});
    connect(ui->iconButton_cat_banjo,&QPushButton::clicked,[=]{
        changeIconSelected(ui->iconButton_cat_banjo);});
    connect(ui->iconButton_cat_lady,&QPushButton::clicked,[=]{
        changeIconSelected(ui->iconButton_cat_lady);});
    connect(ui->iconButton_cat_food,&QPushButton::clicked,[=]{
        changeIconSelected(ui->iconButton_cat_food);});
    connect(ui->iconButton_cat_cupie,&QPushButton::clicked,[=]{
        changeIconSelected(ui->iconButton_cat_cupie);});
    connect(ui->iconButton_banana,&QPushButton::clicked,[=]{
        changeIconSelected(ui->iconButton_banana);});
}

Page_char::~Page_char()
{
    delete ui;
}

void Page_char::setPageAsComputerMode()
{
    this->ui->label_p1->setText("Player");
    this->ui->label_p2->setText("Computer");
    this->ui->radioButton_player2->setVisible(false);
}

void Page_char::setPageAsPlayersMode()
{
    this->ui->label_p1->setText("Player 1");
    this->ui->label_p2->setText("Player 2");
    this->ui->radioButton_player2->setVisible(true);
}

void Page_char::changeIconSelected(QPushButton *iconButtonSelected)
{
    if(iconButtonSelected->objectName()=="iconButton_pig"){
        iconSelected = PIG;
        //qDebug() << "Pig button clicked, iconSelected = " << iconSelected;
    }
    else if(iconButtonSelected->objectName()=="iconButton_owl"){
        iconSelected = OWL;
        //qDebug() << "Owl button clicked, iconSelected = " << iconSelected;
    }
    else if(iconButtonSelected->objectName()=="iconButton_rubber"){
        iconSelected = RUBBER;
        //qDebug() << "Rubber button clicked, iconSelected = " << iconSelected;
    }
    else if(iconButtonSelected->objectName()=="iconButton_snowman"){
        iconSelected = SNOWMAN;
        //qDebug() << "Snowman button clicked, iconSelected = " << iconSelected;
    }
    else if(iconButtonSelected->objectName()=="iconButton_cat_banjo"){
        iconSelected = CAT_BANJO;
    }
    else if(iconButtonSelected->objectName()=="iconButton_cat_cupie"){
        iconSelected = CAT_CUPIE;
    }
    else if(iconButtonSelected->objectName()=="iconButton_cat_food"){
        iconSelected = CAT_FOOD;
    }
    else if(iconButtonSelected->objectName()=="iconButton_cat_lady"){
        iconSelected = CAT_LADIE;
    }
    else if(iconButtonSelected->objectName()=="iconButton_banana"){
        iconSelected = BANANA;
    }

    if(ui->radioButton_player1->isChecked()){
        icon_p1 = iconSelected;
        QPixmap pm(addressOfIcon_Large(icon_p1));
        ui->label_Player1Pic->setPixmap(pm);
        ui->label_Player1Pic->setScaledContents(true);
        qDebug() << "Set Player1: iconSelected = " << icon_p1;
    }
    else if(ui->radioButton_player2->isChecked()){
        icon_p2 = iconSelected;
        QPixmap pm(addressOfIcon_Large(icon_p2));
        ui->label_Player2Pic->setPixmap(pm);
        ui->label_Player2Pic->setScaledContents(true);
        qDebug() << "Set Player2: iconSelected = " << icon_p2;
    }
}





void Page_char::setPlayerIcon()
{
    if(ui->radioButton_player1->isChecked()){
        icon_p1 = iconSelected;
        qDebug() << "Set Player1: iconSelected = " << icon_p1;
    }
    else if(ui->radioButton_player2->isChecked()){
        icon_p2 = iconSelected;
        qDebug() << "Set Player1: iconSelected = " << icon_p2;
    }
}

int Page_char::getIconPlayer1()
{
    return (int)icon_p1;
}
int Page_char::getIconPlayer2()
{
    return (int)icon_p2;
}






