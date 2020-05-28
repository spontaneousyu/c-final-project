#ifndef BULLET_H
#define BULLET_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>

class QPainter;
class Enemy;
class MainWindow;

class Bullet : QObject{
    Q_OBJECT
    Q_PROPERTY(QPoint mycurrentPos READ currentPos WRITE setCurrentPos)

public:
    Bullet(QPoint startPos,QPoint targetPoint,int damage,Enemy *target,
           MainWindow *game,const QPixmap &sprite = QPixmap(":/image/bullet.png"));
        void setCurrentPos(QPoint pos);
    void draw(QPainter *painter) const;
    void move();
    QPoint currentPos() const;
private slots:
    void hitTarget();
private:
    const QPoint mystartPos;
        Enemy *	mytarget;
    MainWindow * mygame;
    const QPoint mytargetPos;
    const QPixmap mysprite;
    QPoint mycurrentPos;
    int	mydamage;
    static const QSize ms_fixedSize;
};

#endif // BULLET_H
