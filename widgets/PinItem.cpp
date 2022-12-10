#include "PinItem.h"

PinItem::PinItem( const QColor & baseColor, QObject * parent )
    : QObject( parent ),
      QGraphicsRectItem()
{
    QPen pen{};
    pen.setWidth( 0 );
    setPen( pen );
    setBrush( baseColor );
}

//void PinItem::setSize(const QSizeF &newSize)
//{
//    m_rect.setWidth( newSize.width() );
//    m_rect.setHeight( newSize.height() );
//}
