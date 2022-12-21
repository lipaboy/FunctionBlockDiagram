#include "ConnectionItem.h"

#include <QPen>

ConnectionItem::ConnectionItem(QObject *parent)
    : QObject ( parent ),
      QGraphicsLineItem ()
{
    QPen pen{};
    pen.setWidth( 2 );
    pen.setColor( QColor::fromRgb( 0, 119, 255 ) );
    this->setPen( pen );
}
