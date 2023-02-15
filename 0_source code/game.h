#ifndef GAME_H
#define GAME_H
#include <string>
#include <QString>
#include <component.h>
#include <definition.h>
#include <unordered_map>
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
    Coor_row_column findBestSpottoSetChess();//找出最佳電腦下棋位置:直接以連線棋子總數給分，橫叉值分別算，取大
    Coor_row_column findBestSpottoSetChess_rev();//找出最佳電腦下棋位置新版: 以棋況給分(活五、活四...)各棋格分為得橫叉直線得分加總
    bool moveOnToNextTurn_playersMode(Coor_row_column currGridCoor);
    bool moveOnToNextTurn_computerMode(Coor_row_column currGridCoor);
    int scoreCaculation(string status);
    unordered_map<string,int> chessScore = {
        {"11111",10000000},//連五
        {"011110",1000000},//活四
        {"001110",10000},//活三
        {"011100",10000},//活三
        {"010110",10000},//活三
        {"011010",10000},//活三
        {"11110",8000},//沖四
        {"01111",8000},//沖四
        {"10111",8000},//沖四
        {"11011",8000},//沖四
        {"11101",8000},//沖四
        {"001112",1000},//眠三
        {"010112",1000},//眠三
        {"011012",1000},//眠三
        {"211100",1000},//眠三
        {"211010",1000},//眠三
        {"010112",1000},//眠三
        {"001100",800},//活二
        {"011000",800},//活二
        {"000110",800},//活二
        {"011200",50},//眠二
        {"001120",50},//眠二
        {"002110",50},//眠二
        {"021100",50},//眠二
        {"001010",50},//眠二
        {"010100",50},//眠二
        {"001200",10},//眠一
        {"002100",10},//眠一
        {"020100",10},//眠一
        {"001020",10},//眠一
        {"021000",10},//眠一
        {"000120",10},//眠一
        {"12",1},//一
        {"21",1},//一
    };

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
