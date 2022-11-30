#include "ai.h"
#include <iostream>
#include <stack>
#include <queue>
#include <cstdlib>
#include "time.h"
using namespace std;

AI::AI()
{
    //初始化
    for(int i=0; i<kBoardSizeNum-1; i++){
        for(int j=0; j<kBoardSizeNum-1; j++){
            state._sln[i][j] = 0;
            state.value[i][j] = 0;
        }
    }
    next.row=0;
    next.col=0;
    next.value=0;
    player=-1;//人先下
    gameStatus = PLAYING;
}

//单个子估值函数，检验上下左右斜方向上连成的棋子，并赋值
int AI::evaluate(Chess _try){
    // 统计玩家或者电脑连成的子
        int personNum = 0; // 玩家连成子的个数
        int botNum = 0; // AI连成子的个数
        int emptyNum = 0; // 各方向空白位的个数
        int row=_try.row;
        int col=_try.col;

        if(row<0||col<0||row>=kBoardSizeNum||col>=kBoardSizeNum){
            srand((unsigned)time(NULL));
            _try.row = rand()%(kBoardSizeNum-1)+1;
            srand((unsigned)time(NULL));
            _try.col = rand()%(kBoardSizeNum-1)+1;
        }//避免AI无子可下
        row=_try.row;
        col=_try.col;

                // 空白点就算
                if (row > 0 && col > 0 &&
                    state._sln[row][col] == 0)
                {
                    // 遍历周围八个方向
                    for (int y = -1; y <= 1; y++)
                        for (int x = -1; x <= 1; x++)
                        {
                            // 重置
                            personNum = 0;
                            botNum = 0;
                            emptyNum = 0;

                            // 原坐标不算
                            if (!(y == 0 && x == 0))
                            {
                                // 每个方向延伸4个子

                                // 对玩家白子评分（正反两个方向）
                                for (int i = 1; i <= 4; i++)
                                {
                                    if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                        col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                        state._sln[row + i * y][col + i * x] == 1) // 玩家的子
                                    {
                                        personNum++;
                                    }
                                    else if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                             col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                             state._sln[row + i * y][col + i * x] == 0) // 空白位
                                    {
                                        emptyNum++;
                                        break;
                                    }
                                    else            // 出边界
                                        break;
                                }

                                for (int i = 1; i <= 4; i++)
                                {
                                    if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                        col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                        state._sln[row - i * y][col - i * x] == 1) // 玩家的子
                                    {
                                        personNum++;
                                    }
                                    else if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                             col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                             state._sln[row - i * y][col - i * x] == 0) // 空白位
                                    {
                                        emptyNum++;
                                        break;//因为我们要的是连续子
                                    }
                                    else            // 出边界
                                        break;
                                }

                                if (personNum == 1)                      // 杀二
                                    state.value[row][col] += 10;
                                else if (personNum == 2)                 // 杀三
                                {
                                    if (emptyNum == 1)
                                        state.value[row][col] += 30;
                                    else if (emptyNum == 2)
                                        state.value[row][col] += 40;
                                }
                                else if (personNum == 3)                 // 杀四
                                {
                                    // 量变空位不一样，优先级不一样
                                    if (emptyNum == 1)
                                        state.value[row][col] += 60;
                                    else if (emptyNum == 2)
                                        state.value[row][col] += 110;
                                }
                                else if (personNum == 4)                 // 杀五
                                    state.value[row][col] += 10100;

                                // 进行一次清空
                                emptyNum = 0;

                                // 对AI黑子评分
                                for (int i = 1; i <= 4; i++)
                                {
                                    if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                        col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                        state._sln[row + i * y][col + i * x] == -1) // 玩家的子
                                    {
                                        botNum++;
                                    }
                                    else if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                             col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                             state._sln[row +i * y][col + i * x] == 0) // 空白位
                                    {
                                        emptyNum++;
                                        break;
                                    }
                                    else            // 出边界
                                        break;
                                }

                                for (int i = 1; i <= 4; i++)
                                {
                                    if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                        col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                        state._sln[row - i * y][col - i * x] == -1) // AI的子
                                    {
                                        botNum++;
                                    }
                                    else if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                             col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                             state._sln[row - i * y][col - i * x] == 0) // 空白位
                                    {
                                        emptyNum++;
                                        break;
                                    }
                                    else            // 出边界
                                        break;
                                }

                                if (botNum == 0)                      // 普通下子
                                    state.value[row][col] +=5;
                                else if (botNum == 1)                 // 活二
                                    state.value[row][col] += 10;
                                else if (botNum == 2)
                                {
                                    if (emptyNum == 1)                // 死三
                                        state.value[row][col] +=25;
                                    else if (emptyNum == 2)
                                        state.value[row][col] += 50;  // 活三
                                }
                                else if (botNum == 3)
                                {
                                    if (emptyNum == 1)                // 死四
                                        state.value[row][col] += 55;
                                    else if (emptyNum == 2)
                                        state.value[row][col] += 100; // 活四
                                }
                                else if (botNum >= 4)
                                    state.value[row][col] +=10000;   // 活五

                            }
                        }
                  _try.value = state.value[row][col];
                  return _try.value;
                }
                _try.value=0;
                return _try.value;

}
//盲搜，返回值为一步棋（Chess结构体，里边存有这步棋的坐标和价值）只看一步，通过对试探棋的估值决定下哪里
Chess AI::Blind(){
    cout<<"enter Blind"<<endl;
    Chess tmpNode,maxNode;
    tmpNode.row=0,tmpNode.col=0;
    tmpNode.value=0;
    maxNode.row=0,maxNode.col=0;
    maxNode.value=-INF;
    for(int i=0; i<kBoardSizeNum-1; i++){
        for(int j=0; j<kBoardSizeNum-1; j++){
            state.value[i][j] = 0;
            if(state._sln[i][j]==0){
                tmpNode.row=i;
                tmpNode.col=j;
                tmpNode.value=evaluate(tmpNode);
                if(tmpNode.value>maxNode.value){
                    maxNode.row=tmpNode.row;
                    maxNode.col=tmpNode.col;
                    maxNode.value =tmpNode.value;
                }
            }
        }
    }
    return maxNode;
}


