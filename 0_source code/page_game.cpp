#include <cmath>
#include "page_game.h"
#include "ui_page_game.h"
#include "pagemechanism.h"
#include "component.h"
#include "definition.h"
#include "iconadress.h"

#include <QPainter>
#include <QHBoxLayout>
#include <QPalette>
#include <QString>
#include <QDateTime>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>


Page_game::Page_game(QWidget *parent) :
    PageMechanism(parent),
    ui(new Ui::Page_game)
{
    ui->setupUi(this);
    game = new Game(this,15); // boardSize is 15
    chessBoard = new ChessBoard();
    plotChessBoard(chessBoard);
    connect(ui->buttonBack,&QPushButton::clicked,
            this, &PageMechanism::signal_changeToPageStart);
    connect(ui->buttonBack,&QPushButton::clicked,[=]{
        //emit Page_game::signal_backToStartPage();
        popUpBox("Back to start page.");
    });
    connect(ui->buttonStartOver,&QPushButton::clicked,[=]{
        //emit Page_game::signal_startOver();
        popUpBox("Start over.");
    });
    connect(this,&Page_game::mousePress,[=]{
        afterMousePressProcedure(mapFromGlobal(QCursor::pos()));
    });
    connect(game,&Game::signal_plotComputerChess,
            this, &Page_game::plotComputerChess);

}

Page_game::~Page_game()
{
    delete ui;
}

void Page_game::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMove(event); //mouseMove被觸發時就釋放一個訊號
}

void Page_game::mousePressEvent(QMouseEvent *event)
{
    emit mousePress(event); //mousePress被觸發時就釋放一個訊號
}



void Page_game::plotChessBoard(ChessBoard* chessBoard)
{
    QLabel *l1 = new QLabel(this); //使用label顯示剛剛在pixmap上的作圖
    int boardWidth = 40*(chessBoard->boardSize-1)+64;
    l1->setGeometry(0,0,boardWidth,boardWidth);//label的大小要跟pixmap相當
    l1->setStyleSheet("QLabel { background-color : #a5824a; }");
    QPoint location = chessBoard->leftTopCornerCoor;
    l1->move(location.x(),location.y());
    for(int i = 0; i<boardWidth; i++){
        QPixmap pm(boardWidth-60,boardWidth-60);//建立100x100 pixmap
        pm.fill(Qt::transparent);
        QPainter linePainter(&pm); // &　ｃａｌｌ　ｂｙ　ｒｅｆｅｒｅｎｃｅ,在pixmap上畫圖
        linePainter.setPen(QColor(248, 248, 248, 255));
        linePainter.drawLine(2, 2+i*40, boardWidth-64+2, 2+i*40); //drawEllipse(x,y,w,h):以物件(在此為label)左上角為原點,向右x向下y寬為w高為h的矩形填滿ellipse
        linePainter.end();
        QLabel *l = new QLabel(this);//使用label顯示剛剛在pixmap上的作圖
        l->setPixmap(pm);
        l->move(location.x()+30,location.y()+30);
        l->show();
    }
    for(int i = 0; i<boardWidth; i++){
        QPixmap pm(boardWidth-60,boardWidth-60);//建立100x100 pixmap
        pm.fill(Qt::transparent);
        QPainter linePainter(&pm); // &　ｃａｌｌ　ｂｙ　ｒｅｆｅｒｅｎｃｅ,在pixmap上畫圖
        linePainter.setPen(QColor(248, 248, 248, 255));
        linePainter.drawLine(2+i*40, 2, 2+i*40, boardWidth-64+2); //drawEllipse(x,y,w,h):以物件(在此為label)左上角為原點,向右x向下y寬為w高為h的矩形填滿ellipse
        linePainter.end();
        QLabel *l = new QLabel(this); //使用label顯示剛剛在pixmap上的作圖
        l->setPixmap(pm);
        l->move(location.x()+30,location.y()+30);
        l->show();
    }
}

