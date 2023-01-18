#include "FunctionBlockDiagramWidget.h"

#include "widgets/FunctionBlockItem.h"
#include "widgets/ConnectionItem.h"

#include <QKeyEvent>
#include <QVBoxLayout>

#include <QDebug>

FunctionBlockDiagramWidget::FunctionBlockDiagramWidget(
        int externalOutPinsCount,
        int externalInPinsCount,
        QVector< SFunctionInfo > const & functionInfoList,
        QWidget * parent )
    : QWidget( parent )
{
    auto * vBox = new QVBoxLayout( this );
    m_view = new QGraphicsView{};
    vBox->addWidget( m_view );

    QSize sceneSize //{ 1000, 700 };
            = m_view->size();
    m_scene = new QGraphicsScene{};
    m_view->setScene( m_scene );
    m_scene->setSceneRect( 0, 0, sceneSize.width(), sceneSize.height() );

    m_blockAppearPoint = QPointF( sceneSize.width() / 2, 50 );

    m_view->setRenderHint( QPainter::Antialiasing );
    m_view->setCacheMode( QGraphicsView::CacheBackground ); // Кэш фона
    m_view->setViewportUpdateMode( QGraphicsView::BoundingRectViewportUpdate );
//    m_view->setDragMode( QGraphicsView::DragMode::ScrollHandDrag );
    m_view->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    m_view->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    /** Устанавливаем фон */
    {
        int textureSize = 32;
        QPixmap texture( textureSize, textureSize );
        texture.fill( Qt::black );
        QImage image = texture.toImage();
        image.fill( Qt::black );
        image.setPixel( image.width() / 2,
                        image.height() / 2,
                        QColor( Qt::red ).rgb() );

        texture.convertFromImage( image );

        QBrush brush( Qt::black, Qt::SolidPattern );
        brush.setTexture( texture );

        m_view->setBackgroundBrush( QBrush( texture ) );
    }

    {
        m_functionGraph = new FunctionGraph{
                externalOutPinsCount,
                externalInPinsCount,
                this };
        connect( m_functionGraph, &FunctionGraph::updated,
                 this, &FunctionBlockDiagramWidget::graphUpdated,
                 Qt::DirectConnection );
        connect( m_functionGraph, &FunctionGraph::connectionChanged,
                 this, &FunctionBlockDiagramWidget::setConnection,
                 Qt::DirectConnection );

        m_functionGraph->loadVertices( functionInfoList );
    }
}

FunctionBlockDiagramWidget::~FunctionBlockDiagramWidget()
{
    for ( auto iter = m_linesMap.begin(); iter != m_linesMap.end(); ++iter )
    {
        delete iter.value();
    }
}

void FunctionBlockDiagramWidget::graphUpdated()
{
    auto nodeList = m_functionGraph->getFunctionNodes();

    QPointF position = m_blockAppearPoint;
    for ( auto & node : nodeList )
    {
        if ( m_blockMap.contains( node.id ) )
        {
            continue;
        }

        auto * blockItem = new FunctionBlockItem( node.name,
                                                  node.inPins.size(),
                                                  node.outPins.size(),
                                                  this );

        if ( node.id != m_functionGraph->getExternalOutPinsId()
             && node.id != m_functionGraph->getExternalInPinsId() )
        {
            position += QPointF( 0, blockItem->size().height() / 2 );
        }

        m_blockMap.insert( node.id, blockItem );
        m_scene->addItem( blockItem );
        blockItem->setPos( position );
        connect( blockItem, & FunctionBlockItem::pinClicked,
                 this,
                 [ this, funcId = node.id ]
                 ( SFunctionPinIndex::EPinType type, int pinIndex ) -> void
        {
            blockPinClicked( SFunctionPinIndex( type, funcId, pinIndex ) );
        }, Qt::DirectConnection );

        if ( node.id != m_functionGraph->getExternalOutPinsId()
             && node.id != m_functionGraph->getExternalInPinsId() )
        {
            position += QPointF( 0, blockItem->size().height() / 2 )
                    + m_stepAppearPoint;
        }
    }

    {
        int outPinsId = m_functionGraph->getExternalOutPinsId();
        auto * outPinBlock = m_blockMap[ outPinsId ];
        outPinBlock->setSize( QSize( outPinBlock->size().width(),
                                     m_scene->sceneRect().height() ) );

        outPinBlock->setTopLeftPos( QPointF( 50, 0 ) );
    }

    {
        int inPinsId = m_functionGraph->getExternalInPinsId();
        auto * inPinBlock = m_blockMap[ inPinsId ];
        inPinBlock->setSize( QSize( inPinBlock->size().width(),
                                    m_scene->sceneRect().height() ) );

        inPinBlock->setTopLeftPos(
                    QPointF(
                        m_scene->sceneRect().right()
                         - inPinBlock->size().width(),
                        0 ) );
    }
}

