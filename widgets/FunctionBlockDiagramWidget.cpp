#include "FunctionBlockDiagramWidget.h"

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
            { "Motor", 2, 3 },
            { "Pressure", 1, 3 }
        } );

    // Построить группу, эллипс, линию, прямоугольник
//    QGraphicsItemGroup *pGroup = new QGraphicsItemGroup();
//    QGraphicsEllipseItem *pFrom = new QGraphicsEllipseItem();
//    QGraphicsLineItem *pLink = new QGraphicsLineItem();
//    QGraphicsRectItem *pTo = new QGraphicsRectItem();

//    // Установить группу необязательно, съемную
//    pGroup->setFlags( //QGraphicsItem::ItemIsSelectable |
//                      QGraphicsItem::ItemIsMovable);

//    // Устанавливаем стиль (цвет кисти, цвет фона кисти)
//    QPen pen = pFrom->pen();
//    pen.setWidth(2);
//    pen.setColor(QColor(0, 160, 230));
//    pFrom->setPen(pen);
//    pLink->setPen(pen);
//    pTo->setPen(pen);
//    pFrom->setBrush(QColor(247, 160, 57));
//    pTo->setBrush(QColor(247, 160, 57));

//    // Добавить элемент в группу
//    pGroup->addToGroup(pFrom);
//    pGroup->addToGroup(pTo);
//    pGroup->addToGroup(pLink);

//    // Установить область эллипса и прямоугольника
//    const double length = 50;
//    pFrom->setRect(QRectF(-length/2.0, -length/2.0, length, length));
//    pTo->setRect(QRectF(-length/2.0, -length/2.0, length, length));

//    // Устанавливаем координаты эллипса, прямоугольника и соединительной линии
//    pFrom->setPos(80, 80);
//    pTo->setPos(200, 150);
//    pLink->setLine(QLineF(pFrom->pos(), pTo->pos()));

//    // Добавить группу к сцене
//    m_scene->addItem(pGroup);

    // Установить сцену для вида
}

void FunctionBlockDiagramWidget::graphUpdated()
{
    auto vertices = m_functionGraph->getVertices();

//    auto castToQPointF = [] ( const QSizeF & size) -> QPointF {
//        return QPointF( size.width(), size.height() );
//    };


    QPointF position = m_blockMap.empty()
            ? m_blockAppearPoint
            : m_blockMap.back()->pos() + QPointF( 0, m_blockMap.back()->size().height() )
              + m_stepAppearPoint;
    for ( int i = m_blockMap.size(); i < vertices.size(); i++ )
    {
        auto & vertex = vertices[i];
        auto * blockItem = new BlockItem( vertex.inputPins.size(),
                                           vertex.outputPins.size() );
        m_blockMap.push_back( blockItem );
        m_scene->addItem( blockItem );
        blockItem->setPos( position );
        position += QPointF( 0, blockItem->size().height() ) + m_stepAppearPoint;
    }
}
