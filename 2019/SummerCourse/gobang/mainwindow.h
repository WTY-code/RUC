#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ai.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    // 绘制
    void paintEvent(QPaintEvent *event);
    // 监听鼠标移动情况，方便落子
    void mouseMoveEvent(QMouseEvent *event);
    // 实际落子
    void mouseReleaseEvent(QMouseEvent *event);

private:
    AI *game; // 游戏指针
    GameType game_type; // 存储游戏类型
    int clickPosRow, clickPosCol; // 存储将点击的位置
    void initGame();
    void checkGame(int y, int x);

private slots:
    void chessOneByPerson(); // 人执行
   // void chessOneByAI_HillClimb(); // AI下棋
    void chessOneByAI_MiniMax();
    void chessOneByAI_Blind();

    void initPVPGame();//人人对战初始化
    //void initPVEGame_HillClimb();//人机对战初始化
    void initPVEGame_MiniMax();
    void initPVEGame_Blind();
};


#endif // MAINWINDOW_H
