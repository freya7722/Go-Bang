#include "game.h"
#include <QDebug>

void Game::updateChessState(Coor_row_column chessGridCoor)
{
    if (currTurn == PLAYER1){
        chessState[chessGridCoor.row][chessGridCoor.column] = chessStateOption[CHESS_PLAYER1];
    }
    else if (currTurn == PLAYER2){
        chessState[chessGridCoor.row][chessGridCoor.column] = chessStateOption[CHESS_PLAYER2];
    }
}

void Game::changeTurns()
{
    if (currTurn == PLAYER1){
        currTurn = PLAYER2;
        //qDebug()<<"changeTurn";
    }
    else if (currTurn == PLAYER2){
        currTurn = PLAYER1;
    }
}

int Game::smallComparison(int a, int b)
{
    if (a<b)
        return a;
    else
        return b;
}

Game::Game(QWidget *parent,int boardSize): QWidget(parent)
{
    this->boardSize = boardSize;
    std::fill(*chessState, *chessState + boardSize*boardSize, "empty");
}

Game::~Game()
{
    qDebug() << "game is dying";
}


bool Game::ableToSetChess(Coor_row_column currGridCoor)
{
    if(gameState == false){
        qDebug() << "gameState = false";
        return false;
    }
    else{ //gameState = true
        if(this->checkChessOccupation(currGridCoor)==true)
            return false;
        else
            this->updateChessState(currGridCoor);
            return true;
    }
}

bool Game::moveOnToNextTurn(Coor_row_column currGridCoor)
{
    if(gameMode == PLAYERS_MODE)
        return moveOnToNextTurn_playersMode(currGridCoor);
    else //COMPUTER_MODE
        return moveOnToNextTurn_computerMode(currGridCoor);
}

bool Game::moveOnToNextTurn_playersMode(Coor_row_column currGridCoor)
{
    if(this->checkVictory(currGridCoor)==NOTYET)
    {
        this->changeTurns();
        return true;
    }
    else
        return false;
}

