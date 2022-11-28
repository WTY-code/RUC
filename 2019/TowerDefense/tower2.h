#ifndef TOWER2_H
#define TOWER2_H
#include "tower.h"

class tower2:public Tower
{
public:
    tower2(QPoint pos, MainWindow *game, const QPixmap &sprite = QPixmap(":/image/brook.png"));
    ~tower2();
    void upgrade();
private:
    int m_damage;
    int m_attackRange;
};

#endif // TOWER2_H
