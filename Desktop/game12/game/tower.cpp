#include "tower.h"
#include "enemy.h"
#include "bullet.h"
#include "mainwindow.h"
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QVector2D>
#include <QtMath>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>

const QSize Tower::ms_fixedSize(84,84);
Tower::Tower(QPoint pos,MainWindow *game,const QPixmap &sprite):myattacking(false),myattackRange(140),mydamage(10),myfireRate(1000),mychooseEnemy(NULL),
       mygame(game),mypos(pos),mysprite(sprite){
    myfireRateTimer=new QTimer(this);
    connect(myfireRateTimer,SIGNAL(timeout()),this,SLOT(shootWeapon()));
}
Tower::~Tower(){
    delete myfireRateTimer;
    myfireRateTimer=NULL;
}
void Tower::attackEnemy(){//timer,put out bullets according to time interval
    myfireRateTimer->start(myfireRate);
}
void Tower::chooseEnemyForAttack(Enemy *enemy){//lock the enemy and pull out timer and attack action
    mychooseEnemy=enemy;
    attackEnemy();
    mychooseEnemy->getAttacked(this);
}
void Tower::shootWeapon(){//show the bollut moving
    Bullet *bullet=new Bullet(mypos,mychooseEnemy->pos(),mydamage,mychooseEnemy,mygame);
    bullet->move();
    mygame->addBullet(bullet);
}
void Tower::checkEnemyInRange(){//if in attack; if no enemy then search
    if (mychooseEnemy){
        QVector2D normalized(mychooseEnemy->pos()-mypos);
        normalized.normalize();
        if (!collisionWithCircle(mypos,myattackRange,mychooseEnemy->pos(),1))//check if any
            lostSightOfEnemy();
    }
    else{
        QList<Enemy *> enemyList=mygame->enemyList();
        foreach (Enemy *enemy, enemyList){
            if (collisionWithCircle(mypos,myattackRange,enemy->pos(),1)){
                chooseEnemyForAttack(enemy);
                break;
            }
        }}}
void Tower::draw(QPainter *painter) const{//paint fire range and tower icon const525
    painter->save();
    painter->setPen(Qt::white);
    painter->drawEllipse(mypos,myattackRange,myattackRange);
    static const QPoint offsetPoint(-ms_fixedSize.width()/2,-ms_fixedSize.height()/2);
    painter->translate(mypos);//further rotate
    painter->drawPixmap(offsetPoint,mysprite);
    painter->restore();
}
void Tower::targetKilled(){// reset choose enemy and stop timer if dead
    if (mychooseEnemy)mychooseEnemy=NULL;
    myfireRateTimer->stop();
}
void Tower::lostSightOfEnemy(){//reset choose enemy and stop timer if missing
    mychooseEnemy->gotLostSight(this);
    if (mychooseEnemy)mychooseEnemy=NULL;
   myfireRateTimer->stop();
}
bool Tower::collisionWithCircle(QPoint point1,int radius1,QPoint point2,int radius2){//basic elements to check if available for attack
     int xx=point1.x()-point2.x();
     int yy=point1.y()-point2.y();
    int distance=qSqrt(xx*xx+yy*yy);
   if (distance<=(radius1+radius2))return true;
   else return false;
}
void Tower::setFireRate(int fireRate)//change fire rate to change type
{
    myfireRate = fireRate;
}
bool Tower::event(QEvent *ev)//tower type 2
{
    if (ev->type() == QEvent::MouseButtonPress)
    {
        if (((QMouseEvent *)ev)->button() == Qt::LeftButton)
        {
            myfireRate = myfireRate*0.8;
            mydamage = mydamage*1.2;
        }

    }
    return true;
}
bool Tower::eventFilter(QObject *obj, QEvent *ev)//check if event
{
    if (obj == this)
    {
        event(ev);
    }
    return true;
}
QPoint Tower::getPos()
{
    return mypos;
}
