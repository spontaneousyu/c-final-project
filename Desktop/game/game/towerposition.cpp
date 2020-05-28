#include "towerposition.h"
#include <QPainter>

const QSize TowerPosition::ms_fixedSize(88, 88);

TowerPosition::TowerPosition(QPoint pos, const QPixmap &sprite):myhasTower(false),mypos(pos),mysprite(sprite){}
void TowerPosition::setHasTower(bool hasTower){//mark not vailable for another tower
    myhasTower=hasTower;
}
void TowerPosition::draw(QPainter *painter) const{//const525 paint tower
    painter->drawPixmap(mypos.x(),mypos.y(),mysprite);
}
const QPoint TowerPosition::centerPos() const{//const 525 change from top left to center
    QPoint offsetPoint(ms_fixedSize.width()/2,ms_fixedSize.height()/2);
    return mypos+offsetPoint;
}
bool TowerPosition::containPoint(const QPoint &pos) const{//const525 to check if the click is in the yellow point
    bool isXInHere=mypos.x()<pos.x()&&pos.x()<(mypos.x()+ms_fixedSize.width());
    bool isYInHere=mypos.y()<pos.y()&&pos.y()<(mypos.y()+ms_fixedSize.height());
    return isXInHere&&isYInHere;
}
bool TowerPosition::hasTower() const{//const 525 check if available
    return myhasTower;
}
