#include "BlockItem.h"

BlockItem::BlockItem( QObject * parent )
    : QObject( parent ),
      QGraphicsRectItem()
{
//    setCursor( m_cursor );
//        QGraphicsItemGroup::setHandlesChildEvents( false );
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
