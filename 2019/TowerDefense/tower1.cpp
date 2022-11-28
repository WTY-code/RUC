#include "tower.h"
#include "tower1.h"
#include "bullet.h"
#include "mainwindow.h"
#include "enemy.h"

tower1::tower1(QPoint pos, MainWindow *game, const QPixmap &sprite/* = QPixmap(":/image/tower.png"*/)
    : Tower(pos,game,sprite), m_damage(0){

}

tower1::~tower1()
{
    delete m_fireRateTimer;
    m_fireRateTimer = NULL;
}

void tower1::shootWeapon()
{
    Bullet *bullet = new Bullet(m_pos, m_chooseEnemy->pos(), m_damage, m_chooseEnemy, m_game);
    bullet->move();
    m_game->addBullet(bullet);
    m_chooseEnemy->getslow();
}
void tower1::upgrade()
{
    setdamage(5);
    setfireRate(500);
    shape++;
}
