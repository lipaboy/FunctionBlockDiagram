#include "BlockItem.h"

BlockItem::BlockItem( QObject * parent )
    : QObject( parent ),
      QGraphicsRectItem()
{
//    setCursor( m_cursor );
    //        QGraphicsItemGroup::setHandlesChildEvents( false );
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
