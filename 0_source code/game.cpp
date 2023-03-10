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

int Game::scoreCaculation(string status)
{
    int score = 0;
    //??????scoreCard?????????status??????scoreCard?????????????????????????????????????????????score???
    for(auto iter=chessScore.begin(); iter!= chessScore.end(); iter++){
        if(strstr (status.c_str(),iter->first.c_str())){
            score += iter->second;
        }
    }
    return score;
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
    //?????????
    for(int i = -chessNumTop; i <= chessNumBottom; i++ ){
        if( chessState[chessGridCoor.row+i][chessGridCoor.column] == currPlayerChess)
            count ++;

        else
            count = 0;
        if(count == 5){
            gameState = false;
        }
    }
    //?????????
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
    //???????????????
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
    //???????????????
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

Coor_row_column Game::findBestSpottoSetChess()
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

                //?????????(????????????????????????)
                int count = 0;
                int count_poten = 0;//??????????????????????????????????????????????????????????????????
                bool isGoodSpot = false; //????????????????????????????????????????????????????????????>=5
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

                //?????????(????????????)
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
                            qDebug()<<"?????????Attack Score:["<<supposedRowtoSetChess_attack<<"]"<<"["<<supposedColumntoSetChess_attack<<"]"<<"/maximumScore: "<< maximumScore_attack;
                        }
                    }
                }

                //?????????(???????????????????????????)
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

                //?????????(????????????)
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
                            qDebug()<<"?????????Defense Score:["<<supposedRowtoSetChess_defense<<"]"<<"["<<supposedColumntoSetChess_defense<<"]"<<"/maximumScore: "<< maximumScore_defense;
                        }
                    }
                }

                //?????????(???????????????????????????)
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
                //?????????(????????????)
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
                            qDebug()<<"?????????:Attack Score:["<<supposedRowtoSetChess_attack<<"]"<<"["<<supposedColumntoSetChess_attack<<"]"<<"/maximumScore: "<< maximumScore_attack;
                        }
                    }
                }

                //?????????(???????????????????????????)
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

                //?????????(????????????)
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
                            qDebug()<<"?????????Defense Score:["<<supposedRowtoSetChess_defense<<"]"<<"["<<supposedColumntoSetChess_defense<<"]"<<"/maximumScore: "<< maximumScore_defense;
                        }
                    }
                }

                //???????????????(???????????????????????????)
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

                //???????????????(????????????)
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
                            qDebug()<<"???????????????Attack Score:["<<supposedRowtoSetChess_attack<<"]"<<"["<<supposedColumntoSetChess_attack<<"]"<<"/maximumScore: "<< maximumScore_attack;
                            qDebug()<<"???????????????Attack_k:"<<k<< " /i-k:"<<i-k<<" /j+k:"<<j+k;
                        }
                    }
                }

                //???????????????(???????????????????????????)
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

                //???????????????(????????????)
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
                            qDebug()<<"???????????????Defense Score:["<<supposedRowtoSetChess_defense<<"]"<<"["<<supposedColumntoSetChess_defense<<"]"<<"/maximumScore: "<< maximumScore_defense;
                            qDebug()<<"???????????????Defense_k:"<<k<< " /i-k:"<<i-k<<" /j+k:"<<j+k;
                        }
                    }
                }


                //???????????????(???????????????????????????)
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


                //???????????????(????????????)
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
                            qDebug()<<"???????????????Attack Score:["<<supposedRowtoSetChess_attack<<"]"<<"["<<supposedColumntoSetChess_attack<<"]"<<"/maximumScore: "<< maximumScore_attack;
                        }
                    }
                }
                //???????????????(???????????????????????????)
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
                //???????????????(????????????)
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
                            qDebug()<<"???????????????Defense Score:["<<supposedRowtoSetChess_defense<<"]"<<"["<<supposedColumntoSetChess_defense<<"]"<<"/maximumScore: "<< maximumScore_defense;
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
    int score_attack = 0;
    int score_defense = 0;
    for(int i=0;i<boardSize;i++){
        for(int j=0;j<boardSize;j++){
            score_attack = 0;
            score_defense = 0;
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

                //?????????(????????????????????????)
                int count_poten = 0;//??????????????????????????????????????????????????????????????????
                bool isGoodSpot = false; //????????????????????????????????????????????????????????????>=5
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

                //?????????(?????????????????????????????????)
                if(isGoodSpot){
                    string status;
                    for(int k = -chessNumTop; k <= chessNumBottom; k++ ){
                        if(chessState[i+k][j] == chessStateOption[CHESS_PLAYER2] || k==0) status.append("1");
                        else if(chessState[i+k][j] == chessStateOption[CHESS_PLAYER1]) status.append("2");
                        else status.append("0"); //empty&&k!=0
                    }
                    score_attack += scoreCaculation(status);
                }


                //?????????(???????????????????????????)
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

                //?????????(????????????)
                if(isGoodSpot){
                    string status;
                    for(int k = -chessNumTop; k <= chessNumBottom; k++ ){
                        if(chessState[i+k][j] == chessStateOption[CHESS_PLAYER1] || k==0) status.append("1");
                        else if(chessState[i+k][j] == chessStateOption[CHESS_PLAYER2]) status.append("2");
                        else status.append("0"); //empty&&k!=0
                    }
                    score_defense += scoreCaculation(status);
                }

                //?????????(???????????????????????????)
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
                //?????????(????????????)
                if(isGoodSpot){
                    string status;
                    for(int k = -chessNumLeft; k <= chessNumRight; k++ ){
                        if(chessState[i][j+k] == chessStateOption[CHESS_PLAYER2] || k==0) status.append("1");
                        else if(chessState[i][j+k] == chessStateOption[CHESS_PLAYER1]) status.append("2");
                        else status.append("0"); //empty&&k!=0
                    }
                    score_attack += scoreCaculation(status);
                }

                //?????????(???????????????????????????)
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

                //?????????(????????????)
                if(isGoodSpot){
                    string status;
                    for(int k = -chessNumLeft; k <= chessNumRight; k++ ){
                        if(chessState[i][j+k] == chessStateOption[CHESS_PLAYER1] || k==0) status.append("1");
                        else if(chessState[i][j+k] == chessStateOption[CHESS_PLAYER2]) status.append("2");
                        else status.append("0"); //empty&&k!=0
                    }
                    score_defense += scoreCaculation(status);
                }

                //???????????????(???????????????????????????)
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

                //???????????????(????????????)
                if(isGoodSpot){
                    string status;
                    for(int k = -lowerBound; k <= upperBound; k++ ){
                        if(chessState[i-k][j+k] == chessStateOption[CHESS_PLAYER2] || k==0) status.append("1");
                        else if(chessState[i-k][j+k] == chessStateOption[CHESS_PLAYER1]) status.append("2");
                        else status.append("0"); //empty&&k!=0
                    }
                    score_attack += scoreCaculation(status);
                }

                //???????????????(???????????????????????????)
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

                //???????????????(????????????)
                if(isGoodSpot){
                    string status;
                    for(int k = -lowerBound; k <= upperBound; k++ ){
                        if(chessState[i-k][j+k] == chessStateOption[CHESS_PLAYER1] || k==0) status.append("1");
                        else if(chessState[i-k][j+k] == chessStateOption[CHESS_PLAYER2]) status.append("2");
                        else status.append("0"); //empty&&k!=0
                    }
                    score_defense += scoreCaculation(status);
                }


                //???????????????(???????????????????????????)
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


                //???????????????(????????????)
                if(isGoodSpot){
                    string status;
                    for(int k = -lowerBound; k <= upperBound; k++ ){
                        if(chessState[i+k][j+k] == chessStateOption[CHESS_PLAYER2] || k==0) status.append("1");
                        else if(chessState[i+k][j+k] == chessStateOption[CHESS_PLAYER1]) status.append("2");
                        else status.append("0"); //empty&&k!=0
                    }
                    score_attack += scoreCaculation(status);
                }
                //???????????????(???????????????????????????)
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
                //???????????????(????????????)
                if(isGoodSpot){
                    string status;
                    for(int k = -lowerBound; k <= upperBound; k++ ){
                        if(chessState[i+k][j+k] == chessStateOption[CHESS_PLAYER1] || k==0) status.append("1");
                        else if(chessState[i+k][j+k] == chessStateOption[CHESS_PLAYER2]) status.append("2");
                        else status.append("0"); //empty&&k!=0
                    }
                    score_defense += scoreCaculation(status);
                }
            }
        if(score_attack > maximumScore_attack){
            maximumScore_attack = score_attack;
            supposedRowtoSetChess_attack = i;
            supposedColumntoSetChess_attack = j;
        }
        if(score_defense > maximumScore_defense){
            maximumScore_defense = score_defense;
            supposedRowtoSetChess_defense = i;
            supposedColumntoSetChess_defense = j;
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


