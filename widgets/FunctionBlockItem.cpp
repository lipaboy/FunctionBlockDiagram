#include "FunctionBlockItem.h"

#include <QDebug>

FunctionBlockItem::FunctionBlockItem( int ins, int outs, QObject * parent )
    : QObject( parent )
    , QGraphicsItemGroup()
{
    int max = std::max( ins, outs );

    m_block = new BlockItem( this );
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
        connect( pinItem, & PinItem::clicked,
                 this,
                 [ this, index = m_inPins.size() - 1 ] () -> void
        {
            emit this->pinClicked( true, index );
        }, Qt::DirectConnection );
    }

    QColor outPinColor = Qt::red;
    for ( int i = 0; i < outs; i++ )
    {
        auto * pinItem = new PinItem( outPinColor, this );
        m_outPins.push_back( pinItem );
        connect( pinItem, & PinItem::clicked,
                 this,
                 [ this, index = m_outPins.size() - 1 ] () -> void
        {
            emit this->pinClicked( false, index );
        }, Qt::DirectConnection );
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

    QGraphicsItemGroup::setHandlesChildEvents( false );
    QGraphicsItemGroup::setFiltersChildEvents( true );
    //    setAcceptHoverEvents( true );

    setFlag( QGraphicsItem::ItemIsMovable, true );


}

void FunctionBlockItem::setPinSelected( bool isIn,
                                        int index,
                                        bool isSelected )
{
    ( isIn ? m_inPins[ index ] : m_outPins[ index ] )
            ->setSelected( isSelected );
}

QPointF FunctionBlockItem::getEdgePinPoint(bool isIn, bool pinIndex) const
{
    if ( isIn )
    {
        auto * pin = m_inPins[ pinIndex ];
        return mapToScene(
                    QPointF( pin->rect().left(),
                             pin->rect().top()
                             + pin->rect().height() / 2 )
                    );
    }
    auto * pin = m_outPins[ pinIndex ];
    return mapToScene(
                QPointF( pin->rect().right(),
                         pin->rect().top()
                         + pin->rect().height() / 2 )
                );
}

void FunctionBlockItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    QPointF mousePoint{ /*mapToScene*/( event->pos() ) };
//    auto rect = m_block->rect();
////    rect.moveCenter( /*mapToScene*/( m_block->pos() ) );
//    qDebug() << rect << mousePoint;
//    bool flag = false;
//    if ( m_block->rect().contains( mousePoint ) )
//    {
//        flag = true;
//        for ( auto * pin : m_inPins )
//        {
//            if ( pin->rect().contains( mousePoint ) )
//            {
//                flag = false;
//                break;
//            }
//        }
//        for ( auto * pin : m_outPins )
//        {
//            if ( pin->rect().contains( mousePoint ) )
//            {
//                flag = false;
//                break;
//            }
//        }
//    }
//    if ( flag )
//    {
//        setCursor( Qt::ClosedHandCursor );
//        setFlag( QGraphicsItem::ItemIsMovable, true );
//    }
//    else
//    {
//        setFlag( QGraphicsItem::ItemIsMovable, false );
//    }
    QGraphicsItemGroup::mousePressEvent( event );
}

void FunctionBlockItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//    setCursor( Qt::OpenHandCursor );
    QGraphicsItemGroup::mouseReleaseEvent( event );
}

void FunctionBlockItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF mousePoint{ event->pos() };
    qDebug() << m_block->rect() << mousePoint;
//    if ( m_block->rect().contains( mousePoint ) )
//    {
//        bool flag = false;
//        for ( auto * pin : m_inPins )
//        {
//            if ( pin->rect().contains( mousePoint ) )
//            {
//                flag = true;
//                break;
//            }
//        }
//        for ( auto * pin : m_outPins )
//        {
//            if ( pin->rect().contains( mousePoint ) )
//            {
//                flag = true;
//                break;
//            }
//        }
//        if ( flag )
//        {
//            setCursor( Qt::CrossCursor );
//        }
//        else
//        {
//            setCursor( Qt::ArrowCursor );
//        }
//    }
    QGraphicsItemGroup::hoverEnterEvent( event );
}
