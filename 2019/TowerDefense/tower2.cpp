#include "tower2.h"
#include "bullet.h"
#include "mainwindow.h"
#include "enemy.h"

tower2::tower2(QPoint pos, MainWindow *game, const QPixmap &sprite/* = QPixmap(":/image/tower.png"*/)
    :Tower(pos,game,sprite), m_attackRange(80),m_damage(15)
{

}

tower2::~tower2()
{
    delete m_fireRateTimer;
    m_fireRateTimer = NULL;
}
void tower2::upgrade()
{
    setattackRange(100);
    setdamage(30);
    shape++;
}
