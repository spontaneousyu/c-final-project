#include "enemy.h"
#include "waypoint.h"
#include "tower.h"
#include "mainwindow.h"
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QMatrix>
#include <QVector2D>
#include <QtMath>


const QSize Enemy::ms_fixedSize(104, 104);

Enemy::Enemy(WayPoint *startWayPoint, MainWindow *game, const QPixmap &sprite):QObject(0),myactive(false),mymaxHp(40),mycurrentHp(160),mywalkingSpeed(1.0),
    mypos(startWayPoint->pos()),mydestinationWayPoint(startWayPoint->nextWayPoint()),mygame(game),mysprite(sprite){}
Enemy::~Enemy(){
    myattackedTowersList.clear();
    mydestinationWayPoint=NULL;
    mygame=NULL;
}
void Enemy::getDamage(int damage){// do hp damaging count
    mycurrentHp=mycurrentHp-damage;
    if (mycurrentHp<=0){//check if dead and give award
        mygame->awardGold(200);
        getRemoved();
    }
}
void Enemy::getAttacked(Tower *attacker){//put in the list of attacking tower
    myattackedTowersList.push_back(attacker);
}
void Enemy::gotLostSight(Tower *attacker){//important!!!put out of list of target
    myattackedTowersList.removeOne(attacker);
}
void Enemy::doActivate(){//check if alive
    myactive=true;
}
void Enemy::move(){//if active then move
    if (myactive){
    if (collisionWithCircle(mypos,1,mydestinationWayPoint->pos(),1)){
        if (mydestinationWayPoint->nextWayPoint()){//next step
            mypos=mydestinationWayPoint->pos();
            mydestinationWayPoint=mydestinationWayPoint->nextWayPoint();
        }
        else{//do damage
            mygame->getHpDamage();
            mygame->removedEnemy(this);
            return;
        }
    }
    QPoint targetPoint=mydestinationWayPoint->pos();
    qreal movementSpeed=mywalkingSpeed;//can be changed later
    QVector2D normalized(targetPoint-mypos);//https://blog.csdn.net/a197p/article/details/46700903?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522159065846719724846433444%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fblog.%2522%257D&request_id=159065846719724846433444&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~blog~first_rank_v2~rank_blog_default-1-46700903.pc_v2_rank_blog_default&utm_term=vector2D%E5%90%91%E9%87%8F%E5%AE%9E%E4%BE%8B
    normalized.normalize();
    mypos=mypos+normalized.toPoint()*movementSpeed;}
}

void Enemy::draw(QPainter *painter) const{//const 525,if active then draw
    if (myactive){
    painter->save();
    static const QPoint offsetPoint(-ms_fixedSize.width()/2,-ms_fixedSize.height()/2);
    painter->translate(mypos);
    painter->drawPixmap(offsetPoint,mysprite);
    painter->restore();//important!!!!
    }
}

void Enemy::getRemoved(){//if not all is attacking, remove the ones finishing ttcking
    if (!myattackedTowersList.empty()){
    foreach (Tower *attacker,myattackedTowersList)
        attacker->targetKilled();
    mygame->removedEnemy(this);//523
    }
}
QPoint Enemy::pos() const{// show current position
    return mypos;
}
bool Enemy::collisionWithCircle(QPoint point1,int radius1,QPoint point2,int radius2){//basic element
int xx=point1.x()-point2.x();
int yy=point1.y()-point2.y();
int distance = qSqrt(xx*xx+yy*yy);
    if (distance<=(radius1+radius2))return true;
   else return false;
}
void Enemy::upgrade()//enemy type2
{
    mymaxHp = 80;
    mycurrentHp = 80;
    mywalkingSpeed = 2.0;
}
