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
    m_label = new QGraphicsSimpleTextItem( this );
    {
        m_label->setText( labelText );
        m_label->setFont( QFont{ "Times", 12 } );
    }
    addToGroup( m_label );
    m_label->setZValue( 1 );

    m_block = new BlockItem( this );
    {
        m_block->setBrush( QColor( 184, 162, 173, 200 ) );
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
            emit this->pinClicked( SFunctionPinIndex::IN, index );
        }, Qt::DirectConnection );
        pinItem->setZValue( 2 );
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
            emit this->pinClicked( SFunctionPinIndex::OUT, index );
        }, Qt::DirectConnection );
        pinItem->setZValue( 2 );
    }

    recalcItemsPos();

    QGraphicsItemGroup::setHandlesChildEvents( false );
    QGraphicsItemGroup::setFiltersChildEvents( true );
    //    setAcceptHoverEvents( true );


    setFlag( QGraphicsItem::ItemIsMovable, true );
    setFlag( QGraphicsItem::ItemSendsScenePositionChanges, true );
}

void FunctionBlockItem::recalcItemsPos()
{
    int maxPins = std::max< int >( m_inPins.size(), m_outPins.size() );

    {
        auto rect = m_label->boundingRect();
        rect.moveCenter( QPoint( 0, 0 ) );
        m_label->setPos( rect.topLeft() );
    }

    {
        QRectF rect{};
        rect.setX( 0 );
        rect.setY( 0 );
        rect.setWidth( m_mainRectBase.width()
                       + m_label->boundingRect().width() );
        rect.setHeight( ( m_pinEdge + m_pinSpace )
                            * ( maxPins > 0 ? maxPins - 1 : 0 )
                        + m_mainRectBase.height() );
        rect.moveCenter( QPointF( 0, 0 ) );
        m_block->setRect( rect );
    }

    auto drawPinsFn =
            [ this ]
            (
            bool isLeft,
            const QVector< PinItem * > & pinItems ) -> void
    {
        qreal centerX =
                ( isLeft ? -1 : 1 ) * m_block->rect().width() / 2;

        int i = 1;
        qreal pinDistance = m_block->rect().height()
                / ( pinItems.size() + 1 );
        qreal firstY = m_block->rect().y();
        QRectF pinRect{ 0, 0, m_pinEdge, m_pinEdge };
        for ( auto * pin : pinItems )
        {
            pinRect.moveCenter( QPointF( centerX,
                                         firstY + i * pinDistance ) );
            pin->setRect( pinRect );
            this->addToGroup( pin );
            i++;
        }
    };

    drawPinsFn( true, m_inPins );
    drawPinsFn( false, m_outPins );
}

void FunctionBlockItem::setPinSelected( SFunctionPinIndex::EPinType type,
                                        int index,
                                        bool isSelected )
{
    ( type == SFunctionPinIndex::IN
            ? m_inPins[ index ] : m_outPins[ index ] )
            ->setSelected( isSelected );
}

void FunctionBlockItem::setSize( const QSizeF & newSize )
{
    m_mainRectBase.rwidth() = newSize.width() - m_pinEdge;

    int maxPins = std::max< int >( m_inPins.size(), m_outPins.size() );
    if ( maxPins > 1 )
    {
        m_pinSpace = ( newSize.height() - m_mainRectBase.height() )
                / ( maxPins - 1 ) - m_pinEdge;
    }

    recalcItemsPos();
}

void FunctionBlockItem::setTopLeftPos(const QPointF &newPos)
{
    setPos( newPos.x() - size().width() / 2,
            newPos.y() + size().height() / 2 );
}

QSizeF FunctionBlockItem::size() const
{
    return QSizeF( m_block->rect().size().width() + m_pinEdge,
                   m_block->rect().size().height() );
}

QPointF FunctionBlockItem::getEdgePinPoint(
        SFunctionPinIndex::EPinType type, int pinIndex) const
{
    if ( type == SFunctionPinIndex::IN )
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

QRectF FunctionBlockItem::boundingRect() const
{
    // must be overloaded, otherwise the boundingrect will only be actualized on
    // additem is actualized. This leads to the fact that the boundingrect
    // will not close around the word items after e.g., moving them.
    return childrenBoundingRect();
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