//mini_max
Chess AI::Minimax_Decision(int player){
    cout<<"enter minimax_decision"<<endl;
    Chess best,tmpNode;
    best.row=5,best.col=6;
    best.value=-INF;
    int alpha = -INF, beta = INF;
    //为了提速，限制搜索范围，获得当前棋局最外围棋子的坐标，在最外围2格处搜索
    int i_min=0,i_max=kBoardSizeNum,j_min=0,j_max=kBoardSizeNum;
    for(int i=0; i<kBoardSizeNum-1; i++){
        for(int j=0; j<kBoardSizeNum-1; j++){
            if(state._sln[i][j]!=0){
                i_min=i;
                j_min=j;
                //cout<<"i_min:"<<i_min<<" j_min:"<<j_min<<endl;
                break;
            }
        }
    }
    for(int i=kBoardSizeNum-2;i>=0; i--){
        for(int j=kBoardSizeNum-2; j>=0; j--){
            if(state._sln[i][j]!=0){
                i_max=i;
                j_max=j;
                //cout<<"i_max:"<<i_max<<"j_max:"<<j_max<<endl;
                break;
            }
        }
    }
    cout<<"i_min:"<<i_min<<" j_min:"<<j_min<<endl;
    cout<<"i_max:"<<i_max<<"j_max:"<<j_max<<endl;
    //开始试探
    for(int i=i_min-deltI; i<i_max+deltI; i++){
        for(int j=j_min-deltJ; j<j_max+deltJ; j++){

            if(state._sln[i][j]==0){
                  tmpNode.row=i;
                  tmpNode.col=j;
                  state._sln[i][j] = 1;

                  tmpNode.value=Min_Value(tmpNode,player,alpha,beta,maxDepth);
                  if(best.value<=tmpNode.value){
                      cout<<"change best wtih tmpnode:"<<endl;
                      cout<<"best is "<<"("<<best.row<<","<<best.col<<") value:"<<best.value<<endl;
                      cout<<"tmpnode is "<<"("<<tmpNode.row<<","<<tmpNode.col<<") value:"<<tmpNode.value<<endl;
                      best = tmpNode;
                  }
                  state._sln[i][j]=0;
            }
        }
    }

    return best;
}

