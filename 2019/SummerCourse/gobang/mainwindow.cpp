#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <math.h>
//#include <QSound>
#include "mainwindow.h"
#include "ai.h"
/*
#define CHESS_ONE_SOUND ":/res/sound/chessone.wav"
#define WIN_SOUND ":/res/sound/win.wav"
#define LOSE_SOUND ":/res/sound/lose.wav"
*/
const int kBoardMargin = 30; // 棋盘边缘空隙
const int kRadius = 15; // 棋子半径
const int kMarkSize = 6; // 落子标记边长
const int kBlockSize = 40; // 格子的大小
const int kPosDelta = 20; // 鼠标点击的模糊距离上限

const int kAIDelay = 700; // AI下棋的思考时间

// -------------------- //

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 设置棋盘大小
    setFixedSize(kBoardMargin * 2 + kBlockSize * kBoardSizeNum, kBoardMargin * 2 + kBlockSize * kBoardSizeNum);
//    setStyleSheet("background-color:yellow;");

    // 开启鼠标hover功能，这两句一般要设置window的
    setMouseTracking(true);
//    centralWidget()->setMouseTracking(true);

    // 添加菜单
    QMenu *gameMenu = menuBar()->addMenu(tr("Game")); // menuBar默认是存在的，直接加菜单就可以了
    QAction *actionPVP = new QAction("Person VS Person", this);
    connect(actionPVP, SIGNAL(triggered()), this, SLOT(initPVPGame()));
    gameMenu->addAction(actionPVP);

/*    QAction *actionPVE_HillClimb = new QAction("Person VS Computer BY HillClimb", this);
    connect(actionPVE_HillClimb, SIGNAL(triggered()), this, SLOT(initPVEGame_HillClimb()));
    gameMenu->addAction(actionPVE_HillClimb);
*/

    QAction *actionPVE_MiniMax = new QAction("Person VS Computer BY MiniMax", this);
    connect(actionPVE_MiniMax, SIGNAL(triggered()), this, SLOT(initPVEGame_MiniMax()));
    gameMenu->addAction(actionPVE_MiniMax);

    QAction *actionPVE_Blind = new QAction("Person VS Computer BY Blind", this);
    connect(actionPVE_Blind, SIGNAL(triggered()), this, SLOT(initPVEGame_Blind()));
    gameMenu->addAction(actionPVE_Blind);
    // 开始游戏
    initGame();
}

MainWindow::~MainWindow()
{
    if (game)
    {
        delete game;
        //game = nullptr;
    }
}

void MainWindow::initGame()
{
    // 初始化游戏模型，初始为人人对战
    game = new AI;
    game->player=-1;
//initPVEGame_MiniMax();
initPVEGame_Blind();
}

void MainWindow::initPVPGame()
{
    game_type = PERSON;
    game->gameStatus = PLAYING;
    game->startGame(game_type);
    update();
}

/*void MainWindow::initPVEGame_HillClimb()
{
    game_type = BOTHillClimb;
    game->gameStatus = PLAYING;
    game->startGame(game_type);
    update();
}
*/
void MainWindow::initPVEGame_MiniMax()
{
    game_type = BOTMINIMAX;
    game->gameStatus = PLAYING;
    game->startGame(game_type);
    update();
}

