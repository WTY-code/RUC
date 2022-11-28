#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "waypoint.h"
#include "enemy.h"
#include "bullet.h"
#include "plistreader.h"
#include "tower.h"
#include "tower1.h"
#include "tower2.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtGlobal>
#include <QMessageBox>
#include <QTimer>
#include <QXmlStreamReader>
#include <QtDebug>
#include <qmediaplayer.h>

static const int TowerCost = 200;
static const int Tower1Cost = 100;
static const int Tower2Cost = 300;
static const int TowerUpgrade = 200;
static const int Tower1Upgrade = 200;
static const int Tower2Upgrade = 500;

const QSize MainWindow::ms_fixedSize(44, 44);

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, m_waves(0)
    , m_playerHp(7)
	, m_playrGold(1000)
	, m_gameEnded(false)
    , m_gameWin(false)
    , whether(0,0)
    , startposition(0,0)
    , lastposition(0,0)
    , position(0,320)
{
	ui->setupUi(this);

    preLoadWavesInfo();
    QMediaPlayer *player=new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile("D:\\project\\0611\\TowerDefenseQt\\TowerDefenseQt\\music\\enemy_destory.mp3"));
    player->setVolume(60);
    player->play();

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
	timer->start(30);


	// 设置300ms后游戏启动
    //QTimer::singleShot(300, this, SLOT(gamestart()));

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::loadTowerPositions()
{
    if(page==2)
    {
        m_towerPositionsList.push_back(QPoint(396, 116));
        m_towerPositionsList.push_back(QPoint(240, 200));
        m_towerPositionsList.push_back(QPoint(296, 119));
        m_towerPositionsList.push_back(QPoint(156, 256));
        m_towerPositionsList.push_back(QPoint(479, 85));
        m_towerPositionsList.push_back(QPoint(240, 368));
        m_towerPositionsList.push_back(QPoint(199, 121));
    }
    else if(page==3)
    {
        m_towerPositionsList.push_back(QPoint(186, 223));
        m_towerPositionsList.push_back(QPoint(262, 223));
        m_towerPositionsList.push_back(QPoint(150, 136));
        m_towerPositionsList.push_back(QPoint(375, 223));
        m_towerPositionsList.push_back(QPoint(105, 248));
        m_towerPositionsList.push_back(QPoint(105, 366));
        m_towerPositionsList.push_back(QPoint(450, 295));
    }
    else if(page==4)
    {
        m_towerPositionsList.push_back(QPoint(75, 223));
        m_towerPositionsList.push_back(QPoint(95, 136));
        m_towerPositionsList.push_back(QPoint(187, 270));
        m_towerPositionsList.push_back(QPoint(258, 272));
        m_towerPositionsList.push_back(QPoint(337, 223));
        m_towerPositionsList.push_back(QPoint(450, 270));
        m_towerPositionsList.push_back(QPoint(520, 270));
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
	if (m_gameEnded || m_gameWin)
    {
        if(m_gameEnded)
        {
            QPixmap picture(":/image/defeat.png");
            QPainter painter(this);
            painter.drawPixmap(0,0,picture);
        }
        else
        {
            QPixmap picture(":/image/victory.png");
            QPainter painter(this);
            painter.drawPixmap(0,0,picture);
        }
		return;
    }
    if(page==0)
    {
        QPixmap startpicture(":/image/stapicture.png");
        QPainter painter(this);
        painter.drawPixmap(0,0,startpicture);
    }
    else if(page==1)
    {
        QPixmap choosepicture(":/image/choose.png");
        QPainter painter(this);
        painter.drawPixmap(0,0,choosepicture);
    }
    else if(page!=0&&page!=1)
    {
        QPixmap cachePix;
        if(page==2)
            cachePix.load(":/image/map1.png");
        else if(page==3)
            cachePix.load(":/image/map2.png");
        else if(page==4)
            cachePix.load(":/image/map3.png");

        QPainter cachePainter(&cachePix);

        if(whether.x()>=0&&whether.y()>=447)
        {
            if(whichtowertoput()==0)
            {
                QPixmap tower(":/image/luffy.png");
                cachePainter.drawPixmap(position,tower);
            }
            else if(whichtowertoput()==1)
            {
                QPixmap tower(":/image/franky.png");
                cachePainter.drawPixmap(position,tower);
            }
            else if(whichtowertoput()==2)
            {
                QPixmap tower(":/image/brook.png");
                cachePainter.drawPixmap(position,tower);
            }
        }

        foreach (const TowerPosition &towerPos, m_towerPositionsList)
            towerPos.draw(&cachePainter);

        foreach (const tower2 *tower, m_tower2List)
            tower->draw(&cachePainter);

        foreach (const tower1 *tower, m_tower1List)
            tower->draw(&cachePainter);

        foreach (const Tower *tower, m_towersList)
            tower->draw(&cachePainter);

        foreach (const Enemy *enemy, m_enemyList)
            enemy->draw(&cachePainter);

        foreach (const Bullet *bullet, m_bulletList)
            bullet->draw(&cachePainter);

        drawWave(&cachePainter);
        drawHP(&cachePainter);
        drawPlayerGold(&cachePainter);

        QPainter painter(this);
        painter.drawPixmap(0, 0, cachePix);
        QPixmap tower(":/image/luffy.png");
        painter.drawPixmap(0,447,tower);
        painter.drawText(QRect(43, 447, 442, 489),
                         QString("BaseTower Range: 70 Damage: 10 FireRate: 1/s Cost: 200 UpGrade: FireRate*2 cost:200"));

        QPixmap tower11(":/image/franky.png");
        painter.drawPixmap(0,489,tower11);
        painter.drawText(QRect(43, 489, 442, 531),
                         QString("SlowTower Range: 70 Damage: 0 FireRate: 1/s  Cost: 100 UpGrade: FireRate*2&Damage+5 cost:200"));
        QPixmap tower22(":/image/brook.png");
        painter.drawPixmap(0,531,tower22);
        painter.drawText(QRect(43, 531, 442, 573),
                          QString("SuperiorTower Range: 80 Damage: 15 FireRate: 1/s Cost: 300 UpGrade: Damage*2&Range+20 cost:500"));




    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    position=event->pos();
    this->update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(page!=0&&page!=1)
    {
        QPoint pressPos = event->pos();
        if(startposition.x()>=0&&startposition.x()<=42&&startposition.y()>=447&&startposition.y()<=489)
        {
            auto it = m_towerPositionsList.begin();
            while (it != m_towerPositionsList.end())
            {
                if (canBuyTower() && it->containPoint(pressPos) && !it->hasTower())
                {
                    m_playrGold -= TowerCost;
                    it->setHasTower();

                    Tower *tower = new Tower(it->centerPos(), this);
                    m_towersList.push_back(tower);
                    update();
                    break;

                }

                ++it;
            }
        }

        if(startposition.x()>=0&&startposition.x()<=42&&startposition.y()>=489&&startposition.y()<=531)
        {//减速塔
            auto it = m_towerPositionsList.begin();
            while (it != m_towerPositionsList.end())
            {
                if (canBuyTower1() && it->containPoint(pressPos) && !it->hasTower())//
                {
                    m_playrGold -= Tower1Cost;//
                    it->setHasTower();

                    tower1 *tower = new tower1(it->centerPos(), this);
                    m_tower1List.push_back(tower);
                    update();
                    break;

                }

                ++it;
            }
        }

        if(startposition.x()>=0&&startposition.x()<=42&&startposition.y()>=531&&startposition.y()<=573)
        {
            auto it = m_towerPositionsList.begin();
            while (it != m_towerPositionsList.end())
            {
                if (canBuyTower2() && it->containPoint(pressPos) && !it->hasTower())//
                {
                    m_playrGold -= Tower2Cost;//
                    it->setHasTower();

                    tower2 *tower = new tower2(it->centerPos(), this);
                    m_tower2List.push_back(tower);
                    update();
                    break;

                }

                ++it;
            }
        }

        QPoint reset(0,0),reset1(0,447),reset2(42,447),reset3(84,447);
        whether=reset;
        startposition=reset;
        this->update();
        if(whichtowertoput()==0)
        {
            position=reset1;
        }
        else if(whichtowertoput()==1)
        {

            position=reset2;
        }
        else if(whichtowertoput()==2)
        {

            position=reset3;
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(page==0)//判断页面，为0即开始界面
    {
        QPoint pressPos = event->pos();
        if(pressPos.x()>=241&&pressPos.x()<=357&&pressPos.y()>=369&&pressPos.y()<=419)
        {

            page++;
            this->update();
        }
    }
    else if(page==1)
    {
        QPoint pressPos = event->pos();
        if(pressPos.x()>=234&&pressPos.x()<=346&&pressPos.y()>=143&&pressPos.y()<=199)
        {//点在第一关
            page=2;
            m_waves=0;
            this->gameStart();
            this->update();
        }
        else if(pressPos.x()>=234&&pressPos.x()<=346&&pressPos.y()>=241&&pressPos.y()<=297)
        {//点在第二关
            page=3;
            m_waves=1;
            this->gameStart();
            this->update();
        }
        else if(pressPos.x()>=234&&pressPos.x()<=346&&pressPos.y()>=340&&pressPos.y()<=397)
        {//点在第三关
            page=4;
            m_waves=2;
            this->gameStart();
            this->update();
        }
    }
    else if(page!=0&&page!=1)//不为0即进入游戏
    {
        QPoint pressPos = event->pos();
        auto it = m_towerPositionsList.begin();
        while (it != m_towerPositionsList.end())
        {
            if (it->containPoint(pressPos) && it->hasTower())
            {
                foreach(TowerPosition towerposition, m_towerPositionsList)
                {
                    if (pressPos.x()<towerposition.centerPos().x()+ms_fixedSize.width()/2
                     && pressPos.x()>towerposition.centerPos().x()-ms_fixedSize.width()/2
                     && pressPos.y()<towerposition.centerPos().y()+ms_fixedSize.height()/2
                     && pressPos.y()>towerposition.centerPos().y()-ms_fixedSize.height()/2)
                    {
                         pressPos.setX(towerposition.centerPos().x());
                         pressPos.setY(towerposition.centerPos().y());
                         break;
                    }

                }
                foreach(Tower *tower, m_towersList)
                {//0611
                    if(tower->getpos()==pressPos && canUpgradeTower()&&tower->showshape()==0)
                    {
                        tower->upgrade();
                        m_playrGold -= TowerUpgrade;
                        return;
                    }
                }
                foreach(tower1 *tower, m_tower1List)
                {
                    if(tower->getpos()==pressPos && canUpgradeTower1()&&tower->showshape()==0)
                    {
                        tower->upgrade();
                        m_playrGold -= Tower1Upgrade;
                        return;
                    }
                }
                foreach(tower2 *tower, m_tower2List)
                {
                    if(tower->getpos()==pressPos && canUpgradeTower2()&&tower->showshape()==0)
                    {
                        tower->upgrade();
                        m_playrGold -= Tower2Upgrade;
                        return;
                    }
                }

            }

            ++it;
        }

        startposition=pressPos;
        whether=pressPos;
    }
}

bool MainWindow::canBuyTower() const
{
	if (m_playrGold >= TowerCost)
		return true;
	return false;
}


void MainWindow::drawWave(QPainter *painter)
{
	painter->setPen(QPen(Qt::red));
	painter->drawText(QRect(400, 5, 100, 25), QString("WAVE : %1").arg(m_waves + 1));
}

void MainWindow::drawHP(QPainter *painter)
{
	painter->setPen(QPen(Qt::red));
	painter->drawText(QRect(30, 5, 100, 25), QString("HP : %1").arg(m_playerHp));
}

void MainWindow::drawPlayerGold(QPainter *painter)
{
	painter->setPen(QPen(Qt::red));
	painter->drawText(QRect(200, 5, 200, 25), QString("GOLD : %1").arg(m_playrGold));
}

void MainWindow::doGameOver()
{
	if (!m_gameEnded)
	{
		m_gameEnded = true;
		// 此处应该切换场景到结束场景
		// 暂时以打印替代,见paintEvent处理
	}
}

void MainWindow::awardGold(int gold)
{
	m_playrGold += gold;
	update();
}


void MainWindow::addWayPoints()
{
    if(page==2)
    {
        WayPoint *wayPoint1 = new WayPoint(QPoint(120, 427));
        m_wayPointsList.push_back(wayPoint1);

        WayPoint *wayPoint2 = new WayPoint(QPoint(220, 427));
        m_wayPointsList.push_back(wayPoint2);
        wayPoint2->setNextWayPoint(wayPoint1);

        WayPoint *wayPoint3 = new WayPoint(QPoint(220, 182));
        m_wayPointsList.push_back(wayPoint3);
        wayPoint3->setNextWayPoint(wayPoint2);

        WayPoint *wayPoint4 = new WayPoint(QPoint(460, 182));
        m_wayPointsList.push_back(wayPoint4);
        wayPoint4->setNextWayPoint(wayPoint3);

        WayPoint *wayPoint5 = new WayPoint(QPoint(460, 0));
        m_wayPointsList.push_back(wayPoint5);
        wayPoint5->setNextWayPoint(wayPoint4);
    }
    else if(page==3)
    {
        WayPoint *wayPoint1 = new WayPoint(QPoint(430, 446));
        m_wayPointsList.push_back(wayPoint1);

        WayPoint *wayPoint2 = new WayPoint(QPoint(430, 200));
        m_wayPointsList.push_back(wayPoint2);
        wayPoint2->setNextWayPoint(wayPoint1);

        WayPoint *wayPoint3 = new WayPoint(QPoint(357, 200));
        m_wayPointsList.push_back(wayPoint3);
        wayPoint3->setNextWayPoint(wayPoint2);

        WayPoint *wayPoint4 = new WayPoint(QPoint(357, 290));
        m_wayPointsList.push_back(wayPoint4);
        wayPoint4->setNextWayPoint(wayPoint3);

        WayPoint *wayPoint5 = new WayPoint(QPoint(245, 290));
        m_wayPointsList.push_back(wayPoint5);
        wayPoint5->setNextWayPoint(wayPoint4);

        WayPoint *wayPoint6 = new WayPoint(QPoint(245, 200));
        m_wayPointsList.push_back(wayPoint6);
        wayPoint6->setNextWayPoint(wayPoint5);

        WayPoint *wayPoint7 = new WayPoint(QPoint(167, 200));
        m_wayPointsList.push_back(wayPoint7);
        wayPoint7->setNextWayPoint(wayPoint6);

        WayPoint *wayPoint8 = new WayPoint(QPoint(167, 446));
        m_wayPointsList.push_back(wayPoint8);
        wayPoint8->setNextWayPoint(wayPoint7);
    }
    else if(page==4)
    {
        WayPoint *wayPoint1 = new WayPoint(QPoint(584, 224));
        m_wayPointsList.push_back(wayPoint1);

        WayPoint *wayPoint2 = new WayPoint(QPoint(584, 338));
        m_wayPointsList.push_back(wayPoint2);
        wayPoint2->setNextWayPoint(wayPoint1);

        WayPoint *wayPoint3 = new WayPoint(QPoint(433, 338));
        m_wayPointsList.push_back(wayPoint3);
        wayPoint3->setNextWayPoint(wayPoint2);

        WayPoint *wayPoint4 = new WayPoint(QPoint(433, 203));
        m_wayPointsList.push_back(wayPoint4);
        wayPoint4->setNextWayPoint(wayPoint3);

        WayPoint *wayPoint5 = new WayPoint(QPoint(320, 203));
        m_wayPointsList.push_back(wayPoint5);
        wayPoint5->setNextWayPoint(wayPoint4);

        WayPoint *wayPoint6 = new WayPoint(QPoint(320, 338));
        m_wayPointsList.push_back(wayPoint6);
        wayPoint6->setNextWayPoint(wayPoint5);

        WayPoint *wayPoint7 = new WayPoint(QPoint(170, 338));
        m_wayPointsList.push_back(wayPoint7);
        wayPoint7->setNextWayPoint(wayPoint6);

        WayPoint *wayPoint8 = new WayPoint(QPoint(170, 203));
        m_wayPointsList.push_back(wayPoint8);
        wayPoint8->setNextWayPoint(wayPoint7);

        WayPoint *wayPoint9 = new WayPoint(QPoint(56, 203));
        m_wayPointsList.push_back(wayPoint9);
        wayPoint9->setNextWayPoint(wayPoint8);

        WayPoint *wayPoint10 = new WayPoint(QPoint(56, 338));
        m_wayPointsList.push_back(wayPoint10);
        wayPoint10->setNextWayPoint(wayPoint9);

        WayPoint *wayPoint11 = new WayPoint(QPoint(0, 338));
        m_wayPointsList.push_back(wayPoint11);
        wayPoint11->setNextWayPoint(wayPoint10);
    }
}

void MainWindow::getHpDamage(int damage/* = 1*/)
{
	m_playerHp -= damage;
	if (m_playerHp <= 0)
		doGameOver();
}

void MainWindow::removedEnemy(Enemy *enemy)
{
	Q_ASSERT(enemy);

	m_enemyList.removeOne(enemy);
	delete enemy;

	if (m_enemyList.empty())
	{
		++m_waves;
        page++;
        if (!loadWave())
        {
            m_gameWin = true;
            // 游戏胜利转到游戏胜利场景
            // 这里暂时以打印处理
        }
        m_playrGold+=1000;
        foreach(Tower *tower, m_towersList)
        {
            delete tower;
        }
        m_towersList.clear();
        foreach(tower1 *tower, m_tower1List)
        {
            delete tower;
        }
        m_tower1List.clear();
        foreach(tower2 *tower, m_tower2List)
        {
            delete tower;
        }
        m_tower2List.clear();
        update();
    }
}

void MainWindow::removedBullet(Bullet *bullet)
{
	Q_ASSERT(bullet);

	m_bulletList.removeOne(bullet);
	delete bullet;
}

void MainWindow::addBullet(Bullet *bullet)
{
	Q_ASSERT(bullet);

	m_bulletList.push_back(bullet);
}

void MainWindow::updateMap()
{
	foreach (Enemy *enemy, m_enemyList)
		enemy->move();
	foreach (Tower *tower, m_towersList)
		tower->checkEnemyInRange();
    foreach (tower1 *tower, m_tower1List)
        tower->checkEnemyInRange();
    foreach (tower2 *tower, m_tower2List)
        tower->checkEnemyInRange();

	update();
}

void MainWindow::preLoadWavesInfo()
{
	QFile file(":/config/Waves.plist");
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(this, "TowerDefense", "Cannot Open TowersPosition.plist");
		return;
	}

	PListReader reader;
	reader.read(&file);

	// 获取波数信息
	m_wavesInfo = reader.data();

	file.close();
}

bool MainWindow::loadWave()
{
	if (m_waves >= m_wavesInfo.size())
		return false;

    m_towerPositionsList.clear();
    m_wayPointsList.clear();
    loadTowerPositions();
    addWayPoints();
	WayPoint *startWayPoint = m_wayPointsList.back();
	QList<QVariant> curWavesInfo = m_wavesInfo[m_waves].toList();

    for (int i = 0; i < curWavesInfo.size(); ++i)
	{
		QMap<QString, QVariant> dict = curWavesInfo[i].toMap();
		int spawnTime = dict.value("spawnTime").toInt();

		Enemy *enemy = new Enemy(startWayPoint, this);
		m_enemyList.push_back(enemy);
		QTimer::singleShot(spawnTime, enemy, SLOT(doActivate()));
	}

	return true;
}

QList<Enemy *> MainWindow::enemyList() const
{
	return m_enemyList;
}

void MainWindow::gameStart()
{
	loadWave();
}

int MainWindow::whichtowertoput()
{
    if(startposition.x()>=0&&startposition.x()<=42&&startposition.y()>=447&&startposition.y()<=489)
    {
        return 0;
    }
    else if(startposition.x()>0&&startposition.x()<=42&&startposition.y()>489&&startposition.y()<=531)
    {
        return 1;
    }
    else if(startposition.x()>0&&startposition.x()<=42&&startposition.y()>531&&startposition.y()<=573)
    {
        return 2;
    }
}
bool MainWindow::canBuyTower1() const
{
    if (m_playrGold >= Tower1Cost)
        return true;
    return false;
}
bool MainWindow::canBuyTower2() const
{
    if (m_playrGold >= Tower2Cost)
        return true;
    return false;
}
bool MainWindow::canUpgradeTower() const
{
    if(m_playrGold >= TowerUpgrade)
        return true;
    return false;
}
bool MainWindow::canUpgradeTower1() const
{
    if (m_playrGold >= Tower1Upgrade)
        return true;
    return false;
}
bool MainWindow::canUpgradeTower2() const
{
    if (m_playrGold >= Tower2Upgrade)
        return true;
    return false;
}
int MainWindow::rpage()
{
    return page;
}
