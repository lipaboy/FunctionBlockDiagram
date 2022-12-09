#include "BlockItem.h"

BlockItem::BlockItem( int ins, int outs, QObject * parent )
    : QObject( parent ),
      QGraphicsItem(),
      m_ins( ins ),
      m_outs( outs )
{
    int max = std::max( ins, outs );
    m_mainRect.setX( 0 );
    m_mainRect.setY( 0 );
    m_mainRect.setWidth( m_mainRectBase.width() );
    m_mainRect.setHeight( ( m_pinEdge + m_pinSpace ) * ( max > 0 ? max - 1 : 0 ) + m_mainRectBase.height() );
    m_mainRect.moveCenter( QPointF( 0, 0 ) );

    setFlags( QGraphicsItem::ItemIsMovable );
    setCursor( m_cursor );
}

// задаёт размер области перерисовки
QRectF BlockItem::boundingRect() const
{
    QRectF bounding( m_mainRect );
    bounding.adjust( -10, -10, 10, 10 );
    return bounding;
}

void BlockItem::paint( QPainter * painter,
                        const QStyleOptionGraphicsItem * option,
                        QWidget * widget )
{
    QPen pen{};
    pen.setWidth( 1 );
    pen.setColor( Qt::black );
    painter->setPen( pen );
    painter->setBrush( Qt::gray );
    painter->drawRect( m_mainRect );

    auto drawPinsFn =
            [ painter, this ]
            ( const QColor & color, bool isLeft, int count ) -> void
    {
        QPen pen{};
        pen.setWidth( 0 );
        pen.setColor( color );
        painter->setPen( pen );
        painter->setBrush( color );
        qreal centerX = ( isLeft ? -1 : 1 ) * m_mainRect.width() / 2;

        qreal pinDistance = ( m_mainRect.height() - m_mainRectBase.height() )
                / ( count > 1 ? count - 1 : 1 );
        qreal firstY = m_mainRectBase.height() / 2 + m_mainRect.y();
        for ( int i = 0; i < count; i++ )
        {
            QRectF inRect{ 0, 0, m_pinEdge, m_pinEdge };
            inRect.moveCenter( QPointF( centerX, firstY + i * pinDistance ) );
            painter->drawRect( inRect );
        }
    };

    drawPinsFn( m_inColor, true, m_ins );
    drawPinsFn( m_outColor, false, m_outs );

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void BlockItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
    this->setCursor( Qt::ClosedHandCursor );
    QGraphicsItem::mousePressEvent( event );
}

void BlockItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
    this->setCursor( m_cursor );
    QGraphicsItem::mouseReleaseEvent( event );
}
