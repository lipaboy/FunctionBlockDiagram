#include "FunctionBlockDiagramWidget.h"

#include "widgets/FunctionBlockItem.h"
#include "widgets/ConnectionItem.h"

#include <QKeyEvent>
#include <QVBoxLayout>
#include <QSet>

namespace view
{

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
        QImage image = texture.toImage();
        image.fill( QColor::fromString( "#040812" ) );
        image.setPixel( image.width() / 2,
                        image.height() / 2,
                        QColor( Qt::red ).rgb() );

        texture.convertFromImage( image );
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

        m_functionGraph->loadFunctions( functionInfoList );
    }

    auto pinSelectFunc =
            [ this ] ( SFunctionPinIndex funcPinIndex, bool isSelected ) -> void
    {
        m_blockMap[ funcPinIndex.funcId ]->setPinSelected( funcPinIndex.pinType,
                                                           funcPinIndex.pin,
                                                           isSelected );
    };
    m_inPinSelected.setSwitchFunc( pinSelectFunc );
    m_outPinSelected.setSwitchFunc( pinSelectFunc );
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
    auto nodeList = m_functionGraph->getNodes();

    QSet< int > blocksToDelete{};
    {
        auto keyList = m_blockMap.keys();
        std::copy( keyList.begin(), keyList.end(),
                   std::inserter( blocksToDelete,
                                  blocksToDelete.begin() ) );
    }

    QPointF position = m_blockAppearPoint;
    for ( auto & node : nodeList )
    {
        /** Если мы встретили в модели функцию, значит её не нужно удалять.
         *  Удаляем те, которых больше нет в модели. */
        blocksToDelete.remove( node.id );

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
        connect( blockItem, & FunctionBlockItem::selectionChanged,
                 this,
                 [ this, funcId = node.id ]
                 ( bool isSelected ) -> void
        {
            if ( isSelected )
            {
                m_blockIdSelected = funcId;
            }
            else
            {
                m_blockIdSelected.reset();
            }
        }, Qt::DirectConnection );

        if ( node.id != m_functionGraph->getExternalOutPinsId()
             && node.id != m_functionGraph->getExternalInPinsId() )
        {
            position += QPointF( 0, blockItem->size().height() / 2 )
                    + m_stepAppearPoint;
        }
    }

    for ( int blockId : blocksToDelete )
    {
        auto * blockItem = m_blockMap.take( blockId );
        delete blockItem;
    }

    if ( m_isFirstGraphVisualization )
    {
        int outPinsId = m_functionGraph->getExternalOutPinsId();
        auto * outPinBlock = m_blockMap[ outPinsId ];
        outPinBlock->setSize( QSize( outPinBlock->size().width(),
                                     m_scene->sceneRect().height() ) );

        outPinBlock->setTopLeftPos( QPointF( 50, 0 ) );
    }

    if ( m_isFirstGraphVisualization )
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

    m_isFirstGraphVisualization = false;
}

void FunctionBlockDiagramWidget::blockPinClicked( SFunctionPinIndex funcPinIndex )
{
    auto & pinSelected = ( funcPinIndex.pinType == SFunctionPinIndex::IN )
            ? m_inPinSelected : m_outPinSelected;
    pinSelected.turnOn( funcPinIndex );

    if ( m_inPinSelected.isOn() && m_outPinSelected.isOn() )
    {
        m_functionGraph->connectPins( m_inPinSelected.value(),
                                          m_outPinSelected.value() );

        m_inPinSelected.turnOff();
        m_outPinSelected.turnOff();
    }
}

void FunctionBlockDiagramWidget::exportGraphToFile(const QString &filename)
{
    m_functionGraph->saveToFile( filename );
}

void FunctionBlockDiagramWidget::addFunction( const QString & functionName )
{
    m_functionGraph->addFunction( functionName );
}

void FunctionBlockDiagramWidget::keyPressEvent(QKeyEvent *event)
{
    if ( event->key() == Qt::Key::Key_Delete
         || event->key() == Qt::Key::Key_Backspace )
    {
        if ( m_connectionSelected.has_value() )
        {
            auto con = m_connectionSelected.value();
            m_functionGraph->disconnectPins( con.inFunc, con.outFunc );
            m_connectionSelected.reset();
        }
        if ( m_blockIdSelected.has_value() )
        {
            m_functionGraph->removeFunction( m_blockIdSelected.value() );
            m_blockIdSelected.reset();
        }
    }
}

void FunctionBlockDiagramWidget::mousePressEvent(QMouseEvent *event)
{
    if ( event->buttons() == Qt::RightButton )
    {
        m_inPinSelected.turnOff();
        m_outPinSelected.turnOff();
    }
}

void FunctionBlockDiagramWidget::resizeEvent(QResizeEvent *event)
{
//    m_view->fitInView(0, 0, 500, 500, Qt::KeepAspectRatio);
//    m_scene->setSceneRect( 0, 0, event->size().width(), event->size().height() );
    QWidget::resizeEvent( event );
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

}