int AI::Max_Value(Chess _try, int player, int alpha, int beta, int depth){
    cout<<"enter max_vlaue"<<endl;
    //以下为递归出口
    if(isWin(_try)) return INF;
    if(isLose()) return -INF;
    if(depth==0){//延伸的深度达到限制，对整个棋局进行评估
        cout<<"depth=0"<<endl;
        int  whitescore=0,blackscore=0;
        //分别计算白子（人工智障）得分和黑子（人玩家）得分
        for(int i=0; i<kBoardSizeNum-1; i++){
            for(int j=0; j<kBoardSizeNum-1; j++){
                if(state._sln[i][j]!=0){
                    Chess tmpNode(i,j);
                    if(state._sln[i][j]==1){
                        whitescore+=evaluate(tmpNode);
                    }
                    else if(state._sln[i][j]==-1){
                        blackscore+=evaluate(tmpNode);
                    }
                }
            }
        }
        cout<<"whitescore is "<<whitescore<<" balckscore is "<<blackscore<<endl;
        cout<<"totalscore is "<<whitescore-blackscore<<endl;
        return whitescore-blackscore;//用白子得分减去黑子得分为整盘棋局得分
    }
    //非出口
    int i_min=0,i_max=kBoardSizeNum,j_min=0,j_max=kBoardSizeNum;
    for(int i=0; i<kBoardSizeNum-1; i++){
        for(int j=0; j<kBoardSizeNum-1; j++){
            if(state._sln[i][j]!=0){
                i_min=i;
                j_min=j;
                break;
            }
        }
    }
    for(int i=kBoardSizeNum-2;i>=0; i--){
        for(int j=kBoardSizeNum-2; j>=0; j--){
            if(state._sln[i][j]!=0){
                i_max=i;
                j_max=j;
                break;
            }
        }
    }
    cout<<"i_min:"<<i_min<<" j_min:"<<j_min<<endl;
    cout<<"i_max:"<<i_max<<"j_max:"<<j_max<<endl;
    for(int i=i_min-deltI; i<i_max+deltI; i++){//搜索范围带限制
        for(int j=j_min-deltJ; j<j_max+deltJ; j++){
            if(state._sln[i][j]==0){
                Chess node(i,j);
                state._sln[i][j]=player;
                node.value=Min_Value(node,-player,alpha, beta,depth-1);
                if(alpha<=node.value){
                    alpha=node.value;
                    state._sln[i][j]=0;
                    if(alpha>=beta){//剪枝
                        return alpha;
                    }
                }
            }
        }
    }
    return alpha;
}

int AI::Min_Value(Chess _try,int player, int alpha, int beta, int depth){
    if(isWin(_try)) return INF;
    if(isLose()) return -INF;
    if(depth==0){
        int  whitescore=0,blackscore=0;
        for(int i=0; i<kBoardSizeNum-1; i++){
            for(int j=0; j<kBoardSizeNum-1; j++){
                if(state._sln[i][j]!=0){
                    Chess tmpNode(i,j);
                    if(state._sln[i][j]==1){
                        whitescore+=evaluate(tmpNode);
                    }
                    else{
                        blackscore+=evaluate(tmpNode);
                    }
                }
            }
        }
        return whitescore-blackscore;
    }
    int i_min=0,i_max=kBoardSizeNum,j_min=0,j_max=kBoardSizeNum;
    for(int i=0; i<kBoardSizeNum-1; i++){
        for(int j=0; j<kBoardSizeNum-1; j++){
            if(state._sln[i][j]!=0){
                i_min=i;
                j_min=j;
                break;
            }
        }
    }
    for(int i=kBoardSizeNum-2;i>=0; i--){
        for(int j=kBoardSizeNum-2; j>=0; j--){
            if(state._sln[i][j]!=0){
                i_max=i;
                j_max=j;
                break;
            }
        }
    }
    for(int i=i_min-deltI; i<i_max+deltI; i++){
        for(int j=j_min-deltI; j<j_max+deltJ; j++){
            if(state._sln[i][j]==0){
                Chess node(i,j);
                state._sln[i][j]=player;
                node.value=Max_Value(node,-player,alpha, beta,depth-1);
                if(beta>=node.value){
                    beta=node.value;
                    state._sln[i][j]=0;
                    if(alpha>=beta){
                        return beta;
                    }
                }
            }
        }
    }
    return beta;
}

