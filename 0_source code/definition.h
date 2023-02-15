#ifndef DEFINITION_H
#define DEFINITION_H


struct Coor_row_column {
    int row;
    int column;
};
enum Icon{
    PIG,
    OWL,
    RUBBER,
    SNOWMAN,
    CAT_BANJO,
    CAT_CUPIE,
    CAT_LADIE,
    CAT_FOOD,
    BANANA
};
enum BoardJudge{
    PLAYER1_WIN,
    PLAYER2_WIN,
    TIE,
    NOTYET
};
enum Turns{//玩家回合
    PLAYER1,
    PLAYER2
};
enum GameMode{
    PLAYERS_MODE,
    COMPUTER_MODE
};



#endif // DEFINITION_H
