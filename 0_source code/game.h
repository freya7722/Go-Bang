#ifndef GAME_H
#define GAME_H
#include <string>
#include <QString>
#include <component.h>
#include <definition.h>
using namespace std;


class Game: public QWidget
{
    Q_OBJECT
private:
    int boardSize;
    bool gameState = true;
    BoardJudge boardStatus = NOTYET;
    Turns currTurn = PLAYER1;
    GameMode gameMode = PLAYERS_MODE; //default
    enum ChessStateOption{  //任一棋格狀態種類
        EMPTY,
        CHESS_PLAYER1,
        CHESS_PLAYER2,
        _CHESSSTATEOPTION_CNT
    };
    std::string chessStateOption[_CHESSSTATEOPTION_CNT] ={
        "empty",
        "chess_player1",
        "chess_player2"
    };
    std::string chessState[15][15]; //開啟一個可以記錄棋盤狀態的陣列 棋盤最大可以到20x20 (字串二維陣列)
    void changeTurns(); //輪換該回合玩家
    int smallComparison(int a, int b);
    void updateChessState(struct Coor_row_column chessGridCoor); //根據該回合中下的棋更新棋盤狀態
    BoardJudge checkVictory(struct Coor_row_column chessGridCoor); //確認當前勝負
    bool checkChessOccupation(struct Coor_row_column currGridCoor); //確認該棋格是否有棋
    Coor_row_column findBestSpottoSetChess_rev();//找出最佳電腦下棋位置
    bool moveOnToNextTurn_playersMode(Coor_row_column currGridCoor);
    bool moveOnToNextTurn_computerMode(Coor_row_column currGridCoor);
signals:
    void signal_plotComputerChess(Coor_row_column currGridCoor);
public:
    Game(QWidget *parent,int boardSize);
    ~Game();
    bool ableToSetChess(Coor_row_column currGridCoor);
    bool moveOnToNextTurn(Coor_row_column currGridCoor);
    Turns getCurrTurns(); //確認當前玩家回合
    BoardJudge getCurrBoardStatus();
    void setGameMode(int gameMode);

};

#endif // GAME_H
