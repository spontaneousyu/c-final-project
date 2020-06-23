#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "waypoint.h"
#include "enemy.h"
#include "bullet.h"
#include <QPainter>//jyb bilibili
#include <QMouseEvent>//https://blog.csdn.net/a8039974/article/details/90728536
#include <QtGlobal>//https://blog.csdn.net/lsfreeing/article/details/89890286
#include <QMessageBox>//https://blog.csdn.net/qq_41453285/article/details/91628383
#include <QTimer>//https://blog.csdn.net/zz2862625432/article/details/79550285
#include <QtDebug>
#include <QMenuBar>//https://blog.csdn.net/founderznd/article/details/51407909
#include <QDebug>

static const int TowerCost = 400;
MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow),mywaves(0),myplayerHp(3),myplayerGold(1000),mygameEnded(false),mygameWin(false){
    ui->setupUi(this);
    QAction *gameStartAction=new QAction(tr("start"),this);
    gameStartAction->setStatusTip(tr("start"));
    connect(gameStartAction,SIGNAL(triggered()),this,SLOT(gameStart()));
    QMenu *gameStartMenu=menuBar()->addMenu(tr("menu"));//menu for future function
    gameStartMenu->addAction(gameStartAction);

    QAction *gameUpgradeAction=new QAction(tr("upgrade"),this);//upgrade menu
    gameStartAction->setStatusTip(tr("upgrade"));
    connect(gameUpgradeAction,SIGNAL(triggered()),this,SLOT(gameUpgrade()));
    QMenu *gameUpgradeMenu=menuBar()->addMenu(tr("upgrade"));//menu for future function
    gameUpgradeMenu->addAction(gameUpgradeAction);

    myTimer=new QTimer(this);
    connect(myTimer,SIGNAL(timeout()),this,SLOT(updateMap()));
    myTimer->setInterval(30);
}
MainWindow::~MainWindow(){
    delete ui;
}
void MainWindow::removedBullet(Bullet *bullet){//delete from list,basic function
    Q_ASSERT(bullet);//523
    mybulletList.removeOne(bullet);
    delete bullet;
}
void MainWindow::addBullet(Bullet *bullet){//add to a list,basic function
    Q_ASSERT(bullet);//523
    mybulletList.push_back(bullet);
}

