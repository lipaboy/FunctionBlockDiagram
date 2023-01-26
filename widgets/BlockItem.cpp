#include "BlockItem.h"

BlockItem::BlockItem( QObject * parent )
    : QObject( parent ),
      QGraphicsItem()
{
//    setCursor( m_cursor );
    //        QGraphicsItemGroup::setHandlesChildEvents( false );
}

QRectF BlockItem::boundingRect() const
{
    QRectF bounding = m_rect;
    return bounding;
}

void BlockItem::paint( QPainter * painter,
                       const QStyleOptionGraphicsItem * option,
                       QWidget * widget )
{
    Q_UNUSED( option )
    Q_UNUSED( widget )
    painter->setBrush( m_brush );
    painter->setPen( m_pen );
    painter->drawRoundedRect( m_rect.x(), m_rect.y(),
                              m_rect.width(), m_rect.height(),
                              m_radiusRounding, m_radiusRounding,
                              Qt::AbsoluteSize);
}

void BlockItem::setRect(const QRectF &rect)
{
    m_rect = rect;
}

void BlockItem::setBrush(const QBrush &brush)
{
    m_brush = brush;
}

void BlockItem::setPen(const QPen &pen)
{
    m_pen = pen;
}

QVariant BlockItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
//    if ( change == ItemPositionChange )
//    {
//        emit positionChanged();
//    }
    return QGraphicsItem::itemChange( change, value );
}

void BlockItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
//    this->setCursor( Qt::ClosedHandCursor );
    QGraphicsItem::mousePressEvent( event );
}

void BlockItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
//    this->setCursor( m_cursor );
    QGraphicsItem::mouseReleaseEvent( event );
}
