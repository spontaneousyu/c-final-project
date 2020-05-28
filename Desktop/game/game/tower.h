#ifndef TOWER_H
#define TOWER_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>

class QPainter;
class Enemy;
class MainWindow;
class QTimer;

class Tower : QObject{
    Q_OBJECT
public:
    Tower(QPoint pos,MainWindow *game,const QPixmap &sprite=QPixmap(":/image/tower.png"));
    ~Tower();
    void attackEnemy();
    void chooseEnemyForAttack(Enemy *enemy);
    void removeBullet();
    void draw(QPainter *painter) const;
    void checkEnemyInRange();
    void targetKilled();
    void damageEnemy();
    void lostSightOfEnemy();

    bool collisionWithCircle(QPoint point1,int radius1,QPoint point2,int radius2);
private slots:
    void shootWeapon();
private:
    bool myattacking;
    int myattackRange;
    int mydamage;
    int myfireRate;
    Enemy * mychooseEnemy;
    MainWindow * mygame;
    QTimer * myfireRateTimer;
    const QPoint mypos;
    const QPixmap mysprite;
    static const QSize ms_fixedSize;
};

#endif // TOWER_H
