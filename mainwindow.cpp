#include "mainwindow.h"

#include "widgets/FunctionBlockDiagramWidget.h"

#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>

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

    this->setCentralWidget( m_fbd );

    QMenu * fileMenu = new QMenu( tr( "&Файл" ) );
    fileMenu->addAction(
                "&Экспорт графа",
                this,
                [ this ] () -> void
    {
        QString fileName = QFileDialog::getSaveFileName(
                    this,
                    tr( "Экспорт в ..." ),
                    "~/graph.txt",
                    tr("Text file (*.txt)"));
        m_fbd->exportGraphToFile( fileName );
    });

    QMenuBar * mainMenu = new QMenuBar{};
    mainMenu->addMenu( fileMenu );
    this->setMenuBar( mainMenu );
}

