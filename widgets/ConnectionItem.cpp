#include "ConnectionItem.h"

#include <QPen>
#include <QPainter>

#include <qmath.h>

ConnectionItem::ConnectionItem(QObject *parent)
    : QObject ( parent ),
      QGraphicsLineItem()
{
    this->setPen( QPen( QColor::fromRgb( 0, 119, 255 ), 3 ) );

//    setFlag( ItemSendsGeometryChanges, true );
//    setFlag( ItemSendsScenePositionChanges, true );

    recalcSelectionPolygon();
}

void ConnectionItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
    emit clicked();
    QPen pen{};
    pen.setColor( this->pen().color() == Qt::yellow ? Qt::blue : Qt::yellow );
    setPen( pen );
    QGraphicsItem::mousePressEvent( event );
}

// Костыль: нужно придумать как пересчитывать область выделения через itemChange method
void ConnectionItem::setLine(const QLineF &line)
{
    QGraphicsLineItem::setLine( line );
    recalcSelectionPolygon();
}

//QVariant ConnectionItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
//{
//    if ( change == ItemPositionChange )
//    {
//        recalcSelectionPolygon();
//    }
//    return QGraphicsItem::itemChange( change, value );
//}

QRectF ConnectionItem::boundingRect() const
{
    return shape().controlPointRect();
}

QPainterPath ConnectionItem::shape() const
{
    QPainterPath ret;
    ret.addPolygon( m_selectionPolygon );
    return ret;
}

void ConnectionItem::paint( QPainter * painter,
                            const QStyleOptionGraphicsItem * option,
                            QWidget * widget )
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(pen());
    painter->drawLine(line());
    {
//        painter->setPen( QPen( Qt::red, 2, Qt::DashLine ) );
//        painter->drawPolygon( m_selectionPolygon );
    }
}

void ConnectionItem::recalcSelectionPolygon()
{
    QPolygonF rotatedRect;
    qreal radAngle = qDegreesToRadians( line().angle() );
    qreal dx = m_selectionOffset * sin(radAngle);
    qreal dy = m_selectionOffset * cos(radAngle);
    QPointF offset1 = QPointF( dx, dy );
    QPointF offset2 = QPointF( -dx, -dy );
    rotatedRect << line().p1() + offset1
             << line().p1() + offset2
             << line().p2() + offset2
             << line().p2() + offset1;
    m_selectionPolygon = rotatedRect;
    update();
}
