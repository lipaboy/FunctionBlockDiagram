#include "PinItem.h"

#include <QCursor>

PinItem::PinItem( const QColor & baseColor, QObject * parent )
    : QObject( parent ),
      QGraphicsRectItem(),
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