bool AI::isWin(){
    for(int i=0; i<kBoardSizeNum-1; i++){
        for(int j=0; j<kBoardSizeNum-1; j++){
            if(state._sln[i][j]==1&&isWin(i,j)){
                return true;
            }
        }
    }
    return false;
}
bool AI::isLose(){
    for(int i=0; i<kBoardSizeNum-1; i++){
        for(int j=0; j<kBoardSizeNum-1; j++){
            if(state._sln[i][j]==-1&&isWin(i,j)){
                return true;
            }
        }
    }
    return false;
}
bool AI::isWin(Chess _try)
{
    // 横竖斜四种大情况，每种情况都根据当前落子往后遍历5个棋子，有一种符合就算赢
    // 水平方向
    for (int i = 0; i < 5; i++)
    {
        // 往左5个，往右匹配4个子，20种情况
        if (_try.col - i > 0 &&
            _try.col - i + 4 < kBoardSizeNum &&
            state._sln[_try.row][_try.col - i] == state._sln[_try.row][_try.col - i + 1] &&
            state._sln[_try.row][_try.col - i] == state._sln[_try.row][_try.col - i + 2] &&
            state._sln[_try.row][_try.col - i] == state._sln[_try.row][_try.col - i + 3] &&
            state._sln[_try.row][_try.col - i] == state._sln[_try.row][_try.col - i + 4])
            return true;
    }

    // 竖直方向(上下延伸4个)
    for (int i = 0; i < 5; i++)
    {
        if (_try.row - i > 0 &&
            _try.row - i + 4 < kBoardSizeNum &&
            state._sln[_try.row - i][_try.col] == state._sln[_try.row - i + 1][_try.col] &&
            state._sln[_try.row - i][_try.col] == state._sln[_try.row - i + 2][_try.col] &&
            state._sln[_try.row - i][_try.col] == state._sln[_try.row - i + 3][_try.col]&&
            state._sln[_try.row - i][_try.col] == state._sln[_try.row - i + 4][_try.col])
            return true;
    }

    // 左斜方向
    for (int i = 0; i < 5; i++)
    {
        if (_try.row + i < kBoardSizeNum &&
            _try.row + i - 4 > 0 &&
            _try.col - i > 0 &&
            _try.col - i + 4 < kBoardSizeNum &&
            state._sln[_try.row + i][_try.col - i] == state._sln[_try.row + i - 1][_try.col - i + 1] &&
            state._sln[_try.row + i][_try.col - i] == state._sln[_try.row + i - 2][_try.col - i + 2] &&
            state._sln[_try.row + i][_try.col - i] == state._sln[_try.row + i - 3][_try.col - i + 3] &&
            state._sln[_try.row + i][_try.col - i] == state._sln[_try.row + i - 4][_try.col - i + 4])
            return true;
    }

    // 右斜方向
    for (int i = 0; i < 5; i++)
    {
        if (_try.row - i > 0 &&
            _try.row - i + 4 < kBoardSizeNum &&
            _try.col - i > 0 &&
            _try.col - i + 4 < kBoardSizeNum &&
            state._sln[_try.row - i][_try.col - i] == state._sln[_try.row - i + 1][_try.col - i + 1] &&
            state._sln[_try.row - i][_try.col - i] == state._sln[_try.row - i + 2][_try.col - i + 2] &&
            state._sln[_try.row - i][_try.col - i] == state._sln[_try.row - i + 3][_try.col - i + 3] &&
            state._sln[_try.row - i][_try.col - i] == state._sln[_try.row - i + 4][_try.col - i + 4] )
            return true;
    }

    return false;
}

