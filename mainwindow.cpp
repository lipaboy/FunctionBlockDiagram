#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "widgets/CBlockItem.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_fbd = new CFunctionBlockDiagramWidget( this );
    this->setCentralWidget( m_fbd );

    QSize size( 1200, 800 );
    this->resize( size );

}

