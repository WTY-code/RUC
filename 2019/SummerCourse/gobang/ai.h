#ifndef AI_H
#define AI_H
#include <iostream>
#include <vector>
#include <queue>

#define maxDepth 4
#define deltI 2
#define deltJ 2

using namespace std;

enum GameType
{
    PERSON,
    BOTHillClimb,
    BOTMINIMAX,
    BOTBlind
};

enum GameStatus
{
    PLAYING,//还在游戏
    WIN,//有一方赢了
    DEAD//死局了
};

// 棋盘尺寸
const int kBoardSizeNum = 15;
const int INF = 10000;
struct State
{
    State() {}
    State(const State &st);
    ~State(){
        //parent = NULL;
    }
    int _sln[kBoardSizeNum-1][kBoardSizeNum-1];
    int value[kBoardSizeNum-1][kBoardSizeNum-1];
    //struct  State* parent;

};

struct Chess
{
    Chess(){}
    Chess(int x, int y) {
        row = x;
        col = y;
    }
    Chess(const Chess& node){
        row=node.row;
        col=node.col;
        value=node.value;
    }
    int row,col;
    int value;
    friend bool operator < (Chess n1, Chess n2){
        return n1.value < n2.value;
    }
};

class AI
{
public:
    AI();
    int player;//标志下棋者
    GameType gameType;//游戏模式
    GameStatus gameStatus;//游戏状态
    void startGame(GameType);
    //Chess IDS(Chess _try);
    Chess Blind(/*State &cur*/);
    //Chess Hill(int row, int col);
    Chess Minimax_Decision(int player/*, int alpha, int beta*/);
    int Max_Value(Chess _try, int player, int alpha, int beta, int depth);
    int Min_Value(Chess _try, int player, int alpha, int beta, int depth);
    //Chess Alpha_Beta(int player, int alpha, int beta, int maxdepth);
    //Chess Minimax(int maxDepth, int player, int alpha, int beta);
    bool isWin(Chess _try);
    bool isWinForPaint(int x, int y);
    bool isWin(int x, int y);
    bool isWin();
    bool isLose();
    //bool isDeadGame(Chess _try);
    bool isDeadGame();
    //Chess IDS(State &cur, int maxDepth);
    int evaluate(Chess _try);
    void actionByPerson(int row, int col); // 人执行下棋
    //void actionByAI_HillClimb(int &clickRow, int &clickCol); // 机器执行下棋
    void actionByAI_MiniMax(int &clickRow, int &clickCol);
    void actionByAI_Blind(int &clickRow, int &clickCol);
    void updateGameMap(int row, int col);
    State state;//当前棋局形势,用来计算
    State stateForPaint;//用来连接mouseReleaseEvent的状态，也是棋局真正的状态
    Chess next;//将要怎么下
};

#endif // AI_H