void Page_game::plotChess(QPoint plotPos, Icon icon)
{
    int w = 36;
    int h = 36;
    QPixmap pm(w,h); //建立100x100 pixmap
    pm.fill(Qt::transparent);
    QPainter circlePainter(&pm); // &　ｃａｌｌ　ｂｙ　ｒｅｆｅｒｅｎｃｅ,在pixmap上畫圖

    if(game->getCurrTurns()==PLAYER1){
        circlePainter.setBrush(QColor(255, 255, 255, 255));
    }
    else if(game->getCurrTurns()==PLAYER2){
        circlePainter.setBrush(QColor(0, 0, 0, 255));
    }
    circlePainter.drawEllipse(1, 1, w-2, h-2); //drawEllipse(x,y,w,h):以物件(在此為label)左上角為原點,向右x向下y寬為w高為h的矩形填滿ellipse
    QLabel *l = new QLabel(this); //使用label顯示剛剛在pixmap上的作圖
    l->setGeometry(plotPos.x()-w/2,plotPos.y()-h/2,w,h);//label的大小要跟pixmap相當
    l->setPixmap(pm);
    l->show();
    QLabel *l2 = new QLabel(this); //使用label顯示剛剛在pixmap上的作圖
    QPixmap pm2( addressOfIcon(icon) );//建立pixmap
    l2->setGeometry(plotPos.x()-12,plotPos.y()-12,24,24);//label的大小要跟pixmap相當
    l2->setPixmap(pm2);
    l2->show();//要設定顯示才會show
    //qDebug() << "Press Location: Page_game::MapFromGlobal" << mapFromGlobal(QCursor::pos());
}

QPoint Page_game::transfromToGridPos(QPoint clickPos,ChessBoard* chessBoard)
{
    int gridTopLefCoor_x = chessBoard->leftTopCornerCoor.x() + 32; //最左上角網格的x座標
    int gridTopLefCoor_y = chessBoard->leftTopCornerCoor.y() + 32; //最左上角網格的y座標
    float delta_x, delta_y = 0; //xy方向的距離
    float distance = 0; //點擊點跟網格的距離
    for(int i=0; i<chessBoard->boardSize; i++){
        for(int j=0; j<chessBoard->boardSize; j++){
            delta_x = ( gridTopLefCoor_x + 40*i ) - clickPos.x();
            delta_y = ( gridTopLefCoor_y + 40*j ) - clickPos.y();
            distance = sqrt( pow(delta_x,2) + pow(delta_y,2) );
            if(distance <= 17){
                return {gridTopLefCoor_x + 40*i, gridTopLefCoor_y + 40*j};
            }
        }
    }
    return {-1,-1};
}

Coor_row_column Page_game::transformToRowAndCol(QPoint gridPos, QPoint boardLeftTopCorner)
{
    int row = (gridPos.y() - (boardLeftTopCorner.y() + 32))/40 ;
    int column = (gridPos.x() - (boardLeftTopCorner.x() + 32))/40 ;
    return {row, column};
}


