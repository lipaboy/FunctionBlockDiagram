#include "mainwindow.h"

#include "widgets/BlockItem.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_fbd = new FunctionBlockDiagramWidget( this );
    this->setCentralWidget( m_fbd );

    QSize size( 1200, 800 );
    this->resize( size );

}

