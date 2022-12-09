#include "PinItem.h"

PinItem::PinItem(const QColor &baseColor, QObject *parent)
{

}

void PinItem::setSize(const QSizeF &newSize)
{
    m_rect.setWidth( newSize.width() );
    m_rect.setHeight( newSize.height() );
}

QRectF PinItem::boundingRect() const
{
    return m_rect;
}
