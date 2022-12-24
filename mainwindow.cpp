#include "mainwindow.h"

#include "widgets/FunctionBlockDiagramWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QVector< SFunctionInfo > functionInfoList =
    {
        { "Temperature", 3, 1 },
        { "Motor", 2, 2 },
        { "Pressure", 1, 1 },
        { "Kek", 2, 2 },
    };

    m_fbd = new FunctionBlockDiagramWidget( 5, 5, functionInfoList, this );
    this->setCentralWidget( m_fbd );

    QSize size( 1200, 800 );
    this->resize( size );
}