void FunctionBlockDiagramWidget::blockPinClicked( SFunctionPinIndex funcPinIndex )
{
    auto & pinSelected = ( funcPinIndex.pinType == SFunctionPinIndex::IN )
            ? m_inPinSelected : m_outPinSelected;
    if ( pinSelected.has_value() )
    {
        setPinSelected( pinSelected.value(), false );
    }
    pinSelected = funcPinIndex;
    setPinSelected( pinSelected.value(), true );

    if ( m_inPinSelected.has_value() && m_outPinSelected.has_value() )
    {
        setPinSelected( m_inPinSelected.value(), false );
        setPinSelected( m_outPinSelected.value(), false );

        m_functionGraph->connectVertices( m_inPinSelected.value(),
                                          m_outPinSelected.value() );

        m_inPinSelected.reset();
        m_outPinSelected.reset();
    }
}

void FunctionBlockDiagramWidget::keyPressEvent(QKeyEvent *event)
{
    if ( event->key() == Qt::Key::Key_Delete
         || event->key() == Qt::Key::Key_Backspace )
    {
        if ( m_connectionSelected.has_value() )
        {
            auto con = m_connectionSelected.value();
            m_functionGraph->disconnectVertices( con.inFunc, con.outFunc );
            m_connectionSelected.reset();
        }
    }
}

void FunctionBlockDiagramWidget::resizeEvent(QResizeEvent *event)
{
    m_view->fitInView(0, 0, 500, 500, Qt::KeepAspectRatio);
//    m_scene->setSceneRect( 0, 0, event->size().width(), event->size().height() );
    QWidget::resizeEvent( event );
}

void FunctionBlockDiagramWidget::setPinSelected(
        const SFunctionPinIndex & funcPinIndex,
        bool isSelected )
{
    m_blockMap[ funcPinIndex.funcId ]->setPinSelected( funcPinIndex.pinType,
                                                       funcPinIndex.pin,
                                                       isSelected );
}

void FunctionBlockDiagramWidget::setConnection( const SFunctionPinIndex & inFuncIndex,
                                                const SFunctionPinIndex & outFuncIndex,
                                                bool isConnected )
{
    SConnection keyCon{ inFuncIndex, outFuncIndex };
    if ( ! isConnected )
    {
        delete m_linesMap.take( keyCon );
    }
    else
    {
        if ( ! m_linesMap.contains( keyCon ) )
        {
            auto * lineItem = new ConnectionItem( nullptr );
            m_linesMap.insert( keyCon, lineItem );
            m_scene->addItem( lineItem );
            lineItem->setZValue( -2 );
            auto * inBlockItem = m_blockMap[ inFuncIndex.funcId ];
            auto * outBlockItem = m_blockMap[ outFuncIndex.funcId ];
            lineItem->setLine(
                        QLineF(
                            inBlockItem->getEdgePinPoint( SFunctionPinIndex::IN,
                                                          inFuncIndex.pin ),
                            outBlockItem->getEdgePinPoint( SFunctionPinIndex::OUT,
                                                           outFuncIndex.pin )
                            )
                        );

            connect( lineItem, & ConnectionItem::selected,
                     this,
                     [ this, keyCon ] ( bool isSelected ) -> void
            {
                if ( ! isSelected )
                {
                    m_connectionSelected.reset();
                }
                else
                {
                    if ( m_connectionSelected.has_value()
                         && m_connectionSelected.value() != keyCon )
                    {
                        auto * lineSelected =
                                *( m_linesMap.find( m_connectionSelected.value() ) );
                        lineSelected->setSelected( false );
                    }
                    m_connectionSelected = keyCon;
                }
            } );

            connect( inBlockItem, & FunctionBlockItem::positionChanged,
                     lineItem,
                     [ blockItem = inBlockItem,
                     lineItem,
                     pin = inFuncIndex.pin ] () -> void
            {
                auto line = lineItem->line();
                line.setP1( blockItem->getEdgePinPoint( SFunctionPinIndex::IN, pin ) );
                lineItem->setLine( line );
            }, Qt::DirectConnection );

            connect( outBlockItem, & FunctionBlockItem::positionChanged,
                     lineItem,
                     [ blockItem = outBlockItem,
                     lineItem,
                     pin = outFuncIndex.pin ] () -> void
            {
                auto line = lineItem->line();
                line.setP2( blockItem->getEdgePinPoint( SFunctionPinIndex::OUT, pin ) );
                lineItem->setLine( line );
            }, Qt::DirectConnection );
        }
    }
}
