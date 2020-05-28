#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTimer>
#include "towerposition.h"
#include "tower.h"

//QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
//QT_END_NAMESPACE


namespace Ui {
class MainWindow;
}

class WayPoint;
class Enemy;
class Bullet;

class MainWindow:public QMainWindow{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent=0);
    ~MainWindow();
    void removedBullet(Bullet *bullet);
    void addBullet(Bullet *bullet);
    void awardGold(int gold);
    void getHpDamage(int damage=1);
    void removedEnemy(Enemy *enemy);
    QList<Enemy *> enemyList() const;
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void loadTowerPositions();
    void addWayPoints();
    bool loadWave();
    bool canBuyTower() const;
    void drawWave(QPainter *painter);
    void drawHP(QPainter *painter);
    void drawPlayerGold(QPainter *painter);
    void doGameOver();
    void preLoadWavesInfo();
    void intiGame();
private slots:
    void updateMap();
    void gameStart();
private:
    Ui::MainWindow * ui;
    int mywaves;
    int myplayerHp;
    int myplayerGold;
    bool mygameEnded;
    bool mygameWin;
    QVector<int> mywavesInfo[2];
    QList<TowerPosition> mytowerPositionsList;
    QList<Tower *> mytowersList;
    QList<WayPoint *> mywayPointsList;
    QList<Enemy *> myenemyList;
    QList<Bullet *> mybulletList;
    QTimer *myTimer;
};

#endif // MAINWINDOW_H