void MainWindow::updateMap(){//basic actions
    foreach (Enemy *enemy,myenemyList)enemy->move();
    foreach (Tower *tower,mytowersList)tower->checkEnemyInRange();
    update();
}
void MainWindow::preLoadWavesInfo(){//enemies initialization
    int spawTimes[2][6]={{100,500,600},{100,500,700,1000,2000,3000}};
    mywavesInfo[0].push_back(spawTimes[0][0]);
    mywavesInfo[0].push_back(spawTimes[0][1]);
    mywavesInfo[0].push_back(spawTimes[0][2]);
    mywavesInfo[1].push_back(spawTimes[1][0]);
    mywavesInfo[1].push_back(spawTimes[1][1]);
    mywavesInfo[1].push_back(spawTimes[1][2]);
    mywavesInfo[1].push_back(spawTimes[1][3]);
    mywavesInfo[1].push_back(spawTimes[1][4]);
    mywavesInfo[1].push_back(spawTimes[1][5]);
}
void MainWindow::loadTowerPositions(){//position initialization
    struct{
        int x;int y;
    }points[8] = {{160,70},{340,70},{520,70},{700,70},{200,250},{390,250},{560,250},{740,250}};
    for (int i=0;i<8;i++){
        int x=points[i].x,y=points[i].y;
        mytowerPositionsList.push_back(QPoint(x, y));
    }
}
void MainWindow::paintEvent(QPaintEvent *){//basic background setting
    if (mygameEnded||mygameWin){
        QString text = mygameEnded ? "Maybe in another life" : "I'll be chasing all the stars";
        QPainter painter(this);
        painter.setPen(QPen(Qt::yellow));
        painter.drawText(rect(),Qt::AlignCenter,text);
        return;
    }
    QPixmap Pix(":/image/background.png");
    QPainter painter(&Pix);
    foreach (const TowerPosition &towerPos,mytowerPositionsList)towerPos.draw(&painter);
    foreach (const Tower *tower,mytowersList)tower->draw(&painter);
    foreach (const Enemy *enemy,myenemyList)enemy->draw(&painter);
    foreach (const Bullet *bullet,mybulletList)bullet->draw(&painter);
    drawWave(&painter);
    drawHP(&painter);
    drawPlayerGold(&painter);
    QPainter rpainter(this);
    rpainter.drawPixmap(0,25,Pix);
}
void MainWindow::mousePressEvent(QMouseEvent *event){//version1, upgrade later
    QPoint pressPos=event->pos();
    auto it=mytowerPositionsList.begin();
    while (it!=mytowerPositionsList.end()){
        if (canBuyTower()&&it->containPoint(pressPos)&& !it->hasTower())
        {
            it->setHasTower();
            myplayerGold=myplayerGold-TowerCost;
            Tower *tower=new Tower(it->centerPos(),this);
            tower->installEventFilter(this);
            mytowersList.push_back(tower);
            update();
            break;
        }
        ++it;
    }

    foreach (Tower *tower,mytowersList)
    {
        if (((tower->getPos().x() > pressPos.x()-44) && (tower->getPos().x() < pressPos.x()+44)) &&
            ((tower->getPos().y() > pressPos.y()-44) && (tower->getPos().y() < pressPos.y()+44)))
        {
            if (event->button() == Qt::LeftButton)//change tower type
            {
                tower->event(event);
            }
            else
            {
                if (event->button() == Qt::RightButton)//remove tower
                {
                    mytowersList.removeOne(tower);
                    delete tower;
                    myplayerGold=myplayerGold+TowerCost/2;//money system advanced
                    auto it=mytowerPositionsList.begin();
                    while (it!=mytowerPositionsList.end()){
                        if (it->containPoint(pressPos) && it->hasTower())
                        {
                            it->setHasTower(false);
                            break;
                        }
                        ++it;
                    }

                }
            }
        }
    }

}
bool MainWindow::canBuyTower() const{//check if enough gold,const 525
    if (myplayerGold>=TowerCost)return true;
    else return false;
}
void MainWindow::drawWave(QPainter *painter){//head changes
    painter->setPen(QPen(Qt::yellow));
    painter->drawText(QRect(800,10,200,50),QString("waves: %1").arg(mywaves+1));
}
void MainWindow::drawHP(QPainter *painter){//head changes
    painter->setPen(QPen(Qt::yellow));
    painter->drawText(QRect(60,10,200,50),QString("lives : %1").arg(myplayerHp));
}
void MainWindow::drawPlayerGold(QPainter *painter){//head changes
    painter->setPen(QPen(Qt::yellow));
    painter->drawText(QRect(400,10,400,50),QString("gold : %1").arg(myplayerGold));
}
void MainWindow::doGameOver(){//change mygame status
    if (!mygameEnded){
        mygameEnded=true;
    }
}
void MainWindow::awardGold(int gold){//basic function
    myplayerGold=myplayerGold+gold;
    update();
}
void MainWindow::addWayPoints(){//advanced calculation,line initialization
    WayPoint *wayPoint1=new WayPoint(QPoint(70,390));
    mywayPointsList.push_back(wayPoint1);
    WayPoint *wayPoint2=new WayPoint(QPoint(890,390));
    mywayPointsList.push_back(wayPoint2);
    wayPoint2->setNextWayPoint(wayPoint1);
    WayPoint *wayPoint3=new WayPoint(QPoint(890,200));
    mywayPointsList.push_back(wayPoint3);
    wayPoint3->setNextWayPoint(wayPoint2);
    WayPoint *wayPoint4=new WayPoint(QPoint(70,200));
    mywayPointsList.push_back(wayPoint4);
    wayPoint4->setNextWayPoint(wayPoint3);
}
void MainWindow::getHpDamage(int damage){//basic functon and check if game over
    myplayerHp=myplayerHp-damage;
    if (myplayerHp<=0)
        doGameOver();
}
void MainWindow::removedEnemy(Enemy *enemy){//remove from list.if no enemy, next wave; if no wave, change mygame status
    Q_ASSERT(enemy);
    myenemyList.removeOne(enemy);
    delete enemy;
    if (myenemyList.empty()){
        ++mywaves;
        if (!loadWave()){
            mygameWin=true;
        }}}
bool MainWindow::loadWave(){//start new enemy and put it in the list
    if (mywaves>=2)return false;//BASIC FUNCTION3
    WayPoint *startWayPoint=mywayPointsList.back();
    for (int i=0;i<mywavesInfo[mywaves].size();++i){
        int spawnTime=mywavesInfo[mywaves][i];
        Enemy *enemy=new Enemy(startWayPoint,this);
        myenemyList.push_back(enemy);
        QTimer::singleShot(spawnTime,enemy,SLOT(doActivate()));
    }
    myenemyList.last()->upgrade();

    return true;
}
QList<Enemy *> MainWindow::enemyList() const{//const 525,list initialization
    return myenemyList;
}
void MainWindow::intiGame(){//entire game initilization
    mygameEnded=false;mygameWin=false;mywaves=0; myplayerHp=5;
    myplayerGold=1000;
    mywavesInfo[0].empty();
    mywavesInfo[1].empty();
    mytowerPositionsList.empty();
    mywayPointsList.empty();
    foreach (Tower *tower,mytowersList){
        mytowersList.removeOne(tower);
        delete tower;
    }
    mytowersList.empty();
    foreach (Enemy *enemy,myenemyList){
        myenemyList.removeOne(enemy);
        delete enemy;
    }
    myenemyList.empty();
    foreach (Bullet *bullet,mybulletList){
        mybulletList.removeOne(bullet);
        delete bullet;
    }
    mybulletList.empty();
    preLoadWavesInfo();
    loadTowerPositions();
    addWayPoints();
}
void MainWindow::gameStart(){//pull out initialization and start letting out enemy
    intiGame();
    myTimer->start();
    loadWave();
}
void MainWindow::gameUpgrade(){//pull out initialization and start letting out enemy
    foreach (Tower *tower,mytowersList)tower->setRateNDamage(500,16);
    myplayerGold=myplayerGold-400;
}
