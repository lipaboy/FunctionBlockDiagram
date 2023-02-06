#include "BodyItem.h"

namespace view
{

BodyItem::BodyItem( QObject * parent )
    : QObject( parent ),
      QGraphicsItem()
{
    m_gradient.setCoordinateMode( QGradient::ObjectBoundingMode );
    m_gradient.setStart( 0.0, 0.0 );
    m_gradient.setFinalStop( 1.0, 0.0 );
    m_gradient.setColorAt( 0.0,  QColor::fromString( "#BB9898cd" ) );
    m_gradient.setColorAt( 0.5,  QColor::fromString( "#BBaef1f9" ) );

//    setCursor( m_cursor );
    //        QGraphicsItemGroup::setHandlesChildEvents( false );
}

QRectF BodyItem::boundingRect() const
{
    QRectF bounding = m_rect;
    return bounding;
}

void BodyItem::paint( QPainter * painter,
                       const QStyleOptionGraphicsItem * option,
                       QWidget * widget )
{
    Q_UNUSED( option )
    Q_UNUSED( widget )

//    painter->setBrush( m_brush );
    painter->setBrush( m_gradient );
    painter->setPen( m_pen );
    painter->drawRoundedRect( m_rect.x(), m_rect.y(),
                              m_rect.width(), m_rect.height(),
                              m_radiusRounding, m_radiusRounding,
                              Qt::AbsoluteSize);
}

void BodyItem::setRect(const QRectF &rect)
{
    m_rect = rect;
}

void BodyItem::setBrush(const QBrush &brush)
{
    m_brush = brush;
}

void BodyItem::setPen(const QPen &pen)
{
    m_pen = pen;
}

QVariant BodyItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
//    if ( change == ItemPositionChange )
//    {
//        emit positionChanged();
//    }
    return QGraphicsItem::itemChange( change, value );
}

void BodyItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
//    this->setCursor( Qt::ClosedHandCursor );
    QGraphicsItem::mousePressEvent( event );
}

void BodyItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
//    this->setCursor( m_cursor );
    QGraphicsItem::mouseReleaseEvent( event );
}

}
