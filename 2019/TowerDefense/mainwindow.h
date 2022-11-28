#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "towerposition.h"
#include "tower.h"
#include "tower1.h"
#include "tower2.h"
#include "page.h"

namespace Ui {
class MainWindow;
}

class WayPoint;
class Enemy;
class Bullet;
class AudioPlayer;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void getHpDamage(int damage = 1);
	void removedEnemy(Enemy *enemy);
	void removedBullet(Bullet *bullet);
	void addBullet(Bullet *bullet);
    void awardGold(int gold);
	AudioPlayer* audioPlayer() const;
	QList<Enemy *> enemyList() const;
    int rpage();

protected:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *);

private slots:
	void updateMap();
    void gameStart();

private:
	void loadTowerPositions();
	void addWayPoints();
	bool loadWave();
	bool canBuyTower() const;
    bool canBuyTower1() const;//用于判断是否买得起塔
    bool canBuyTower2() const;//
    bool canUpgradeTower() const;//0611
    bool canUpgradeTower1() const;//用于判断是否有钱升级
    bool canUpgradeTower2() const;//
	void drawWave(QPainter *painter);
	void drawHP(QPainter *painter);
	void drawPlayerGold(QPainter *painter);

    void doGameOver();
	void preLoadWavesInfo();
    QPoint whether;//该次鼠标单机操作是否为选塔
    QPoint startposition;//鼠标单击位置
    QPoint lastposition;//鼠标松开位置
    QPoint position;//鼠标当前位置
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    int whichtowertoput();


private:
	Ui::MainWindow *		ui;
	int						m_waves;
	int						m_playerHp;
	int						m_playrGold;
	bool					m_gameEnded;
	bool					m_gameWin;
	AudioPlayer *			m_audioPlayer;
	QList<QVariant>			m_wavesInfo;
	QList<TowerPosition>	m_towerPositionsList;
	QList<Tower *>			m_towersList;
    QList<tower1 *>         m_tower1List;
    QList<tower2 *>         m_tower2List;
	QList<WayPoint *>		m_wayPointsList;
	QList<Enemy *>			m_enemyList;
    QList<Bullet *>			m_bulletList;
    int                    page=0;

    static const QSize      ms_fixedSize;
};

#endif // MAINWINDOW_H