bool AI::isWinForPaint(int row, int col)
{
    // 横竖斜四种大情况，每种情况都根据当前落子往后遍历5个棋子，有一种符合就算赢
    // 水平方向
    for (int i = 0; i < 5; i++)
    {
        // 往左5个，往右匹配4个子，20种情况
        if (col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            stateForPaint._sln[row][col - i] == stateForPaint._sln[row][col - i + 1] &&
            stateForPaint._sln[row][col - i] == stateForPaint._sln[row][col - i + 2] &&
            stateForPaint._sln[row][col - i] == stateForPaint._sln[row][col - i + 3] &&
            stateForPaint._sln[row][col - i] == stateForPaint._sln[row][col - i + 4])
            return true;
    }

    // 竖直方向(上下延伸4个)
    for (int i = 0; i < 5; i++)
    {
        if (row - i > 0 &&
            row - i + 4 < kBoardSizeNum &&
            stateForPaint._sln[row - i][col] == stateForPaint._sln[row - i + 1][col] &&
            stateForPaint._sln[row - i][col] == stateForPaint._sln[row - i + 2][col] &&
            stateForPaint._sln[row - i][col] == stateForPaint._sln[row - i + 3][col] &&
            stateForPaint._sln[row - i][col] == stateForPaint._sln[row - i + 4][col])
            return true;
    }

    // 左斜方向
    for (int i = 0; i < 5; i++)
    {
        if (row + i < kBoardSizeNum &&
            row + i - 4 > 0 &&
            col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            stateForPaint._sln[row + i][col - i] == stateForPaint._sln[row + i - 1][col - i + 1] &&
            stateForPaint._sln[row + i][col - i] == stateForPaint._sln[row + i - 2][col - i + 2] &&
            stateForPaint._sln[row + i][col - i] == stateForPaint._sln[row + i - 3][col - i + 3] &&
            stateForPaint._sln[row + i][col - i] == stateForPaint._sln[row + i - 4][col - i + 4])
            return true;
    }

    // 右斜方向
    for (int i = 0; i < 5; i++)
    {
        if (row - i > 0 &&
            row - i + 4 < kBoardSizeNum &&
            col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            stateForPaint._sln[row - i][col - i] == stateForPaint._sln[row - i + 1][col - i + 1] &&
            stateForPaint._sln[row - i][col - i] == stateForPaint._sln[row - i + 2][col - i + 2] &&
            stateForPaint._sln[row - i][col - i] == stateForPaint._sln[row - i + 3][col - i + 3] &&
            stateForPaint._sln[row - i][col - i] == stateForPaint._sln[row - i + 4][col - i + 4])
            return true;
    }

    return false;
}