void MainWindow::initPVEGame_Blind()
{
    game_type = BOTBlind;
    game->gameStatus = PLAYING;
    game->startGame(game_type);
    update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    // 绘制棋盘
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿：防止圆形图片有锯齿
//    QPen pen; // 调整线条宽度
//    pen.setWidth(2);
//    painter.setPen(pen);
    for (int i = 0; i < kBoardSizeNum + 1; i++)
    {
        painter.drawLine(kBoardMargin + kBlockSize * i, kBoardMargin, kBoardMargin + kBlockSize * i, size().height() - kBoardMargin);
        painter.drawLine(kBoardMargin, kBoardMargin + kBlockSize * i, size().width() - kBoardMargin, kBoardMargin + kBlockSize * i);
    }

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);//填充样式
    // 绘制落子标记(防止鼠标出框越界)
    if (clickPosRow > 0 && clickPosRow < kBoardSizeNum &&
        clickPosCol > 0 && clickPosCol < kBoardSizeNum &&
        game->stateForPaint._sln[clickPosRow][clickPosCol] == 0)
    {
        if (game->player==1)
            brush.setColor(Qt::black);
        else
            brush.setColor(Qt::white);
        painter.setBrush(brush);
        painter.drawRect(kBoardMargin + kBlockSize * clickPosCol - kMarkSize / 2, kBoardMargin + kBlockSize * clickPosRow - kMarkSize / 2, kMarkSize, kMarkSize);
    }

    // 绘制棋子
    for (int i = 0; i < kBoardSizeNum-1; i++)
        for (int j = 0; j < kBoardSizeNum-1; j++)
        {
            if (game->stateForPaint._sln[i][j] == -1)//默认-1为白棋子
            {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(kBoardMargin + kBlockSize * j - kRadius, kBoardMargin + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
            }
            else if (game->stateForPaint._sln[i][j] == 1)
            {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(kBoardMargin + kBlockSize * j - kRadius, kBoardMargin + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
            }
        }

    // 判断输赢
    if (clickPosRow > 0 && clickPosRow < kBoardSizeNum &&
        clickPosCol > 0 && clickPosCol < kBoardSizeNum &&
        (game->stateForPaint._sln[clickPosRow][clickPosCol] == 1 ||
            game->stateForPaint._sln[clickPosRow][clickPosCol] == -1))
    {
        if (game->isWinForPaint(clickPosRow, clickPosCol) && game->gameStatus == PLAYING)
        {
            qDebug() << "win";
            game->gameStatus = WIN;
            //QSound::play(WIN_SOUND);//播放音乐
            QString str;
            if (game->stateForPaint._sln[clickPosRow][clickPosCol] == -1)
                str = "white player";
            else if (game->stateForPaint._sln[clickPosRow][clickPosCol] == 1)
                str = "black player";
            for(int i=0; i<kBoardMargin; i++){
                for(int j=0; j<kBoardMargin; j++){
                    //cout<<"state._sln["<<i<<"]["<<j<<"]"<<game->state._sln[i][j]<<endl;
                    cout<<"stateForPraint["<<i<<"]["<<j<<"]"<<game->stateForPaint._sln[i][j]<<endl;
                }
            }
            QMessageBox::StandardButton btnValue = QMessageBox::information(this, "congratulations", str + "win!");

            // 重置游戏状态，否则容易死循环
            if (btnValue == QMessageBox::Ok)
            {
                game->startGame(game_type);
                game->gameStatus = PLAYING;
            }
        }
    }


    // 判断死局
    if (game->isDeadGame())
    {   //QSound::play(LOSE_SOUND);
        QMessageBox::StandardButton btnValue = QMessageBox::information(this, "oops", "dead game!");
        if (btnValue == QMessageBox::Ok)
        {
            game->startGame(game_type);
            game->gameStatus = PLAYING;
        }

    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 通过鼠标的hover确定落子的标记
    int x = event->x();
    int y = event->y();

    // 棋盘边缘不能落子
    if (x >= kBoardMargin + kBlockSize / 2 &&
            x < size().width() - kBoardMargin &&
            y >= kBoardMargin + kBlockSize / 2 &&
            y < size().height()- kBoardMargin)
    {
        // 获取最近的左上角的点
        int col = x / kBlockSize;
        int row = y / kBlockSize;

        int leftTopPosX = kBoardMargin + kBlockSize * col;
        int leftTopPosY = kBoardMargin + kBlockSize * row;

        // 根据距离算出合适的点击位置,一共四个点，根据半径距离选最近的
        clickPosRow = -1; // 初始化最终的值
        clickPosCol = -1;
        int len = 0; // 计算完后取整就可以了

        // 确定一个误差在范围内的点，且只可能确定一个出来
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < kPosDelta)
        {
            clickPosRow = row;
            clickPosCol = col;
        }
        len = sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < kPosDelta)
        {
            clickPosRow = row;
            clickPosCol = col + 1;
        }
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize));
        if (len < kPosDelta)
        {
            clickPosRow = row + 1;
            clickPosCol = col;
        }
        len = sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize));
        if (len < kPosDelta)
        {
            clickPosRow = row + 1;
            clickPosCol = col + 1;
        }
    }
    cout<<"clickPosRow and clickPosCol:"<<clickPosRow<<" "<<clickPosCol<<endl;
    // 存了坐标后也要重绘
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    cout<<"enter mouseReleaseEvent"<<endl;
    cout<<"player is "<<game->player<<endl;
    // 人下棋，并且不能抢机器的棋
    /*if ((!(game_type == BOTHillClimb && !game->playerFlag))
         &&(!(game_type == BOTMINIMAX && !game->playerFlag))
         &&(!(game_type == BOTBlind && !game->playerFlag)) )
    {*/
    if(game->player==-1){
        cout<<"enter if(player==-1)"<<endl;
        chessOneByPerson();


        // 如果是人机模式，需要调用AI下棋
 /*       if (game->gameType == BOTHillClimb && !game->playerFlag)
        {
            // 用定时器做一个延迟
            QTimer::singleShot(kAIDelay, this, SLOT(chessOneByAI_HillClimb()));
        }
        else */if (game->gameType == BOTMINIMAX && game->player==1)
        {
            cout<<"enter if(gametype=botminimax)&&player=1"<<endl;
            QTimer::singleShot(kAIDelay, this, SLOT(chessOneByAI_MiniMax()));
        }
        else if (game->gameType == BOTBlind && game->player==1)
        {
            cout<<"enter if(gametype==botblind)&&player=1"<<endl;
            QTimer::singleShot(kAIDelay, this, SLOT(chessOneByAI_Blind()));
        }
    }
/*    for(int i=0; i<kBoardSizeNum; i++){
        for(int j=0; j<kBoardSizeNum; j++){
            if()
        }
    }
*/

}

void MainWindow::chessOneByPerson()
{
    cout<<"enter chessOneBy Person"<<endl;
    // 根据当前存储的坐标下子
    // 只有有效点击才下子，并且该处没有子
    if (clickPosRow != -1 && clickPosCol != -1 && game->stateForPaint._sln[clickPosRow][clickPosCol] == 0)
    {
        cout<<"_sln=0 and will change the value"<<endl;
        game->actionByPerson(clickPosRow, clickPosCol);
        //QSound::play(CHESS_ONE_SOUND);
        // 重绘
        update();
    }
}

/*void MainWindow::chessOneByAI_HillClimb()
{
    game->actionByAI_HillClimb(clickPosRow, clickPosCol);
    update();
}*/

void MainWindow::chessOneByAI_MiniMax()
{

    game->actionByAI_MiniMax(clickPosRow, clickPosCol);
    update();
}

void MainWindow::chessOneByAI_Blind()
{
    cout<<"enter chessOneByAI_Blind"<<endl;
    game->actionByAI_Blind(clickPosRow, clickPosCol);
    update();
}
