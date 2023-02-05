#include "PinItem.h"

#include <QCursor>

PinItem::PinItem( const QColor & baseColor, QObject * parent )
    : QObject( parent ),
      QGraphicsItem(),
      m_baseColor( baseColor )
{
    QPen pen{};
    pen.setWidth( 0 );
    setPen( pen );
    setBrush( baseColor );
}

void PinItem::setSelected( bool isSelected )
{
    setBrush( isSelected ? m_selectColor : m_baseColor );
    update();
}

QRectF PinItem::boundingRect() const
{
    return m_rect;
}

void PinItem::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *option,
                    QWidget *widget)
{
    Q_UNUSED( option )
    Q_UNUSED( widget )
    painter->setPen( m_pen );
    painter->setBrush( m_brush );
    switch( m_type )
    {
    case RECT:
        painter->drawRect( m_rect );
        break;
    case CIRCULAR:
        painter->drawEllipse( m_rect );
        break;
    }
}

void PinItem::setRect(const QRectF &rect)
{
    m_rect = rect;
}

void PinItem::setBrush(const QBrush &brush)
{
    m_brush = brush;
}

void PinItem::setPen(const QPen &pen)
{
    m_pen = pen;
}

void PinItem::setType(const Type &type)
{
    m_type = type;
}

void PinItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
    emit clicked();
    QGraphicsItem::mousePressEvent( event );
//    Q_UNUSED( event );
}

//void PinItem::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
//{
//    setCursor( Qt::CrossCursor );
//    QGraphicsItem::mouseMoveEvent( event );
//}

//void PinItem::setSize(const QSizeF &newSize)
//{
//    m_rect.setWidth( newSize.width() );
//    m_rect.setHeight( newSize.height() );
//}