void Page_game::afterMousePressProcedure(QPoint clickPos)
{
    int gridCoor_x = transfromToGridPos(clickPos,chessBoard).x();
    int gridCoor_y = transfromToGridPos(clickPos,chessBoard).y();
    int gridRow = transformToRowAndCol({gridCoor_x,gridCoor_y},chessBoard->leftTopCornerCoor).row;
    int gridColumn = transformToRowAndCol({gridCoor_x,gridCoor_y},chessBoard->leftTopCornerCoor).column;

    if(game->ableToSetChess({gridRow,gridColumn})==true){
        if(game->getCurrTurns()==PLAYER1){
            currIcon = icon_p1;
            //qDebug()<<"current player: player1/ current icon"<<currIcon;
        }
        else if(game->getCurrTurns()==PLAYER2){
            currIcon = icon_p2;
            //qDebug()<<"current player: player2/ current icon"<<currIcon;
        }
        plotChess({gridCoor_x,gridCoor_y},currIcon);
        if(game->moveOnToNextTurn({gridRow,gridColumn})==false){
            QLabel *label_gameStatus = new QLabel(this);
            //qDebug()<<game->getCurrBoardStatus();
            switch (game->getCurrBoardStatus()) {
            case PLAYER1_WIN:
                //qDebug()<<"player1_win";
                label_gameStatus->setText("Player1 Win!");
                break;
            case PLAYER2_WIN:
                label_gameStatus->setText("Player2 Win!");
                //qDebug()<<"player2_win";
                break;
            case TIE:
                label_gameStatus->setText("Tie");
                break;
            default:
                break;
            }
            label_gameStatus->move(400,50);
            label_gameStatus->show();
            label_gameStatus->setStyleSheet("font-size: 48px; color: red");
            label_gameStatus->adjustSize();
            QLabel *label_gameOver = new QLabel(this);
            label_gameOver->setText("Game Over");
            label_gameOver->move(400,110);
            label_gameOver->show();
            label_gameOver->setStyleSheet("font-size: 48px; color: blue");
            label_gameOver->adjustSize();
            QGraphicsDropShadowEffect *pLabelTextShadowEffect = new QGraphicsDropShadowEffect(this);
            QGraphicsDropShadowEffect *pLabelTextShadowEffect2 = new QGraphicsDropShadowEffect(this);
            pLabelTextShadowEffect->setColor(QColor("#ffffff"));
            pLabelTextShadowEffect->setBlurRadius(2);
            pLabelTextShadowEffect->setOffset(2, 2);
            pLabelTextShadowEffect2->setColor(QColor("#ffffff"));
            pLabelTextShadowEffect2->setBlurRadius(2);
            pLabelTextShadowEffect2->setOffset(2, 2);
            label_gameStatus->setGraphicsEffect(pLabelTextShadowEffect );
            label_gameOver->setGraphicsEffect(pLabelTextShadowEffect2 );
        }
        else{ // moveOnToNextTurn == true
            this->changeHintOfCurrentPlayer();
        }

    }
}

void Page_game::changeHintOfCurrentPlayer()
{
    if(game->getCurrTurns()==PLAYER1){
        currIcon = icon_p1;
        ui->label_turnHint->setText("It's Player1 Turn");
        ui->label_currPlayerPic->setStyleSheet("QLabel{background-color: white}");
    }
    else {// PLAYER2
        currIcon = icon_p2;
        ui->label_turnHint->setText("It's Player2 Turn");
        ui->label_currPlayerPic->setStyleSheet("QLabel{background-color: black}");
    }
    QPixmap pm(addressOfIcon_Large(currIcon));
    ui->label_currPlayerPic->setPixmap(pm);
    ui->label_currPlayerPic->setScaledContents(true);
    ui->label_turnHint->adjustSize();


}

void Page_game::plotComputerChess(Coor_row_column chessGridCoor)
{
    //this->plotChess(plotPos,icon_p2);
    QPoint gridPos;
    gridPos.setX( 40*chessGridCoor.column + (chessBoard->leftTopCornerCoor.x()+32) );
    gridPos.setY( 40*chessGridCoor.row + (chessBoard->leftTopCornerCoor.y()+32) );
    this->plotChess(gridPos,icon_p2);
}

void Page_game::sleep(int msec)
{
    QDateTime last = QDateTime::currentDateTime();
    QDateTime now;
    while(1){
        now = QDateTime::currentDateTime();
        if (last.msecsTo(now) >= msec){
            break;
        }
    }
}

void Page_game::popUpBox(QString text)
{
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setText(text);
    msgBox->setInformativeText("Are you sure?");
    msgBox->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox->setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox->exec();
    switch (ret) {
      case QMessageBox::Ok:
          if (text == "Back to start page.")
              emit Page_game::signal_backToStartPage();
          else // text == "Start over."
              emit Page_game::signal_startOver();
          break;
      case QMessageBox::Cancel:
          break;
      default:
          break;
    }
}


void Page_game::setIcon1(int icon)
{
    icon_p1 = (Icon)icon;
}
void Page_game::setIcon2(int icon)
{
    icon_p2 = (Icon)icon;
}

void Page_game::initializeHintOfCurrentPlayer()
{
    currIcon = icon_p1;
    ui->label_turnHint->setText("It's Player1 Turn");
    ui->label_currPlayerPic->setStyleSheet("QLabel{background-color: white}");
    QPixmap pm(addressOfIcon_Large(currIcon));
    ui->label_currPlayerPic->setPixmap(pm);
    ui->label_currPlayerPic->setScaledContents(true);
    ui->label_turnHint->adjustSize();
}

