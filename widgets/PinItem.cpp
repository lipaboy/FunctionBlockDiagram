#include "PinItem.h"

#include <QCursor>

PinItem::PinItem( const QColor & baseColor, QObject * parent )
    : QObject( parent ),
      QGraphicsRectItem()
{
    QPen pen{};
    pen.setWidth( 0 );
    setPen( pen );
    setBrush( baseColor );
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
