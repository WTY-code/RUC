#ifndef TOWER1_H
#define TOWER1_H
#include <tower.h>

class tower1:public Tower
{
public:
    tower1(QPoint pos, MainWindow *game, const QPixmap &sprite = QPixmap(":/image/franky.png"));
    ~tower1();
    void upgrade();
private slots:
    void shootWeapon();
private:
    int m_damage;
};

#endif // TOWER1_H