bool AI::isWin(int row, int col)
{
    // 横竖斜四种大情况，每种情况都根据当前落子往后遍历5个棋子，有一种符合就算赢
    // 水平方向
    for (int i = 0; i < 5; i++)
    {
        // 往左5个，往右匹配4个子，20种情况
        if (col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            state._sln[row][col - i] == state._sln[row][col - i + 1] &&
            state._sln[row][col - i] == state._sln[row][col - i + 2] &&
            state._sln[row][col - i] == state._sln[row][col - i + 3] &&
            state._sln[row][col - i] == state._sln[row][col - i + 4])
            return true;
    }

    // 竖直方向(上下延伸4个)
    for (int i = 0; i < 5; i++)
    {
        if (row - i > 0 &&
            row - i + 4 < kBoardSizeNum &&
            state._sln[row - i][col] == state._sln[row - i + 1][col] &&
            state._sln[row - i][col] == state._sln[row - i + 2][col] &&
            state._sln[row - i][col] == state._sln[row - i + 3][col] &&
            state._sln[row - i][col] == state._sln[row - i + 4][col])
            return true;
    }

    // 左斜方向
    for (int i = 0; i < 5; i++)
    {
        if (row + i < kBoardSizeNum &&
            row + i - 4 > 0 &&
            col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            state._sln[row + i][col - i] == state._sln[row + i - 1][col - i + 1] &&
            state._sln[row + i][col - i] == state._sln[row + i - 2][col - i + 2] &&
            state._sln[row + i][col - i] == state._sln[row + i - 3][col - i + 3] &&
            state._sln[row + i][col - i] == state._sln[row + i - 4][col - i + 4])
            return true;
    }

    // 右斜方向
    for (int i = 0; i < 5; i++)
    {
        if (row - i > 0 &&
            row - i + 4 < kBoardSizeNum &&
            col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            state._sln[row - i][col - i] == state._sln[row - i + 1][col - i + 1] &&
            state._sln[row - i][col - i] == state._sln[row - i + 2][col - i + 2] &&
            state._sln[row - i][col - i] == state._sln[row - i + 3][col - i + 3] &&
            state._sln[row - i][col - i] == state._sln[row - i + 4][col - i + 4])
            return true;
    }

    return false;
}


bool AI::isDeadGame()
{
    // 所有空格全部填满
    for (int i = 1; i < kBoardSizeNum; i++)
        for (int j = 1; j < kBoardSizeNum; j++)
        {
            if (!(state._sln[i][j] == 1 || state._sln[i][j] == -1))
                return false;
        }
    return true;
}
void AI::startGame(GameType type)
{
    gameType = type;
    // 初始棋盘
    for(int i=0; i<kBoardSizeNum-1; i++){
        for(int j=0; j<kBoardSizeNum; j++){
            state._sln[i][j]=0;
            state.value[i][j]=0;
            stateForPaint._sln[i][j]=0;
        }
    }

    // 己方下为true,对方下为false
    player = -1;//初始化为人下


}

void AI::updateGameMap(int row, int col)
{//这就是实际上的下棋操作，如果是己方将传进来的行列位置记为1，对方置为-1
    cout<<"change _sln"<<endl;
    if (player==1){
        cout<<"if player=1 ";
        stateForPaint._sln[row][col] = 1;
        state._sln[row][col] = 1;
        cout<<"state._sln["<<row<<"]["<<col<<"]="<<stateForPaint._sln[row][col]<<endl;
    }
    else{
        cout<<"if player=-1 ";
        state._sln[row][col] = -1;
        stateForPaint._sln[row][col] = -1;
        cout<<"state._sln["<<row<<"]["<<col<<"]="<<stateForPaint._sln[row][col]<<endl;
    }
    // 换手
    cout<<"change player to "<<-player<<endl;
    player = -player;//在下完棋之后马上换
}

void AI::actionByPerson(int row, int col)
{
    cout<<"enter actionByPerson"<<endl;
    cout<<"row is "<<row<<" "<<"col is "<<col<<endl;
    updateGameMap(row, col);

}


void AI::actionByAI_MiniMax(int &clickRow, int &clickCol)
{
    next=this->Minimax_Decision(player/*, -INF, INF*/);
    clickRow = next.row; // 记录落子点，其实修改了
    clickCol = next.col;
    updateGameMap(clickRow, clickCol);
}

void AI::actionByAI_Blind(int &clickRow, int &clickCol){
    cout<<"enter actionByAIBlind and clickRow is "<<clickRow<<" clickCol is "<<clickCol<<endl;
    next=this->Blind();
    clickRow = next.row; // 记录落子点，其实修改了
    clickCol = next.col;
    cout<<"after blind() get cliclrow "<<clickRow<<" and clicklcol"<<clickCol<<endl;
    updateGameMap(clickRow, clickCol);
}



