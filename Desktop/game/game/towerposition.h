#ifndef TOWERPOSITION_H
#define TOWERPOSITION_H

#include <QPoint>
#include <QSize>
#include <QPixmap>

class QPainter;

class TowerPosition{
public:
    TowerPosition(QPoint pos,const QPixmap &sprite=QPixmap(":/image/towerposition.png"));
    bool containPoint(const QPoint &pos) const;
    void draw(QPainter *painter) const;
   void setHasTower(bool hasTower=true);
    bool hasTower() const;
    const QPoint centerPos() const;  
private:
    bool myhasTower;
    QPoint mypos;
    QPixmap mysprite;
    static const QSize ms_fixedSize;
};

#endif // TOWERPOSITION_H
