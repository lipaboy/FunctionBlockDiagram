#include "FunctionBlockDiagramWidget.h"

#include <QDebug>
#include <QVBoxLayout>

FunctionBlockDiagramWidget::FunctionBlockDiagramWidget( QWidget * parent )
    : QWidget( parent )
{
    auto * vBox = new QVBoxLayout( this );
    m_graphicsView = new QGraphicsView{};
    vBox->addWidget( m_graphicsView );

    QSize sceneSize { 1000, 800 };
    m_scene = new QGraphicsScene{};
    m_graphicsView->setScene( m_scene );
    m_scene->setSceneRect( 0, 0, sceneSize.width(), sceneSize.height() );

    QBrush brush( Qt::black, Qt::SolidPattern );

    /** TODO: установить точечный фон */
//    QPixmap texture( 11, 11 );
//    texture.fill( Qt::black );
//    QImage image = texture.toImage();
//    image.setPixel( texture.size().width() / 2, texture.size().height() / 2, Qt::white );
//    texture.fromImage( image );

//    brush.setTexture( texture );
    m_graphicsView->setBackgroundBrush( brush );

    m_graphicsView->setRenderHint( QPainter::Antialiasing );
    m_graphicsView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    m_graphicsView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    m_graphicsView->setCacheMode( QGraphicsView::CacheBackground ); // Кэш фона
    m_graphicsView->setViewportUpdateMode( QGraphicsView::BoundingRectViewportUpdate );

    m_functionGraph = new FunctionGraph{ this };
    connect( m_functionGraph, &FunctionGraph::updated,
             this, &FunctionBlockDiagramWidget::graphUpdated,
             Qt::DirectConnection );

    m_functionGraph->loadVertices(
        {
            { "Temperature", 3, 1 },
            { "Motor", 2, 2 },
            { "Pressure", 1, 1 },
            { "Kek", 2, 2 },
            { "Lol", 0, 0 },
        } );

}

void FunctionBlockDiagramWidget::graphUpdated()
{
    auto vertices = m_functionGraph->getVertices();

    QPointF position = m_blockMap.empty()
            ? m_blockAppearPoint
            : m_blockMap.back()->pos() + QPointF( 0, m_blockMap.back()->size().height() )
              + m_stepAppearPoint;
    for ( int i = m_blockMap.size(); i < vertices.size(); i++ )
    {
        auto & vertex = vertices[i];
        auto * blockItem = new FunctionBlockItem( vertex.inputPins.size(),
                                                  vertex.outputPins.size(),
                                                  this );
        position += QPointF( 0, blockItem->size().height() / 2 );
        m_blockMap.push_back( blockItem );
        m_scene->addItem( blockItem );
        blockItem->setPos( position );
        position += QPointF( 0,
                             blockItem->size().height() / 2
                             ) + m_stepAppearPoint;
    }
}
