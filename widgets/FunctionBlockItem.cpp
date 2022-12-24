#include "FunctionBlockItem.h"

#include "BlockItem.h"
#include "PinItem.h"

#include <QDebug>

FunctionBlockItem::FunctionBlockItem( const QString & labelText,
                                      int ins,
                                      int outs,
                                      QObject * parent )
    : QObject( parent )
    , QGraphicsItemGroup()
{
    int max = std::max( ins, outs );


    m_label = new QGraphicsSimpleTextItem( this );
    {
        m_label->setText( labelText );
        m_label->setFont( QFont{ "Times", 12 } );
        auto rect = m_label->boundingRect();
        rect.moveCenter( QPoint( 0, 0 ) );
        m_label->setPos( rect.topLeft() );
    }
    addToGroup( m_label );
    m_label->setZValue( 1 );

    m_block = new BlockItem( this );
    {
        QRectF rect{};
        rect.setX( 0 );
        rect.setY( 0 );
        rect.setWidth( m_mainRectBase.width()
                       + m_label->boundingRect().width() );
        rect.setHeight( ( m_pinEdge + m_pinSpace ) * ( max > 0 ? max - 1 : 0 )
                        + m_mainRectBase.height() );
        rect.moveCenter( QPointF( 0, 0 ) );
        m_block->setRect( rect );
        m_block->setBrush( Qt::gray );
        QPen pen{};
        pen.setColor( Qt::black );
        pen.setWidth( 1 );
        m_block->setPen( pen );
        connect( m_block, & BlockItem::positionChanged,
                 this, & FunctionBlockItem::positionChanged,
                 Qt::DirectConnection );
    }
    addToGroup( m_block );
    m_block->setZValue( 0 );

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
            pin->setZValue( 2 );
            i++;
        }
    };

    drawPinsFn( true, m_inPins );
    drawPinsFn( false, m_outPins );

    QGraphicsItemGroup::setHandlesChildEvents( false );
    QGraphicsItemGroup::setFiltersChildEvents( true );
    //    setAcceptHoverEvents( true );


    setFlag( QGraphicsItem::ItemIsMovable, true );
    setFlag( QGraphicsItem::ItemSendsScenePositionChanges, true );
}

void FunctionBlockItem::setPinSelected( bool isIn,
                                        int index,
                                        bool isSelected )
{
    ( isIn ? m_inPins[ index ] : m_outPins[ index ] )
            ->setSelected( isSelected );
}

QSizeF FunctionBlockItem::size() const
{
    return m_block->rect().size();
}

QPointF FunctionBlockItem::getEdgePinPoint(bool isIn, int pinIndex) const
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

QVariant FunctionBlockItem::itemChange( QGraphicsItem::GraphicsItemChange change,
                                        const QVariant & value )
{
    if ( change == ItemPositionChange )
    {
        emit positionChanged();
    }
    return QGraphicsItem::itemChange( change, value );
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
