#include "waypoint.h"
#include <QPainter>
#include <QColor>
WayPoint::WayPoint(QPoint pos):mypos(pos),mynextWayPoint(NULL){}
void WayPoint::setNextWayPoint(WayPoint *nextPoint){//mark the next moving direction
    mynextWayPoint=nextPoint;
}
WayPoint* WayPoint::nextWayPoint() const{//to show the next direction const525
    return mynextWayPoint;
}
const QPoint WayPoint::pos() const{//to show current position const525
    return mypos;
}


