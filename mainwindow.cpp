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

    QSize size( 1000, 700 );
    this->resize( size );

    m_fbd = new FunctionBlockDiagramWidget( 5, 5, functionInfoList, this );
    qDebug() << m_fbd->view()->size();
    qDebug() << m_fbd->view()->viewport()->size();
    qDebug() << m_fbd->view()->sceneRect().size();
    this->setCentralWidget( m_fbd );
    qDebug() << m_fbd->view()->size();
    qDebug() << m_fbd->view()->viewport()->size();
    qDebug() << m_fbd->view()->sceneRect().size();

}

