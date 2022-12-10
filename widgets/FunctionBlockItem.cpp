#include "FunctionBlockItem.h"

FunctionBlockItem::FunctionBlockItem( int ins, int outs, QObject * parent )
    : QObject( parent )
{
    int max = std::max( ins, outs );

    m_block = new BlockItem( 0, 0, this );
    {
        QRectF rect{};
        rect.setX( 0 );
        rect.setY( 0 );
        rect.setWidth( m_mainRectBase.width() );
        rect.setHeight( ( m_pinEdge + m_pinSpace ) * ( max > 0 ? max - 1 : 0 )
                        + m_mainRectBase.height() );
        rect.moveCenter( QPointF( 0, 0 ) );
        m_block->setRect( rect );
        m_block->setBrush( Qt::gray );
        QPen pen{};
        pen.setColor( Qt::black );
        pen.setWidth( 1 );
        m_block->setPen( pen );
    }
    addToGroup( m_block );



    QColor inPinColor = Qt::green;
    for ( int i = 0; i < ins; i++ )
    {
        auto * pinItem = new PinItem( inPinColor, this );
        m_inPins.push_back( pinItem );
    }

    QColor outPinColor = Qt::red;
    for ( int i = 0; i < outs; i++ )
    {
        auto * pinItem = new PinItem( outPinColor, this );
        m_outPins.push_back( pinItem );
    }

    auto drawPinsFn =
            [ this ]
            (
            bool isLeft,
            const QVector< PinItem * > & pinItems ) -> void
    {
        qreal centerX = ( isLeft ? -1 : 1 ) * m_block->rect().width() / 2;

        int i = 1;
        qreal pinDistance = m_block->rect().height() / ( pinItems.size() + 1 );
        qreal firstY = m_block->rect().y();
        QRectF pinRect{ 0, 0, m_pinEdge, m_pinEdge };
        for ( auto * pin : pinItems )
        {
            pinRect.moveCenter( QPointF( centerX, firstY + i * pinDistance ) );
            pin->setRect( pinRect );
            this->addToGroup( pin );
            i++;
        }
    };

    drawPinsFn( true, m_inPins );
    drawPinsFn( false, m_outPins );

    {
        QRectF rect{ 0, 0, 10, 10 };
        auto * p = new QGraphicsEllipseItem( this );
        rect.moveCenter( QPointF( 0, 0 ) );
        p->setRect( rect );
        p->setPos( 0, 0 );
    }

    setFlags( QGraphicsItem::ItemIsMovable );
}