bool Game::moveOnToNextTurn_computerMode(Coor_row_column currGridCoor)
{
    if(this->checkVictory(currGridCoor)==NOTYET)
    {
        this->changeTurns();
        Coor_row_column LocationToSetChess_computer = this->findBestSpottoSetChess_rev();
        this->updateChessState(LocationToSetChess_computer);
        emit signal_plotComputerChess(LocationToSetChess_computer);
        if(this->checkVictory(LocationToSetChess_computer)==NOTYET){
            this->changeTurns();
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

BoardJudge Game::checkVictory(Coor_row_column chessGridCoor)
{
    string currPlayerChess = chessState[chessGridCoor.row][chessGridCoor.column];
    int chessNumTop = chessGridCoor.row;
    int chessNumBottom = boardSize-chessGridCoor.row;
    int chessNumLeft = chessGridCoor.column;
    int chessNumRight = boardSize-chessGridCoor.column;
    if( chessNumTop > 4){
        chessNumTop = 4;
    }
    if( chessNumBottom > 4){
        chessNumBottom = 4;
    }
    if( chessNumRight > 4){
        chessNumRight = 4;
    }
    if( chessNumLeft > 4){
        chessNumLeft = 4;
    }
    int count = 0;
    //上到下
    for(int i = -chessNumTop; i <= chessNumBottom; i++ ){
        if( chessState[chessGridCoor.row+i][chessGridCoor.column] == currPlayerChess)
            count ++;

        else
            count = 0;
        if(count == 5){
            gameState = false;
        }
    }
    //左到右
    count = 0;
    for(int i = -chessNumLeft; i <= chessNumRight; i++ ){
        if( chessState[chessGridCoor.row][chessGridCoor.column+i] == currPlayerChess)
            count ++;

        else
            count = 0;
        if(count == 5){
            gameState = false;
        }
    }
    //左上到右下
    count = 0;
    int lowerBound = smallComparison(chessNumLeft, chessNumTop);
    int upperBound = smallComparison(chessNumRight, chessNumBottom);
    for(int i = -lowerBound; i <= upperBound; i++ ){
        if( chessState[chessGridCoor.row+i][chessGridCoor.column+i] == currPlayerChess)
            count ++;

        else
            count = 0;
        if(count == 5){
            gameState = false;
        }
    }
    //左下到右上
    count = 0;
    lowerBound = smallComparison(chessNumLeft, chessNumBottom);
    upperBound = smallComparison(chessNumRight, chessNumTop);
    for(int i = -lowerBound; i <= upperBound; i++ ){
        if( chessState[chessGridCoor.row-i][chessGridCoor.column+i] == currPlayerChess)
            count ++;

        else
            count = 0;
        if(count == 5){
            gameState = false;
        }
    }
    if(gameState == false){
        if(currPlayerChess == "chess_player1"){
            boardStatus = PLAYER1_WIN;
            return PLAYER1_WIN;
        }
        else if(currPlayerChess == "chess_player2"){
            boardStatus = PLAYER2_WIN;
            return PLAYER2_WIN;
        }
    }
    return NOTYET;
}



bool Game::checkChessOccupation(Coor_row_column currGridCoor)
{
    if(chessState[currGridCoor.row][currGridCoor.column] == chessStateOption[EMPTY]){
        qDebug() << "chessOccupation = false";
        return false;
    }
    else
        qDebug() << "chessOccupation = true";
        return true;
}

Coor_row_column Game::findBestSpottoSetChess_rev()
{
    string currPlayerChess;
    int chessNumTop;
    int chessNumBottom;
    int chessNumLeft;
    int chessNumRight;
    int maximumScore_attack = 0;
    int supposedRowtoSetChess_attack = 0;
    int supposedColumntoSetChess_attack = 0;
    int maximumScore_defense = 0;
    int supposedRowtoSetChess_defense = 0;
    int supposedColumntoSetChess_defense = 0;
    int lowerBound;
    int upperBound;
    for(int i=0;i<boardSize;i++){
        for(int j=0;j<boardSize;j++){
            if(chessState[i][j]==chessStateOption[EMPTY]){
                currPlayerChess = chessStateOption[CHESS_PLAYER2];
                chessNumTop = i; //row
                chessNumBottom = boardSize-i-1;
                chessNumLeft = j; //column
                chessNumRight = boardSize-j-1;
                if( chessNumTop > 4){
                    chessNumTop = 4;
                }
                if( chessNumBottom > 4){
                    chessNumBottom = 4;
                }
                if( chessNumRight > 4){
                    chessNumRight = 4;
                }
                if( chessNumLeft > 4){
                    chessNumLeft = 4;
                }

                //上到下(攻擊最大可能檢查)
                int count = 0;
                int count_poten = 0;//用來記錄如果所有空格都可以填，最大可能連線數
                bool isGoodSpot = false; //用來記錄是否是好的下棋點，最大可能連線數>=5
                for(int k = -chessNumTop; k <= chessNumBottom; k++ ){
                    if (chessState[i+k][j] == chessStateOption[EMPTY] || chessState[i+k][j] == chessStateOption[CHESS_PLAYER2]){
                        count_poten++;
                    }
                    else count_poten=0;
                    if(count_poten >= 5){
                        isGoodSpot = true;
                        break;
                    }
                }

                //上到下(攻擊檢查)
                if(isGoodSpot){
                    count = 0;
                    for(int k = -chessNumTop; k <= chessNumBottom; k++ ){
                        if(k>0 && chessState[i+k][j] == chessStateOption[CHESS_PLAYER1]) break;
                        if (k==0 or chessState[i+k][j] == chessStateOption[CHESS_PLAYER2])
                            count ++;
                        else
                            count = 0;
                        if(k>=0 && count > maximumScore_attack){
                            maximumScore_attack = count;
                            supposedRowtoSetChess_attack = i;
                            supposedColumntoSetChess_attack = j;
                            qDebug()<<"上到下Attack Score:["<<supposedRowtoSetChess_attack<<"]"<<"["<<supposedColumntoSetChess_attack<<"]"<<"/maximumScore: "<< maximumScore_attack;
                        }
                    }
                }

                //上到下(防禦最大可能姓檢查)
                isGoodSpot = false;
                count_poten = 0;
                for(int k = -chessNumTop; k <= chessNumBottom; k++ ){
                    if (chessState[i+k][j] == chessStateOption[EMPTY] || chessState[i+k][j] == chessStateOption[CHESS_PLAYER1]){
                        count_poten++;
                    }
                    else count_poten=0;
                    if(count_poten >= 5){
                        isGoodSpot = true;
                        break;
                    }
                }

                //上到下(防禦檢查)
                if(isGoodSpot){
                    count = 0;
                    for(int k = -chessNumTop; k <= chessNumBottom; k++ ){
                        if(k>0 && chessState[i+k][j] == chessStateOption[CHESS_PLAYER2]) break;
                        if (k==0 or chessState[i+k][j] == chessStateOption[CHESS_PLAYER1])
                            count ++;
                        else
                            count = 0;
                        if(k>=0 && count > maximumScore_defense){
                            maximumScore_defense = count;
                            supposedRowtoSetChess_defense = i;
                            supposedColumntoSetChess_defense = j;
                            qDebug()<<"上到下Defense Score:["<<supposedRowtoSetChess_defense<<"]"<<"["<<supposedColumntoSetChess_defense<<"]"<<"/maximumScore: "<< maximumScore_defense;
                        }
                    }
                }

                //左到右(攻擊最大可能姓檢查)
                isGoodSpot = false;
                count_poten = 0;
                for(int k = -chessNumLeft; k <= chessNumRight; k++ ){
                    if (chessState[i][j+k] == chessStateOption[EMPTY] || chessState[i][j+k] == chessStateOption[CHESS_PLAYER2]){
                        count_poten++;
                    }
                    else count_poten=0;
                    if(count_poten >= 5){
                        isGoodSpot = true;
                        break;
                    }
                }
                //左到右(攻擊檢查)
                if(isGoodSpot){
                    count = 0;
                    for(int k = -chessNumLeft; k <= chessNumRight; k++ ){
                        if(k>0 && chessState[i][j+k] == chessStateOption[CHESS_PLAYER1]) break;
                        if( k==0 or chessState[i][j+k] == chessStateOption[CHESS_PLAYER2])
                            count ++;
                        else
                            count = 0;
                        if(k>=0 && count > maximumScore_attack){
                            maximumScore_attack = count;
                            supposedRowtoSetChess_attack = i;
                            supposedColumntoSetChess_attack = j;
                            qDebug()<<"左到右:Attack Score:["<<supposedRowtoSetChess_attack<<"]"<<"["<<supposedColumntoSetChess_attack<<"]"<<"/maximumScore: "<< maximumScore_attack;
                        }
                    }
                }

                //左到右(防禦最大可能性檢查)
                isGoodSpot = false;
                count_poten = 0;
                for(int k = -chessNumLeft; k <= chessNumRight; k++ ){
                    if (chessState[i][j+k] == chessStateOption[EMPTY] || chessState[i][j+k] == chessStateOption[CHESS_PLAYER1]){
                        count_poten++;
                    }
                    else count_poten=0;
                    if(count_poten >= 5){
                        isGoodSpot = true;
                        break;
                    }
                }

                //左到右(防禦檢查)
                if(isGoodSpot){
                    count = 0;
                    for(int k = -chessNumLeft; k <= chessNumRight; k++ ){
                        if(k>0 && chessState[i][j+k] == chessStateOption[CHESS_PLAYER2]) break;
                        if( k==0 or chessState[i][j+k] == chessStateOption[CHESS_PLAYER1])
                            count ++;
                        else
                            count = 0;
                        if(k>=0 && count > maximumScore_defense){
                            maximumScore_defense = count;
                            supposedRowtoSetChess_defense = i;
                            supposedColumntoSetChess_defense = j;
                            qDebug()<<"左到右Defense Score:["<<supposedRowtoSetChess_defense<<"]"<<"["<<supposedColumntoSetChess_defense<<"]"<<"/maximumScore: "<< maximumScore_defense;
                        }
                    }
                }

                //左下到右上(攻擊最大可能姓檢查)
                lowerBound = smallComparison(chessNumLeft, chessNumBottom);
                upperBound = smallComparison(chessNumRight, chessNumTop);
                isGoodSpot = false;
                count_poten = 0;
                for(int k = -lowerBound; k <= upperBound; k++ ){
                    if (chessState[i-k][j+k] == chessStateOption[EMPTY] || chessState[i-k][j+k] == chessStateOption[CHESS_PLAYER2]){
                        count_poten++;
                    }
                    else count_poten=0;
                    if(count_poten >= 5){
                        isGoodSpot = true;
                        break;
                    }
                }

                //左下到右上(攻擊檢查)
                if(isGoodSpot){
                    count = 0;
                    for(int k = -lowerBound; k <= upperBound; k++ ){
                        if(k>0 && chessState[i-k][j+k] == chessStateOption[CHESS_PLAYER1]) break;
                        if( k==0 or chessState[i-k][j+k] == chessStateOption[CHESS_PLAYER2])
                            count ++;
                        else
                            count = 0;
                        if(k>=0 && count > maximumScore_attack){
                            maximumScore_attack = count;
                            supposedRowtoSetChess_attack = i;
                            supposedColumntoSetChess_attack = j;
                            qDebug()<<"左下到右上Attack Score:["<<supposedRowtoSetChess_attack<<"]"<<"["<<supposedColumntoSetChess_attack<<"]"<<"/maximumScore: "<< maximumScore_attack;
                            qDebug()<<"左下到右上Attack_k:"<<k<< " /i-k:"<<i-k<<" /j+k:"<<j+k;
                        }
                    }
                }

                //左下到右上(防禦最大可能姓檢查)
                isGoodSpot = false;
                count_poten = 0;
                for(int k = -lowerBound; k <= upperBound; k++ ){
                    if (chessState[i-k][j+k] == chessStateOption[EMPTY] || chessState[i-k][j+k] == chessStateOption[CHESS_PLAYER1]){
                        count_poten++;
                    }
                    else count_poten=0;
                    if(count_poten >= 5){
                        isGoodSpot = true;
                        break;
                    }
                }

                //左下到右上(防禦檢查)
                if(isGoodSpot){
                    count = 0;
                    for(int k = -lowerBound; k <= upperBound; k++ ){
                        if(k>0 && chessState[i-k][j+k] == chessStateOption[CHESS_PLAYER2]) break;
                        if( k==0 or chessState[i-k][j+k] == chessStateOption[CHESS_PLAYER1])
                            count ++;
                        else
                            count = 0;
                        if( k>=0 && count > maximumScore_defense){
                            maximumScore_defense = count;
                            supposedRowtoSetChess_defense = i;
                            supposedColumntoSetChess_defense = j;
                            qDebug()<<"左下到右上Defense Score:["<<supposedRowtoSetChess_defense<<"]"<<"["<<supposedColumntoSetChess_defense<<"]"<<"/maximumScore: "<< maximumScore_defense;
                            qDebug()<<"左下到右上Defense_k:"<<k<< " /i-k:"<<i-k<<" /j+k:"<<j+k;
                        }
                    }
                }


                //左上到右下(攻擊最大可能姓檢查)
                lowerBound = smallComparison(chessNumLeft, chessNumTop);
                upperBound = smallComparison(chessNumRight, chessNumBottom);
                isGoodSpot = false;
                count_poten = 0;
                for(int k = -lowerBound; k <= upperBound; k++ ){
                    if (chessState[i+k][j+k] == chessStateOption[EMPTY] || chessState[i+k][j+k] == chessStateOption[CHESS_PLAYER2]){
                        count_poten++;
                    }
                    else count_poten=0;
                    if(count_poten >= 5){
                        isGoodSpot = true;
                        break;
                    }
                }


                //左上到右下(攻擊檢查)
                if(isGoodSpot){
                    count = 0;
                    for(int k = -lowerBound; k <= upperBound; k++ ){
                        if(k>0 && chessState[i+k][j+k] == chessStateOption[CHESS_PLAYER1]) break;
                        if( k==0 or chessState[i+k][j+k] == chessStateOption[CHESS_PLAYER2])
                            count ++;
                        else
                            count = 0;
                        if(k>=0 && count > maximumScore_attack){
                            maximumScore_attack = count;
                            supposedRowtoSetChess_attack = i;
                            supposedColumntoSetChess_attack = j;
                            qDebug()<<"左上到右下Attack Score:["<<supposedRowtoSetChess_attack<<"]"<<"["<<supposedColumntoSetChess_attack<<"]"<<"/maximumScore: "<< maximumScore_attack;
                        }
                    }
                }
                //左上到右下(防禦最大可能姓檢查)
                isGoodSpot = false;
                count_poten = 0;
                for(int k = -lowerBound; k <= upperBound; k++ ){
                    if (chessState[i+k][j+k] == chessStateOption[EMPTY] || chessState[i+k][j+k] == chessStateOption[CHESS_PLAYER1]){
                        count_poten++;
                    }
                    else count_poten=0;
                    if(count_poten >= 5){
                        isGoodSpot = true;
                        break;
                    }
                }
                //左上到右下(防禦檢查)
                if(isGoodSpot){
                    count = 0;
                    for(int k = -lowerBound; k <= upperBound; k++ ){
                        if(k>0 && chessState[i+k][j+k] == chessStateOption[CHESS_PLAYER2]) break;
                        if( k==0 or chessState[i+k][j+k] == chessStateOption[CHESS_PLAYER1])
                            count ++;
                        else
                            count = 0;
                        if(k>=0 && count > maximumScore_defense){
                            maximumScore_defense = count;
                            supposedRowtoSetChess_defense = i;
                            supposedColumntoSetChess_defense = j;
                            qDebug()<<"左上到右下Defense Score:["<<supposedRowtoSetChess_defense<<"]"<<"["<<supposedColumntoSetChess_defense<<"]"<<"/maximumScore: "<< maximumScore_defense;
                        }
                    }
                }
            }
        }
    }
    qDebug()<<"~    Attack Score:["<<supposedRowtoSetChess_attack<<"]"<<"["<<supposedColumntoSetChess_attack<<"]"<<"/maximumScore: "<< maximumScore_attack;
    qDebug()<<"~   Defense Score:["<<supposedRowtoSetChess_defense<<"]"<<"["<<supposedColumntoSetChess_defense<<"]"<<"/maximumScore: "<< maximumScore_defense;
    if (maximumScore_attack >= maximumScore_defense)
        return {supposedRowtoSetChess_attack, supposedColumntoSetChess_attack};
    else//(maximumScore_attack < maximumScore_defense)
        return {supposedRowtoSetChess_defense, supposedColumntoSetChess_defense};
}

Turns Game::getCurrTurns()
{
    return currTurn;
}

BoardJudge Game::getCurrBoardStatus()
{
    return boardStatus;
}

void Game::setGameMode(int gameMode)
{
    this->gameMode = GameMode(gameMode);
}


