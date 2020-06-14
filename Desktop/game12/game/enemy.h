#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QPoint>
#include <QSize>
#include <QPixmap>

class WayPoint;
class QPainter;
class MainWindow;
class Tower;

class Enemy : public QObject{
    Q_OBJECT
public:
    Enemy(WayPoint *startWayPoint,MainWindow *game,const QPixmap &sprite = QPixmap(":/image/enemy.png"));
    ~Enemy();
    void getDamage(int damage);
    void getRemoved();
    void getAttacked(Tower *attacker);
    void draw(QPainter *painter) const;
    void move();  
    void gotLostSight(Tower *attacker);
    bool collisionWithCircle(QPoint point1, int radius1, QPoint point2, int radius2);
    void upgrade();
    QPoint pos() const;
public slots:
    void doActivate();
private:
    bool myactive;
    int mymaxHp;
    int mycurrentHp;
    qreal mywalkingSpeed;
    QPoint mypos;
    WayPoint * mydestinationWayPoint;
    MainWindow * mygame;
    QList<Tower *> myattackedTowersList;
   const QPixmap mysprite;
    static const QSize ms_fixedSize;
};

#endif // ENEMY_H
