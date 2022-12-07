#include "CBlockItem.h"

CBlockItem::CBlockItem( int ins, int outs, QObject * parent )
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
    m_mainRect.moveCenter( QPoint( 0, 0 ) );
}

// задаёт размер области перерисовки
QRectF CBlockItem::boundingRect() const
{
    QRectF bounding( m_mainRect );
    bounding.adjust( -10, -10, 10, 10 );
    return bounding;
}

void CBlockItem::paint( QPainter * painter,
                        const QStyleOptionGraphicsItem * option,
                        QWidget * widget )
{
    QPen pen{};
    pen.setWidth( 1 );
    pen.setColor( Qt::black );
    painter->setPen( pen );
    painter->setBrush( Qt::gray );
    painter->drawRect( m_mainRect );

    auto fn =
            [ painter, this ]
            ( const QColor & color, bool isLeft, int count ) -> void
    {
        QPen pen{};
        pen.setWidth( 0 );
        pen.setColor( color );
        painter->setPen( pen );
        painter->setBrush( color );
        int centerX = ( isLeft ? -1 : 1 ) * m_mainRect.width() / 2;

        int pinDistance = ( m_mainRect.height() - m_mainRectBase.height() )
                / ( count > 1 ? count - 1 : 1 );
        int firstY = m_mainRectBase.height() / 2 + m_mainRect.y();
        for ( int i = 0; i < count; i++ )
        {
            QRect inRect{ 0, 0, m_pinEdge, m_pinEdge };
            inRect.moveCenter( QPoint( centerX, firstY + i * pinDistance ) );
            painter->drawRect( inRect );
        }
    };

    fn( m_inColor, true, m_ins );
    fn( m_outColor, false, m_outs );

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void CBlockItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    /* Устанавливаем позицию графического элемента
     * в графической сцене, транслировав координаты
     * курсора внутри графического элемента
     * в координатную систему графической сцены
     * */
    this->setPos(mapToScene(event->pos()));
}

void CBlockItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    /* При нажатии мышью на графический элемент
     * заменяем курсор на руку, которая держит этот элемента
     * */
    this->setCursor(QCursor(Qt::ClosedHandCursor));
    Q_UNUSED(event);
}

void CBlockItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    /* При отпускании мышью элемента
     * заменяем на обычный курсор стрелку
     * */
    this->setCursor(QCursor(Qt::ArrowCursor));
    Q_UNUSED(event);
}
