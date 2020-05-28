#include "bullet.h"
#include "enemy.h"
#include "mainwindow.h"
#include <QPainter>
#include <QPropertyAnimation>

const QSize Bullet::ms_fixedSize(10, 10);

Bullet::Bullet(QPoint startPos,QPoint targetPoint,int damage,Enemy *target,MainWindow *game,const QPixmap &sprite):mystartPos(startPos),mytargetPos(targetPoint),
    mysprite(sprite),mycurrentPos(startPos),mytarget(target),mygame(game),mydamage(damage){}
void Bullet::hitTarget(){
    if (mygame->enemyList().indexOf(mytarget)!=-1)
        mytarget->getDamage(mydamage);// check if the enemy is still valid(multiple simultaneous hits)
    mygame->removedBullet(this);
}
void Bullet::draw(QPainter *painter) const{//paint bullet
    painter->drawPixmap(mycurrentPos,mysprite);
}
void Bullet::move(){//alternatives and varietion in moving for change
    static const int duration=100;//STATIC CONST hit duration
    QPropertyAnimation *animation=new QPropertyAnimation(this,"mycurrentPos");
    animation->setDuration(duration);//alter later
    animation->setStartValue(mystartPos);
    animation->setEndValue(mytargetPos);
    connect(animation,SIGNAL(finished()),this,SLOT(hitTarget()));
    animation->start();
}
void Bullet::setCurrentPos(QPoint pos){//initialize position
    mycurrentPos=pos;
}
QPoint Bullet::currentPos() const{//show current position
    return mycurrentPos;
}
