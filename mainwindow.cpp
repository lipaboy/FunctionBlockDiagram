#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "widgets/CBlockItem.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSize size( 800, 600 );
    this->resize( size );
    this->setFixedSize( size );

    auto * scene = new QGraphicsScene;
    auto * graphicsView = ui->graphicsView;

    graphicsView->setGeometry( 0, 0 , 10, 10);
    graphicsView->resize( size );
    QBrush brush( Qt::black, Qt::SolidPattern );

//    QPixmap texture( 11, 11 );
//    texture.fill( Qt::black );
//    QImage image = texture.toImage();
//    image.setPixel( texture.size().width() / 2, texture.size().height() / 2, Qt::white );
//    texture.fromImage( image );

//    brush.setTexture( texture );
    graphicsView->setBackgroundBrush( brush );

    graphicsView->setScene(scene);
    graphicsView->setRenderHint(QPainter::Antialiasing);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setCacheMode(QGraphicsView::CacheBackground); // Кэш фона
    graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    scene->setSceneRect( -350,-300,700,600 );
    {
        auto * blockItem = new CBlockItem( 6, 1 );
        scene->addItem( blockItem );
        blockItem->setPos( 120, -60 );
    }
    {
        auto * blockItem = new CBlockItem( 1, 1 );
        scene->addItem( blockItem );
        blockItem->setPos( -140, -80 );
    }
    {
        auto * blockItem = new CBlockItem( 3, 4 );
        scene->addItem( blockItem );
        blockItem->setPos( -100, 100 );
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
